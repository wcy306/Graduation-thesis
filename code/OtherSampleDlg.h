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
	BOOL OtherSampleDlg::PreTranslateMessage(MSG* pMsg)   //控件ENTER响应
	{
		if (pMsg->message == WM_KEYDOWN && pMsg->wParam == VK_ESCAPE) return TRUE;
		if (pMsg->message == WM_KEYDOWN && pMsg->wParam == VK_RETURN) return TRUE;
		else
			return CDialog::PreTranslateMessage(pMsg);
	}

	///
    /// \brief SingleProfileDataDisplay   单条轮廓转图像显示
    /// \param _data                      轮廓数据
    /// \param _width                     轮廓宽度
    ///
    void SingleProfileDataDisplay(int* _data, int _width);

private:
	bool bConnectFlag;             //相机连接标志
	double DispRange_Height;       //高度显示范围---用于单条轮廓显示

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
