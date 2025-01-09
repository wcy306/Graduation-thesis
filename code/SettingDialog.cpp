// SettingDialog.cpp : implementation file
//


#include "../pch.h"
#include "../Detec3Dsr.h"
#include "SettingDialog.h"


// SettingDialog dialog

IMPLEMENT_DYNAMIC(SettingDialog, CDialog)

SettingDialog::SettingDialog(BOOL isDisplay, CWnd* pParent /*=NULL*/)
	: CDialog(SettingDialog::IDD, pParent)
	, m_isDispay(isDisplay)
	, m_xvDepth("1")
	, m_xvType(_T("10"))
	, m_xvCategory(_T("0"))
	, m_xvItem(_T("2"))
	, m_xvTarget1(_T("0"))
	, m_xvTarget2(_T("0"))
	, m_xvTarget3(_T("0"))
	, m_xvTarget4(_T("0"))
	, m_xvParameters(_T(""))
	, m_xvDataAmount(1)
{

}

SettingDialog::~SettingDialog()
{
}

void SettingDialog::DoDataExchange(CDataExchange* pDX)
{
	CDialog::DoDataExchange(pDX);
	DDX_Text(pDX, IDC_EDIT_DEPTH, m_xvDepth);
	DDX_Text(pDX, IDC_EDIT_TYPE, m_xvType);
	DDX_Text(pDX, IDC_EDIT_CATEGORY, m_xvCategory);
	DDX_Text(pDX, IDC_EDIT_ITEM, m_xvItem);
	DDX_Text(pDX, IDC_EDIT_Target1, m_xvTarget1);
	DDX_Text(pDX, IDC_EDIT_Target2, m_xvTarget2);
	DDX_Text(pDX, IDC_EDIT_Target3, m_xvTarget3);
	DDX_Text(pDX, IDC_EDIT_Target4, m_xvTarget4);
	DDX_Text(pDX, IDC_EDIT_PARAM, m_xvParameters);
	DDX_Text(pDX, IDC_EDIT_DATAAMOUNT, m_xvDataAmount);
	DDV_MaxChars(pDX, m_xvDepth, 2);
	DDV_MaxChars(pDX, m_xvType, 2);
	DDV_MaxChars(pDX, m_xvCategory, 2);
	DDV_MaxChars(pDX, m_xvItem, 2);
	DDV_MaxChars(pDX, m_xvTarget1, 2);
	DDV_MaxChars(pDX, m_xvTarget2, 2);
	DDV_MaxChars(pDX, m_xvTarget3, 2);
	DDV_MaxChars(pDX, m_xvTarget4, 2);
	DDV_MinMaxInt(pDX, m_xvDataAmount, 0, 32767);
	DDV_MaxChars(pDX, m_xvParameters, 30);
}


BEGIN_MESSAGE_MAP(SettingDialog, CDialog)
	ON_WM_SHOWWINDOW()
	ON_BN_CLICKED(IDC_BUTTON_OK, &SettingDialog::OnBnClickedBtnSet)
END_MESSAGE_MAP()



// SettingDialog message handlers

/*
 Displya only the necessary parameter
 @param Display flag
 @param Status
*/
void SettingDialog::OnShowWindow(BOOL bShow, UINT nStatus)
{
	CDialog::OnShowWindow(bShow, nStatus);

	if(!m_isDispay)
	{
		GetDlgItem(IDC_STATIC_Dh1)->ShowWindow(SW_HIDE);
		GetDlgItem(IDC_STATIC_NOTE)->ShowWindow(SW_HIDE);
		GetDlgItem(IDC_STATIC_Dh2)->ShowWindow(SW_HIDE);
		GetDlgItem(IDC_STATIC_Dh3)->ShowWindow(SW_HIDE);
		GetDlgItem(IDC_EDIT_DEPTH)->ShowWindow(SW_HIDE);
		CString title("获取设定");
		this->SetWindowText(title);
	}
	else
	{
		CString title("设置设定");
		this->SetWindowText(title);
	}
}

