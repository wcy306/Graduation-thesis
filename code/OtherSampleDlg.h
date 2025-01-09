#pragma once


// OtherSampleDlg dialog

class OtherSampleDlg : public CDialog
{
	DECLARE_DYNAMIC(OtherSampleDlg)

public:
	OtherSampleDlg(CWnd* pParent = NULL);   // standard constructor
	virtual ~OtherSampleDlg();

public:
	void setUIInit();
	void setConnectFlag(bool flag) { bConnectFlag = flag; }
    void setHeightDispRange(double _val) { DispRange_Height = _val; }

// Dialog Data
	enum { IDD = IDD_OTHER_DIALOG };

protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV support
	virtual BOOL OnInitDialog();

	DECLARE_MESSAGE_MAP()

private:
	BOOL OtherSampleDlg::PreTranslateMessage(MSG* pMsg)   //�ؼ�ENTER��Ӧ
	{
		if (pMsg->message == WM_KEYDOWN && pMsg->wParam == VK_ESCAPE) return TRUE;
		if (pMsg->message == WM_KEYDOWN && pMsg->wParam == VK_RETURN) return TRUE;
		else
			return CDialog::PreTranslateMessage(pMsg);
	}

	///
    /// \brief SingleProfileDataDisplay   ��������תͼ����ʾ
    /// \param _data                      ��������
    /// \param _width                     �������
    ///
    void SingleProfileDataDisplay(int* _data, int _width);

private:
	bool bConnectFlag;             //������ӱ�־
	double DispRange_Height;       //�߶���ʾ��Χ---���ڵ���������ʾ

public:
	afx_msg void OnBnClickedGetProfile(); 
	afx_msg void OnBnClickedProgSw(); 
	afx_msg void OnBnClickedGetVersion(); 
	afx_msg void OnBnClickedGetModel(); 
	afx_msg void OnBnClickedCamBOnline(); 
	afx_msg void OnBnClickedGetHeadS(); 
	afx_msg void OnBnClickedRemainDay(); 
	afx_msg void OnBnClickedIoOutput(); 
	afx_msg void OnBnClickedIoRead(); 
	afx_msg void OnBnClickedGetError(); 
	afx_msg void OnBnClickedClearError(); 
	afx_msg void OnBnClickedClear(); 
	afx_msg void OnBnClickedGetEncoderV(); 
	afx_msg void OnBnClickedBtnGettemp();
};
