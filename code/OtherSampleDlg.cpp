// E:\Work\VS\SR_SDK_Demo\SR_SDK_Demo\OtherSampleDialog\OtherSampleDlg.cpp : implementation file
//


#include "../pch.h"
#include "../Detec3Dsr.h"
#include "../SR_SdkDll/SR7Link.h"
#include "OtherSampleDlg.h"


// OtherSampleDlg dialog

IMPLEMENT_DYNAMIC(OtherSampleDlg, CDialog)

OtherSampleDlg::OtherSampleDlg(CWnd* pParent /*=NULL*/)
	: CDialog(OtherSampleDlg::IDD, pParent)
{
	bConnectFlag = false;
    DispRange_Height = 8.4;
}

OtherSampleDlg::~OtherSampleDlg()
{
}

void OtherSampleDlg::DoDataExchange(CDataExchange* pDX)
{
	CDialog::DoDataExchange(pDX);
}


BEGIN_MESSAGE_MAP(OtherSampleDlg, CDialog)
	ON_BN_CLICKED(IDC_BTN_GetProfile, &OtherSampleDlg::OnBnClickedGetProfile)
	ON_BN_CLICKED(IDC_BTN_ProgSwitch, &OtherSampleDlg::OnBnClickedProgSw)
	ON_BN_CLICKED(IDC_BTN_DllVersion, &OtherSampleDlg::OnBnClickedGetVersion)
	ON_BN_CLICKED(IDC_BTN_HeadModel, &OtherSampleDlg::OnBnClickedGetModel)
	ON_BN_CLICKED(IDC_BTN_BOnline, &OtherSampleDlg::OnBnClickedCamBOnline)
	ON_BN_CLICKED(IDC_BTN_HeadSerial, &OtherSampleDlg::OnBnClickedGetHeadS)
	ON_BN_CLICKED(IDC_BTN_Day, &OtherSampleDlg::OnBnClickedRemainDay)
	ON_BN_CLICKED(IDC_BTN_Output, &OtherSampleDlg::OnBnClickedIoOutput)
	ON_BN_CLICKED(IDC_BTN_IoRead, &OtherSampleDlg::OnBnClickedIoRead)
	ON_BN_CLICKED(IDC_BTN_Error, &OtherSampleDlg::OnBnClickedGetError)
	ON_BN_CLICKED(IDC_BTN_ErrorClear, &OtherSampleDlg::OnBnClickedClearError)
	ON_BN_CLICKED(IDC_BTN_InfoCleaer, &OtherSampleDlg::OnBnClickedClear)
	ON_BN_CLICKED(IDC_BTN_Encoder, &OtherSampleDlg::OnBnClickedGetEncoderV)
	ON_BN_CLICKED(IDC_BTN_GetTemp, &OtherSampleDlg::OnBnClickedBtnGettemp)
END_MESSAGE_MAP()


// OtherSampleDlg message handlers


BOOL OtherSampleDlg::OnInitDialog()
{
	CDialog::OnInitDialog();

	setUIInit();

	
	return TRUE;  // return TRUE  unless you set the focus to a control
}

void OtherSampleDlg::setUIInit()
{
	CString strprg;
	for(int i = 0; i < 10; i ++)
	{
		strprg.Format(_T("%d"), i);
		((CComboBox*)GetDlgItem(IDC_COMBO_PRGSWITCH))->AddString(strprg);
	}
	((CComboBox*)GetDlgItem(IDC_COMBO_PRGSWITCH))->SetCurSel(0);

	CString strio;
	for(int i = 0; i < 8; i ++)
	{
		strio.Format(_T("%d"), i);
		((CComboBox*)GetDlgItem(IDC_COMBO_Port))->AddString(strio);
	}
	((CComboBox*)GetDlgItem(IDC_COMBO_Port))->SetCurSel(0);

	for(int i = 0; i < 2; i ++)
	{
		strio.Format(_T("%d"), i);
		((CComboBox*)GetDlgItem(IDC_COMBO_Output))->AddString(strio);
	}
	((CComboBox*)GetDlgItem(IDC_COMBO_Output))->SetCurSel(0);

	CButton* Radio_A = (CButton*)GetDlgItem(IDC_RADIO_CamA);
	CButton* Radio_B = (CButton*)GetDlgItem(IDC_RADIO_CamB);
	Radio_A->SetCheck(1);
	Radio_B->SetCheck(0);
}

