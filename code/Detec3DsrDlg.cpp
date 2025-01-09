
// Detec3DsrDlg.cpp: implementation file
//


#include "pch.h"
#include "framework.h"
#include "Detec3Dsr.h"
#include "Detec3DsrDlg.h"
#include "afxdialogex.h"

//Add by peter 20231001
//#include "stdafx.h"
#include "SettingDialog/SettingDialog.h"
#include "3DShowDll/SR3dexe.h"
#include <fstream>
#include <string>
#include <iostream>
#include <sstream>
#include <vector>
//Add by wcy 20231116
//添加pcl显示的头文件



#if defined _M_IX86
#pragma comment(lib, "SR_SdkDll/x86/SR7Link.lib")
#pragma comment(lib, "3DShowDll/x86/SR3dexe.lib")
#endif

#if defined _M_X64
#pragma comment(lib, "SR_SdkDll/x64/SR7Link.lib")
#pragma comment(lib, "3DShowDll/x64/SR3dexe.lib")
#endif

#ifdef _DEBUG
#define new DEBUG_NEW
#endif

//Add by peter 20231001
using namespace std;
//Add by wcy 20231116

// CAboutDlg dialog used for App About

class CAboutDlg : public CDialogEx
{
public:
	CAboutDlg();

// Dialog Data
#ifdef AFX_DESIGN_TIME
	enum { IDD = IDD_ABOUTBOX };
#endif

	protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV support

// Implementation
protected:
	DECLARE_MESSAGE_MAP()
};

CAboutDlg::CAboutDlg() : CDialogEx(IDD_ABOUTBOX)
{
}

void CAboutDlg::DoDataExchange(CDataExchange* pDX)
{
	CDialogEx::DoDataExchange(pDX);
}

BEGIN_MESSAGE_MAP(CAboutDlg, CDialogEx)
END_MESSAGE_MAP()


// CDetec3DsrDlg dialog



CDetec3DsrDlg::CDetec3DsrDlg(CWnd* pParent /*=nullptr*/)
	: CDialog(IDD_DETEC3DSR_DIALOG, pParent)
{
	m_hIcon = AfxGetApp()->LoadIcon(IDR_MAINFRAME);
}

void CDetec3DsrDlg::DoDataExchange(CDataExchange* pDX)
{
	CDialog::DoDataExchange(pDX);
}

BEGIN_MESSAGE_MAP(CDetec3DsrDlg, CDialog)
	ON_WM_SYSCOMMAND()
	ON_WM_PAINT()
	ON_WM_QUERYDRAGICON()
	ON_BN_CLICKED(IDC_BTN_Connect, &CDetec3DsrDlg::OnBnClickedBtnConnect)
	ON_BN_CLICKED(IDC_BTN_DisConnect, &CDetec3DsrDlg::OnBnClickedBtnDisconnect)
	ON_BN_CLICKED(IDC_BTN_StartM, &CDetec3DsrDlg::OnBnClickedBtnStartm)
	ON_BN_CLICKED(IDC_BTN_StopM, &CDetec3DsrDlg::OnBnClickedBtnStopm)
	ON_BN_CLICKED(IDC_BTN_Clear, &CDetec3DsrDlg::OnBnClickedBtnClear)
	ON_BN_CLICKED(IDC_BTN_GrayS, &CDetec3DsrDlg::OnBnClickedBtnGrays)
	ON_BN_CLICKED(IDC_BTN_HeightS, &CDetec3DsrDlg::OnBnClickedBtnHeights)
	ON_BN_CLICKED(IDC_BTN_EncoderS, &CDetec3DsrDlg::OnBnClickedBtnEncoders)
	ON_BN_CLICKED(IDC_BTN_SetSet, &CDetec3DsrDlg::OnBnClickedBtnSetset)
	ON_BN_CLICKED(IDC_BTN_GetSet, &CDetec3DsrDlg::OnBnClickedBtnGetset)
	ON_BN_CLICKED(IDC_BTN_Load, &CDetec3DsrDlg::OnBnClickedBtnLoad)
	ON_BN_CLICKED(IDC_BTN_Export, &CDetec3DsrDlg::OnBnClickedBtnExport)
	ON_BN_CLICKED(IDC_BTN_Other, &CDetec3DsrDlg::OnBnClickedBtnOther)
	ON_BN_CLICKED(IDC_BTN_3DShow, &CDetec3DsrDlg::OnBnClickedBtn3dshow)
END_MESSAGE_MAP()


// CDetec3DsrDlg message handlers

BOOL CDetec3DsrDlg::OnInitDialog()
{
	CDialog::OnInitDialog();

	// Add "About..." menu item to system menu.

	// IDM_ABOUTBOX must be in the system command range.
	ASSERT((IDM_ABOUTBOX & 0xFFF0) == IDM_ABOUTBOX);
	ASSERT(IDM_ABOUTBOX < 0xF000);

	CMenu* pSysMenu = GetSystemMenu(FALSE);
	if (pSysMenu != nullptr)
	{
		BOOL bNameValid;
		CString strAboutMenu;
		bNameValid = strAboutMenu.LoadString(IDS_ABOUTBOX);
		ASSERT(bNameValid);
		if (!strAboutMenu.IsEmpty())
		{
			pSysMenu->AppendMenu(MF_SEPARATOR);
			pSysMenu->AppendMenu(MF_STRING, IDM_ABOUTBOX, strAboutMenu);
		}
	}

	// Set the icon for this dialog.  The framework does this automatically
	//  when the application's main window is not a dialog
	SetIcon(m_hIcon, TRUE);			// Set big icon
	SetIcon(m_hIcon, FALSE);		// Set small icon

	// TODO: Add extra initialization here
	//GetLastBuildTime(); // 获取编译时间，这个函数不用，peter 20231001
	VariableInit();		//参数初始化
	InterfaceCtrlInit();

	//屏蔽 peter 20231001
	/*CButton* Radio_Infi = (CButton*)GetDlgItem(IDC_InfinLoop);
	Radio_Infi->SetCheck(BST_CHECKED);*/
	CDetec3DsrDlg::OldBnClickedInfinloop();

	/*GetDlgItem(IDC_InfinLoop)->ShowWindow(SW_HIDE);
	GetDlgItem(IDC_MultiCall)->ShowWindow(SW_HIDE);
	GetDlgItem(IDC_OneCall)->ShowWindow(SW_HIDE);*/

	return TRUE;  // return TRUE  unless you set the focus to a control
}

void CDetec3DsrDlg::OnSysCommand(UINT nID, LPARAM lParam)
{
	if ((nID & 0xFFF0) == IDM_ABOUTBOX)
	{
		CAboutDlg dlgAbout;
		dlgAbout.DoModal();
	}
	else
	{
		CDialog::OnSysCommand(nID, lParam);
	}
}

// If you add a minimize button to your dialog, you will need the code below
//  to draw the icon.  For MFC applications using the document/view model,
//  this is automatically done for you by the framework.

void CDetec3DsrDlg::OnPaint()
{
	if (IsIconic())
	{
		CPaintDC dc(this); // device context for painting

		SendMessage(WM_ICONERASEBKGND, reinterpret_cast<WPARAM>(dc.GetSafeHdc()), 0);

		// Center icon in client rectangle
		int cxIcon = GetSystemMetrics(SM_CXICON);
		int cyIcon = GetSystemMetrics(SM_CYICON);
		CRect rect;
		GetClientRect(&rect);
		int x = (rect.Width() - cxIcon + 1) / 2;
		int y = (rect.Height() - cyIcon + 1) / 2;

		// Draw the icon
		dc.DrawIcon(x, y, m_hIcon);
	}
	else
	{
		CDialog::OnPaint();
	}
}

// The system calls this function to obtain the cursor to display while the user drags
//  the minimized window.
HCURSOR CDetec3DsrDlg::OnQueryDragIcon()
{
	return static_cast<HCURSOR>(m_hIcon);
}


// Add by peter 20231001 下面为OnInitDialog()里面的初始函数 ------------------------------
//参数初始化
void CDetec3DsrDlg::VariableInit()
{
	bConnectFlag = false;
	b_HighSpeedInitFail = false;
	b_BatchStartFlag = false;
	mCurDispType = 0;
	mCurCamId = 0;
	d_HeightUpper = 8.4;
	d_HeightLower = -8.4;

	otherSampleDlg = new OtherSampleDlg();
	//_callMultiTimes = NULL;
	//_callOneTimes = NULL;
	_infiniteL = NULL;
}

