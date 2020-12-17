#pragma once
#include "MyHTTP.h"

#include <openssl/ssl.h>

class MyHTTPS :
	public MyHTTP
{
public:
	MyHTTPS();
	~MyHTTPS();

	//����ָ��ip���˿�
	//�׳��쳣������ʧ��
	void Connect(std::string ip, std::string hostname, unsigned short port=443)throw (std::runtime_error);

private:
	SSL_CTX* ctx; 
	SSL* ssl;

	int inner_send(SOCKET s, const char* data, int len, int flags);
	int inner_recv(SOCKET s, char* data, int len, int flags);
};

