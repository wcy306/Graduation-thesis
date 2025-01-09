#pragma once


#include "../pch.h"
#include "InfiniteLoop.h"
#include "../Detec3Dsr.h"
#include "../SR_SdkDll/SR7Link.h"
#include <fstream>

using namespace std;

InfiniteLoop::InfiniteLoop(void)
{

}

InfiniteLoop::~InfiniteLoop(void)
{

}

void InfiniteLoop::VariableInit()
{
    bReceiveData = false;
    bCamBonline = false;
    mBatchWidth = 0;
    mProfileData = NULL;
    mIntensityData = NULL;
    mEncoderData = NULL;
    mDispH = 2000;
    mDelayTime = 10;
    mXinterVal = 0;
    mYinterVal = 0.1;
}

void InfiniteLoop::setDelayTime(int _time)
{
    mDelayTime = _time;
}

void InfiniteLoop::setProfileWidth(int _width)
{
    mBatchWidth = _width;
}

void InfiniteLoop::setCamBonlineFlag(bool flag)
{
    bCamBonline = flag;
}

int InfiniteLoop::StartMeasure(int iTimeout)
{
    int reT = SR7IF_StartMeasure(DEVICEID, iTimeout);
    return reT;
}

int InfiniteLoop::StartIOTriggerMeasure(int iTimeout)
{
    int reT = SR7IF_StartIOTriggerMeasure(DEVICEID, iTimeout);
    return reT;
}

void InfiniteLoop::setTextInfo(CString _text)
{
	m_pFunReceiveback(m_pWindow, _text, mBatchWidth, mDispH, 0);
}

void InfiniteLoop::DataShow()
{
	m_pFunReceiveback(m_pWindow, _T(""), mBatchWidth, mDispH, 1);
}

void InfiniteLoop::startReceiveThread()
{
    bReceiveData = true;
	AfxBeginThread(ThreadImageDisplay, this);  /* 启动线程 */
}

UINT InfiniteLoop::ThreadImageDisplay(LPVOID pParam)
{
	((InfiniteLoop *)pParam)->ImageDisplayFunc();
    return 0;
}

