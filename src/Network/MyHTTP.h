#pragma once
#include "MySocket.h"

#include <unordered_map>

class MyHTTP :
	public MySocket
{
public:
	MyHTTP() :MySocket() {}

	//请求头
	struct RequestHeader
	{
		std::string host;
		std::string url;
		std::string user_agent;
		std::vector<std::pair<std::string, std::string>> post_data;
	};
	RequestHeader request_header;

	//响应数据
	struct Response
	{
		std::string version, state_code, phrase;//版本 状态码 短语
		std::unordered_map<std::string, std::string> header;//响应头
		std::string raw_response_header;//响应头原始数据
		std::string content;//正文
	};
	Response response;

	//连接指定ip及端口
	//抛出异常：连接失败
	void Connect(std::string ip, unsigned short port = 80)throw (std::runtime_error);

	//发送请求头
	void SendGet();

	//发送请求头
	void SendPost();

	//接收响应头
	void RecvHTTP(int bufsize = 1024, int flags = 0) throw(std::runtime_error,std::invalid_argument);

protected:
	MyHTTP(FnSend in_fn_send, FnRecv in_fn_recv);
};

