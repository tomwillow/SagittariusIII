#pragma once
#include "MySocket.h"

#include <unordered_map>

class MyHTTP :
	public MySocket
{
public:
	MyHTTP() :MySocket() {}

	//����ͷ
	struct RequestHeader
	{
		std::string host;
		std::string url;
		std::string user_agent;
		std::vector<std::pair<std::string, std::string>> post_data;
	};
	RequestHeader request_header;

	//��Ӧ����
	struct Response
	{
		std::string version, state_code, phrase;//�汾 ״̬�� ����
		std::unordered_map<std::string, std::string> header;//��Ӧͷ
		std::string raw_response_header;//��Ӧͷԭʼ����
		std::string content;//����
	};
	Response response;

	//����ָ��ip���˿�
	//�׳��쳣������ʧ��
	void Connect(std::string ip, unsigned short port = 80)throw (std::runtime_error);

	//��������ͷ
	void SendGet();

	//��������ͷ
	void SendPost();

	//������Ӧͷ
	void RecvHTTP(int bufsize = 1024, int flags = 0) throw(std::runtime_error,std::invalid_argument);

protected:
	MyHTTP(FnSend in_fn_send, FnRecv in_fn_recv);
};