//界面控件初始化
void CDetec3DsrDlg::InterfaceCtrlInit()
{
	int IpAddr[4];
	GetLastIpAddress(IpAddr);
	/*((CIPAddressCtrl*)GetDlgItem(IDC_IPAddr))->SetAddress(IpAddr[0], IpAddr[1], IpAddr[2], IpAddr[3]);

	((CEdit*)GetDlgItem(IDC_CallFreq))->ShowWindow(false);
	((CStatic*)GetDlgItem(IDC_STATIC_CallFreq))->ShowWindow(false);
	((CStatic*)GetDlgItem(IDC_STATIC_CountLine))->ShowWindow(false);

	CString strTmp;
	strTmp.Format(_T("%d"), 10);
	((CEdit*)GetDlgItem(IDC_CallFreq))->SetWindowTextW(strTmp);

	strTmp.Format(_T("%.3f"), -8.4);
	((CEdit*)GetDlgItem(IDC_EDIT_Lower))->SetWindowTextW(strTmp);
	strTmp.Format(_T("%.3f"), 8.4);
	((CEdit*)GetDlgItem(IDC_EDIT_Upper))->SetWindowTextW(strTmp);

	strTmp.Format(_T("%.3f"), 1.0);
	((CEdit*)GetDlgItem(IDC_EDIT_ZScale))->SetWindowTextW(strTmp);

	strTmp.Format(_T("%.3f"), 0.1);
	((CEdit*)GetDlgItem(IDC_EDIT_YInter))->SetWindowTextW(strTmp);

	strTmp.Format(_T("%d"), 50000);
	((CEdit*)GetDlgItem(IDC_EDIT_TimeOut))->SetWindowTextW(strTmp);

	strTmp.Format(_T("%d"), 10);
	((CEdit*)GetDlgItem(IDC_EDIT_DelayT))->SetWindowTextW(strTmp);*/

	/*((CButton*)GetDlgItem(IDC_HeightI))->SetCheck(1);
	((CButton*)GetDlgItem(IDC_GrayI))->SetCheck(0);
	((CButton*)GetDlgItem(IDC_CamA))->SetCheck(1);
	((CButton*)GetDlgItem(IDC_CamB))->SetCheck(0); */

	((CButton*)GetDlgItem(IDC_SaveCamA))->SetCheck(1);	
	((CButton*)GetDlgItem(IDC_SaveCamB))->SetCheck(0);

	((CButton*)GetDlgItem(IDC_BTN_Connect))->EnableWindow(true);
	((CButton*)GetDlgItem(IDC_BTN_DisConnect))->EnableWindow(false);

	/*((CButton*)GetDlgItem(IDC_CamA))->EnableWindow(false);
	((CButton*)GetDlgItem(IDC_CamB))->EnableWindow(false); */

	((CButton*)GetDlgItem(IDC_SaveCamA))->EnableWindow(false);
	((CButton*)GetDlgItem(IDC_SaveCamB))->EnableWindow(false);

	/*((CButton*)GetDlgItem(IDC_CHECK1))->EnableWindow(false);//原界面 一次性回调模式 里 等待外部开始批处理
	((CButton*)GetDlgItem(IDC_BTN_StartM_OneCall))->EnableWindow(false);
	((CButton*)GetDlgItem(IDC_BTN_SoftTrigger))->EnableWindow(false);

	((CEdit*)GetDlgItem(IDC_EDIT_TimeOut))->EnableWindow(false);
	((CEdit*)GetDlgItem(IDC_EDIT_DelayT))->EnableWindow(false);
	((CButton*)GetDlgItem(IDC_BTN_StartM_IO))->EnableWindow(false); */
	((CButton*)GetDlgItem(IDC_BTN_StartM))->EnableWindow(false);
	((CButton*)GetDlgItem(IDC_BTN_StopM))->EnableWindow(false);
}

//按了原界面 无线循环 按钮
void CDetec3DsrDlg::OldBnClickedInfinloop()
{
	// TODO: Add your control notification handler code here
	/*((CEdit*)GetDlgItem(IDC_CallFreq))->ShowWindow(false);
	((CStatic*)GetDlgItem(IDC_STATIC_CallFreq))->ShowWindow(false);
	((CStatic*)GetDlgItem(IDC_STATIC_CountLine))->ShowWindow(true);

	((CButton*)GetDlgItem(IDC_CamA))->EnableWindow(false);
	((CButton*)GetDlgItem(IDC_CamB))->EnableWindow(false);*/

	((CButton*)GetDlgItem(IDC_SaveCamA))->EnableWindow(false);
	((CButton*)GetDlgItem(IDC_SaveCamB))->EnableWindow(false);

	/*((CButton*)GetDlgItem(IDC_CHECK1))->EnableWindow(false);//原界面 一次性回调模式 里 等待外部开始批处理
	((CButton*)GetDlgItem(IDC_BTN_StartM_OneCall))->EnableWindow(false);
	((CButton*)GetDlgItem(IDC_BTN_SoftTrigger))->EnableWindow(false);

	((CEdit*)GetDlgItem(IDC_EDIT_TimeOut))->EnableWindow(true);
	((CEdit*)GetDlgItem(IDC_EDIT_DelayT))->EnableWindow(true);
	((CButton*)GetDlgItem(IDC_BTN_StartM_IO))->EnableWindow(true); */
	((CButton*)GetDlgItem(IDC_BTN_StartM))->EnableWindow(true);

}

// Add by peter 20231001 下面为获取、保存上次连接的IP地址 ------------------------------
void CDetec3DsrDlg::GetLastIpAddress(int* ip)
{
	int IpSet[4];
	/*IpSet[0] = 192;
	IpSet[1] = 168;
	IpSet[2] = 0;
	IpSet[3] = 10;*/  //默认值 //从config.txt读取后，应该是可以屏蔽，peter 20231001

	/* 读取保存的IP地址文件 */
	FILE* inputfile;
	if ((inputfile = fopen("IpSet.bin", "r")) == NULL)  //读取方案文件
	{
		ip[0] = IpSet[0];
		ip[1] = IpSet[1];
		ip[2] = IpSet[2];
		ip[3] = IpSet[3];
		return;
	}
	int addr_d = 0;
	fscanf(inputfile, "%d,", &addr_d);
	ip[0] = addr_d;
	fscanf(inputfile, "%d,", &addr_d);
	ip[1] = addr_d;
	fscanf(inputfile, "%d,", &addr_d);
	ip[2] = addr_d;
	fscanf(inputfile, "%d,", &addr_d);
	ip[3] = addr_d;
}

void CDetec3DsrDlg::saveIpAddress(SR7IF_ETHERNET_CONFIG _SREthernetConFig)
{
	/* 保存当前IP */
	std::fstream file("IpSet.bin", std::ios::out);
	file << (int)_SREthernetConFig.abyIpAddress[0]
		<< ","
		<< (int)_SREthernetConFig.abyIpAddress[1]
		<< ","
		<< (int)_SREthernetConFig.abyIpAddress[2]
		<< ","
		<< (int)_SREthernetConFig.abyIpAddress[3]
		<< "," << "\n";
	file.close();
}

//获取3D相机的型号，以便知道测量范围。peter 20231001
void CDetec3DsrDlg::getHeightUpperLower(double& _upper, double& _lower)
{
	const char* _version = SR7IF_GetModels(DEVICEID);   //型号获取
	CString str_Version;
	str_Version = _version;

	double m_dHeightRange = 1500;
	if (str_Version == "SR7050" || str_Version == "SR7060D")
	{
		m_dHeightRange = 3;
	}
	else if (str_Version == "SR7080")
	{
		m_dHeightRange = 9;
	}
	else if (str_Version == "SR7140")
	{
		m_dHeightRange = 15;
	}
	else if (str_Version == "SR7240")
	{
		m_dHeightRange = 24;
	}
	else if (str_Version == "SR7400")
	{
		m_dHeightRange = 60;
	}
	else if (str_Version == "SR7300")
	{
		m_dHeightRange = 150;
	}
	else if (str_Version == "SR6060")
	{
		m_dHeightRange = 15;
	}
	else if (str_Version == "SR6030")
	{
		m_dHeightRange = 8;
	}
	else if (str_Version == "SR6070")
	{
		m_dHeightRange = 16;
	}
	else if (str_Version == "SR6071")
	{
		m_dHeightRange = 40;
	}
	else if (str_Version == "SR6130")
	{
		m_dHeightRange = 100;
	}
	else if (str_Version == "SR6260")
	{
		m_dHeightRange = 240;
	}
	else if (str_Version == "SR8020")
	{
		m_dHeightRange = 6;
	}
	else if (str_Version == "SR8060")
	{
		m_dHeightRange = 20;
	}
	else if (str_Version == "SR7900")
	{
		m_dHeightRange = 450;
	}
	else if (str_Version == "SR7060")
	{
		m_dHeightRange = 6;
	}
	else if (str_Version == "SR71600")
	{
		m_dHeightRange = 1500;
	}

	_upper = m_dHeightRange;
	_lower = -m_dHeightRange;
}

// Add by peter 20231001 下面为跟 SR SDK 紧密相连的函数 ------------------------------
//**************************************************************
// Method:      ReceiveMessage
// FullName:    CSR_SDK_DemoDlg::ReceiveMessage
// Description: 接收信息
// Others:      这个是跟相机交互的消息么？ peter 20231001
//**************************************************************
void CDetec3DsrDlg::ReceiveMessage(void* pWindow, CString _text, int _width, int _height, int _id)
{
	CDetec3DsrDlg* view = (CDetec3DsrDlg*)pWindow;
	if (view == NULL)
	{
		return;
	}
	view->DetalMessage(_text, _width, _height, _id);
}

//信息显示
void CDetec3DsrDlg::DetalMessage(CString _text, int _width, int _height, int _id)
{
	if (_id == 0)   //文本显示
	{
		bool bEditShow = true;
		// peter 20231001 固定为 无限循环
		BOOL Radio_Infi = TRUE;
		/*
		CButton* Radio_Infi = (CButton*)GetDlgItem(IDC_InfinLoop);
		CButton* Radio_OneC = (CButton*)GetDlgItem(IDC_OneCall);
		CButton* Radio_MultiC = (CButton*)GetDlgItem(IDC_MultiCall);*/
		/*
		if (Radio_MultiC->GetCheck())
		{
			if (_text == _T("数据接收完成")
				|| _text.Find(_T("批处理超时")) >= 0)
			{
				//界面按钮变化
				BtnEnableCallMultiAndLoop(true);

				//开始批处理标志复位
				b_BatchStartFlag = false;
			}
		}*/
		//if (Radio_Infi->GetCheck())
		if (Radio_Infi)
		{
			if (_text == _T("数据缓存中间变量内存申请失败")
				|| _text == _T("当前为非循环模式")
				|| _text == _T("外部IO或其他因素导致批处理正常停止"))
			{
				//界面按钮变化
				//BtnEnableCallMultiAndLoop(true);

				//开始批处理标志复位
				b_BatchStartFlag = false;

				//退出数据接收线程
				//if (_callMultiTimes)
				//{
					_infiniteL->exitReceiveThread();
				//}
			}

			if (_text.Find(_T("当前总行数")) >= 0)
			{
				//((CStatic*)GetDlgItem(IDC_STATIC_CountLine))->SetWindowTextW(_text);
				//统一放到 IDC_EDIT_Info 对话框，peter 20231002
				((CEdit*)GetDlgItem(IDC_EDIT_Info))->LineScroll(((CEdit*)GetDlgItem(IDC_EDIT_Info))->GetLineCount());
				bEditShow = false;
			}
		}
		if (bEditShow)
		{
			InfoShow(_text);
			((CEdit*)GetDlgItem(IDC_EDIT_Info))->LineScroll(((CEdit*)GetDlgItem(IDC_EDIT_Info))->GetLineCount());
		}
	}
	else if (_id == 1) //图像显示
	{
		// peter 20231001 固定为 无限循环
		/*CButton* Radio_Infi = (CButton*)GetDlgItem(IDC_InfinLoop);
		CButton* Radio_OneC = (CButton*)GetDlgItem(IDC_OneCall);
		CButton* Radio_MultiC = (CButton*)GetDlgItem(IDC_MultiCall);
		if (Radio_MultiC->GetCheck()){
			CallMultiTimes_DataShowFunc(_width, _height);
		}
		if (Radio_OneC->GetCheck()){
			CallOneTimes_DataShowFunc(_width, _height);
		}
		if (Radio_Infi->GetCheck()){
			InfiniteLoop_DataShowFunc(_width, _height);
		}*/
		InfiniteLoop_DataShowFunc(_width, _height);
	}
}