void OtherSampleDlg::SingleProfileDataDisplay(int* _data, int _width)
{
	CRect _rect;
    CDC* pDc;
    CStatic* pStc = (CStatic*)GetDlgItem(IDC_STATIC_IMAGE);   //图像控件 
    if(pStc == NULL)
        return;
    pStc->GetClientRect(&_rect);
    pDc = pStc->GetDC();

    CBrush newBrush;
    CBrush *pOldBrush;
    newBrush.CreateSolidBrush(RGB(0, 0, 0));     			 // 创建黑色新画刷   
    pOldBrush = pDc->SelectObject(&newBrush);    			 // 选择新画刷，并将旧画刷的指针保存到pOldBrush   
    pDc->Rectangle(_rect);                       			 // 以黑色画刷为绘图控件填充黑色，形成黑色背景 
    pDc->SelectObject(pOldBrush);                			 // 恢复旧画刷   
    newBrush.DeleteObject();                     			 // 删除新画刷  
	COLORREF point_color = RGB(255, 0, 0);                   // 设定颜色

	//高度显示比例计算---显示范围与图像控件高度
    double yScale = double(_rect.Height() * 0.5) / DispRange_Height;
    double xScale = double(_rect.Width()) / _width;

	POINTF temp_point;
    for(int i = 0; i < _width; i ++ )
    {
        temp_point.x = float(i * xScale + _rect.left);
        temp_point.y = float(_data[i] * 0.00001);

        if(temp_point.y > (-10000))
            pDc->SetPixel((int)temp_point.x, int(_rect.Height() * 0.5 - temp_point.y * yScale), point_color);
    }
}

void OtherSampleDlg::OnBnClickedGetProfile()
{
	if(!bConnectFlag)
    {
		((CEdit*)GetDlgItem(IDC_EDITINFO))->SetWindowTextW(_T("相机未连接"));
        return;
    }

	/* 获取轮廓宽度 --- 用于申请存放轮廓数据内存 */
    int mProfileWidth = SR7IF_ProfileDataWidth(DEVICEID, NULL);
	CString strInfo;
	strInfo.Format(_T("轮廓宽度:%d"), mProfileWidth);
    strInfo += _T("\r\n");

    int* pProfileData = new int[mProfileWidth];     //轮廓缓存
    for(int i = 0; i < mProfileWidth; i ++)
        pProfileData[i] = -1000000000;

    unsigned int pEncoder = 0;                      //编码器值

    /***获取当前一条轮廓（非批处理下,需在EdgeImaging中设置为2.5D模式）****/
    int reT = SR7IF_GetSingleProfile(DEVICEID, pProfileData, &pEncoder);
    if(reT < 0)
    {
        strInfo += _T("SR7IF_GetSingleProfile 失败");
		strInfo += _T("\r\n");

		CString strTmp;
		strTmp.Format(_T("返回值:%d"), reT);
		strInfo += strTmp;
		((CEdit*)GetDlgItem(IDC_EDITINFO))->SetWindowTextW(strInfo);

        if(pProfileData)
        {
            delete[] pProfileData;
            pProfileData = NULL;
        }

        return;
    }

    //转为图像显示
    SingleProfileDataDisplay(pProfileData, mProfileWidth);

    if(pProfileData)
    {
        delete[] pProfileData;
        pProfileData = NULL;
    }

    strInfo += _T("SR7IF_GetSingleProfile 成功");
    ((CEdit*)GetDlgItem(IDC_EDITINFO))->SetWindowTextW(strInfo);
}

void OtherSampleDlg::OnBnClickedProgSw()
{
	if(!bConnectFlag)
    {
		((CEdit*)GetDlgItem(IDC_EDITINFO))->SetWindowTextW(_T("相机未连接"));
        return;
    }

	int mProgNo = ((CComboBox*)GetDlgItem(IDC_COMBO_PRGSWITCH))->GetCurSel();
    int reT = SR7IF_SwitchProgram(DEVICEID, mProgNo);
    if(reT < 0)
    {
		CString strInfo;
		strInfo.Format(_T("SR7IF_SwitchProgram 失败，返回值:%d"), reT);
		((CEdit*)GetDlgItem(IDC_EDITINFO))->SetWindowTextW(strInfo);
    }
    else
    {
		((CEdit*)GetDlgItem(IDC_EDITINFO))->SetWindowTextW(_T("SR7IF_SwitchProgram 成功"));
    }
}

void OtherSampleDlg::OnBnClickedGetVersion()
{
	if(!bConnectFlag)
    {
		((CEdit*)GetDlgItem(IDC_EDITINFO))->SetWindowTextW(_T("相机未连接"));
        return;
    }

	const char* _version = SR7IF_GetVersion();
    if(_version == NULL)
    {
		((CEdit*)GetDlgItem(IDC_EDITINFO))->SetWindowTextW(_T("SR7IF_GetVersion 失败"));
    }
    else
    {
		CString strInfo = _T("");
		strInfo = _version;
		((CStatic*)GetDlgItem(IDC_STATIC_Version))->SetWindowTextW(strInfo);
		((CEdit*)GetDlgItem(IDC_EDITINFO))->SetWindowTextW(_T("SR7IF_GetVersion 成功"));
    }
}

