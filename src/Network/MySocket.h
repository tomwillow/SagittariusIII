#pragma once
//windows.h必须包含在winsock2.h的后面，否则报错

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

	//启动监听，如果bind或者listen失败，将抛出异常
	void Listen(unsigned short port) throw (std::runtime_error);

	//启动监听循环
	void StartListenLoop(std::function<void(MySocket & client, std::string ip)> fn);

	//启动监听循环
	//抛出异常：连接数量超出、getpeername失败
	void StartSelectModeListenLoop(std::function<void(MySocket & client, std::string ip)> fn,std::function<void(std::exception e)> fnIfException)throw ();

	//启动异步监听循环
	void StartListenLoopAsync(std::function<void(MySocket & client, std::string ip)> fn, std::function<void(std::exception e)> fnIfException)throw ();

	//停止异步监听循环
	void StopListenLoopAsync();

	//连接指定ip及端口
	//抛出异常：连接失败
	void Connect(std::string ip, unsigned short port)throw (std::runtime_error);

	//发送数据
	void Send(const char* data, int len, int flags = 0)throw (std::runtime_error);
	void Send(const std::string s, int flags = 0)throw (std::runtime_error);
	void SendWithLen(const char *data, uint32_t len, int flags = 0)throw (std::runtime_error);//len不包括自身的大小
	void SendWithLen(const std::string& data, int flags = 0)throw (std::runtime_error);//len不包括自身的大小

	//设置接收超时
	void SetRecvTimeOut(int ms)throw ();

	//接收数据
	uint32_t RecvUINT(int flags = 0)throw (std::runtime_error);
	void RecvWithLen(char *&data,int flags = 0)throw (std::runtime_error);//调用前需保证data==nullptr，由调用者负责释放  len不包括自身的大小
	std::string RecvWithLen(int flags = 0)throw (std::runtime_error);//len不包括自身的大小
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

//解析域名为ip
std::string DnsParse(const std::string& domain)throw (std::runtime_error);

//计算校验和
USHORT CheckSum(USHORT* buffer, int size) throw ();

bool CheckIP(std::string sip) throw ();

bool CheckNum(std::string s, int a, int b) throw ();

bool CheckPort(std::string s) throw ();

bool CheckName(std::string sip) throw ();

bool CheckEmailAddress(std::string addr) throw ();