void CDetec3DsrDlg::InfiniteLoop_DataShowFunc(int _width, int _height)
{
	//缩放显示系数计算---_width X _height 缩放为 800 X 560 大小图像
	int mScaleW = 800;
	int mScaleH = 560;
	int mXsacle = int(double(_width) / mScaleW);    /* X方向缩放倍数 */
	int mYscale = int(double(_height) / mScaleH);   /* Y方向缩放倍数 */

	//灰度图
	GrayDataShow(_infiniteL->getIntensityData(),
		_width,
		_height,
		mXsacle,
		mYscale,
		mScaleW,
		mScaleH);
	//高度图
	BatchDataShow(_infiniteL->getBatchData(),
		d_HeightUpper,
		d_HeightLower,
		255,
		_width,
		_height,
		mXsacle,
		mYscale,
		mScaleW,
		mScaleH);
}
//灰度图
void CDetec3DsrDlg::GrayDataShow(unsigned char* _grayData,
	int img_w,
	int img_h,
	int _xscale,
	int _yscale,
	int _scaleW,
	int _scaleH)
{
	if (_grayData == NULL || img_h == 0 || img_w == 0)
	{
		return;
	}

	/* 抽帧抽点显示 */
	int imgW = _scaleW;
	int imgH = _scaleH;

	int TmpX = 0;
	int Tmppx = 0;

	if (img_h < imgH)
	{
		imgH = img_h;
	}

	if (img_w < imgW)
	{
		imgW = img_w;
	}

	if (_xscale <= 0)
	{
		_xscale = 1;
	}
	if (_yscale <= 0)
	{
		_yscale = 1;
	}

	int TT = (imgW * 8 + 31) / 32;  //图像4字节对齐
	TT = TT * 4;

	unsigned char* BatchImage = new unsigned char[TT * imgH];
	memset(BatchImage, 0, sizeof(BatchImage));

	for (int i = 0; i < imgH; i++)
	{
		TmpX = (imgH - 1 - i) * _yscale * img_w;
		Tmppx = i * TT;
		for (int j = 0; j < imgW; j++)
		{
			BatchImage[Tmppx + j] = _grayData[TmpX + j * _xscale];
		}
	}

	/* 显示到图像控件 */
	BITMAPINFO* bitMapinfo = NULL;
	bitMapinfo = (BITMAPINFO*)BatchImage;
	bitMapinfo->bmiHeader.biSize = sizeof(BITMAPINFOHEADER);
	bitMapinfo->bmiHeader.biHeight = imgH;
	bitMapinfo->bmiHeader.biWidth = imgW;
	bitMapinfo->bmiHeader.biPlanes = 1;              // 目标设备的级别，必须为1 
	bitMapinfo->bmiHeader.biBitCount = 8;            // 每个像素所需的位数，必须是1(双色), 4(16色)，8(256色)或24(真彩色)之一
	bitMapinfo->bmiHeader.biCompression = BI_RGB;    // 位图压缩类型，必须是 0(不压缩), 1(BI_RLE8压缩类型)或2(BI_RLE4压缩类型)之一 
	bitMapinfo->bmiHeader.biSizeImage = 0;           // 位图的大小，以字节为单位  
	bitMapinfo->bmiHeader.biXPelsPerMeter = 0;       // 位图水平分辨率，每米像素数  
	bitMapinfo->bmiHeader.biYPelsPerMeter = 0;       // 位图垂直分辨率，每米像素数  
	bitMapinfo->bmiHeader.biClrUsed = 0;             // 位图实际使用的颜色表中的颜色数  
	bitMapinfo->bmiHeader.biClrImportant = 0;        // 位图显示过程中重要的颜色数  

	/* 颜色转换 */
	for (int i = 0; i < 256; i++)
	{
		/* 颜色的取值范围 (0-255) */
		bitMapinfo->bmiColors[i].rgbBlue = bitMapinfo->bmiColors[i].rgbGreen = bitMapinfo->bmiColors[i].rgbRed = (BYTE)i;
	}
	bitMapinfo->bmiHeader.biClrUsed = 256;           // 位图实际使用的颜色表中的颜色数  

	CRect _rect;
	CDC* pDc;
	CStatic* pStc = (CStatic*)GetDlgItem(IDC_IMAGE_GRAY);   //图像控件 
	if (pStc == NULL)
		return;
	pStc->GetClientRect(&_rect);
	pDc = pStc->GetDC();
	HDC hDC = pDc->GetSafeHdc();

	int n_WndWidth = _rect.right - _rect.left;
	int n_WndHeight = _rect.bottom - _rect.top;
	SetStretchBltMode(hDC, COLORONCOLOR);
	StretchDIBits(hDC,
		0,
		0,
		n_WndWidth,     //显示窗口宽度  
		n_WndHeight,    //显示窗口高度  
		0,
		0,
		imgW,           //图像宽度  
		imgH,           //图像高度  
		BatchImage,
		bitMapinfo,
		DIB_RGB_COLORS,
		SRCCOPY
	);

	ReleaseDC(pDc);

	if (BatchImage)
	{
		delete[] BatchImage;
		BatchImage = NULL;
	}
}
//高度图
void CDetec3DsrDlg::BatchDataShow(int* _BatchData,
	double max_height,
	double min_height,
	int _ColorMax,
	int img_w,
	int img_h,
	int _xscale,
	int _yscale,
	int _scaleW,
	int _scaleH)
{
	if (_BatchData == NULL || img_h == 0 || img_w == 0)
	{
		return;
	}

	/* 数据转换 */
	double mSub = max_height - min_height;
	if (mSub <= 0.000001)
	{
		mSub = 1;
	}
	double fscale = double(_ColorMax) / mSub;   //颜色区间与高度区间比例

	/* 抽帧抽点显示 */
	int imgW = _scaleW;
	int imgH = _scaleH;

	int TmpX = 0;
	int Tmppx = 0;

	if (img_h < imgH)
	{
		imgH = img_h;
	}

	if (img_w < imgW)
	{
		imgW = img_w;
	}

	if (_xscale <= 0)
	{
		_xscale = 1;
	}
	if (_yscale <= 0)
	{
		_yscale = 1;
	}

	int TT = (imgW * 8 + 31) / 32;   //图像宽度4字节对齐
	TT = TT * 4;

	int dwDataSize = TT * imgH;
	unsigned char* BatchImage = new unsigned char[dwDataSize];
	memset(BatchImage, 0, sizeof(BatchImage));

	for (int i = 0; i < imgH; i++)
	{
		TmpX = (imgH - 1 - i) * _yscale * img_w;
		Tmppx = i * TT;
		for (int j = 0; j < imgW; j++)
		{
			double Tmp = double(_BatchData[TmpX + j * _xscale]) * 0.00001;
			if (Tmp < min_height)
				BatchImage[Tmppx + j] = 0;
			else if (Tmp > max_height)
				BatchImage[Tmppx + j] = 0xff;
			else
			{
				unsigned char tmpt = unsigned char((Tmp - min_height) * fscale);
				BatchImage[Tmppx + j] = tmpt;
			}
		}
	}

	/* 显示到图像控件 */
	BITMAPINFO* bitMapinfo = NULL;
	bitMapinfo = (BITMAPINFO*)BatchImage;
	bitMapinfo->bmiHeader.biSize = sizeof(BITMAPINFOHEADER);
	bitMapinfo->bmiHeader.biHeight = imgH;
	bitMapinfo->bmiHeader.biWidth = imgW;
	bitMapinfo->bmiHeader.biPlanes = 1;              // 目标设备的级别，必须为1 
	bitMapinfo->bmiHeader.biBitCount = 8;            // 每个像素所需的位数，必须是1(双色), 4(16色)，8(256色)或24(真彩色)之一
	bitMapinfo->bmiHeader.biCompression = BI_RGB;    // 位图压缩类型，必须是 0(不压缩), 1(BI_RLE8压缩类型)或2(BI_RLE4压缩类型)之一 
	bitMapinfo->bmiHeader.biSizeImage = 0;           // 位图的大小，以字节为单位  
	bitMapinfo->bmiHeader.biXPelsPerMeter = 0;       // 位图水平分辨率，每米像素数  
	bitMapinfo->bmiHeader.biYPelsPerMeter = 0;       // 位图垂直分辨率，每米像素数  
	bitMapinfo->bmiHeader.biClrUsed = 0;             // 位图实际使用的颜色表中的颜色数  
	bitMapinfo->bmiHeader.biClrImportant = 0;        // 位图显示过程中重要的颜色数  

	/* 颜色转换 */
	for (int i = 0; i < 256; i++)
	{
		/* 颜色的取值范围 (0-255) */
		bitMapinfo->bmiColors[i].rgbBlue = bitMapinfo->bmiColors[i].rgbGreen = bitMapinfo->bmiColors[i].rgbRed = (BYTE)i;
	}
	bitMapinfo->bmiHeader.biClrUsed = 256;           // 位图实际使用的颜色表中的颜色数  

	CRect _rect;
	CDC* pDc;
	CStatic* pStc = (CStatic*)GetDlgItem(IDC_IMAGE_HIGH);   //图像控件 

	if (pStc == NULL)
		return;
	pStc->GetClientRect(&_rect);
	pDc = pStc->GetDC();
	HDC hDC = pDc->GetSafeHdc();

	int n_WndWidth = _rect.right - _rect.left;
	int n_WndHeight = _rect.bottom - _rect.top;
	SetStretchBltMode(hDC, COLORONCOLOR);
	StretchDIBits(hDC,
		0,
		0,
		n_WndWidth,     //显示窗口宽度  
		n_WndHeight,    //显示窗口高度  
		0,
		0,
		imgW,           //图像宽度  
		imgH,           //图像高度  
		BatchImage,
		bitMapinfo,
		DIB_RGB_COLORS,
		SRCCOPY
	);

	ReleaseDC(pDc);

	if (BatchImage)
	{
		delete[] BatchImage;
		BatchImage = NULL;
	}
}
//3D图
//void SR_3D_Show(const int* _BatchData,
//	double x_true_step,
//	double y_true_step,
//	int x_Point_num,
//	int y_batchPoint_num,
//	double z_scale,
//	double Ho,
//	double Lo
//)
//{
//
//	cloud->clear();
//	cloud->empty();
//
//	//判断有效点个数
//	int TmpHo = Ho * Y_AMPLIFY_RATIO;
//	int TmpLo = Lo * Y_AMPLIFY_RATIO;
//
//	int m_valid_num = 0;
//	int TmpN = x_Point_num * y_batchPoint_num;
//	for (int i = 0; i < TmpN; i++)
//	{
//		if (_BatchData[i] < MIN_VALID_HEIGHT_INT)
//			continue;
//		if (_BatchData[i] > TmpHo || _BatchData[i] < TmpLo)
//			continue;
//		m_valid_num++;
//	}
//
//	int XPNum = x_Point_num;
//	int YPNum = y_batchPoint_num;
//	int m_scale = 1;
//	if (y_batchPoint_num >= 5000 && y_batchPoint_num < 9000)
//	{
//		if (m_valid_num > 4000000 && m_valid_num < 16000000)
//		{
//			m_scale = 4;
//			XPNum = x_Point_num / m_scale;
//			YPNum = y_batchPoint_num / m_scale;
//		}
//		else if (m_valid_num >= 16000000)
//		{
//			m_scale = 6;
//			XPNum = x_Point_num / m_scale;
//			YPNum = y_batchPoint_num / m_scale;
//		}
//
//	}
//	else if (y_batchPoint_num > 2000 && y_batchPoint_num < 5000)
//	{
//		if (m_valid_num > 4000000)
//		{
//			m_scale = 2;
//			XPNum = x_Point_num / m_scale;
//			YPNum = y_batchPoint_num / m_scale;
//		}
//	}
//	else if (y_batchPoint_num >= 9000)
//	{
//		if (m_valid_num > 4000000 && m_valid_num < 16000000)
//		{
//			m_scale = 4;
//			if (x_Point_num == 6400)
//				m_scale = 6;
//			XPNum = x_Point_num / m_scale;
//			YPNum = y_batchPoint_num / m_scale;
//		}
//		else if (m_valid_num >= 16000000 && m_valid_num < 36000000)
//		{
//			m_scale = 6;
//			if (x_Point_num == 6400)
//				m_scale = 8;
//			XPNum = x_Point_num / m_scale;
//			YPNum = y_batchPoint_num / m_scale;
//		}
//		else if (m_valid_num >= 36000000)
//		{
//			m_scale = 8;
//			if (x_Point_num == 6400)
//				m_scale = 10;
//			XPNum = x_Point_num / m_scale;
//			YPNum = y_batchPoint_num / m_scale;
//		}
//		else if (m_valid_num > 400000 && m_valid_num <= 4000000)
//		{
//			m_scale = 2;
//			if (x_Point_num == 6400)
//				m_scale = 4;
//			XPNum = x_Point_num / m_scale;
//			YPNum = y_batchPoint_num / m_scale;
//		}
//		else
//		{
//			m_scale = 8;
//			XPNum = x_Point_num / m_scale;
//			YPNum = y_batchPoint_num / m_scale;
//		}
//	}
//	else
//	{
//		if (m_valid_num > 4000000)
//		{
//			m_scale = 2;
//			XPNum = x_Point_num / m_scale;
//			YPNum = y_batchPoint_num / m_scale;
//		}
//		else if (m_valid_num >= 16000000 && m_valid_num < 36000000)
//		{
//			m_scale = 3;
//			if (x_Point_num == 6400)
//				m_scale = 6;
//			XPNum = x_Point_num / m_scale;
//			YPNum = y_batchPoint_num / m_scale;
//		}
//		else if (m_valid_num >= 36000000)
//		{
//			m_scale = 8;
//			if (x_Point_num == 6400)
//				m_scale = 10;
//			XPNum = x_Point_num / m_scale;
//			YPNum = y_batchPoint_num / m_scale;
//		}
//	}
//
//	try
//	{
//		//点云数据生成
//		double Tmpx = x_true_step * m_scale;
//		double Tmpy = y_true_step * m_scale;
//		int div = TmpHo - TmpLo;
//		for (int j = 0; j < YPNum; j++)
//		{
//			int TMPX = j * x_Point_num * m_scale;
//			for (int i = 0; i < XPNum; i++)
//			{
//				pcl::PointXYZRGBA temp_point;
//				int pos = TMPX + i * m_scale;
//				if (_BatchData[pos] < MIN_VALID_HEIGHT_INT)
//				{
//					continue;
//				}
//				if (_BatchData[pos] > TmpHo || _BatchData[pos] < TmpLo)
//					continue;
//
//				double x_tmp = i * Tmpx;
//				double y_tmp = j * Tmpy;
//				double z_tmp = (double)_BatchData[pos] * 0.00001 * z_scale;
//
//				int gray = (255 * (_BatchData[pos] - TmpLo)) / div;
//				if (gray > 255)
//					gray = 255;
//				else if (gray < 0)
//					gray = 0;
//
//				temp_point.x = x_tmp;
//				temp_point.y = y_tmp;
//				temp_point.z = z_tmp;
//				temp_point.rgba = grayToRgb[gray] | 0xff000000;
//
//				cloud->points.push_back(temp_point);
//			}
//		}
//		pcl::visualization::PCLVisualizer viewer("3DShow");
//		viewer.addPointCloud(cloud, "original_cloud");
//		viewer.addCoordinateSystem(5.0);
//
//		while (!viewer.wasStopped())
//		{
//			viewer.spinOnce();
//		}
//
//		viewer.removeAllPointClouds();
//		viewer.removeAllCoordinateSystems();
//		viewer.resetStoppedFlag();
//		viewer.removeAllShapes();
//	}
//	catch (...)
//	{
//		cout << "Standard exception: " << endl;
//		MessageBox(NULL, TEXT("3D显示内存不足"), TEXT("标题"), MB_OK);
//	}


