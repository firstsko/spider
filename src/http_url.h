#ifndef _HTTP_URL_H_
#define _HTTP_URL_H_

#include <cstdlib>
#include <stdio.h>
#include <vector>
#include <map>
#include <string>
#include <strings.h>
#include "log.h"
#include "server.h"

class HttpUrl {
public:
    HttpUrl():port_(0), ip_(0) {
    };

    ~HttpUrl() {
    };
	
	int Parse(const std::string &source);

	std::string Protocol() {
		return protocol_;
	}

	std::string Hostname(const std::string &source) {
		return hostname_;
	}

	std::string Path(const std::string &source) {
		return path_;
	}

	std::string QueryString(const std::string &source) {
		return querystring_;
	}

	int Port() {
		return port_;
	}

	unsigned Ip() {
		return 0;
	}

private:
	int UrlValid(const std::string &source);
	
	int GetHostName(const std::string &source);

	int GetProtocol(const std::string &source);

	int GetPort(const std::string &source);

	int GetPath(const std::string &source);

	int GetQueryString(const std::string &source);

	unsigned GetIp();

	std::string GetIpStr();

private:
	std::string protocol_;	
	std::string hostname_;	
	std::string ipstr_;	
	std::string path_;	
	std::string querystring_;
	int port_;
	unsigned ip_;
};

#endif