void OtherSampleDlg::OnBnClickedGetModel()
{
	if(!bConnectFlag)
    {
		((CEdit*)GetDlgItem(IDC_EDITINFO))->SetWindowTextW(_T("相机未连接"));
        return;
    }

	const char* strModel = SR7IF_GetModels(DEVICEID);   //型号获取
    if(strModel == NULL)
    {
		((CEdit*)GetDlgItem(IDC_EDITINFO))->SetWindowTextW(_T("SR7IF_GetModels 失败"));
    }
    else
    {
		CString strInfo = _T("");
		strInfo = strModel;
		((CStatic*)GetDlgItem(IDC_STATIC_Head))->SetWindowTextW(strInfo);
		((CEdit*)GetDlgItem(IDC_EDITINFO))->SetWindowTextW(_T("SR7IF_GetModels 成功"));
    }
} 

void OtherSampleDlg::OnBnClickedCamBOnline()
{
	if(!bConnectFlag)
    {
		((CEdit*)GetDlgItem(IDC_EDITINFO))->SetWindowTextW(_T("相机未连接"));
        return;
    }

	int reT = SR7IF_GetOnlineCameraB(DEVICEID);
    if(reT < 0)
    {
        if(reT == -982)
        {
			((CStatic*)GetDlgItem(IDC_STATIC_BOnline))->SetWindowTextW(_T("false"));
			((CEdit*)GetDlgItem(IDC_EDITINFO))->SetWindowTextW(_T("SR7IF_GetOnlineCameraB 成功"));
        }
        else
        {
			CString strInfo;
			strInfo.Format(_T("SR7IF_GetOnlineCameraB 失败，返回值:%d"), reT);
			((CEdit*)GetDlgItem(IDC_EDITINFO))->SetWindowTextW(strInfo);
        }
    }
    else
    {
		((CStatic*)GetDlgItem(IDC_STATIC_BOnline))->SetWindowTextW(_T("true"));
		((CEdit*)GetDlgItem(IDC_EDITINFO))->SetWindowTextW(_T("SR7IF_GetOnlineCameraB 成功"));
    }
}

void OtherSampleDlg::OnBnClickedGetHeadS()
{
	if(!bConnectFlag)
    {
		((CEdit*)GetDlgItem(IDC_EDITINFO))->SetWindowTextW(_T("相机未连接"));
        return;
    }

	CButton* Radio_A = (CButton*)GetDlgItem(IDC_RADIO_CamA);
	CButton* Radio_B = (CButton*)GetDlgItem(IDC_RADIO_CamB);
	int Head = 0;
    if(Radio_A->GetCheck())
        Head = 0;
    if(Radio_B->GetCheck())
        Head = 1;
    const char * _headSeris = SR7IF_GetHeaderSerial(DEVICEID, Head);
    if(_headSeris == NULL)
    {
        CString strInfo = _T("相机A不存在或者参数错误");
        if(Head == 1)
            strInfo = _T("相机B不存在或者参数错误");
		strInfo += _T("\r\n");
		strInfo += _T("SR7IF_GetHeaderSerial 失败");
		((CEdit*)GetDlgItem(IDC_EDITINFO))->SetWindowTextW(strInfo);
    }
    else
    {
		CString strInfo = _T("");
		strInfo = _headSeris;
		((CStatic*)GetDlgItem(IDC_STATIC_Serial))->SetWindowTextW(strInfo);
		((CEdit*)GetDlgItem(IDC_EDITINFO))->SetWindowTextW(_T("SR7IF_GetHeaderSerial 成功"));
    }
}