void CDetec3DsrDlg::InfoShow(CString _text)
{
	((CEdit*)GetDlgItem(IDC_EDIT_Info))->SetWindowTextW(_text);
}

// Add by peter 20231001 下面为按钮功能区 ------------------------------
void CDetec3DsrDlg::OnBnClickedBtnConnect()
{
	// peter 20231001 固定为 无限循环
	BOOL Radio_Infi = TRUE;
	/*
	CButton* Radio_Infi = (CButton*)GetDlgItem(IDC_InfinLoop);		//原界面无线循环选择按钮
	CButton* Radio_OneC = (CButton*)GetDlgItem(IDC_OneCall);		//原界面单次回调选择按钮
	CButton* Radio_MultiC = (CButton*)GetDlgItem(IDC_MultiCall);	//原界面多次回调选择按钮
	if (!Radio_Infi->GetCheck()
		&& !Radio_OneC->GetCheck()
		&& !Radio_MultiC->GetCheck())
	{
		AfxMessageBox(_T("请预先选择一种数据获取模式!"), MB_OK | MB_ICONQUESTION);
		return;
	}*/


	//sdk dll 连接参数配置
	SR7IF_ETHERNET_CONFIG SREthernetConFig;

	//王朝禹wcy更改：以下注释部分为原界面通过文本对话框获取IP值
	//获取IP控件地址4个值
	//从界面获取IP地址--界面IP输入值
	/*
	((CIPAddressCtrl*)GetDlgItem(IDC_IPAddr))->GetAddress(SREthernetConFig.abyIpAddress[0],
														  SREthernetConFig.abyIpAddress[1],
														  SREthernetConFig.abyIpAddress[2],
														  SREthernetConFig.abyIpAddress[3]);*/
	//wcy更改：以下部分为更改的，通过文本文档 config.txt 获取IP值 -------------------
	std::string ipAddress;

	std::ifstream configFile("config.txt");
	if (configFile.is_open()) 
	{
		std::getline(configFile, ipAddress);
		configFile.close();
	}
	//读取txt文档第一行，获取ip地址，而非手动输入
	std::vector<int> ipParts;
	std::istringstream ipStream(ipAddress);
	std::string ipPart;
	while (std::getline(ipStream, ipPart, '.'))
	{
		ipParts.push_back(std::stoi(ipPart));
	}

	if (ipParts.size() == 4) 
	{
		SREthernetConFig.abyIpAddress[0] = ipParts[0];
		SREthernetConFig.abyIpAddress[1] = ipParts[1];
		SREthernetConFig.abyIpAddress[2] = ipParts[2];
		SREthernetConFig.abyIpAddress[3] = ipParts[3];
	}
	//wcy更改：以上部分为更改的，通过文本文档 ipaddress.txt 获取IP值 -------------------

	//连接相机
	int reT = SR7IF_EthernetOpen(DEVICEID, &SREthernetConFig);
	if (reT < 0)   //失败
	{
		CString strInfo;
		strInfo.Format(_T("相机连接失败,返回值:%d"), reT);
		AfxMessageBox(strInfo, MB_OK | MB_ICONQUESTION);
		return;
	}

	//成功
	bConnectFlag = true;
	saveIpAddress(SREthernetConFig);
	getHeightUpperLower(d_HeightUpper, d_HeightLower);
	
	// IDC_EDIT_Lower 和 IDC_EDIT_Upper 这两个ID资源是原界面Z轴显示范围 下限和上线
	// 这个地方也是要改到文本文档里面去，peter 20231001
	// modify by wcy 20231001 下限和上限从 config.txt 读取第2行和第3行 获取 ----------
	
	//wcy添加：通过读取txt文本文件第二行，将读取的值赋给d_HeightLower（20231001）
	CString strTmp_HeightL;
	CStdioFile file;
	if (file.Open(_T("config.txt"), CFile::modeRead))//此处为读取文档路径
	{
		CString line;
		int lineCount = 0;
		while (file.ReadString(line))
		{
			if (lineCount == 1) // 第二行的索引是 1
			{
				strTmp_HeightL = line;
				break;
			}
			lineCount++;
		}
		file.Close();
	}//循环的功能是为了读取txt文本的第二行
	DWORD dwNum_HeightL = WideCharToMultiByte(CP_OEMCP, NULL, strTmp_HeightL, -1, NULL, NULL, 0, NULL);
	char* c = new char[dwNum_HeightL];
	WideCharToMultiByte(CP_OEMCP, NULL, strTmp_HeightL, -1, c, dwNum_HeightL, 0, NULL);
	d_HeightLower = atof(c);//将读取的值赋给d_HeightLower
	delete[]  c;
	c = NULL;

	//wcy添加：以下为读取txt文本文件的第三行，并将读取值赋给d_HeightUpper
	CString strTmp_HeightUp;
	if (file.Open(_T("config.txt"), CFile::modeRead))//此处为读取文档路径
	{
		CString line;
		int lineCount = 0;
		while (file.ReadString(line))
		{
			if (lineCount == 2) // 第三行的索引是 2
			{
				strTmp_HeightUp = line;
				break;
			}
			lineCount++;
		}
		file.Close();
	}//循环的功能是为了读取txt文本的第三行
	DWORD dwNum_HeightUp = WideCharToMultiByte(CP_OEMCP, NULL, strTmp_HeightUp, -1, NULL, NULL, 0, NULL);
	char* d = new char[dwNum_HeightUp];
	WideCharToMultiByte(CP_OEMCP, NULL, strTmp_HeightUp, -1, d, dwNum_HeightUp, 0, NULL);
	d_HeightUpper = atof(d);//将读取的值赋给d_HeightUpper
	delete[]  d;
	d = NULL;

	/*
	strTmp.Format(_T("%.3f"), d_HeightLower);
	((CEdit*)GetDlgItem(IDC_EDIT_Lower))->SetWindowTextW(strTmp);
	strTmp.Format(_T("%.3f"), d_HeightUpper);
	((CEdit*)GetDlgItem(IDC_EDIT_Upper))->SetWindowTextW(strTmp);*/
	otherSampleDlg->setHeightDispRange(d_HeightUpper);

	//相机B是否在线
	bool bCamBOnline = false;
	int RT = SR7IF_GetOnlineCameraB(DEVICEID);
	if (RT == 0)
	{
		bCamBOnline = true;
	}
	//相机C是否在线 add by peter 20231001
	bool bCamCOnline = false;
	//一个终端上面只有A B两个口，如果有C相机，则必须用另一个终端，则IP要重新读取另一个，
	//在另一个IP下面调用A 口，因为该函数在SR7Link.h中只有A B两个口，没有C口。peter 20231002
	/*int RT = SR7IF_GetOnlineCameraC(DEVICEID);//注意如下：	
	if (RT == 0)
	{
		bCamCOnline = true;
	}*/

	int mBatchW = SR7IF_ProfileDataWidth(DEVICEID, NULL);

	//采集模型的选择，原界面有3种，这里改成一种 无线循环 
	// peter 20231001 把 Radio_Infi 直接改成BOOL变量，初始化为 TRUE
	//if (Radio_Infi->GetCheck())
	if (Radio_Infi)
	{
		if (_infiniteL == NULL)
		{
			_infiniteL = new InfiniteLoop();
			_infiniteL->UserReceiveBack(this->ReceiveMessage, this);
		}

		//初始化
		_infiniteL->VariableInit();
		_infiniteL->setCamBonlineFlag(bCamBOnline);
		_infiniteL->DataMemoryInit(bCamBOnline, mBatchW);
		//初始化 C相机 , add by peter 20231001
		//_infiniteL->setCamConlineFlag(bCamCOnline);//注意如下：
		//一个终端上面只有A B两个口，如果有C相机，则必须用另一个终端，则IP要重新读取另一个，
		//在另一个IP下面调用A 口，因为该函数在SR7Link.h中只有A B两个口，没有C口。peter 20231002
		//_infiniteL->DataMemoryInit(bCamCOnline, mBatchW);
	}
	/*
	if (Radio_OneC->GetCheck())
	{
		if (_callOneTimes == NULL)
		{
			_callOneTimes = new CallOneTimes();
			_callOneTimes->UserReceiveBack(this->ReceiveMessage, this);
		}

		//初始化
		_callOneTimes->VariableInit();
		_callOneTimes->DataMemoryInit(mBatchW);
	}
	if (Radio_MultiC->GetCheck())
	{
		if (_callMultiTimes == NULL)
		{
			_callMultiTimes = new CallMultiTimes();
			_callMultiTimes->UserReceiveBack(this->ReceiveMessage, this);
		}

		//初始化
		_callMultiTimes->VariableInit();
		_callMultiTimes->DataMemoryInit(bCamBOnline, mBatchW);
		_callMultiTimes->setCamBonlineFlag(bCamBOnline);

		CString str;
		((CEdit*)GetDlgItem(IDC_CallFreq))->GetWindowTextW(str);
		DWORD dwNum = WideCharToMultiByte(CP_OEMCP, NULL, str, -1, NULL, NULL, 0, NULL);
		char* c = new char[dwNum];
		WideCharToMultiByte(CP_OEMCP, NULL, str, -1, c, dwNum, 0, NULL);
		unsigned int dwProfileCnt = atoi(c);
		delete[]  c;
		c = NULL;

		reT = _callMultiTimes->HighSpeedEthInit(SREthernetConFig, dwProfileCnt);
		if (reT < 0)
		{
			b_HighSpeedInitFail = true;

			CString strInfo;
			strInfo.Format(_T("高速数据以太网通信初始化失败,返回值:%d"), reT);
			AfxMessageBox(strInfo, MB_OK | MB_ICONQUESTION);

			strInfo.Format(_T("SR7IF_HighSpeedDataEthernetCommunicationInitalize 失败\n 返回值:%d"), reT);
			InfoShow(strInfo);
		}
		else
		{
			b_HighSpeedInitFail = false;
		}
	}*/

	//界面按钮状态变化
	//gBoxModeEnable(false);
	((CButton*)GetDlgItem(IDC_BTN_Connect))->EnableWindow(false);
	((CButton*)GetDlgItem(IDC_BTN_DisConnect))->EnableWindow(true);
	//下面的3个，貌似跟 无限循环 采集无关，只跟 一次性回调 有关，屏蔽，peter 20231002
	/*((CButton*)GetDlgItem(IDC_CamA))->SetCheck(1);
	((CButton*)GetDlgItem(IDC_CamB))->SetCheck(0);
	((CButton*)GetDlgItem(IDC_CamC))->SetCheck(0);// C相机 , add by peter 20231001*/
	mCurCamId = 0;
	((CButton*)GetDlgItem(IDC_SaveCamA))->SetCheck(1);
	((CButton*)GetDlgItem(IDC_SaveCamB))->SetCheck(0);
	((CButton*)GetDlgItem(IDC_SaveCamC))->SetCheck(0);// C相机 , add by peter 20231001

	//修改为无限循环，下面的屏蔽掉 , add by peter 20231001
	/*
	//相机B在线--针对一次性回调模式
	if (bCamBOnline && Radio_OneC->GetCheck())  
	{
		((CButton*)GetDlgItem(IDC_CamB))->EnableWindow(true);
		((CButton*)GetDlgItem(IDC_SaveCamB))->EnableWindow(true);
	}
	else
	{
		((CButton*)GetDlgItem(IDC_CamB))->EnableWindow(false);
		((CButton*)GetDlgItem(IDC_SaveCamB))->EnableWindow(false);
	}*/
}