/*
 确定按钮响应
*/
void SettingDialog::OnBnClickedBtnSet()
{
	// TODO: Add your control notification handler code here
	TCHAR* pszEnd = NULL;
	CString strCode;

	int Target[4] = { 0 };
	((CEdit*)GetDlgItem(IDC_EDIT_Target1))->GetWindowTextW(strCode);
	Target[0] = (int)( ::_tcstol(strCode, &pszEnd, 16) );
	((CEdit*)GetDlgItem(IDC_EDIT_Target2))->GetWindowTextW(strCode);
    Target[1] = (int)( ::_tcstol(strCode, &pszEnd, 16) );
	((CEdit*)GetDlgItem(IDC_EDIT_Target3))->GetWindowTextW(strCode);
    Target[2] = (int)( ::_tcstol(strCode, &pszEnd, 16) );
	((CEdit*)GetDlgItem(IDC_EDIT_Target4))->GetWindowTextW(strCode);
    Target[3] = (int)( ::_tcstol(strCode, &pszEnd, 16) );

	((CEdit*)GetDlgItem(IDC_EDIT_TYPE))->GetWindowTextW(strCode);
	int Type = (int)( ::_tcstol(strCode, &pszEnd, 16) );

	((CEdit*)GetDlgItem(IDC_EDIT_DEPTH))->GetWindowTextW(strCode);
	int byDepth = (int)( ::_tcstol(strCode, &pszEnd, 16) );

	((CEdit*)GetDlgItem(IDC_EDIT_CATEGORY))->GetWindowTextW(strCode);
	int Category = (int)( ::_tcstol(strCode, &pszEnd, 16) );

	((CEdit*)GetDlgItem(IDC_EDIT_ITEM))->GetWindowTextW(strCode);
    int Item = (int)( ::_tcstol(strCode, &pszEnd, 16) );

	CString str;
	((CEdit*)GetDlgItem(IDC_EDIT_DATAAMOUNT))->GetWindowTextW(str);
	DWORD dwNum = WideCharToMultiByte(CP_OEMCP, NULL, str, -1, NULL, NULL, 0, NULL);
	char *c = new char[dwNum];
	WideCharToMultiByte(CP_OEMCP, NULL, str, -1, c, dwNum, 0, NULL);
	int dwDataSize = atoi(c);
	delete[]  c;

	vector<BYTE> vecData;

	if(m_isDispay)
	{
		//int 转 byte
		((CEdit*)GetDlgItem(IDC_EDIT_PARAM))->GetWindowTextW(str);
		DWORD dwNum = WideCharToMultiByte(CP_OEMCP, NULL, str, -1, NULL, NULL, 0, NULL);
		char *c = new char[dwNum];
		WideCharToMultiByte(CP_OEMCP, NULL, str, -1, c, dwNum, 0, NULL);
		int mData = atoi(c);
		delete[]  c;
		unsigned char* b = new unsigned char[4];
		for(int i = 0; i < 4; i++)
		{
			b[i] = (unsigned char)( mData >> (24 - i * 8));
		}

		for(int i = 0; i < dwDataSize; i ++)
		{
			vecData.push_back(b[3 - i]);
		}

		int reT = SR7IF_SetSetting(DEVICEID,
                               byDepth,
                               Type,
                               Category,
                               Item,
                               Target,
                               &vecData.at(0),
                               dwDataSize);
		if(reT < 0)
		{
			CString strInfo;
			strInfo.Format(_T("SR7IF_SetSetting 失败，返回值:%d"), reT);
			AfxMessageBox(strInfo, MB_OK | MB_ICONQUESTION);
		}
		else
		{
			AfxMessageBox(_T("SR7IF_SetSetting 成功"), MB_OK | MB_ICONQUESTION);
		}
	}
	else
	{
		for(int i = 0; i < dwDataSize; i++)
		{
			vecData.push_back(0);
		}

		int reT = SR7IF_GetSetting(DEVICEID,
                               Type,
                               Category,
                               Item,
                               Target,
                               &vecData.at(0),
                               dwDataSize);
		if(reT < 0)
		{
			CString strInfo;
			strInfo.Format(_T("SR7IF_GetSetting 失败，返回值:%d"), reT);
			((CEdit*)GetDlgItem(IDC_EDIT_PARAM))->SetWindowTextW(strInfo);
		}
		else
		{
			CString strLog ;
			CString m_strCommandLog;
			for(int i = dwDataSize - 1; i >= 0; i--)
			{
				strLog.Format(_T("%02X"), vecData.at(i));
				m_strCommandLog += (LPCTSTR) strLog;
			}

			int mResult = (int)( ::_tcstol(m_strCommandLog, &pszEnd, 16) );
			CString strInfo = _T("");
			strInfo.Format(_T("%d"), mResult);
			((CEdit*)GetDlgItem(IDC_EDIT_PARAM))->SetWindowTextW(strInfo);
		}
	}
}
