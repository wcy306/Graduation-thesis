#pragma once
#include "afxcmn.h"
#include "afxwin.h"

//MFC���ڽ����ָ��,ʹ��ʱ��ȷ����ָ��ָ��Ĵ��ڴ�������Ч,��������. id 0:�ı� 1��ͼ������
typedef void(*FUNC_RECEIVEBACK)(void *pWindow,
                                CString _text, 
								int _width,
								int _height,
								int _id);  
class InfiniteLoop 
{
public:
    InfiniteLoop(void);
    ~InfiniteLoop(void);

public:
	///
    /// \brief VariableInit             ������ʼ��
    ///
    void VariableInit();
	///
    /// \brief setDelayTime   �����ӳ���ʾʱ��
    /// \param _time
    ///
    void setDelayTime(int _time);
    ///
    /// \brief setProfileWidth  �����������
    /// \param _width
    ///
    void setProfileWidth(int _width);
    ///
    /// \brief setXPitch   ����X���
    /// \param _val
    ///
    void setXPitch(double _val) { mXinterVal = _val; }
    ///
    /// \brief setCamBonlineFlag  ���B���߱�־
    /// \param flag
    ///
    void setCamBonlineFlag(bool flag);
    ///
    /// \brief StartMeasure     ��ʼ������--��IO����
    /// \param iTimeout         ��ʱʱ��
    /// \return
    ///
    int StartMeasure(int iTimeout);
    ///
    /// \brief StartIOTriggerMeasure  ��ʼ������--IO����
    /// \param iTimeout         ��ʱʱ��
    /// \return
    ///
    int StartIOTriggerMeasure(int iTimeout);
    ///
    /// \brief startReceiveThread   �������������߳�
    ///
    void startReceiveThread();
    ///
    /// \brief exitReceiveThread    �˳����������߳�
    ///
    void exitReceiveThread();
    ///
    /// \brief DataMemoryInit          �����ڴ��ʼ��
    /// \param _BonLine                ���B�Ƿ�����
	/// \param mProW                   �������
    ///
    void DataMemoryInit(bool _BonLine, int mProW = 3200);
    ///
    /// \brief deleteDataMemory         �����ڴ��ͷ�
    ///
    void deleteDataMemory();
    ///
    /// \brief setDispHeight            ������ʾ�߶�
    /// \param _height
    ///
    void setDispHeight(int _height);
    ///
    /// \brief clearInfo               �����Ϣ
    ///
    void clearInfo();
    ///
    /// \brief saveHeightData      �߶����ݱ���
    /// \param _path               ·��
    /// \return                    0:�ɹ� <0:ʧ��
    ///
    int saveHeightData(CString _path);
    ///
    /// \brief saveIntensityData   �Ҷ����ݱ���
    /// \param _path               ·��
    /// \return                    0:�ɹ� <0:ʧ��
    ///
    int saveIntensityData(CString _path);
    ///
    /// \brief saveEncoderData     ���������ݱ���
    /// \param _path               ·��
    /// \return                    0:�ɹ� <0:ʧ��
    ///
    int saveEncoderData(CString _path);
    ///
    /// \brief set3DYinterval      ����3D��ʾY������
    /// \param _val
    ///
    void set3DYinterval(double _val) { mYinterVal = _val; }
    ///
    /// \brief getBatchData        ��ȡ����������
    /// \return                    ���ݵ�ַ
    ///
    int* getBatchData();
    ///
    /// \brief getIntensityData    ��ȡ����(�Ҷ�)����
    /// \return                    ����(�Ҷ�)���ݵ�ַ
    ///
    unsigned char* getIntensityData();
    ///
    /// \brief getXpitch           ��ȡX���
    /// \return
    ///
    double getXpitch() { return mXinterVal; }
    ///
    /// \brief getBatchHeight      ��ȡ����������
    /// \return
    ///
    int getBatchHeight();
    ///
    /// \brief getProfileWidth     ��ȡ�������
    /// \return
    ///
    int getProfileWidth();
    ///
    /// \brief ImageUpdate         ����ͼ��
    ///
    void ImageUpdate();

public:
	FUNC_RECEIVEBACK m_pFunReceiveback;					//ʾ������()
    void *m_pWindow;									//�ص���MFC���ڽ����ָ��(),ʹ��ʱ��ȷ����ָ��ָ��Ĵ��ڴ�������Ч

	void UserReceiveBack(FUNC_RECEIVEBACK pFun, void *pWindow)   //�û�����ʾ���ص�����()
    {
        m_pFunReceiveback = pFun;
        m_pWindow = pWindow;//MFC���ڽ����ָ��,ʹ��ʱ��ȷ����ָ��ָ��Ĵ��ڴ�������Ч
    }

private:
	static UINT ThreadImageDisplay(LPVOID pParam);            //�̺߳�������=---ͼ����ʾ
    UINT ImageDisplayFunc();                                  //ͼ����ʾʵ�ֺ���
	///
    /// \brief setTextInfo       ��Ϣ��ʾ
    /// \param _text
    ///
    void setTextInfo(CString _text);
	///
    /// \brief DataShow      ������ʾ
    ///
    void DataShow();

private:
	double mXinterVal;              //X���
    double mYinterVal;              //Y���
	bool bReceiveData;              //�������ݱ�־
    int mBatchWidth;                //�������
    bool bCamBonline;               //���B���߱�־
    int mDispH;                     //��ǰ��ʾ��Χ
    int* mProfileData;              //���������ݻ���--��ǰ��ʾ��Χ
    unsigned char* mIntensityData;  //�Ҷ����ݻ���
    unsigned int* mEncoderData;     //���������ݻ���
    CString strInfo;                //��Ϣ
    int mDelayTime;                 //�ӳ�ʱ��
};