
#include "MyInitSock.h"
#include <string>
#include <stdexcept>

#include <winsock2.h>
#pragma comment(lib,"WS2_32.lib")

MyInitSock::MyInitSock()
{
	//װ��Winsock��
	WSADATA wsaData;
	WORD version = MAKEWORD(2, 2);
	if (WSAStartup(version, &wsaData))
	{
		throw std::runtime_error("WSAStartup error.");
	}
}

MyInitSock::~MyInitSock()
{
	//�ͷ�Winsock��
	WSACleanup();
}