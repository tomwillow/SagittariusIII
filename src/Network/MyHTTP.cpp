#include "MyHTTP.h"

#ifdef _DEBUG
#include <iostream>
#endif

using namespace std;

void MyHTTP::RecvHTTP(int bufsize, int flags) throw(std::runtime_error)
{
	if (bufsize <= 0)
		throw runtime_error("bufsize<=0");

	char* buf = new(nothrow) char[bufsize + 1];
	if (buf == nullptr)
		throw runtime_error("memory is not enough.");

	unique_ptr<char> up_buf(buf);

	string& raw_head = response.raw_response_header;
	string& content = response.content;
	auto dic = response.header;

	raw_head.clear();
	content.clear();
	while (1)
	{
		int rs = fn_recv(m_socket, buf, bufsize, flags);

		if (rs <= 0)
			throw runtime_error("Recieve error. Error code:" + to_string(WSAGetLastError()));

		buf[rs] = 0;
		raw_head += buf;

		//������CRLF�з�����ͷ������
		auto pos = raw_head.find("\r\n\r\n");
		if (pos != string::npos)
		{
			content += raw_head.substr(pos + 4);//��CRLF���ȡ
			raw_head.erase(raw_head.begin() + pos + 2, raw_head.end());//����1��CRLF��ȡ
			break;
		}
	}

	//ʶ���һ��
	//��ʽ���汾 ״̬�� ����
	size_t start = 0;
	auto pos = raw_head.find("\r\n", start);
	if (pos != string::npos)
	{
		stringstream ss(raw_head.substr(start, pos - start));
		ss >> response.version >> response.state_code >> response.phrase;
		start = pos + 2;
	}
	else
		throw runtime_error("Can not parse the first line of request head:" + raw_head.substr(start, pos - start));

	//��������ͷ
	dic.clear();
	while (1)
	{
		auto pos = raw_head.find("\r\n", start);

		//��CRLF�з�
		if (pos != string::npos)
		{
			string line = raw_head.substr(start, pos - start);//�õ�1��

			//�зֳ�key��value
			auto pos_space = line.find(": ");
			if (pos_space != string::npos)
			{
				string key = line.substr(0, pos_space);
				string value = line.substr(pos_space + 2);
				dic[key] = value;
			}
			else
			{
				throw runtime_error("Can not parse the line:" + line);
			}

			start = pos + 2;//������ʼ��
		}
		else
		{
			break;
		}
	}

	//��������
	const char sz_content_length[] = "Content-Length";
	auto it_content_length = dic.find(sz_content_length);
	if (it_content_length != dic.end())
	{
		//lengthģʽ
		int content_length = stoi(dic[sz_content_length]);

		int remain = content_length - content.length();
		if (remain < 0)//ʵ�ʴ�С>��Ǵ�С
			throw runtime_error("Field Content-Length is less than real content length.");

		//����ʣ�ಿ��
		while (remain)
		{
			int rs = fn_recv(m_socket, buf, bufsize, flags);
			if (rs <= 0)//����������С�ڱ��ֵ���˴�rs=-1
				throw runtime_error("Recieve error. Error code:" + to_string(WSAGetLastError()));

			buf[rs] = 0;
			content += buf;
			remain -= rs;
		}
	}
	else
	{
		//chunkedģʽ
		const char sz_transfer_encoding[] = "Transfer-Encoding";
		auto it_transfer_encoding = dic.find(sz_transfer_encoding);
		if (it_transfer_encoding != dic.end() && dic[sz_transfer_encoding] == "chunked")
		{
			string temp = content;
			content.clear();

			int state = 0;
			while (1)
			{
				auto pos = temp.find("\r\n");
				if (pos != string::npos)
				{
					//�˴���֤ temp ��chunked��С��ʼ

					string s_len = temp.substr(0, pos);//�õ���С
					//cout << s_len << endl;
					int len = stoi(s_len, nullptr, 16);
					temp = temp.substr(pos + 2);//�ص���С��content�����Ǵ�����

					int remain = len - temp.length();
					if (remain <= -2)//�������ݳ�����С��ȡ�㣬ֱ�ӽ��н�ȡ
					{
						//��һ����ʽ��ȡ
						content += temp.substr(0, len);
						temp = temp.substr(len + 2);//Խ����β��\r\n
					}
					else
					{
						//���ղ��㲿��
						while (1)
						{
							int rs = fn_recv(m_socket, buf, bufsize, flags);
							if (rs <= 0)
								throw runtime_error("Recieve error. Error code:" + to_string(WSAGetLastError()));

							buf[rs] = 0;
							temp += buf;
							remain -= rs;

							if (remain <= -2)//[len���ȵ�chunk]�������һ��\r\n��ҪԽ��\r\n��Ҫ���ٶ����2B
							{
								//�ڶ�����ʽ��ȡ
								content += temp.substr(0, len);
								temp = temp.substr(len + 2);//Խ����β��\r\n
								break;
							}
						}
					}

					//�����걾�ֿ�
					if (len == 0)//���һ��chunk��0\r\n\r\n��β
					{
						//�������н����ڲ��Խ�β�ֿ��Ƿ��߼���ȷ
						//��ȷ�Ļ��˴�socket������Ӧ�����ݣ�recvӦʼ������
						//int rs = recv(m_socket, buf, bufsize, flags);
						//if (rs <= 0)
						//	throw runtime_error("Recieve error. Error code:" + to_string(WSAGetLastError()));
						break;
					}
				}
				else
				{
					int rs = fn_recv(m_socket, buf, bufsize, flags);
					if (rs <= 0)
						throw runtime_error("Recieve error. Error code:" + to_string(WSAGetLastError()));

					buf[rs] = 0;
					temp += buf;
				}
			}
		}
		else
			throw runtime_error("No Content-Type or Transfer-Encoding field.");

	}
}

MyHTTP::MyHTTP(FnSend in_fn_send, FnRecv in_fn_recv):MySocket(in_fn_send,in_fn_recv)
{
}

void MyHTTP::Connect(std::string ip, unsigned short port) throw(std::runtime_error)
{
	MySocket::Connect(ip, port);
}

void MyHTTP::SendGet()
{
	if (request_header.url.empty())
		request_header.url = "/";
	string header = "GET " + request_header.url + " HTTP/1.1\r\n"
		"Host: " + request_header.host + "\r\n"
		"user-agent: " + request_header.user_agent + "\r\n"
		"\r\n";

	Send(header);
}

void MyHTTP::SendPost()
{
	string data;
	for (auto& pr : request_header.post_data)
	{
		data += pr.first + "=" + pr.second + "&";
	}
	data.pop_back();

	if (request_header.url.empty())
		request_header.url = "/";
	string header = "POST "+request_header.url+" HTTP/1.1\r\n"
		"Host: " + request_header.host + "\r\n"
		"user-agent: " + request_header.user_agent + "\r\n"
		"Content-Length: " + to_string(data.size()) + "\r\n"
		"Content-Type: application/x-www-form-urlencoded"+"\r\n"
		"\r\n"
		"" + data;

	Send(header);
}