void CDetec3DsrDlg::OnBnClickedBtnDisconnect()
{
	//断开连接前做相关处理
	InitConfigBeforeDisConnect();

	//断开连接
	int reT = SR7IF_CommClose(DEVICEID);
	if (reT < 0)
	{
		AfxMessageBox(_T("连接断开失败!"), MB_OK | MB_ICONQUESTION);
		return;
	}

	bConnectFlag = false;
	b_HighSpeedInitFail = false;

	//界面按钮状态变化
	//gBoxModeEnable(true);

	((CButton*)GetDlgItem(IDC_BTN_Connect))->EnableWindow(true);
	((CButton*)GetDlgItem(IDC_BTN_DisConnect))->EnableWindow(false);

	//固定为 无限循环，下面屏蔽， peter 20231001
	/*
	CButton* Radio_Infi = (CButton*)GetDlgItem(IDC_InfinLoop);
	CButton* Radio_OneC = (CButton*)GetDlgItem(IDC_OneCall);
	CButton* Radio_MultiC = (CButton*)GetDlgItem(IDC_MultiCall);

	if (Radio_OneC->GetCheck()){
		BtnEnableCallOne(false);
	}
	if (Radio_MultiC->GetCheck()){

		BtnEnableCallMultiAndLoop(true);
	}*/

	//图像清空
	CStatic* pStc_gray = (CStatic*)GetDlgItem(IDC_IMAGE_GRAY);
	CStatic* pStc_high = (CStatic*)GetDlgItem(IDC_IMAGE_HIGH);
	if (pStc_gray)
	{
		CRect lRect;
		pStc_gray->GetClientRect(&lRect);
		pStc_gray->GetDC()->FillSolidRect(lRect.left, lRect.top, lRect.Width(), lRect.Height(), RGB(240, 240, 240));	
	}
	if (pStc_high)
	{
		CRect lRect;
		pStc_high->GetClientRect(&lRect);
		pStc_high->GetDC()->FillSolidRect(lRect.left, lRect.top, lRect.Width(), lRect.Height(), RGB(240, 240, 240));
	}
	//是否添加3D显示的清空？？ peter 20231001
	CStatic* pStc_3d = (CStatic*)GetDlgItem(IDC_IMAGE_3D);
}


