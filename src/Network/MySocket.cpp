#define _WINSOCK_DEPRECATED_NO_WARNINGS
#define _CRT_SECURE_NO_WARNINGS

#include "MySocket.h"

#include <WS2tcpip.h>

#include <iostream>

#include <memory>

#pragma comment(lib, "WS2_32.lib")

using namespace std;

MySocket::MySocket() :stopListenLoop(false),fn_recv(&recv),fn_send(&send)
{
	CreateSocket();
}

MySocket::MySocket(SOCKET s) :stopListenLoop(false), fn_recv(&recv), fn_send(&send)
{
	m_socket = s;
}


MySocket::~MySocket()
{
	Close();
}

void MySocket::Listen(unsigned short port)
{
	sockaddr_in svraddr;
	svraddr.sin_family = AF_INET;
	svraddr.sin_port = htons(port);
	svraddr.sin_addr.S_un.S_addr = INADDR_ANY;
	int ret = ::bind(m_socket, (const sockaddr*)&svraddr, sizeof(svraddr));
	if (ret == SOCKET_ERROR)
	{
		throw runtime_error("Failed bind(): port = " + to_string(port));
	}

	ret = listen(m_socket, 0);
	if (ret == SOCKET_ERROR)
	{
		throw runtime_error("Failed listen(): port = " + to_string(port));
	}

}

void MySocket::StartListenLoop(function<void(MySocket & client, std::string ip)> fn)
{
	sockaddr_in remoteAddr;
	int addrLen = sizeof(remoteAddr);
	while (stopListenLoop == false)
	{
		//接收到客户端的连接
		SOCKET sClient = accept(m_socket, (sockaddr*)&remoteAddr, &addrLen);
		if (sClient == INVALID_SOCKET)
		{
			cout << "Failed accept()." << endl;
			continue;
		}

		//创建新socket对象
		MySocket client(sClient);

		//得到ip地址
		string client_ip = inet_ntoa(remoteAddr.sin_addr);

		//调用传入函数
		fn(client, client_ip);

		//关闭连接
	}

	return;
}

void MySocket::StartSelectModeListenLoop(std::function<void(MySocket & client, std::string ip)> fn, std::function<void(std::exception e)> fnIfException)
{
	stopListenLoop = false;
	// select模型处理过程
	// 1）初始化一个套节字集合fdSocket，添加监听套节字句柄到这个集合
	fd_set fdSocket;		// 所有可用套节字集合
	FD_ZERO(&fdSocket);
	FD_SET(m_socket, &fdSocket);
	while (stopListenLoop == false)
	{
		// 2）将fdSocket集合的一个拷贝fdRead传递给select函数，
		// 当有事件发生时，select函数移除fdRead集合中没有未决I/O操作的套节字句柄，然后返回。
		fd_set fdRead = fdSocket;
		timeval timeout = { 1,0 };
		int nRet = ::select(0, &fdRead, NULL, NULL, &timeout);
		//if (nRet <= 0)
		//	throw runtime_error(" Failed select() \n");

		// 3）通过将原来fdSocket集合与select处理过的fdRead集合比较，
		// 确定都有哪些套节字有未决I/O，并进一步处理这些I/O。
		for (int i = 0; i < (int)fdSocket.fd_count; i++)
		{
			if (FD_ISSET(fdSocket.fd_array[i], &fdRead))
			{
				if (fdSocket.fd_array[i] == m_socket)		// （1）监听套节字接收到新连接
				{
					if (fdSocket.fd_count < FD_SETSIZE)
					{
						sockaddr_in remoteAddr;
						int nAddrLen = sizeof(remoteAddr);
						SOCKET sClient = ::accept(m_socket, (SOCKADDR*)&remoteAddr, &nAddrLen);
						FD_SET(sClient, &fdSocket);

						//printf("接收到连接（%s）\n", ::inet_ntoa(addrRemote.sin_addr));
					}
					else
					{
						fnIfException(runtime_error("Connections number is over limition."));
						continue;
					}
				}
				else
				{
					{
						sockaddr_in remoteAddr;
						socklen_t addrlen = sizeof(remoteAddr);
						if (getpeername(fdSocket.fd_array[i], (struct sockaddr*) & remoteAddr, &addrlen) == -1)
							fnIfException(runtime_error("Failed getpeername()."));

						//创建新socket对象
						MySocket client(fdSocket.fd_array[i]);

						//得到ip地址
						string client_ip = inet_ntoa(remoteAddr.sin_addr);

						//调用传入函数
						fn(client, client_ip);
					}
					FD_CLR(fdSocket.fd_array[i], &fdSocket);
				}
			}
		}
	}

}