void OtherSampleDlg::OnBnClickedBtnGettemp()
{
	// TODO: Add your control notification handler code here
	if(!bConnectFlag)
    {
		((CEdit*)GetDlgItem(IDC_EDITINFO))->SetWindowTextW(_T("相机未连接"));
        return;
    }

	CButton* Radio_A = (CButton*)GetDlgItem(IDC_RADIO_CamA);
	CButton* Radio_B = (CButton*)GetDlgItem(IDC_RADIO_CamB);
	int Head = 0;
    if(Radio_A->GetCheck())
        Head = 0;
    if(Radio_B->GetCheck())
        Head = 1;

	unsigned int mTmpA = 0;
	unsigned int mTmpB = 0;

	if(Head == 0)
	{

		int reT = SR7IF_GetCameraTemperature(DEVICEID, &mTmpA, &mTmpB);
		if(reT < 0)
		{
			CString strInfo;
			strInfo.Format(_T("SR7IF_GetCameraTemperature 失败，返回值:%d"), reT);
			((CEdit*)GetDlgItem(IDC_EDITINFO))->SetWindowTextW(strInfo);
			((CStatic*)GetDlgItem(IDC_STATIC_Temp))->SetWindowTextW(_T("---"));
		}
		else
		{
			CString strInfo;
			strInfo.Format(_T("%.2f"), mTmpA * 0.01);
			((CStatic*)GetDlgItem(IDC_STATIC_Temp))->SetWindowTextW(strInfo);
			((CEdit*)GetDlgItem(IDC_EDITINFO))->SetWindowTextW(_T("SR7IF_GetCameraTemperature 成功"));
		}
	}

	if(Head == 1)
	{
		unsigned int mTmpB = 0;
		int reT = SR7IF_GetCameraTemperature(DEVICEID, &mTmpA, &mTmpB);
		if(reT < 0)
		{
			CString strInfo;
			strInfo.Format(_T("SR7IF_GetCameraTemperature 失败，返回值:%d"), reT);
			((CEdit*)GetDlgItem(IDC_EDITINFO))->SetWindowTextW(strInfo);
			((CStatic*)GetDlgItem(IDC_STATIC_Temp))->SetWindowTextW(_T("---"));
		}
		else
		{
			CString strInfo;
			strInfo.Format(_T("%.2f"), mTmpB * 0.01);
			((CStatic*)GetDlgItem(IDC_STATIC_Temp))->SetWindowTextW(strInfo);
			((CEdit*)GetDlgItem(IDC_EDITINFO))->SetWindowTextW(_T("SR7IF_GetCameraTemperature 成功"));
		}
	}
}


void OtherSampleDlg::OnBnClickedRemainDay()
{
	if(!bConnectFlag)
    {
		((CEdit*)GetDlgItem(IDC_EDITINFO))->SetWindowTextW(_T("相机未连接"));
        return;
    }

	unsigned short RemainDay = 0;
    int reT = SR7IF_GetLicenseKey(DEVICEID,  &RemainDay);
    if(reT < 0)
    {
        CString strInfo = _T("SR7IF_GetLicenseKey 失败");
		strInfo += _T("\r\n");
		strInfo += _T("参数错误或产品未注册");
		((CEdit*)GetDlgItem(IDC_EDITINFO))->SetWindowTextW(strInfo);
    }
    else
    {
		CString strInfo;
		strInfo.Format(_T("%d"), RemainDay);
		((CStatic*)GetDlgItem(IDC_STATIC_Day))->SetWindowTextW(strInfo);
		((CEdit*)GetDlgItem(IDC_EDITINFO))->SetWindowTextW(_T("SR7IF_GetLicenseKey 成功"));
    }
}

void OtherSampleDlg::OnBnClickedIoOutput()
{
	if(!bConnectFlag)
    {
		((CEdit*)GetDlgItem(IDC_EDITINFO))->SetWindowTextW(_T("相机未连接"));
        return;
    }

	int uiPort = ((CComboBox*)GetDlgItem(IDC_COMBO_Port))->GetCurSel();
    bool bLevel = true;
    if(((CComboBox*)GetDlgItem(IDC_COMBO_Output))->GetCurSel() == 0)
    {
        bLevel = false;
    }
    else if(((CComboBox*)GetDlgItem(IDC_COMBO_Output))->GetCurSel() == 1)
    {
        bLevel = true;
    }
    int reT = SR7IF_SetOutputPortLevel(DEVICEID, uiPort, bLevel);
    if(reT < 0)
    {
		CString strInfo;
		strInfo.Format(_T("SR7IF_SetOutputPortLevel 失败，返回值:%d"), reT);
		((CEdit*)GetDlgItem(IDC_EDITINFO))->SetWindowTextW(strInfo);
    }
    else
    {
		((CEdit*)GetDlgItem(IDC_EDITINFO))->SetWindowTextW(_T("SR7IF_SetOutputPortLevel 成功"));
    }
} 

