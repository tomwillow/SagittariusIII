#include "MyHTTPS.h"

#include <iostream>
#include <functional>

#include <openssl/err.h>

#pragma comment(lib,"libssl_static.lib")
#pragma comment(lib,"libcrypto_static.lib")
//#pragma comment(lib,"libssl.lib")
//#pragma comment(lib,"libcrypto.lib")

using namespace std;

MyHTTPS::MyHTTPS():MyHTTP(
	std::bind(&MyHTTPS::inner_send, this, std::placeholders::_1, std::placeholders::_2, std::placeholders::_3, std::placeholders::_4),
	std::bind(&MyHTTPS::inner_recv, this, std::placeholders::_1, std::placeholders::_2, std::placeholders::_3, std::placeholders::_4)
	),ctx(NULL),ssl(NULL)
{
	//加载SSL错误信息 
	SSL_load_error_strings();

	//添加SSL的加密/HASH算法 
	OpenSSL_add_all_algorithms();

	//客户端，服务端选择SSLv23_server_method() 
	const SSL_METHOD* meth = TLS_client_method();//SSLv23_client_method

	//建立新的SSL上下文 
	ctx = SSL_CTX_new(meth);
	if (ctx == NULL)
	{
		ERR_print_errors_fp(stderr);
		throw runtime_error("SSL_CTX_new error !");
	}
}

MyHTTPS::~MyHTTPS()
{
	if (ssl)
	{
		//关闭SSL套接字 
		SSL_shutdown(ssl);

		//释放SSL套接字 
		SSL_free(ssl);
	}

	//释放SSL会话环境 
	SSL_CTX_free(ctx);
}

void MyHTTPS::Connect(std::string ip, std::string hostname, unsigned short port) throw(std::runtime_error)
{
	MySocket::Connect(ip, port);

	//建立SSL 
	ssl = SSL_new(ctx);
	if (ssl == NULL)
	{
		throw runtime_error("SSL NEW error");
	}

	//将SSL与TCP SOCKET 连接 
	SSL_set_fd(ssl, m_socket);

	SSL_set_tlsext_host_name(ssl, hostname.c_str());

	//SSL连接 
	int ret = SSL_connect(ssl);
	if (ret == -1)
	{
		char msg[1024];
		ERR_error_string_n(ERR_get_error(), msg, sizeof(msg));
		throw runtime_error("SSL_connect error:"+string(msg));
	}
}

int MyHTTPS::inner_send(SOCKET s, const char* data, int len, int flags)
{
	return SSL_write(ssl,data,len);
}

int MyHTTPS::inner_recv(SOCKET s, char* data, int len, int flags)
{
	return SSL_read(ssl,data,len);
}