UINT InfiniteLoop::ImageDisplayFunc()
{
	/******数据初始化*****/
    int mDataNum = mDispH * mBatchWidth;
    int mEncoderN = 1;
    if(bCamBonline)
    {
        mEncoderN = 2;
    }
    for(int i = 0; i < mDataNum; i ++)
    {
        mProfileData[i] = -10000 * 100000;
    }
    memset(mIntensityData, 0, sizeof(unsigned char) * mDataNum);
    memset(mEncoderData, 0, sizeof(unsigned int) * mDispH * mEncoderN);

    //数据缓存中间变量
    int* TmpBatchPoint = NULL;
    unsigned char* TmpGrayData = NULL;
    unsigned int* FrameLoss = NULL;
    long long *FrameId  = NULL;
    unsigned int* Encoder = NULL;
    int mTmpH = 1000;   //行数应设置为小于显示的高度mDispH
    try
    {
        TmpBatchPoint = new int[mTmpH * mBatchWidth];           //当前批次高度数据缓存
        TmpGrayData = new unsigned char[mTmpH * mBatchWidth];   //当前批次灰度数据缓存
        FrameLoss =  new unsigned int [mTmpH * mEncoderN];      //批处理过快掉帧数量数据缓存
        FrameId = new long long[mTmpH];                         //帧编号数据缓存
        Encoder = new unsigned int [mTmpH * mEncoderN];         //编码器数据缓存
    }
    catch (...)
    {
        setTextInfo(_T("数据缓存中间变量内存申请失败"));

        //停止批处理
        SR7IF_StopMeasure(DEVICEID);
        return 0;
    }

    long long BatchPoint_CurNo = 0;     //当前批处理编号
    long long OverFlowStartId = 0;      //溢出起始帧号
    long long m_BatchPoint_CurNo = 0;   //当前总行数
    int FrameLossID = 0;                //丢帧数
    int EncoderID = 0;                  //编码器值
    strInfo = _T("");

    //循环接收
    bool bError = false;
	CString strTotalC = _T("");
    do
    {
        /* 接收数据---当前批次高度数据、灰度数据、编码器数据、帧编号、掉帧数量数据 */
        int m_curBatchPoint = SR7IF_GetBatchRollData(DEVICEID,
                                                     NULL,
                                                     TmpBatchPoint,
                                                     TmpGrayData,
                                                     Encoder,
                                                     FrameId,
                                                     FrameLoss,
                                                     500);
        if(m_curBatchPoint < 0)
        {
            if(m_curBatchPoint == SR7IF_ERROR_MODE)
            {
                setTextInfo(_T("当前为非循环模式"));
                bReceiveData = false;
                bError = true;
                break;
            }
            else if(m_curBatchPoint == SR7IF_NORMAL_STOP)
            {
                bReceiveData = false;
                bError = true;
                setTextInfo(_T("外部IO或其他因素导致批处理正常停止"));
                break;
            }
            else
            {
                /* 获取错误码 */
                int EthErrCnt, UserErrCnt;
                SR7IF_GetBatchRollError(0, &EthErrCnt, &UserErrCnt);
				SYSTEMTIME sttT;
                CString strDate, strTime;
                GetLocalTime(&sttT);
                strDate.Format(_T("  %4d-%2d-%2d  "), sttT.wYear, sttT.wMonth, sttT.wDay);
                strTime.Format(_T("  %2d:%2d:%2d  "), sttT.wHour, sttT.wMinute, sttT.wSecond);
                CString strrt;
                strrt = " ";
				if(m_curBatchPoint == SR7IF_ERROR_ROLL_DATA_OVERFLOW)
                {
                    CString strTmp;
                    strTmp.Format(_T("数据获取过慢，数据覆盖.网络原因溢出量 %d, 用户原因溢出量 %d, 溢出起始帧号 %d， 丢帧数 %d  编码器值 %d"), 
						EthErrCnt, UserErrCnt, OverFlowStartId, FrameLossID, EncoderID);
                    strInfo += strTmp + strrt + strDate + strrt + strTime;
                    strInfo += _T("\r\n");
					setTextInfo(strInfo);
                }
                else if(m_curBatchPoint == SR7IF_ERROR_ROLL_BUSY)
                {
                    CString stmp;
                    stmp = _T("busy");
                    strInfo += stmp + strrt + strDate + strrt + strTime;
                    strInfo += _T("\r\n");
					setTextInfo(strInfo);
                }
                continue;
            }
        }

        if(m_curBatchPoint == 0)
            continue;

        //上一次最后一行对应的帧等信息
        int TmpID = m_curBatchPoint - 1;
        OverFlowStartId = FrameId[TmpID];
        FrameLossID = FrameLoss[TmpID];
        EncoderID = Encoder[TmpID];
		if(bCamBonline)
		{
			FrameLossID = FrameLoss[TmpID * mEncoderN];
			EncoderID = Encoder[TmpID * mEncoderN];
		}


        //数据拷贝显示
        int TmpN = m_curBatchPoint * mBatchWidth;
        if(BatchPoint_CurNo < mDispH)
        {
            if(BatchPoint_CurNo + m_curBatchPoint > mDispH)
            {
                int TMPP = int(mBatchWidth * (mDispH - BatchPoint_CurNo));

                /* 高度 */
                memcpy(&mProfileData[BatchPoint_CurNo * mBatchWidth], TmpBatchPoint, sizeof(int) * TMPP);

                /* 灰度 */
                memcpy(&mIntensityData[BatchPoint_CurNo * mBatchWidth], TmpGrayData, TMPP);

                /* 编码器 */
                memcpy(&mEncoderData[BatchPoint_CurNo * mEncoderN], Encoder,
                        sizeof(unsigned int) * (mDispH - BatchPoint_CurNo) * mEncoderN);

                //显示 mDispH - BatchPoint_CurNo 行数据
                DataShow();

                //多出的行数重新排列
                int TMPPT = int(BatchPoint_CurNo + m_curBatchPoint - mDispH);
                int TTTT = TMPPT * mBatchWidth;
                int Tmpx = (mDispH - TMPPT) * mBatchWidth;

                /* 高度 */
                memcpy(&mProfileData[0], &mProfileData[TTTT], sizeof(int) * Tmpx);
                memcpy(&mProfileData[Tmpx], &TmpBatchPoint[TMPP], sizeof(int) * TTTT);

                /* 灰度 */
                memcpy(&mIntensityData[0], &mIntensityData[TTTT], Tmpx);
                memcpy(&mIntensityData[Tmpx], &TmpGrayData[TMPP], TTTT);

                /* 编码器 */
                memcpy(&mEncoderData[0], &mEncoderData[TMPPT * mEncoderN],
                        (mDispH - TMPPT) * mEncoderN * sizeof(unsigned int));
                memcpy(&mEncoderData[(mDispH - TMPPT) * mEncoderN],
                        &Encoder[(mDispH - BatchPoint_CurNo) * mEncoderN],
                        TMPPT * mEncoderN * sizeof(unsigned int));

                //显示
                DataShow();

                BatchPoint_CurNo += m_curBatchPoint;
                m_BatchPoint_CurNo = BatchPoint_CurNo;

				//显示当前接收的总行数
				strTotalC.Format(_T("当前总行数:%d"), m_BatchPoint_CurNo);
				setTextInfo(strTotalC);
                continue;
            }

            /* 高度 */
            memcpy(&mProfileData[BatchPoint_CurNo * mBatchWidth], TmpBatchPoint, sizeof(int) * TmpN);

            /* 灰度 */
            memcpy(&mIntensityData[BatchPoint_CurNo * mBatchWidth], TmpGrayData, TmpN);

            /* 编码器 */
            memcpy(&mEncoderData[BatchPoint_CurNo * mEncoderN], Encoder,
                    m_curBatchPoint * mEncoderN * sizeof(unsigned int));
        }
        else
        {
            /* 重新排列 */
            int TTTT = (mDispH - m_curBatchPoint) * mBatchWidth;
            memcpy(&mProfileData[0], &mProfileData[TmpN], sizeof(int) * TTTT);
            memcpy(&mProfileData[TTTT], TmpBatchPoint, sizeof(int) * TmpN);

            /* 灰度 */
            memcpy(&mIntensityData[0], &mIntensityData[TmpN], TTTT);
            memcpy(&mIntensityData[TTTT], TmpGrayData, TmpN);

            /* 编码器 */
            memcpy(&mEncoderData[0], &mEncoderData[m_curBatchPoint * mEncoderN],
                    sizeof(unsigned int) * (mDispH - m_curBatchPoint));
            memcpy(&mIntensityData[(mDispH - m_curBatchPoint) * mEncoderN], Encoder,
                    sizeof(unsigned int) * m_curBatchPoint * mEncoderN);
        }

        //显示
        DataShow();

        BatchPoint_CurNo += m_curBatchPoint;
        m_BatchPoint_CurNo = BatchPoint_CurNo;

		//显示当前接收的总行数
		strTotalC.Format(_T("当前总行数:%d"), m_BatchPoint_CurNo);
        setTextInfo(strTotalC);

        //根据需要加个延迟显示
        Sleep(mDelayTime);
    }while(bReceiveData);

    if(bError)  //因错误导致循环退出
    {
        //停止批处理
        SR7IF_StopMeasure(DEVICEID);
    }

    /* 内存释放 */
    if(TmpBatchPoint)
    {
        delete[] TmpBatchPoint;
        TmpBatchPoint = NULL;
    }
    if(TmpGrayData)
    {
        delete[] TmpGrayData;
        TmpGrayData = NULL;
    }
    if(Encoder)
    {
        delete[] Encoder;
        Encoder = NULL;
    }
    if(FrameLoss)
    {
        delete[] FrameLoss;
        FrameLoss = NULL;
    }
    if(FrameId)
    {
        delete[] FrameId;
        FrameId = NULL;
    }
	return 0;
}