void OtherSampleDlg::OnBnClickedIoRead()
{
	if(!bConnectFlag)
    {
		((CEdit*)GetDlgItem(IDC_EDITINFO))->SetWindowTextW(_T("相机未连接"));
        return;
    }

	int uiPort = ((CComboBox*)GetDlgItem(IDC_COMBO_Port))->GetCurSel();
    bool bLevel = true;
    int reT = SR7IF_GetInputPortLevel(DEVICEID, uiPort, &bLevel);
    if(reT < 0)
    {
		CString strInfo;
		strInfo.Format(_T("SR7IF_GetInputPortLevel 失败，返回值:%d"), reT);
		((CEdit*)GetDlgItem(IDC_EDITINFO))->SetWindowTextW(strInfo);
    }
    else
    {
		((CEdit*)GetDlgItem(IDC_EDITINFO))->SetWindowTextW(_T("SR7IF_GetInputPortLevel 成功"));
        if(bLevel)
        {
			((CStatic*)GetDlgItem(IDC_STATIC_IOLevel))->SetWindowTextW(_T("1"));
        }
        else
        {
			((CStatic*)GetDlgItem(IDC_STATIC_IOLevel))->SetWindowTextW(_T("0"));
        }
    }
} 

void OtherSampleDlg::OnBnClickedGetError()
{
	if(!bConnectFlag)
    {
		((CEdit*)GetDlgItem(IDC_EDITINFO))->SetWindowTextW(_T("相机未连接"));
        return;
    }

	int pbyErrCnt = 0;
    int pwErrCode[2048];
    memset(pwErrCode, 0, sizeof(pwErrCode));
    int reT = SR7IF_GetError(DEVICEID, &pbyErrCnt, pwErrCode);
    if(reT < 0)
    {
		CString strInfo;
		strInfo.Format(_T("SR7IF_GetError 失败，返回值:%d"), reT);
		((CEdit*)GetDlgItem(IDC_EDITINFO))->SetWindowTextW(strInfo);
    }
    else
    {
		CString strEC;
		strEC.Format(_T("错误码数量:%d"), pbyErrCnt);
        CString strEcoder = _T("");
		CString strTmp = _T(""); 
        for(int i = 0; i < pbyErrCnt; i ++)
        {
			strTmp.Format(_T("%x"),pwErrCode[i]);
            strEcoder += _T("0x");
			strEcoder += strTmp;
            strEcoder += _T("\r\n");
        }

		CString strInfo = _T("");
		strInfo += _T("SR7IF_GetError 成功");
		strInfo += _T("\r\n");
		strInfo += strEC;
		strInfo += _T("\r\n");
		strInfo += strEcoder;
        ((CEdit*)GetDlgItem(IDC_EDITINFO))->SetWindowTextW(strInfo);
    }
} 

void OtherSampleDlg::OnBnClickedClearError()
{
	if(!bConnectFlag)
    {
		((CEdit*)GetDlgItem(IDC_EDITINFO))->SetWindowTextW(_T("相机未连接"));
        return;
    }

	CString strCode;
	((CEdit*)GetDlgItem(IDC_EDIT_ErrorCode))->GetWindowTextW(strCode);
	TCHAR* pszEnd = NULL;
	int wErrCode = (int)( ::_tcstol(strCode, &pszEnd, 16) );
    int reT = SR7IF_ClearError(DEVICEID, wErrCode);
    if(reT < 0)
    {
		CString strInfo;
		strInfo.Format(_T("SR7IF_ClearError 失败，返回值:%d"), reT);
		((CEdit*)GetDlgItem(IDC_EDITINFO))->SetWindowTextW(strInfo);
    }
    else
    {
		((CEdit*)GetDlgItem(IDC_EDITINFO))->SetWindowTextW(_T("SR7IF_ClearError 成功"));
    }
}

void OtherSampleDlg::OnBnClickedClear()
{
	((CEdit*)GetDlgItem(IDC_EDITINFO))->SetWindowTextW(_T(""));
}

void OtherSampleDlg::OnBnClickedGetEncoderV()
{
	if(!bConnectFlag)
    {
		((CEdit*)GetDlgItem(IDC_EDITINFO))->SetWindowTextW(_T("相机未连接"));
        return;
    }

	unsigned int value = 0;
    int reT = SR7IF_GetCurrentEncoder(DEVICEID, &value);
    if(reT < 0)
    {
		CString strInfo;
		strInfo.Format(_T("SR7IF_GetCurrentEncoder 失败，返回值:%d"), reT);
		((CEdit*)GetDlgItem(IDC_EDITINFO))->SetWindowTextW(strInfo);
    }
    else
    {
		CString strInfo;
		strInfo.Format(_T("%u"), value);
		((CStatic*)GetDlgItem(IDC_STATIC_Encoder))->SetWindowTextW(strInfo);
		((CEdit*)GetDlgItem(IDC_EDITINFO))->SetWindowTextW(_T("SR7IF_GetCurrentEncoder 成功"));
    }
}