void CDetec3DsrDlg::OnBnClickedBtnStartm()
{
	if (!bConnectFlag)
	{
		InfoShow(_T("相机未连接"));
		return;
	}

	//modify by peter 20231001
	BOOL Radio_Infi = TRUE;
	/*CButton* Radio_Infi = (CButton*)GetDlgItem(IDC_InfinLoop);
	CButton* Radio_MultiC = (CButton*)GetDlgItem(IDC_MultiCall);*/

	//下面是对 原界面 超时时间ms的修改，改成在 config.txt 中读取第6行
	CString str_mTimeOut;
	//((CEdit*)GetDlgItem(IDC_EDIT_TimeOut))->GetWindowTextW(str);

	//peter 20231002 添加：以下为读取txt文本文件的第6行，并将读取值赋给mTimeOut
	CStdioFile file;
	if (file.Open(_T("config.txt"), CFile::modeRead))//此处为读取文档路径
	{
		CString line;
		int lineCount = 0;
		while (file.ReadString(line))
		{
			if (lineCount == 5) // 第六行的索引是 5
			{
				str_mTimeOut = line;
				break;
			}
			lineCount++;
		}
		file.Close();
	}//循环的功能是为了读取txt文本的第6行

	DWORD dwNum = WideCharToMultiByte(CP_OEMCP, NULL, str_mTimeOut, -1, NULL, NULL, 0, NULL);
	char* c = new char[dwNum];
	WideCharToMultiByte(CP_OEMCP, NULL, str_mTimeOut, -1, c, dwNum, 0, NULL);
	int mTimeOut = atoi(c);
	delete[]  c;
	c = NULL;

	//多次回调模式 //屏蔽 peter 20231001
	/*
	if (Radio_MultiC->GetCheck())
	{
		if (b_HighSpeedInitFail)  //高速数据通信初始化失败时不进行处理
		{
			InfoShow(_T("高速数据以太网通信初始化失败"));
			return;
		}
		if (_callMultiTimes)
		{
			//批处理行数、轮廓宽度、X间距获取
			int mBatchH = SR7IF_ProfilePointSetCount(DEVICEID, NULL);
			int mBatchW = SR7IF_ProfileDataWidth(DEVICEID, NULL);
			double mXpitch = SR7IF_ProfileData_XPitch(DEVICEID, NULL);
			_callMultiTimes->setDataHeightWidth(mBatchH, mBatchW, mXpitch);
			_callMultiTimes->setStartInit();

			//图像清空
			CStatic* pStc = (CStatic*)GetDlgItem(IDC_IMAGE);
			if (pStc)
			{
				CRect lRect;
				pStc->GetClientRect(&lRect);
				pStc->GetDC()->FillSolidRect(lRect.left, lRect.top, lRect.Width(), lRect.Height(), RGB(240, 240, 240));
			}

			//开始批处理
			int reT = _callMultiTimes->StartMeasure(mTimeOut);
			if (reT < 0)
			{
				CString strInfo;
				strInfo.Format(_T("SR7IF_StartMeasure 失败,返回值:%d"), reT);
				InfoShow(strInfo);
				AfxMessageBox(strInfo, MB_OK | MB_ICONQUESTION);
				return;
			}

			//界面按钮变化
			BtnEnableCallMultiAndLoop(false);
		}
	}*/

	//无限循环
	//if (Radio_Infi->GetCheck())
	if (Radio_Infi)
	{
		if (_infiniteL)
		{
			//轮廓宽度获取
			int mBatchW = SR7IF_ProfileDataWidth(DEVICEID, NULL);
			double mXpitch = SR7IF_ProfileData_XPitch(DEVICEID, NULL);
			_infiniteL->setProfileWidth(mBatchW);   //设置轮廓宽度
			_infiniteL->setXPitch(mXpitch);         //设置X间距--3D显示用
			_infiniteL->setDispHeight(2000);        //设置显示高度

			//图像清空
			CStatic* pStc_gray = (CStatic*)GetDlgItem(IDC_IMAGE_GRAY);
			CStatic* pStc_high = (CStatic*)GetDlgItem(IDC_IMAGE_HIGH);
			if (pStc_gray)
			{
				CRect lRect;
				pStc_gray->GetClientRect(&lRect);
				pStc_gray->GetDC()->FillSolidRect(lRect.left, lRect.top, lRect.Width(), lRect.Height(), RGB(240, 240, 240));
			}
			if (pStc_high)
			{
				CRect lRect;
				pStc_high->GetClientRect(&lRect);
				pStc_high->GetDC()->FillSolidRect(lRect.left, lRect.top, lRect.Width(), lRect.Height(), RGB(240, 240, 240));
			}
			//是否添加3D显示的清空？？ peter 20231001
			CStatic* pStc_3d = (CStatic*)GetDlgItem(IDC_IMAGE_3D);

			//开始批处理
			int reT = _infiniteL->StartMeasure(mTimeOut);
			if (reT < 0)
			{
				CString strInfo;
				strInfo.Format(_T("SR7IF_StartMeasure 失败,返回值:%d"), reT);
				InfoShow(strInfo);
				AfxMessageBox(strInfo, MB_OK | MB_ICONQUESTION);
				return;
			}

			//启动数据接收线程
			_infiniteL->startReceiveThread();

			//界面按钮变化
			//BtnEnableCallMultiAndLoop(false);
			((CButton*)GetDlgItem(IDC_BTN_StopM))->EnableWindow(true);//wcy添加20231002
		}
	}

	b_BatchStartFlag = true;
}


void CDetec3DsrDlg::OnBnClickedBtnStopm()
{
	if (!bConnectFlag)
	{
		InfoShow(_T("相机未连接"));
		return;
	}

	int reT = SR7IF_StopMeasure(DEVICEID);
	if (reT < 0)
	{
		CString strInfo;
		strInfo.Format(_T("SR7IF_StopMeasure 失败,返回值:%d"), reT);
		InfoShow(strInfo);
		AfxMessageBox(strInfo, MB_OK | MB_ICONQUESTION);
		return;
	}

	//modify by peter 20231001
	BOOL Radio_Infi = TRUE;
	/*CButton* Radio_Infi = (CButton*)GetDlgItem(IDC_InfinLoop);
	CButton* Radio_OneC = (CButton*)GetDlgItem(IDC_OneCall);
	CButton* Radio_MultiC = (CButton*)GetDlgItem(IDC_MultiCall);*/

	//屏蔽 peter 20231001
	/*
	//一次回调模式
	if (Radio_OneC->GetCheck())
	{
		BtnEnableCallOne(false);

		//退出图像显示线程
		if (_callOneTimes)
		{
			_callOneTimes->exitImageDiap();

			CString strError = _callOneTimes->getErrorInfo();
			if (strError != _T(""))
			{
				InfoShow(strError);
			}
		}
	}

	//多次回调模式
	if (Radio_MultiC->GetCheck())
	{
		BtnEnableCallMultiAndLoop(true);

		//数据未接收完时停止批处理显示已接收数据
		if (_callMultiTimes)
		{
			_callMultiTimes->ImageUpdate();
		}
	}*/

	//无限循环
	//if (Radio_Infi->GetCheck())
	if (Radio_Infi)
	{
		BtnEnableCallMultiAndLoop(true);

		//退出数据接收线程
		if (_infiniteL)
		{
			_infiniteL->exitReceiveThread();
		}
	}

	b_BatchStartFlag = false;
}


void CDetec3DsrDlg::OnBnClickedBtnClear()
{
	InfoShow(_T(""));
	//modify by peter 20231001
	BOOL Radio_Infi = TRUE;
	//CButton* Radio_Infi = (CButton*)GetDlgItem(IDC_InfinLoop);
	//if (Radio_Infi->GetCheck())
	if (Radio_Infi)
	{
		if (_infiniteL)
		{
			_infiniteL->clearInfo();
		}
	}
}


// Add by peter 20231001 一次性回调保存数据
void CDetec3DsrDlg::OnBnClickedBtnGrays()
{
	//modify by peter 20231001
	BOOL Radio_Infi = TRUE;
	/*CButton* Radio_Infi = (CButton*)GetDlgItem(IDC_InfinLoop);
	CButton* Radio_OneC = (CButton*)GetDlgItem(IDC_OneCall);
	CButton* Radio_MultiC = (CButton*)GetDlgItem(IDC_MultiCall);*/

	TCHAR szFilter[] = _T("(*.bmp)|*.bmp|(*.bin)|*.bin|");

	/* 构造打开文件对话框 */
	CFileDialog fileDlg(FALSE, _T("bmp"), NULL, OFN_HIDEREADONLY | OFN_OVERWRITEPROMPT, szFilter, this);
	CString fileName;
	if (IDOK == fileDlg.DoModal())
	{
		fileName = fileDlg.GetPathName();

		int reT = -1;
		//屏蔽 peter 20231001
		/*
		if (Radio_OneC->GetCheck())
		{
			int mCamId = 0;
			if (((CButton*)GetDlgItem(IDC_SaveCamA))->GetCheck())
			{
				mCamId = 0;
			}
			else if (((CButton*)GetDlgItem(IDC_SaveCamB))->GetCheck())
			{
				mCamId = 1;
			}
			if (_callOneTimes)
			{
				reT = _callOneTimes->saveIntensityData(fileName, mCamId);
			}
		}
		if (Radio_MultiC->GetCheck())
		{
			if (_callMultiTimes)
			{
				reT = _callMultiTimes->saveIntensityData(fileName);
			}
		}*/
		//if (Radio_Infi->GetCheck())
		if (Radio_Infi)
		{
			if (_infiniteL)
			{
				reT = _infiniteL->saveIntensityData(fileName);
			}
		}

		if (reT < 0)
		{
			AfxMessageBox(_T("保存失败!"), MB_OK | MB_ICONQUESTION);
		}
		else
		{
			CString strInfo = _T("");
			strInfo += _T("灰度数据保存路径：");
			strInfo += _T("\r\n");
			strInfo += fileName;
			InfoShow(strInfo);

			AfxMessageBox(_T("保存完成!"), MB_OK | MB_ICONQUESTION);
		}
	}
}