void InfiniteLoop::exitReceiveThread()
{
    bReceiveData = false;
}

void InfiniteLoop::DataMemoryInit(bool _BonLine, int mProW)
{
    //数据缓存申请
    try
    {
        int mWidthC = mProW;
        int MAXECODER = 1;
        if(_BonLine)
        {
            mWidthC = 6400;
            MAXECODER = 2;
        }
        int mDataC = mDispH * mWidthC;
        mProfileData = new int[mDataC];
        mIntensityData = new unsigned char[mDataC];
        mEncoderData = new unsigned int[mDispH * MAXECODER];
    }
    catch (...)  
	{
		AfxMessageBox(_T("数据内存申请失败!"), MB_OK | MB_ICONQUESTION);
		exit(0);
	}
}

void InfiniteLoop::deleteDataMemory()
{
    if(mProfileData)
    {
        delete[] mProfileData;
        mProfileData = NULL;
    }
    if(mIntensityData)
    {
        delete[] mIntensityData;
        mIntensityData = NULL;
    }
    if(mEncoderData)
    {
        delete[] mEncoderData;
        mEncoderData = NULL;
    }
}

void InfiniteLoop::setDispHeight(int _height)
{
    if(_height != mDispH)
    {
        mDispH = _height;

        //释放之前的内存
        deleteDataMemory();

        //重新申请内存
        DataMemoryInit(bCamBonline);
    }
}

