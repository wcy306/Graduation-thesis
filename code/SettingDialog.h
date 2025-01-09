#pragma once

#include "../SR_SdkDll/SR7Link.h"
#include<vector>
using namespace std;

// SettingDialog dialog


class SettingDialog : public CDialog
{
	DECLARE_DYNAMIC(SettingDialog)

public:
	SettingDialog(BOOL isDisplay, CWnd* pParent = NULL);   // standard constructor
	virtual ~SettingDialog();

public:
	afx_msg void OnShowWindow(BOOL bShow, UINT nStatus);   //控件显示
	afx_msg void OnBnClickedBtnSet();                      //确定按钮响应

// Dialog Data
	enum { IDD = IDD_SETTING_DIALOG };

protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV support

	DECLARE_MESSAGE_MAP()

public:
	BOOL m_isDispay;
	CString m_xvDepth;			//设定级别
	CString m_xvType;			//设定类型
	CString m_xvCategory;		//指定种类 
	CString m_xvItem;           //指定种类设定项目
	CString m_xvTarget1;		//设定目标1
	CString m_xvTarget2;		//设定目标2
	CString m_xvTarget3;		//设定目标3
	CString m_xvTarget4;		//设定目标4
	CString m_xvParameters;		//输入参数
	int m_xvDataAmount;			//输入参数字节数

	vector<BYTE> m_vecSendData;		//存储发送设定的数据
};