void CDetec3DsrDlg::OnBnClickedBtnHeights()
{
	//modify by peter 20231001
	BOOL Radio_Infi = TRUE;
	/*CButton* Radio_Infi = (CButton*)GetDlgItem(IDC_InfinLoop);
	CButton* Radio_OneC = (CButton*)GetDlgItem(IDC_OneCall);
	CButton* Radio_MultiC = (CButton*)GetDlgItem(IDC_MultiCall);*/

	TCHAR szFilter[] = _T("(*.bin)|*.bin|(*.ecd)|*.ecd|");

	/* 构造打开文件对话框 */
	CFileDialog fileDlg(FALSE, _T("ecd"), NULL, OFN_HIDEREADONLY | OFN_OVERWRITEPROMPT, szFilter, this);
	CString fileName;
	if (IDOK == fileDlg.DoModal())
	{
		fileName = fileDlg.GetPathName();

		int reT = -1;
		//屏蔽 peter 20231001
		/*
		if (Radio_OneC->GetCheck())
		{
			int mCamId = 0;
			if (((CButton*)GetDlgItem(IDC_SaveCamA))->GetCheck())
			{
				mCamId = 0;
			}
			else if (((CButton*)GetDlgItem(IDC_SaveCamB))->GetCheck())
			{
				mCamId = 1;
			}
			if (_callOneTimes)
			{
				reT = _callOneTimes->saveHeightData(fileName, mCamId);
			}
		}
		if (Radio_MultiC->GetCheck())
		{
			if (_callMultiTimes)
			{
				reT = _callMultiTimes->saveHeightData(fileName);
			}
		}*/
		//if (Radio_Infi->GetCheck())
		if (Radio_Infi)
		{
			if (_infiniteL)
			{
				reT = _infiniteL->saveHeightData(fileName);
			}
		}

		if (reT < 0)
		{
			AfxMessageBox(_T("保存失败!"), MB_OK | MB_ICONQUESTION);
		}
		else
		{
			CString strInfo = _T("");
			strInfo += _T("高度数据保存路径：");
			strInfo += _T("\r\n");
			strInfo += fileName;
			InfoShow(strInfo);

			AfxMessageBox(_T("保存完成!"), MB_OK | MB_ICONQUESTION);
		}
	}
}


void CDetec3DsrDlg::OnBnClickedBtnEncoders()
{
	//modify by peter 20231001
	BOOL Radio_Infi = TRUE;
	/*CButton* Radio_Infi = (CButton*)GetDlgItem(IDC_InfinLoop);
	CButton* Radio_OneC = (CButton*)GetDlgItem(IDC_OneCall);
	CButton* Radio_MultiC = (CButton*)GetDlgItem(IDC_MultiCall);*/

	TCHAR szFilter[] = _T("(*.txt)|*.txt|(*.bin)|*.bin|");

	/* 构造打开文件对话框 */
	CFileDialog fileDlg(FALSE, _T("txt"), NULL, OFN_HIDEREADONLY | OFN_OVERWRITEPROMPT, szFilter, this);
	CString fileName;
	if (IDOK == fileDlg.DoModal())
	{
		fileName = fileDlg.GetPathName();
		int reT = -1;
		//屏蔽 peter 20231001
		/*
		if (Radio_OneC->GetCheck())
		{
			int mCamId = 0;
			if (((CButton*)GetDlgItem(IDC_SaveCamA))->GetCheck())
			{
				mCamId = 0;
			}
			else if (((CButton*)GetDlgItem(IDC_SaveCamB))->GetCheck())
			{
				mCamId = 1;
			}
			if (_callOneTimes)
			{
				reT = _callOneTimes->saveEncoderData(fileName, mCamId);
			}
		}
		if (Radio_MultiC->GetCheck())
		{
			if (_callMultiTimes)
			{
				reT = _callMultiTimes->saveEncoderData(fileName);
			}
		}*/
		//if (Radio_Infi->GetCheck())
		if (Radio_Infi)
		{
			if (_infiniteL)
			{
				reT = _infiniteL->saveEncoderData(fileName);
			}
		}

		if (reT < 0)
		{
			AfxMessageBox(_T("保存失败!"), MB_OK | MB_ICONQUESTION);
		}
		else
		{
			CString strInfo = _T("");
			strInfo += _T("编码器数据保存路径：");
			strInfo += _T("\r\n");
			strInfo += fileName;
			InfoShow(strInfo);

			AfxMessageBox(_T("保存完成!"), MB_OK | MB_ICONQUESTION);
		}
	}
}


// Add by peter 20231001 3D相机参数
void CDetec3DsrDlg::OnBnClickedBtnSetset()
{
	if (!bConnectFlag)
	{
		InfoShow(_T("相机未连接"));
		return;
	}
	if (b_BatchStartFlag)
	{
		AfxMessageBox(_T("请先停止批处理!"), MB_OK | MB_ICONQUESTION);
		return;
	}
	SettingDialog setSettingDlg(TRUE);
	setSettingDlg.DoModal();
}


void CDetec3DsrDlg::OnBnClickedBtnGetset()
{
	if (!bConnectFlag)
	{
		InfoShow(_T("相机未连接"));
		return;
	}
	if (b_BatchStartFlag)
	{
		AfxMessageBox(_T("请先停止批处理!"), MB_OK | MB_ICONQUESTION);
		return;
	}
	SettingDialog getSettingDlg(FALSE);
	getSettingDlg.DoModal();
}


// Add by peter 20231001 3D相机配置
void CDetec3DsrDlg::OnBnClickedBtnLoad()
{
	if (!bConnectFlag)
	{
		InfoShow(_T("相机未连接"));
		return;
	}
	if (b_BatchStartFlag)
	{
		AfxMessageBox(_T("请先停止批处理!"), MB_OK | MB_ICONQUESTION);
		return;
	}

	/* 设置过滤 */
	TCHAR szFilter[] = _T("文本文件(*.ssp)|*.ssp|");

	/* 构造打开文件对话框 */
	CFileDialog fileDlg(TRUE, _T("ssp"), NULL, OFN_HIDEREADONLY | OFN_OVERWRITEPROMPT, szFilter, this);
	CString strFilePath;
	if (IDOK == fileDlg.DoModal())
	{
		strFilePath = fileDlg.GetPathName();

		int filesize = 0;
		CFileStatus fileStatus;
		if (CFile::GetStatus(strFilePath, fileStatus))
		{
			filesize = fileStatus.m_size;
		}
		else
		{
			InfoShow(_T("文件读取失败"));
			return;
		}
		char* SettingParamTmp = new char[filesize];

		/* 定义一个句柄 */
		HANDLE hFile;
		hFile = CreateFile(strFilePath,
			GENERIC_READ,
			0,
			NULL,
			OPEN_EXISTING,
			FILE_ATTRIBUTE_NORMAL,
			NULL);
		DWORD ret;
		ReadFile(hFile, SettingParamTmp, sizeof(char) * filesize, &ret, NULL);

		int reT = SR7IF_LoadParameters(DEVICEID, SettingParamTmp, filesize);
		if (reT < 0)
		{
			CString strInfo;
			strInfo.Format(_T("SR7IF_LoadParameters 失败，返回值:%d"), reT);
			InfoShow(strInfo);
		}
		else
		{
			InfoShow(_T("SR7IF_LoadParameters 成功"));
		}

		CloseHandle(hFile);
		delete[] SettingParamTmp;
		SettingParamTmp = NULL;
	}
}


void CDetec3DsrDlg::OnBnClickedBtnExport()
{
	if (!bConnectFlag)
	{
		InfoShow(_T("相机未连接"));
		return;
	}
	if (b_BatchStartFlag)
	{
		AfxMessageBox(_T("请先停止批处理!"), MB_OK | MB_ICONQUESTION);
		return;
	}

	/* 设置过滤 */
	TCHAR szFilter[] = _T("文本文件(*.ssp)|*.ssp|");

	/* 构造打开文件对话框 */
	CFileDialog fileDlg(FALSE, _T("ssp"), NULL, OFN_HIDEREADONLY | OFN_OVERWRITEPROMPT, szFilter, this);
	CString strFilePath;
	if (IDOK == fileDlg.DoModal())
	{
		strFilePath = fileDlg.GetPathName();

		unsigned int filesize = 0;
		char* SettingParam = (char*)SR7IF_ExportParameters(DEVICEID, &filesize);

		if (SettingParam == NULL)
		{
			InfoShow(_T("SR7IF_ExportParameters 失败"));
			return;
		}

		//保存为文件
		/* 定义一个句柄 */
		HANDLE hFile;
		hFile = CreateFile(strFilePath,
			GENERIC_WRITE,
			0,
			NULL,
			CREATE_ALWAYS,
			FILE_FLAG_WRITE_THROUGH,
			NULL);

		/* 定义一个变量来接收实际写入的字符数 */
		DWORD realWriteSize;
		WriteFile(hFile, SettingParam, sizeof(char) * filesize, &realWriteSize, NULL);
		CloseHandle(hFile); /*关闭文件 关闭句柄。*/

		if (realWriteSize != filesize)
		{
			CString strTmp;
			strTmp.Format(_T("配置总字节数:%d"), filesize);

			CString strInfo = _T("");
			strInfo += _T("SR7IF_ExportParameters 成功");
			strInfo += _T("\r\n");
			strInfo += strTmp;
			strInfo += _T("\r\n");
			strTmp.Format(_T("实际写入文件字节数:%d"), realWriteSize);
			strInfo += strTmp;
			InfoShow(strInfo);
		}
		else
		{
			CString strInfo = _T("");
			strInfo += _T("SR7IF_ExportParameters 成功");
			strInfo += _T("\r\n");
			strInfo += _T("配置文件导出完成，路径：");
			strInfo += strFilePath;
			InfoShow(strInfo);
		}
	}
}


