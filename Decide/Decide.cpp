
// Decide.cpp : ����Ӧ�ó��������Ϊ��
//

#include "stdafx.h"
#include "Decide.h"
#include "DecideDlg.h"
#include "LinkDlg.h"
#include "ServerSocket.h"
#include "cJSON.h"
#include <time.h>
using namespace std;
#ifdef _DEBUG
#define new DEBUG_NEW
#endif


// CDecideApp

BEGIN_MESSAGE_MAP(CDecideApp, CWinApp)
	ON_COMMAND(ID_HELP, &CWinApp::OnHelp)
END_MESSAGE_MAP()


// CDecideApp ����

CDecideApp::CDecideApp()
{
	// ֧����������������
	m_dwRestartManagerSupportFlags = AFX_RESTART_MANAGER_SUPPORT_RESTART;

	// TODO: �ڴ˴���ӹ�����룬
	// ��������Ҫ�ĳ�ʼ�������� InitInstance ��
}


// Ψһ��һ�� CDecideApp ����

CDecideApp theApp;


// CDecideApp ��ʼ��

BOOL CDecideApp::InitInstance()
{
	// ���һ�������� Windows XP �ϵ�Ӧ�ó����嵥ָ��Ҫ
	// ʹ�� ComCtl32.dll �汾 6 ����߰汾�����ÿ��ӻ���ʽ��
	//����Ҫ InitCommonControlsEx()��  ���򣬽��޷��������ڡ�
	INITCOMMONCONTROLSEX InitCtrls;
	InitCtrls.dwSize = sizeof(InitCtrls);
	// ��������Ϊ��������Ҫ��Ӧ�ó�����ʹ�õ�
	// �����ؼ��ࡣ
	InitCtrls.dwICC = ICC_WIN95_CLASSES;
	InitCommonControlsEx(&InitCtrls);

	CWinApp::InitInstance();

	if (!AfxSocketInit())
	{
		AfxMessageBox(IDP_SOCKETS_INIT_FAILED);
		return FALSE;
	}


	AfxEnableControlContainer();

	// ���� shell ���������Է��Ի������
	// �κ� shell ����ͼ�ؼ��� shell �б���ͼ�ؼ���
	CShellManager *pShellManager = new CShellManager;

	// ���Windows Native���Ӿ����������Ա��� MFC �ؼ�����������
	CMFCVisualManager::SetDefaultManager(RUNTIME_CLASS(CMFCVisualManagerWindows));

	// ��׼��ʼ��
	// ���δʹ����Щ���ܲ�ϣ����С
	// ���տ�ִ���ļ��Ĵ�С����Ӧ�Ƴ�����
	// ����Ҫ���ض���ʼ������
	// �������ڴ洢���õ�ע�����
	// TODO: Ӧ�ʵ��޸ĸ��ַ�����
	// �����޸�Ϊ��˾����֯��
	SetRegistryKey(_T("TALK"));
	
	m_Socket = new CServerSocket();
	if (!m_Socket)
	{
		AfxMessageBox(_T("��̬�����������׽��ֳ���"));
		return false;
	}
	CLinkDlg Linkdlg;
	INT_PTR nResponse = Linkdlg.DoModal();
	if (nResponse == IDOK)
	{
		// ���û�����IP�Ͷ˿���Ϣ�󣬴�������ָ���˿ڵķ������׽���
		if (!m_Socket->Create(theApp.m_Port))
		{
			AfxMessageBox(_T("�����׽��ִ���"));
			m_Socket->Close();
			return false;
		}
		if (!m_Socket->Listen())
		{
			AfxMessageBox(_T("����ʧ�ܣ�"));
			m_Socket->Close();
			return false;
		}
		// �ж��Ƿ����Ⱥ��
		if (!((m_IP.CompareNoCase(groupIP) == 0) && (m_Port == groupPort)))
		{
			CClientSocket *pClient = new CClientSocket();
			if (!pClient)
			{
				AfxMessageBox(_T("�ڴ治�㣡"));
				return false;
			}
			if (!pClient->Create())
			{
				AfxMessageBox(_T("�����׽���ʧ�ܣ�"));
				return false;
			}
			if (!pClient->Connect(theApp.groupIP.GetBuffer(0), theApp.groupPort))
			{
				AfxMessageBox(_T("����Ⱥ��ʧ�ܣ�"));
				return false;
			}
			MSGHEAD msg;
			cJSON *root = cJSON_CreateObject();
			if (!root)
			{
				AfxMessageBox(_T("Memory malloc error"));
				return false;
			}
			_bstr_t b(m_IP);
			char* ip = b;
			cJSON_AddStringToObject(root, "ip", ip);
			cJSON_AddNumberToObject(root, "port", m_Port);
			char *pBuf = cJSON_PrintUnformatted(root);

			msg.type = MSG_LOGIN;
			msg.length = strlen(pBuf);
			pClient->SendMSG(pBuf, &msg);

			cJSON_Delete(root);
			free(pBuf);

			pClient->Close();
			delete pClient;
		}
		else {
			_bstr_t b(m_IP);
			char* ip = b;
			IPINFO NewNode;
			strncpy(NewNode.ip, ip, 20);
			NewNode.port = m_Port;
			theApp.IPList.push_back(NewNode);
		}
		type = MSG_VERSION;
		version = -1;
		AfxBeginThread(gossip, this);
		CDecideDlg dlg;
		m_pMainWnd = &dlg;
		nResponse = dlg.DoModal();
		if (nResponse == IDCANCEL)
		{

		}
	}
	else if (nResponse == IDCANCEL)
	{
		// TODO: �ڴ˷��ô����ʱ��
		//  ��ȡ�������رնԻ���Ĵ���
	}
	else if (nResponse == -1)
	{
		TRACE(traceAppMsg, 0, "����: �Ի��򴴽�ʧ�ܣ�Ӧ�ó���������ֹ��\n");
		TRACE(traceAppMsg, 0, "����: ������ڶԻ�����ʹ�� MFC �ؼ������޷� #define _AFX_NO_MFC_CONTROLS_IN_DIALOGS��\n");
	}

	// ɾ�����洴���� shell ��������
	if (pShellManager != NULL)
	{
		delete pShellManager;
	}

#if !defined(_AFXDLL) && !defined(_AFX_NO_MFC_CONTROLS_IN_DIALOGS)
	ControlBarCleanUp();
#endif

	// ���ڶԻ����ѹرգ����Խ����� FALSE �Ա��˳�Ӧ�ó���
	//  ����������Ӧ�ó������Ϣ�á�
	return FALSE;
}

