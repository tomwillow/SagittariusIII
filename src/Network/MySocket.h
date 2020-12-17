#pragma once
//windows.h���������winsock2.h�ĺ��棬���򱨴�

#include <memory>
#include <sstream>
#include <string>
#include <functional>

#define FD_SETSIZE 1024
#include <thread>

#include <winsock2.h>

class MySocket
{
public:
	MySocket()throw (std::runtime_error);
	MySocket(SOCKET s);
	~MySocket();

	//�������������bind����listenʧ�ܣ����׳��쳣
	void Listen(unsigned short port) throw (std::runtime_error);

	//��������ѭ��
	void StartListenLoop(std::function<void(MySocket & client, std::string ip)> fn);

	//��������ѭ��
	//�׳��쳣����������������getpeernameʧ��
	void StartSelectModeListenLoop(std::function<void(MySocket & client, std::string ip)> fn,std::function<void(std::exception e)> fnIfException)throw ();

	//�����첽����ѭ��
	void StartListenLoopAsync(std::function<void(MySocket & client, std::string ip)> fn, std::function<void(std::exception e)> fnIfException)throw ();

	//ֹͣ�첽����ѭ��
	void StopListenLoopAsync();

	//����ָ��ip���˿�
	//�׳��쳣������ʧ��
	void Connect(std::string ip, unsigned short port)throw (std::runtime_error);

	//��������
	void Send(const char* data, int len, int flags = 0)throw (std::runtime_error);
	void Send(const std::string s, int flags = 0)throw (std::runtime_error);
	void SendWithLen(const char *data, uint32_t len, int flags = 0)throw (std::runtime_error);//len����������Ĵ�С
	void SendWithLen(const std::string& data, int flags = 0)throw (std::runtime_error);//len����������Ĵ�С

	//���ý��ճ�ʱ
	void SetRecvTimeOut(int ms)throw ();

	//��������
	uint32_t RecvUINT(int flags = 0)throw (std::runtime_error);
	void RecvWithLen(char *&data,int flags = 0)throw (std::runtime_error);//����ǰ�豣֤data==nullptr���ɵ����߸����ͷ�  len����������Ĵ�С
	std::string RecvWithLen(int flags = 0)throw (std::runtime_error);//len����������Ĵ�С
	std::string RecvWithEndMark(const char* end_mark, int bufsize = 1024, int flags = 0)throw (std::runtime_error);

	void Close();

protected:
	SOCKET m_socket;

	using FnSend = std::function<int(SOCKET, const char*, int, int)>;//int(__stdcall*)(SOCKET, const char*, int, int);
	using FnRecv = std::function<int(SOCKET, char*, int, int)>;
	FnSend fn_send;
	FnRecv fn_recv;
	MySocket(FnSend in_fn_send, FnRecv in_fn_recv);

private:
	bool stopListenLoop;
	//bool listenLoopIsRunning;
	std::thread threadListen;

	void CreateSocket();
};

//��������Ϊip
std::string DnsParse(const std::string& domain)throw (std::runtime_error);

//����У���
USHORT CheckSum(USHORT* buffer, int size) throw ();

bool CheckIP(std::string sip) throw ();

bool CheckNum(std::string s, int a, int b) throw ();

bool CheckPort(std::string s) throw ();

bool CheckName(std::string sip) throw ();

bool CheckEmailAddress(std::string addr) throw ();