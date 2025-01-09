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
	afx_msg void OnShowWindow(BOOL bShow, UINT nStatus);   //�ؼ���ʾ
	afx_msg void OnBnClickedBtnSet();                      //ȷ����ť��Ӧ

// Dialog Data
	enum { IDD = IDD_SETTING_DIALOG };

protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV support

	DECLARE_MESSAGE_MAP()

public:
	BOOL m_isDispay;
	CString m_xvDepth;			//�趨����
	CString m_xvType;			//�趨����
	CString m_xvCategory;		//ָ������ 
	CString m_xvItem;           //ָ�������趨��Ŀ
	CString m_xvTarget1;		//�趨Ŀ��1
	CString m_xvTarget2;		//�趨Ŀ��2
	CString m_xvTarget3;		//�趨Ŀ��3
	CString m_xvTarget4;		//�趨Ŀ��4
	CString m_xvParameters;		//�������
	int m_xvDataAmount;			//��������ֽ���

	vector<BYTE> m_vecSendData;		//�洢�����趨������
};