void MySocket::StartListenLoopAsync(std::function<void(MySocket & client, std::string ip)> fn, std::function<void(std::exception e)> fnIfException)
{
	threadListen = thread(&MySocket::StartSelectModeListenLoop, this, fn, fnIfException);
}

void MySocket::StopListenLoopAsync()
{
	stopListenLoop = true;
	if (threadListen.joinable())
		threadListen.join();
}

void MySocket::Connect(string ip, unsigned short port)
{
	sockaddr_in svraddr;
	svraddr.sin_family = AF_INET;
	svraddr.sin_addr.s_addr = inet_addr(ip.c_str());
	svraddr.sin_port = htons(port);
	int ret = connect(m_socket, (struct sockaddr*) & svraddr, sizeof(svraddr));
	if (ret == SOCKET_ERROR)
	{
		throw runtime_error("Failed connect(): ip = " + ip + " port = " + to_string(port));
	}
}

void MySocket::SendWithLen(const char* data, uint32_t len, int flags)
{
	uint32_t net_len = htonl(len);
	int bytes = fn_send(m_socket, (char*)&net_len, sizeof(net_len), flags);
	if (bytes <= 0)
	{
		throw runtime_error("Send fail.");
	}

	uint32_t pos = 0;
	while (pos < len)
	{
		int bytes = fn_send(m_socket, &data[pos], len - pos, flags);
		if (bytes <= 0)
		{
			throw runtime_error("Send fail.");
		}
		pos += bytes;
	}
}

void MySocket::RecvWithLen(char*& data, int flags)
{
	uint32_t len;
	int bytes = fn_recv(m_socket, (char*)&len, sizeof(len), flags);
	if (bytes <= 0)
	{
		throw runtime_error("Recieve error. Error code:"+to_string(WSAGetLastError()));
	}

	len = ntohl(len);

	data = new(nothrow) char[len + 1];
	if (data == nullptr)
		throw runtime_error("Allocate memory failed in MySocket::RecvWithLen. Need: " + to_string(len + 1)+"B");

	uint32_t pos = 0;
	while (pos < len)
	{
		int bytes = fn_recv(m_socket, &data[pos], len - pos, flags);
		if (bytes <= 0)
		{
			delete[] data;
			throw runtime_error("Recieve error. Error code:" + to_string(WSAGetLastError()));
		}
		pos += bytes;

	}
	data[len] = 0;
}

std::string MySocket::RecvWithLen(int flags)
{
	char* data = nullptr;
	RecvWithLen(data, flags);

	string ret = data;
	delete[] data;
	return ret;
}

std::string MySocket::RecvWithEndMark(const char* end_mark, int bufsize, int flags)
{
	char* buf = new(nothrow) char[bufsize + 1];
	if (buf == nullptr)
		throw runtime_error("Allocate memory failed in MySocket::RecvWithEndMark. Need: " + to_string(bufsize + 1) + "B");

	string s;
	while (1)
	{
		int rs = fn_recv(m_socket, buf, bufsize, flags);
		if (rs > 0)
		{
			buf[rs] = 0;
			s += buf;
			if (rs < bufsize)
				break;
		}
		else
			break;
	}

	delete[] buf;

	if (s.find(end_mark) == string::npos)
		throw runtime_error("Fail to receive correct end-mark. Recv() Got string: " + s);

	return s;
}