void InfiniteLoop::clearInfo()
{
    strInfo = _T("");
}

int* InfiniteLoop::getBatchData()
{
    return mProfileData;
}

unsigned char* InfiniteLoop::getIntensityData()
{
    return mIntensityData;
}

int InfiniteLoop::getBatchHeight()
{
    return mDispH;
}

int InfiniteLoop::getProfileWidth()
{
    return mBatchWidth;
}

void InfiniteLoop::ImageUpdate()
{
	DataShow();
}

int InfiniteLoop::saveHeightData(CString _path)
{
    if(mProfileData == NULL
            || mBatchWidth == 0)
    {
        return -1;
    }

    //获取后缀
    CString strA;
    strA = _path.Mid(_path.GetLength() - 4);
    if(strA == ".bin")
    {
		HANDLE hFile;
		hFile = CreateFile(_path,
			GENERIC_WRITE,
			0,
			NULL,
			CREATE_ALWAYS,
			FILE_FLAG_WRITE_THROUGH,
			NULL);

		/* 定义一个变量来接收实际写入的字符数 */
		DWORD realWriteSize;
		WriteFile(hFile, (char *)mProfileData, sizeof(int) * mDispH * mBatchWidth, &realWriteSize, NULL);
    }
    else if(strA == ".ecd")
    {
		BATCH_INFO mBatchInfo;
        mBatchInfo.version = BATCH_SAVE_FILE_VERSION;
        mBatchInfo.width = mBatchWidth;
        mBatchInfo.height = mDispH;
        mBatchInfo.xInterval = mXinterVal;
        mBatchInfo.yInterval = mYinterVal;
		strcpy(mBatchInfo.info, "SSZN2021 V00000002");

		HANDLE hFile;
		hFile = CreateFile(_path,
			GENERIC_WRITE,
			0,
			NULL,
			CREATE_ALWAYS,
			FILE_FLAG_WRITE_THROUGH,
			NULL);

		/* 定义一个变量来接收实际写入的字符数 */
		DWORD realWriteSize;
		WriteFile(hFile, (char *)&mBatchInfo, sizeof(BATCH_INFO), &realWriteSize, NULL);
		WriteFile(hFile, (char *)mProfileData, sizeof(int) * mDispH * mBatchWidth, &realWriteSize, NULL);

		//写入编码器信息
		if(mEncoderData)
		{
			int MAXECODER = 1;
			if(bCamBonline)
			{
				MAXECODER = 2;
			}
			WriteFile(hFile, (char *)mEncoderData, sizeof(unsigned int) * mDispH * MAXECODER, &realWriteSize, NULL);
		}

		CloseHandle(hFile); /*关闭文件 关闭句柄。*/
    }
    else
    {
        return -1;
    }

    return 0;
}

