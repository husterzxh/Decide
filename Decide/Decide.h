
// Decide.h : PROJECT_NAME Ӧ�ó������ͷ�ļ�
//

#pragma once

#ifndef __AFXWIN_H__
	#error "�ڰ������ļ�֮ǰ������stdafx.h�������� PCH �ļ�"
#endif

#include "resource.h"		// ������
#include "ServerSocket.h"
#include "ClientSocket.h"
#include "MSG.h"
#include <vector>
using namespace std;

// CDecideApp: 
// �йش����ʵ�֣������ Decide.cpp
//

class CDecideApp : public CWinApp
{
private:
	CServerSocket*m_Socket;
public:
	CDecideApp();

// ��д
public:
	virtual BOOL InitInstance();
	CString m_IP;		//����IP
	UINT m_Port;		//�����˿�
	CString groupIP;	//Ⱥ�ķ���IP
	UINT groupPort;		//Ⱥ�ķ���˿�
	vector<IPINFO> IPList;	//IP����
	int type;			//gossipͬ��������
	int version;		//�汾��
// ʵ��

	CServerSocket* GetServerSocket() const;
	DECLARE_MESSAGE_MAP()
	virtual int ExitInstance();
	BOOL WChar2MByte(LPCWSTR srcBuff, LPSTR destBuff, int nlen);
	static UINT gossip(LPVOID lpParam);
	char* prepareMsg(int type);
public:
	void setVersion(int version);
	void VersionCompare(int receive_version, CClientSocket *socket);
};

extern CDecideApp theApp;