CServerSocket * CDecideApp::GetServerSocket() const
{
	return m_Socket;
}

int CDecideApp::ExitInstance()
{
	if (m_Socket)
	{
		delete m_Socket;
		m_Socket = NULL;
	}
	IPList.clear();
	return CWinApp::ExitInstance();
}

BOOL CDecideApp::WChar2MByte(LPCWSTR srcBuff, LPSTR destBuff, int nlen)
{
	int n = 0;
	n = WideCharToMultiByte(CP_OEMCP, 0, srcBuff, -1, destBuff, 0, 0, FALSE);
	if (n > nlen)
		return FALSE;

	WideCharToMultiByte(CP_OEMCP, 0, srcBuff, -1, destBuff, nlen, 0, FALSE);

	return TRUE;
}

UINT CDecideApp::gossip(LPVOID lpParam) {
	int num = 0, select = 0;
	while (1) {
		num = theApp.IPList.size();
		if (num == 0) {
			Sleep(1000);
			continue;
		}
		srand((unsigned)time(NULL));
		select = rand() % num;
		char *ip = theApp.IPList[select].ip;
		int port = theApp.IPList[select].port;
		CClientSocket *pClient = new CClientSocket();
		if (!pClient || !pClient->Create() || !pClient->Connect(CString(theApp.IPList[select].ip).GetBuffer(0), (UINT)theApp.IPList[select].port))
			continue;
		MSGHEAD msg;
		char* msg_toBsend = theApp.prepareMsg(theApp.type);
		if (!msg_toBsend)
			continue;
		msg.type = theApp.type;
		msg.length = strlen(msg_toBsend);
		pClient->SendMSG(msg_toBsend, &msg);
		Sleep(2000);
		pClient->Close();
		delete pClient;
	}
}

char* CDecideApp::prepareMsg(int type) {
	if (type == MSG_VERSION) {
		int num = IPList.size();
		int i = 0;
		cJSON *root = cJSON_CreateObject();
		cJSON_AddNumberToObject(root, "version", version);
		return cJSON_PrintUnformatted(root);
	}
	else if (type == MSG_VOTE) {

	}
	else {
		return NULL;
	}
}

void CDecideApp::setVersion(int version) {
	this->version = version;
}

void VersionCompare(int receive_version, CClientSocket *socket)
{
	//�յ��汾����ͬ
	if (theApp.version == receive_version)
	{
		return;
	}
	//�յ��汾��С�ڽ��յģ���Է�����list
	else if (theApp.version < receive_version)
	{
		MSGHEAD msg;
		msg.type = MSG_REQUEST;
		msg.length = 0;	//�����ں������Ϣ

		socket->SendMSG(NULL, &msg);
	}
	else
	{
		MSGHEAD msg;

		cJSON *json_root = cJSON_CreateObject();
		if (!json_root)
		{
			AfxMessageBox(_T("Memory malloc error"));
			return;
		}

		cJSON *root = cJSON_CreateArray();
		if (!root)
		{
			AfxMessageBox(_T("Memory malloc error"));
			return;
		}

		cJSON_AddNumberToObject(json_root, "version", theApp.version);
		int num = theApp.IPList.size();

		for (int i = 0; i < num; i++)
		{
			cJSON *temp = cJSON_CreateObject();
			cJSON_AddStringToObject(temp, "ip", theApp.IPList[i].ip);
			cJSON_AddNumberToObject(temp, "port", theApp.IPList[i].port);
			cJSON_AddItemToArray(root, temp);
		}
		cJSON_AddItemToObject(json_root, "data", root);

		char *pBuff = cJSON_PrintUnformatted(json_root);

		msg.type = MSG_LIST;
		msg.length = strlen(pBuff);

		socket->SendMSG(pBuff, &msg);

		cJSON_Delete(root);
		free(pBuff);
	}
}
