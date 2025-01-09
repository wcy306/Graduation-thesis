
// MFCApplication1.h : main header file for the PROJECT_NAME application
//

#pragma once

#ifndef __AFXWIN_H__
#error "include 'pch.h' before including this file for PCH"
#endif

#include "resource.h"		// main symbols

//Add by peter 20231002
#define DEVICEID 0
#define MAXHEIGHT 15000

#pragma pack(4)
typedef struct {
    unsigned int version;
    int width;
    int height;
    double xInterval;
    double yInterval;
    char info[32];
    int reserve2[2545];
}BATCH_INFO;    //确保10240字节
#pragma pack()

enum {
    BATCH_SAVE_FILE_VERSION = /*0x00000101*/0x00000002,
};


// CMFCApplication1App:
// See MFCApplication1.cpp for the implementation of this class
//

class CDetec3DsrApp : public CWinApp
{
public:
	CDetec3DsrApp();

// Overrides
public:
	virtual BOOL InitInstance();

// Implementation

	DECLARE_MESSAGE_MAP()
};

extern CDetec3DsrApp theApp;
