#ifndef REQUESTPARSER_HPP
# define REQUESTPARSER_HPP

# include "utils.hpp"
# include <stdio.h>
# include <unistd.h>
# include <map>

# define GET_LINE_BUF 1024
# define ERROR -1
# define SUCCESS 0

class RequestParser
{
public:
	const int _socket_fd;
	const FILE* _socket_read;
	const FILE* _socket_write;

	std::string _method;
	std::string _path;
	std::string _protocol;
	std::string _request_body;
	std::map<std::string, std::string> _request_header;

public:
	RequestParser(int socket_fd);
	~RequestParser();
	int parseRequest();

	int getSocketFD() const;
	FILE* getSocketReadFP() const;
	FILE* getSocketWriteFP() const;
	std::string getMethod() const;
	std::string getPath() const;
	std::string getProtocol() const;
	std::string getRequestBody() const;
	std::map<std::string, std::string> getRequestHeader() const;

private:
	int parseRequestLine();
	int parseRequestHeader();
	int parseRequestBody();
};


RequestParser::RequestParser(int socket_fd)
	: _socket_fd(socket_fd), _socket_read(fdopen(socket_fd, "r")), _socket_write(fdopen(dup(socket_fd), "w"))
{
	//
}

RequestParser::~RequestParser()
{
	// fclose(_socket_read);
	// fclose(_socket_write);
	// close(_socket_fd);
}

int RequestParser::getSocketFD() const { return _socket_fd; }
FILE* RequestParser::getSocketReadFP() const { return const_cast<FILE*>(_socket_read); }
FILE* RequestParser::getSocketWriteFP() const { return const_cast<FILE*>(_socket_write); }
std::string RequestParser::getMethod() const { return _method; }
std::string RequestParser::getPath() const { return _path; }
std::string RequestParser::getProtocol() const { return _protocol; }
std::string RequestParser::getRequestBody() const { return _request_body; }
std::map<std::string, std::string> RequestParser::getRequestHeader() const { return _request_header; }

int RequestParser::parseRequest()
{
	if (parseRequestLine() == ERROR)
		return ERROR;
	if (parseRequestHeader() == ERROR)
		return ERROR;
	if (this->_method == "POST")
		parseRequestBody();
	return SUCCESS;
}

int RequestParser::parseRequestLine()
{
	char line[GET_LINE_BUF];
	std::string str_line;
	std::vector<std::string> request_line;
	size_t pos;

	fgets(line, GET_LINE_BUF, getSocketReadFP());
	str_line = std::string(line);
	if (str_line == "" || str_line == "\r\n")
		return ERROR;

	request_line = split(str_line, " ");
	if (request_line.size() != 3)
		return ERROR;

	_method = request_line[0];
	_path = request_line[1];
	_protocol = request_line[2];
	
	if (_method != "GET" && _method != "POST" && _method != "DELETE")
		return ERROR;
	if (_path[0] != '/')
		return ERROR;
	if ((pos = _protocol.find("HTTP/")) == std::string::npos)
		return ERROR;

	return SUCCESS;
}

int RequestParser::parseRequestHeader()
{
	char line[GET_LINE_BUF];
	std::string str_line, key, value;
	std::vector<std::string> key_value;

	fgets(line, GET_LINE_BUF, getSocketReadFP());
	str_line = std::string(line);
	while (str_line != "" && str_line != "\r\n")
	{
		key_value = split(std::string(line), ": ");

		if (key_value.size() != 2) {
			_request_header.clear();
			return ERROR;
		}

		key = key_value[0];
		value = key_value[1].replace(key_value[1].find("\r\n"), 2, "\0");
		_request_header.insert(std::make_pair<std::string, std::string>(key, value));

		fgets(line, GET_LINE_BUF, getSocketReadFP());
		str_line = std::string(line);
	}
	if (_request_header.find("Host") == _request_header.end())
		return ERROR;
	
	return SUCCESS;
}

int RequestParser::parseRequestBody()
{
	char line[GET_LINE_BUF];

	while (fgets(line, GET_LINE_BUF, getSocketReadFP()) != NULL)
	{
		_request_body += (std::string(line) + "\n");
	}
	return SUCCESS;
}

#endif