void CDetec3DsrDlg::OnBnClickedBtnOther()
{
	otherSampleDlg->setConnectFlag(bConnectFlag);
	otherSampleDlg->DoModal();
}


//显示3D图像， peter 20231001
void CDetec3DsrDlg::OnBnClickedBtn3dshow()
{
	CString str_z, str_y;
	// IDC_EDIT_ZScale 和 IDC_EDIT_YInter 这两个ID资源是原界面3D显示参数 Z向倍数和Y向间距
	// 这个地方也是要改到文本文档里面去，peter 20231001
	// 下限和上限从 config.txt 读取第4行和第5行 获取
	//((CEdit*)GetDlgItem(IDC_EDIT_ZScale))->GetWindowTextW(str);

	//peter 20231002 添加：以下为读取txt文本文件的第4行，并将读取值赋给z_scale
	CStdioFile file;
	if (file.Open(_T("config.txt"), CFile::modeRead))//此处为读取文档路径
	{
		CString line;
		int lineCount = 0;
		while (file.ReadString(line))
		{
			if (lineCount == 3) // 第四行的索引是 3
			{
				str_z = line;
				break;
			}
			lineCount++;
		}
		file.Close();
	}//循环的功能是为了读取txt文本的第4行
	DWORD dwNum = WideCharToMultiByte(CP_OEMCP, NULL, str_z, -1, NULL, NULL, 0, NULL);
	char* c = new char[dwNum];
	WideCharToMultiByte(CP_OEMCP, NULL, str_z, -1, c, dwNum, 0, NULL);
	double z_scale = atof(c);
	delete[]  c;
	c = NULL;

	//((CEdit*)GetDlgItem(IDC_EDIT_YInter))->GetWindowTextW(str);

	//peter 20231002 添加：以下为读取txt文本文件的第5行，并将读取值赋给y_true_step
	if (file.Open(_T("config.txt"), CFile::modeRead))//此处为读取文档路径
	{
		CString line;
		int lineCount = 0;
		while (file.ReadString(line))
		{
			if (lineCount == 4) // 第五行的索引是 4
			{
				str_y = line;
				break;
			}
			lineCount++;
		}
		file.Close();
	}//循环的功能是为了读取txt文本的第5行
	dwNum = WideCharToMultiByte(CP_OEMCP, NULL, str_y, -1, NULL, NULL, 0, NULL);
	char* cc = new char[dwNum];
	WideCharToMultiByte(CP_OEMCP, NULL, str_y, -1, cc, dwNum, 0, NULL);
	double y_true_step = atof(cc);
	delete[] cc;
	cc = NULL;

	double Ho = d_HeightUpper;
	double Lo = d_HeightLower;

	//modify by peter 20231001
	BOOL Radio_Infi = TRUE;
	/*CButton* Radio_Infi = (CButton*)GetDlgItem(IDC_InfinLoop);
	CButton* Radio_OneC = (CButton*)GetDlgItem(IDC_OneCall);
	CButton* Radio_MultiC = (CButton*)GetDlgItem(IDC_MultiCall);*/
	//屏蔽 peter 20231001
	/*
	if (Radio_OneC->GetCheck())
	{
		if (_callOneTimes)
		{
			int mCamId = 0;
			if (((CButton*)GetDlgItem(IDC_CamA))->GetCheck())
			{
				mCamId = 0;
			}
			if (((CButton*)GetDlgItem(IDC_CamB))->GetCheck())
			{
				mCamId = 1;
			}

			if (_callOneTimes->getBatchData(mCamId)
				&& _callOneTimes->getProfileWidth() != 0
				&& _callOneTimes->getBatchHeight() != 0)
			{
				SR_3D_EXE_Show(_callOneTimes->getBatchData(mCamId),
					_callOneTimes->getXpitch(),
					y_true_step,
					_callOneTimes->getProfileWidth(),
					_callOneTimes->getBatchHeight(),
					z_scale,
					Ho,
					Lo);
			}
			else
			{
				AfxMessageBox(_T("无可用数据!"), MB_OK | MB_ICONQUESTION);
			}
		}
	}
	if (Radio_MultiC->GetCheck())
	{
		if (_callMultiTimes)
		{
			if (_callMultiTimes->getBatchData()
				&& _callMultiTimes->getProfileWidth() != 0
				&& _callMultiTimes->getBatchHeight() != 0)
			{
				SR_3D_EXE_Show(_callMultiTimes->getBatchData(),
					_callMultiTimes->getXpitch(),
					y_true_step,
					_callMultiTimes->getProfileWidth(),
					_callMultiTimes->getBatchHeight(),
					z_scale,
					Ho,
					Lo);
			}
			else
			{
				AfxMessageBox(_T("无可用数据!"), MB_OK | MB_ICONQUESTION);
			}
		}
	}*/
	//if (Radio_Infi->GetCheck())
	if (Radio_Infi)
	{
		if (b_BatchStartFlag)
		{
			AfxMessageBox(_T("请先停止批处理"), MB_OK | MB_ICONQUESTION);
			return;
		}
		if (_infiniteL)
		{
			if (_infiniteL->getBatchData()
				&& _infiniteL->getProfileWidth() != 0
				&& _infiniteL->getBatchHeight() != 0)
			{
				/********3D显示****************
				****_BatchData： 批处理数据***********
				****x_true_step：x方向间矩/mm*********
				****y_true_step：y方向间距/mm**
				****x_validPoint_num：x方向数据个数***
				****y_batchPoint_num：批处理行数***
				****z_scale：z方向缩放系数********
				****Ho：z方向最大值*****
				****Lo：z方向最小值*****/
				SR_3D_EXE_Show(_infiniteL->getBatchData(),
					_infiniteL->getXpitch(),
					y_true_step,
					_infiniteL->getProfileWidth(),
					_infiniteL->getBatchHeight(),
					z_scale,
					Ho,
					Lo);
				//Add by wcy 20231116
				/*SR_3D_Show(_infiniteL->getBatchData(),
					_infiniteL->getXpitch(),
					y_true_step,
					_infiniteL->getProfileWidth(),
					_infiniteL->getBatchHeight(),
					z_scale,
					Ho,
					Lo
				);*/
				//Add by peter 20231002 上面的函数把窗口封装到了dll里面，完全没法操作
				//只能使用下面的保存函数，先存为png格式的图，再贴到第三个窗口控件上 IDC_IMAGE_3D
				//Add by wcy 20231026 暂时将1905行至1925行注释
				/*int save3d_png;
				CString str_save3d_file;

				//获取当前的系统时间
				CTime currentTime = CTime::GetCurrentTime();
				CString strTime;
				strTime.Format(_T("%04d-%02d-%02d-%02d-%02d-%02d"),
					currentTime.GetYear(), currentTime.GetMonth(), currentTime.GetDay(),
					currentTime.GetHour(), currentTime.GetMinute(), currentTime.GetSecond());
				CString strPath = _T("Save3Dfile/");
				CString strType = _T(".png");
				str_save3d_file = strPath + strTime+ strType;//"Save3Dfile/2023-10-02-17-37-48.png"

				DWORD dwNum = WideCharToMultiByte(CP_OEMCP, NULL, str_save3d_file, -1, NULL, NULL, 0, NULL);
				char* save3d_file = new char[dwNum];
				WideCharToMultiByte(CP_OEMCP, NULL, str_save3d_file, -1, save3d_file, dwNum, 0, NULL);
				
//				save3d_png = SR_3D_EXE_SaveCurScreen(save3d_file);

				delete[]  save3d_file;
				save3d_file = NULL;		*/		
			}
			else
			{
				AfxMessageBox(_T("无可用数据!"), MB_OK | MB_ICONQUESTION);
			}
		}
	}
}

//增补功能函数，peter 20231002 ----------------------------------
void CDetec3DsrDlg::InitConfigBeforeDisConnect()
{
	if (b_BatchStartFlag)
	{
		//停止批处理
		SR7IF_StopMeasure(DEVICEID);

		b_BatchStartFlag = false;
	}
	/*
	CButton* Radio_Infi = (CButton*)GetDlgItem(IDC_InfinLoop);
	CButton* Radio_OneC = (CButton*)GetDlgItem(IDC_OneCall);
	CButton* Radio_MultiC = (CButton*)GetDlgItem(IDC_MultiCall);
	if (Radio_OneC->GetCheck()) {
		InitCallOneTimesBeforeDisConnect();
	}
	if (Radio_MultiC->GetCheck()){
		InitCallMultiTimesBeforeDisConnect();
	}*/
	//modify by peter 20231002
	BOOL Radio_Infi = TRUE;
	//if (Radio_Infi->GetCheck())
	if (Radio_Infi)
	{
		InitInfiniteBeforeDisConnect();
		//((CStatic*)GetDlgItem(IDC_STATIC_CountLine))->SetWindowTextW(_T("当前总行数："));
		//统一放到 IDC_EDIT_Info 对话框，peter 20231002
		((CEdit*)GetDlgItem(IDC_EDIT_Info))->LineScroll(((CEdit*)GetDlgItem(IDC_EDIT_Info))->GetLineCount());

	}
}

void CDetec3DsrDlg::InitInfiniteBeforeDisConnect()
{
	if (_infiniteL)
	{
		//退出数据接收线程
		_infiniteL->exitReceiveThread();

		//数据缓存释放
		_infiniteL->deleteDataMemory();
	}
}

void CDetec3DsrDlg::BtnEnableCallMultiAndLoop(bool bEnalbe)
{
	((CButton*)GetDlgItem(IDC_BTN_StartM))->EnableWindow(bEnalbe);
	//((CButton*)GetDlgItem(IDC_BTN_StartM_IO))->EnableWindow(bEnalbe);
	((CButton*)GetDlgItem(IDC_BTN_StopM))->EnableWindow(!bEnalbe);
	/*
	CButton* Radio_MultiC = (CButton*)GetDlgItem(IDC_MultiCall);
	if (Radio_MultiC->GetCheck()){
		((CButton*)GetDlgItem(IDC_HeightI))->EnableWindow(bEnalbe);
		((CButton*)GetDlgItem(IDC_GrayI))->EnableWindow(bEnalbe);
	}*/
}