void MySocket::Close()
{
	closesocket(m_socket);
}

MySocket::MySocket(FnSend in_fn_send, FnRecv in_fn_recv) :stopListenLoop(false), fn_send(in_fn_send), fn_recv(in_fn_recv)
{
	CreateSocket();
}

void MySocket::CreateSocket()
{
	//创建socket
	m_socket = socket(AF_INET, SOCK_STREAM, IPPROTO_TCP);

	if (m_socket == INVALID_SOCKET)
	{
		throw runtime_error("Invalid socket. Reason: maybe not call WSAStartup.");
	}
}

uint32_t MySocket::RecvUINT(int flags)
{
	uint32_t u;
	int rs=fn_recv(m_socket, (char*)&u, sizeof(u), flags);
	if (rs <= 0)
		throw runtime_error("Recieve error. Error code:" + to_string(WSAGetLastError()));
	return ntohl(u);
}

void MySocket::SendWithLen(const std::string& data, int flags)
{
	SendWithLen(data.c_str(), data.size(), flags);
}

void MySocket::SetRecvTimeOut(int ms)
{
	setsockopt(m_socket, SOL_SOCKET, SO_RCVTIMEO, (char*)&ms, sizeof(int));
}

void MySocket::Send(const char* data, int len, int flags)
{
	int count = 0;
	while (count < len)
	{
		int bytes = fn_send(m_socket, data + count, len - count, flags);
		if (bytes <= 0)
		{
			throw runtime_error("Send fail.");
		}
		count += bytes;
	}
}

void MySocket::Send(const std::string s, int flags)
{
	Send(s.c_str(), s.size());
}

string DnsParse(const string& domain)
{
	hostent* p;

	if ((p = gethostbyname(domain.c_str())) == NULL)
	{
		throw runtime_error("DNS Parse fail on \""+domain+"\"");
	}

	//string ip;

	string sip;
	sip += to_string((unsigned char)p->h_addr_list[0][0]) + ".";
	sip += to_string((unsigned char)p->h_addr_list[0][1]) + ".";
	sip += to_string((unsigned char)p->h_addr_list[0][2]) + ".";
	sip += to_string((unsigned char)p->h_addr_list[0][3]);

	return sip;
}

USHORT CheckSum(USHORT* buffer, int size)
{
	unsigned long cksum = 0;
	while (size > 1)
	{
		cksum += *buffer;
		buffer++;
		size -= sizeof(USHORT);
	}
	if (size)
		cksum += *(UCHAR*)buffer;

	cksum = (cksum >> 16) + (cksum & 0xffff);
	cksum += (cksum >> 16);
	return (USHORT)(~cksum);
}

bool CheckIP(std::string sip)
{
#ifdef WINXP
	return INADDR_NONE != inet_addr(sip.c_str());
#else
	sockaddr_in sa;
	int ret = inet_pton(AF_INET, sip.c_str(), &sa.sin_addr);
	return ret == 1;//if success
#endif
}

bool CheckNum(std::string s, int a, int b)
{
	try
	{
		int port = stoi(s);
		return port >= a && port <= b;
	}
	catch (...)
	{
		return false;
	}
}

bool CheckPort(std::string s)
{
	return CheckNum(s, 1025, 65534);
}

bool CheckName(std::string sip)
{
	auto found = sip.find_first_of("-+=|\\:;'\",.<>/?");
	return found == string::npos && !sip.empty();//if success
}

bool CheckEmailAddress(std::string addr)
{
	string name, web;
	auto at_pos = addr.find('@');
	if (at_pos == string::npos)
		return false;

	name = addr.substr(0, at_pos);
	web = addr.substr(at_pos + 1);

	if (name.empty() || web.empty())
		return false;

	auto is_alpha_num_underline = [](std::string s)->bool
	{
		for (auto c : s)
		{
			if (isalnum(c) ==0)
				if (c!='_')
					if (c!='.')
						return false;
		}
		return true;
	};

	return is_alpha_num_underline(name) && is_alpha_num_underline(web);
}
