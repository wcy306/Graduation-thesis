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
	AfxBeginThread(ThreadImageDisplay, this);  /* �����߳� */
}

UINT InfiniteLoop::ThreadImageDisplay(LPVOID pParam)
{
	((InfiniteLoop *)pParam)->ImageDisplayFunc();
    return 0;
}

UINT InfiniteLoop::ImageDisplayFunc()
{
	/******���ݳ�ʼ��*****/
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

    //���ݻ����м����
    int* TmpBatchPoint = NULL;
    unsigned char* TmpGrayData = NULL;
    unsigned int* FrameLoss = NULL;
    long long *FrameId  = NULL;
    unsigned int* Encoder = NULL;
    int mTmpH = 1000;   //����Ӧ����ΪС����ʾ�ĸ߶�mDispH
    try
    {
        TmpBatchPoint = new int[mTmpH * mBatchWidth];           //��ǰ���θ߶����ݻ���
        TmpGrayData = new unsigned char[mTmpH * mBatchWidth];   //��ǰ���λҶ����ݻ���
        FrameLoss =  new unsigned int [mTmpH * mEncoderN];      //����������֡�������ݻ���
        FrameId = new long long[mTmpH];                         //֡������ݻ���
        Encoder = new unsigned int [mTmpH * mEncoderN];         //���������ݻ���
    }
    catch (...)
    {
        setTextInfo(_T("���ݻ����м�����ڴ�����ʧ��"));

        //ֹͣ������
        SR7IF_StopMeasure(DEVICEID);
        return 0;
    }

    long long BatchPoint_CurNo = 0;     //��ǰ��������
    long long OverFlowStartId = 0;      //�����ʼ֡��
    long long m_BatchPoint_CurNo = 0;   //��ǰ������
    int FrameLossID = 0;                //��֡��
    int EncoderID = 0;                  //������ֵ
    strInfo = _T("");

    //ѭ������
    bool bError = false;
	CString strTotalC = _T("");
    do
    {
        /* ��������---��ǰ���θ߶����ݡ��Ҷ����ݡ����������ݡ�֡��š���֡�������� */
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
                setTextInfo(_T("��ǰΪ��ѭ��ģʽ"));
                bReceiveData = false;
                bError = true;
                break;
            }
            else if(m_curBatchPoint == SR7IF_NORMAL_STOP)
            {
                bReceiveData = false;
                bError = true;
                setTextInfo(_T("�ⲿIO���������ص�������������ֹͣ"));
                break;
            }
            else
            {
                /* ��ȡ������ */
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
                    strTmp.Format(_T("���ݻ�ȡ���������ݸ���.����ԭ������� %d, �û�ԭ������� %d, �����ʼ֡�� %d�� ��֡�� %d  ������ֵ %d"), 
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

        //��һ�����һ�ж�Ӧ��֡����Ϣ
        int TmpID = m_curBatchPoint - 1;
        OverFlowStartId = FrameId[TmpID];
        FrameLossID = FrameLoss[TmpID];
        EncoderID = Encoder[TmpID];
		if(bCamBonline)
		{
			FrameLossID = FrameLoss[TmpID * mEncoderN];
			EncoderID = Encoder[TmpID * mEncoderN];
		}


        //���ݿ�����ʾ
        int TmpN = m_curBatchPoint * mBatchWidth;
        if(BatchPoint_CurNo < mDispH)
        {
            if(BatchPoint_CurNo + m_curBatchPoint > mDispH)
            {
                int TMPP = int(mBatchWidth * (mDispH - BatchPoint_CurNo));

                /* �߶� */
                memcpy(&mProfileData[BatchPoint_CurNo * mBatchWidth], TmpBatchPoint, sizeof(int) * TMPP);

                /* �Ҷ� */
                memcpy(&mIntensityData[BatchPoint_CurNo * mBatchWidth], TmpGrayData, TMPP);

                /* ������ */
                memcpy(&mEncoderData[BatchPoint_CurNo * mEncoderN], Encoder,
                        sizeof(unsigned int) * (mDispH - BatchPoint_CurNo) * mEncoderN);

                //��ʾ mDispH - BatchPoint_CurNo ������
                DataShow();

                //�����������������
                int TMPPT = int(BatchPoint_CurNo + m_curBatchPoint - mDispH);
                int TTTT = TMPPT * mBatchWidth;
                int Tmpx = (mDispH - TMPPT) * mBatchWidth;

                /* �߶� */
                memcpy(&mProfileData[0], &mProfileData[TTTT], sizeof(int) * Tmpx);
                memcpy(&mProfileData[Tmpx], &TmpBatchPoint[TMPP], sizeof(int) * TTTT);

                /* �Ҷ� */
                memcpy(&mIntensityData[0], &mIntensityData[TTTT], Tmpx);
                memcpy(&mIntensityData[Tmpx], &TmpGrayData[TMPP], TTTT);

                /* ������ */
                memcpy(&mEncoderData[0], &mEncoderData[TMPPT * mEncoderN],
                        (mDispH - TMPPT) * mEncoderN * sizeof(unsigned int));
                memcpy(&mEncoderData[(mDispH - TMPPT) * mEncoderN],
                        &Encoder[(mDispH - BatchPoint_CurNo) * mEncoderN],
                        TMPPT * mEncoderN * sizeof(unsigned int));

                //��ʾ
                DataShow();

                BatchPoint_CurNo += m_curBatchPoint;
                m_BatchPoint_CurNo = BatchPoint_CurNo;

				//��ʾ��ǰ���յ�������
				strTotalC.Format(_T("��ǰ������:%d"), m_BatchPoint_CurNo);
				setTextInfo(strTotalC);
                continue;
            }

            /* �߶� */
            memcpy(&mProfileData[BatchPoint_CurNo * mBatchWidth], TmpBatchPoint, sizeof(int) * TmpN);

            /* �Ҷ� */
            memcpy(&mIntensityData[BatchPoint_CurNo * mBatchWidth], TmpGrayData, TmpN);

            /* ������ */
            memcpy(&mEncoderData[BatchPoint_CurNo * mEncoderN], Encoder,
                    m_curBatchPoint * mEncoderN * sizeof(unsigned int));
        }
        else
        {
            /* �������� */
            int TTTT = (mDispH - m_curBatchPoint) * mBatchWidth;
            memcpy(&mProfileData[0], &mProfileData[TmpN], sizeof(int) * TTTT);
            memcpy(&mProfileData[TTTT], TmpBatchPoint, sizeof(int) * TmpN);

            /* �Ҷ� */
            memcpy(&mIntensityData[0], &mIntensityData[TmpN], TTTT);
            memcpy(&mIntensityData[TTTT], TmpGrayData, TmpN);

            /* ������ */
            memcpy(&mEncoderData[0], &mEncoderData[m_curBatchPoint * mEncoderN],
                    sizeof(unsigned int) * (mDispH - m_curBatchPoint));
            memcpy(&mIntensityData[(mDispH - m_curBatchPoint) * mEncoderN], Encoder,
                    sizeof(unsigned int) * m_curBatchPoint * mEncoderN);
        }

        //��ʾ
        DataShow();

        BatchPoint_CurNo += m_curBatchPoint;
        m_BatchPoint_CurNo = BatchPoint_CurNo;

		//��ʾ��ǰ���յ�������
		strTotalC.Format(_T("��ǰ������:%d"), m_BatchPoint_CurNo);
        setTextInfo(strTotalC);

        //������Ҫ�Ӹ��ӳ���ʾ
        Sleep(mDelayTime);
    }while(bReceiveData);

    if(bError)  //�������ѭ���˳�
    {
        //ֹͣ������
        SR7IF_StopMeasure(DEVICEID);
    }

    /* �ڴ��ͷ� */
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
    //���ݻ�������
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
		AfxMessageBox(_T("�����ڴ�����ʧ��!"), MB_OK | MB_ICONQUESTION);
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

        //�ͷ�֮ǰ���ڴ�
        deleteDataMemory();

        //���������ڴ�
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

    //��ȡ��׺
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

		/* ����һ������������ʵ��д����ַ��� */
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

		/* ����һ������������ʵ��д����ַ��� */
		DWORD realWriteSize;
		WriteFile(hFile, (char *)&mBatchInfo, sizeof(BATCH_INFO), &realWriteSize, NULL);
		WriteFile(hFile, (char *)mProfileData, sizeof(int) * mDispH * mBatchWidth, &realWriteSize, NULL);

		//д���������Ϣ
		if(mEncoderData)
		{
			int MAXECODER = 1;
			if(bCamBonline)
			{
				MAXECODER = 2;
			}
			WriteFile(hFile, (char *)mEncoderData, sizeof(unsigned int) * mDispH * MAXECODER, &realWriteSize, NULL);
		}

		CloseHandle(hFile); /*�ر��ļ� �رվ����*/
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

    //��ȡ��׺
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

		/* ����һ������������ʵ��д����ַ��� */
		DWORD realWriteSize;
		WriteFile(hFile, (char *)mIntensityData, sizeof(unsigned char) * mDispH * mBatchWidth, &realWriteSize, NULL);
		CloseHandle(hFile); /*�ر��ļ� �رվ����*/
    }
    else if(strA == ".bmp")
    {
		BITMAPFILEHEADER targetfileheader;
        BITMAPINFOHEADER targetinfoheader;
        memset(&targetfileheader, 0, sizeof(BITMAPFILEHEADER));
        memset(&targetinfoheader, 0, sizeof(BITMAPINFOHEADER));

        /* ����Ҷ�ͼ���ļ�ͷ */
        targetfileheader.bfOffBits = (DWORD)sizeof(BITMAPFILEHEADER) + (DWORD)sizeof(BITMAPINFOHEADER) + sizeof(RGBQUAD) * 256;
        targetfileheader.bfSize = 192 * 192 + sizeof(RGBQUAD) * 256 + sizeof(BITMAPFILEHEADER) + sizeof(BITMAPINFOHEADER);
        targetfileheader.bfReserved1 = 0;
        targetfileheader.bfReserved2 = 0;
        targetfileheader.bfType = 0x4d42;

        /* ����Ҷ�ͼ����Ϣͷ */
        targetinfoheader.biBitCount = 8;          // ÿ�����������λ����������1(˫ɫ), 4(16ɫ)��8(256ɫ)��24(���ɫ)֮һ
        targetinfoheader.biSize = sizeof(BITMAPINFOHEADER);
        targetinfoheader.biHeight = mDispH;
        targetinfoheader.biWidth = mBatchWidth;
        targetinfoheader.biPlanes = 1;            // Ŀ���豸�ļ��𣬱���Ϊ1 
        targetinfoheader.biCompression = BI_RGB;  // λͼѹ�����ͣ������� 0(��ѹ��), 1(BI_RLE8ѹ������)��2(BI_RLE4ѹ������)֮һ 
        targetinfoheader.biSizeImage = 0;         // λͼ�Ĵ�С�����ֽ�Ϊ��λ  
        targetinfoheader.biXPelsPerMeter = 0;     // λͼˮƽ�ֱ��ʣ�ÿ��������  
        targetinfoheader.biYPelsPerMeter = 0;     // λͼ��ֱ�ֱ��ʣ�ÿ��������  
        targetinfoheader.biClrImportant = 0;      // λͼ��ʾ��������Ҫ����ɫ��  
        targetinfoheader.biClrUsed = 0;           // λͼʵ��ʹ�õ���ɫ���е���ɫ��  

        /* ����Ҷ�ͼ�ĵ�ɫ�� */
        RGBQUAD rgbquad[256];
        int i;
        for (i = 0; i<256; i++)
        {
            rgbquad[i].rgbBlue = i;
            rgbquad[i].rgbGreen = i;
            rgbquad[i].rgbRed = i;
            rgbquad[i].rgbReserved = 0;
        }

        /* ����BMPͼ��������ǵ��õģ���ͼ����ʾ�ĵ�һ�������һ�����ݣ�����Ҫ���� */
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

        /* д���ļ� */
        CFile cf;
        if (!cf.Open(_path, CFile::modeCreate | CFile::modeWrite))
        {
            delete[] targetbuf;
            return -1;
        }
        cf.Write(&targetfileheader, sizeof(BITMAPFILEHEADER));
        cf.Write(&targetinfoheader, sizeof(BITMAPINFOHEADER));
        cf.Write(&rgbquad, sizeof(RGBQUAD) * 256);
        cf.Write(targetbuf, mDispH * mBatchWidth); //����targetbuf�Ĵ�СΪ192x192
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

    //��ȡ��׺
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

            /* �Զ��Ÿ���д�� */
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

		/* ����һ������������ʵ��д����ַ��� */
		DWORD realWriteSize;
		WriteFile(hFile, (char *)mEncoderData, sizeof(unsigned int) * mDispH * MAXECODER, &realWriteSize, NULL);
		CloseHandle(hFile); /*�ر��ļ� �رվ����*/
    }
    else
    {
        return -1;
    }

    return 0;
}