int InfiniteLoop::saveIntensityData(CString _path)
{
    if(mIntensityData == NULL
            || mBatchWidth == 0)
    {
        return -1;
    }

    //获取后缀
    CString strA;
    strA = _path.Mid(_path.GetLength() - 4);
    if(strA == ".bin")
    {
		HANDLE hFile;
		hFile = CreateFile(_path,
			GENERIC_WRITE,
			0,
			NULL,
			CREATE_ALWAYS,
			FILE_FLAG_WRITE_THROUGH,
			NULL);

		/* 定义一个变量来接收实际写入的字符数 */
		DWORD realWriteSize;
		WriteFile(hFile, (char *)mIntensityData, sizeof(unsigned char) * mDispH * mBatchWidth, &realWriteSize, NULL);
		CloseHandle(hFile); /*关闭文件 关闭句柄。*/
    }
    else if(strA == ".bmp")
    {
		BITMAPFILEHEADER targetfileheader;
        BITMAPINFOHEADER targetinfoheader;
        memset(&targetfileheader, 0, sizeof(BITMAPFILEHEADER));
        memset(&targetinfoheader, 0, sizeof(BITMAPINFOHEADER));

        /* 构造灰度图的文件头 */
        targetfileheader.bfOffBits = (DWORD)sizeof(BITMAPFILEHEADER) + (DWORD)sizeof(BITMAPINFOHEADER) + sizeof(RGBQUAD) * 256;
        targetfileheader.bfSize = 192 * 192 + sizeof(RGBQUAD) * 256 + sizeof(BITMAPFILEHEADER) + sizeof(BITMAPINFOHEADER);
        targetfileheader.bfReserved1 = 0;
        targetfileheader.bfReserved2 = 0;
        targetfileheader.bfType = 0x4d42;

        /* 构造灰度图的信息头 */
        targetinfoheader.biBitCount = 8;          // 每个像素所需的位数，必须是1(双色), 4(16色)，8(256色)或24(真彩色)之一
        targetinfoheader.biSize = sizeof(BITMAPINFOHEADER);
        targetinfoheader.biHeight = mDispH;
        targetinfoheader.biWidth = mBatchWidth;
        targetinfoheader.biPlanes = 1;            // 目标设备的级别，必须为1 
        targetinfoheader.biCompression = BI_RGB;  // 位图压缩类型，必须是 0(不压缩), 1(BI_RLE8压缩类型)或2(BI_RLE4压缩类型)之一 
        targetinfoheader.biSizeImage = 0;         // 位图的大小，以字节为单位  
        targetinfoheader.biXPelsPerMeter = 0;     // 位图水平分辨率，每米像素数  
        targetinfoheader.biYPelsPerMeter = 0;     // 位图垂直分辨率，每米像素数  
        targetinfoheader.biClrImportant = 0;      // 位图显示过程中重要的颜色数  
        targetinfoheader.biClrUsed = 0;           // 位图实际使用的颜色表中的颜色数  

        /* 构造灰度图的调色版 */
        RGBQUAD rgbquad[256];
        int i;
        for (i = 0; i<256; i++)
        {
            rgbquad[i].rgbBlue = i;
            rgbquad[i].rgbGreen = i;
            rgbquad[i].rgbRed = i;
            rgbquad[i].rgbReserved = 0;
        }

        /* 由于BMP图像对于行是倒置的，即图像显示的第一行是最后一行数据，所以要倒置 */
        BYTE* targetbuf = new BYTE[mDispH * mBatchWidth];
        int TmpP = mDispH - 1;
        for (int i = TmpP; i >= 0; i--)
        {
            int ii = i * mBatchWidth;
            int iii = (TmpP - i) * mBatchWidth;
            for (int j = 0; j < mBatchWidth; j++)
            {
                targetbuf[ii + j] = mIntensityData[iii + j];
            }
        }

        /* 写入文件 */
        CFile cf;
        if (!cf.Open(_path, CFile::modeCreate | CFile::modeWrite))
        {
            delete[] targetbuf;
            return -1;
        }
        cf.Write(&targetfileheader, sizeof(BITMAPFILEHEADER));
        cf.Write(&targetinfoheader, sizeof(BITMAPINFOHEADER));
        cf.Write(&rgbquad, sizeof(RGBQUAD) * 256);
        cf.Write(targetbuf, mDispH * mBatchWidth); //这里targetbuf的大小为192x192
        cf.Close();

        delete[] targetbuf;
		targetbuf = NULL;
    }
    else
    {
        return -1;
    }

    return 0;
}

int InfiniteLoop::saveEncoderData(CString _path)
{
    if(mEncoderData == NULL
            || mBatchWidth == 0)
    {
        return -1;
    }

    //获取后缀
    CString strA;
    strA = _path.Mid(_path.GetLength() - 4);

    int MAXECODER = 1;
    if(bCamBonline)
    {
        MAXECODER = 2;
    }
    if(strA == ".txt")
    {
        try
        {
            std::fstream file(_path, std::ios::out);

            /* 以逗号隔开写入 */
            char aa[32];
            memset(aa, 0, sizeof(aa));
            for (unsigned int i = 0; i < mDispH; i++)
            {
                int tt = i * MAXECODER;
                for (int j = 0; j < MAXECODER; j++)
                {
                    int len = sprintf(aa, "%u ", mEncoderData[tt + j]);
                    file.write(aa, len);
                }
                file << "\n";
            }
            file.close();
        }
        catch (...)
        {
            return -1;
        }
    }
    else if(strA == ".bin")
    {
		HANDLE hFile;
		hFile = CreateFile(_path,
			GENERIC_WRITE,
			0,
			NULL,
			CREATE_ALWAYS,
			FILE_FLAG_WRITE_THROUGH,
			NULL);

		/* 定义一个变量来接收实际写入的字符数 */
		DWORD realWriteSize;
		WriteFile(hFile, (char *)mEncoderData, sizeof(unsigned int) * mDispH * MAXECODER, &realWriteSize, NULL);
		CloseHandle(hFile); /*关闭文件 关闭句柄。*/
    }
    else
    {
        return -1;
    }

    return 0;
}
