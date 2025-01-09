
// Detec3DsrDlg.h: header file
//Add by peter 20231002
#include "OtherSampleDialog/OtherSampleDlg.h"
//#include "CallBack_OneTimes/CallOneTimes.h"
//#include "CallBack_MultipleTimes/CallMultiTimes.h"
#include "InfiniteLoop/InfiniteLoop.h"
#include "SR_SdkDll/SR7Link.h"

#pragma once


// CDetec3DsrDlg dialog
class CDetec3DsrDlg : public CDialog
{
// Construction
public:
	CDetec3DsrDlg(CWnd* pParent = nullptr);	// standard constructor

// Dialog Data
#ifdef AFX_DESIGN_TIME
	enum { IDD = IDD_DETEC3DSR_DIALOG };
#endif

	protected:
	virtual void DoDataExchange(CDataExchange* pDX);	// DDX/DDV support


// Implementation
protected:
	HICON m_hIcon;

	// Generated message map functions
	virtual BOOL OnInitDialog();
	afx_msg void OnSysCommand(UINT nID, LPARAM lParam);
	afx_msg void OnPaint();
	afx_msg HCURSOR OnQueryDragIcon();
	DECLARE_MESSAGE_MAP()
public:
	afx_msg void OnBnClickedBtnConnect();
	afx_msg void OnBnClickedBtnDisconnect();
	afx_msg void OnBnClickedBtnStartm();
	afx_msg void OnBnClickedBtnStopm();
	afx_msg void OnBnClickedBtnClear();
	afx_msg void OnBnClickedBtnGrays();
	afx_msg void OnBnClickedBtnHeights();
	afx_msg void OnBnClickedBtnEncoders();
	afx_msg void OnBnClickedBtnSetset();
	afx_msg void OnBnClickedBtnGetset();
	afx_msg void OnBnClickedBtnLoad();
	afx_msg void OnBnClickedBtnExport();
	afx_msg void OnBnClickedBtnOther();
	afx_msg void OnBnClickedBtn3dshow();

//Add by peter 20231002
private:
    BOOL CDetec3DsrDlg::PreTranslateMessage(MSG* pMsg)   //控件ENTER响应，干啥用的？？peter 20231002
    {
        if (pMsg->message == WM_KEYDOWN && pMsg->wParam == VK_ESCAPE) return TRUE;
        if (pMsg->message == WM_KEYDOWN && pMsg->wParam == VK_RETURN) return TRUE;
        else
            return CDialog::PreTranslateMessage(pMsg);
    }
    
    void VariableInit();        //参数初始化
    void InterfaceCtrlInit();   //界面控件初始化
    void OldBnClickedInfinloop();  		//无限循环选择按钮响应
    //brief GetLastIpAddress   获取上次连接的IP
    //param ip                 IP地址指针
    void GetLastIpAddress(int* ip);
    //brief saveIpAddress				保存IP地址
    //param _SREthernetConFig			IP配置参数
    void saveIpAddress(SR7IF_ETHERNET_CONFIG _SREthernetConFig);
    //brief getHeightUpperLower 高度区间显示上下限获取
    //param _upper              上限
    //param _lower              下限
    void getHeightUpperLower(double& _upper, double& _lower);
    //brief InfiniteLoop_DataShowFunc    无限循环数据显示
    //param _width                       宽
    //param _height                      高
    void InfiniteLoop_DataShowFunc(int _width, int _height);
    /// \brief GrayDataShow  灰度(亮度)图显示
    /// \param _grayData     灰度数据缓存
    /// \param img_w         图像宽度-----轮廓宽度
    /// \param img_h         图像高度-----批处理行数
    /// \param _xscale       图像宽度缩放倍数
    /// \param _yscale       图像高度缩放倍数
    /// \param _scaleW       缩放图像宽
    /// \param _scaleH       缩放图像高
    void GrayDataShow(unsigned char* _grayData,
        int img_w,
        int img_h,
        int _xscale,
        int _yscale,
        int _scaleW,
        int _scaleH);
    /// \brief BatchDataShow  高度图显示
   /// \param _BatchData     高度数据缓存
   /// \param max_height     高度显示区间上限
   /// \param min_height     高度显示区间下限
   /// \param _ColorMax      高度区间范围对应的灰度最大值 255
   /// \param img_w          图像宽度-----轮廓宽度
   /// \param img_h          图像高度-----批处理行数
   /// \param _xscale        图像宽度缩放倍数
   /// \param _yscale        图像高度缩放倍数
   /// \param _scaleW        缩放图像宽
   /// \param _scaleH        缩放图像高
    void BatchDataShow(int* _BatchData,
        double max_height,
        double min_height,
        int _ColorMax,
        int img_w,
        int img_h,
        int _xscale,
        int _yscale,
        int _scaleW,
        int _scaleH);
    //Add by wcy 20231116
    //void SR_3D_Show(const int* _BatchData,
    //    double x_true_step,
    //    double y_true_step,
    //    int x_Point_num,
    //    int y_batchPoint_num,
    //    double z_scale,
    //    double Ho,
    //    double Lo
    //);

    void InfoShow(CString _text);//brief InfoShow 信息显示
   
    void InitConfigBeforeDisConnect();//断开连接前的处理  
    void InitInfiniteBeforeDisConnect();//断开 无限循环设置

    void BtnEnableCallMultiAndLoop(bool bEnalbe);//多次回调和无效循环模式下按钮状态变化


   
private:
    bool bConnectFlag;                      //连接标志
    bool b_HighSpeedInitFail;               //多次回调高速数据通信初始化失败标志
    bool b_BatchStartFlag;                  //开始批处理标志
    double d_HeightUpper;                   //高度区间显示上限
    double d_HeightLower;                   //高度区间显示下限
    int mCurDispType;                       //当前显示类型 0:高度 1:灰度
    int mCurCamId;                          //当前选择显示的相机
    
    //CallOneTimes* _callOneTimes;            //一次性回调
    //CallMultiTimes* _callMultiTimes;        //多次回调
    InfiniteLoop* _infiniteL;               //无限循环
    OtherSampleDlg* otherSampleDlg;			//其他接口调用对话框

public:
    static void ReceiveMessage(void* pWindow, CString _text,
        int _width,int _height,int _id);					//回调显示信息
    void DetalMessage(CString _text, 
        int _width,int _height,int _id);                    //信息显示

};
