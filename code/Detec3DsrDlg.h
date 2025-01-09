
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
    BOOL CDetec3DsrDlg::PreTranslateMessage(MSG* pMsg)   //�ؼ�ENTER��Ӧ����ɶ�õģ���peter 20231002
    {
        if (pMsg->message == WM_KEYDOWN && pMsg->wParam == VK_ESCAPE) return TRUE;
        if (pMsg->message == WM_KEYDOWN && pMsg->wParam == VK_RETURN) return TRUE;
        else
            return CDialog::PreTranslateMessage(pMsg);
    }
    
    void VariableInit();        //������ʼ��
    void InterfaceCtrlInit();   //����ؼ���ʼ��
    void OldBnClickedInfinloop();  		//����ѭ��ѡ��ť��Ӧ
    //brief GetLastIpAddress   ��ȡ�ϴ����ӵ�IP
    //param ip                 IP��ַָ��
    void GetLastIpAddress(int* ip);
    //brief saveIpAddress				����IP��ַ
    //param _SREthernetConFig			IP���ò���
    void saveIpAddress(SR7IF_ETHERNET_CONFIG _SREthernetConFig);
    //brief getHeightUpperLower �߶�������ʾ�����޻�ȡ
    //param _upper              ����
    //param _lower              ����
    void getHeightUpperLower(double& _upper, double& _lower);
    //brief InfiniteLoop_DataShowFunc    ����ѭ��������ʾ
    //param _width                       ��
    //param _height                      ��
    void InfiniteLoop_DataShowFunc(int _width, int _height);
    /// \brief GrayDataShow  �Ҷ�(����)ͼ��ʾ
    /// \param _grayData     �Ҷ����ݻ���
    /// \param img_w         ͼ����-----�������
    /// \param img_h         ͼ��߶�-----����������
    /// \param _xscale       ͼ�������ű���
    /// \param _yscale       ͼ��߶����ű���
    /// \param _scaleW       ����ͼ���
    /// \param _scaleH       ����ͼ���
    void GrayDataShow(unsigned char* _grayData,
        int img_w,
        int img_h,
        int _xscale,
        int _yscale,
        int _scaleW,
        int _scaleH);
    /// \brief BatchDataShow  �߶�ͼ��ʾ
   /// \param _BatchData     �߶����ݻ���
   /// \param max_height     �߶���ʾ��������
   /// \param min_height     �߶���ʾ��������
   /// \param _ColorMax      �߶����䷶Χ��Ӧ�ĻҶ����ֵ 255
   /// \param img_w          ͼ����-----�������
   /// \param img_h          ͼ��߶�-----����������
   /// \param _xscale        ͼ�������ű���
   /// \param _yscale        ͼ��߶����ű���
   /// \param _scaleW        ����ͼ���
   /// \param _scaleH        ����ͼ���
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

    void InfoShow(CString _text);//brief InfoShow ��Ϣ��ʾ
   
    void InitConfigBeforeDisConnect();//�Ͽ�����ǰ�Ĵ���  
    void InitInfiniteBeforeDisConnect();//�Ͽ� ����ѭ������

    void BtnEnableCallMultiAndLoop(bool bEnalbe);//��λص�����Чѭ��ģʽ�°�ť״̬�仯


   
private:
    bool bConnectFlag;                      //���ӱ�־
    bool b_HighSpeedInitFail;               //��λص���������ͨ�ų�ʼ��ʧ�ܱ�־
    bool b_BatchStartFlag;                  //��ʼ�������־
    double d_HeightUpper;                   //�߶�������ʾ����
    double d_HeightLower;                   //�߶�������ʾ����
    int mCurDispType;                       //��ǰ��ʾ���� 0:�߶� 1:�Ҷ�
    int mCurCamId;                          //��ǰѡ����ʾ�����
    
    //CallOneTimes* _callOneTimes;            //һ���Իص�
    //CallMultiTimes* _callMultiTimes;        //��λص�
    InfiniteLoop* _infiniteL;               //����ѭ��
    OtherSampleDlg* otherSampleDlg;			//�����ӿڵ��öԻ���

public:
    static void ReceiveMessage(void* pWindow, CString _text,
        int _width,int _height,int _id);					//�ص���ʾ��Ϣ
    void DetalMessage(CString _text, 
        int _width,int _height,int _id);                    //��Ϣ��ʾ

};
