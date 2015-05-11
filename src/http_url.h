#ifndef _HTTP_URL_H_
#define _HTTP_URL_H_

#include <cstdlib>
#include <stdio.h>
#include <vector>
#include <map>
#include <string>
#include <strings.h>

class HttpUrl {
public:
    HttpUrl(const std::string &source) {
    };

    ~HttpUrl() {
    };
	
	int Parse(const std::string &source);
	
	std::string GetDomain();

	unsigned GetIp();

	std::string GetIpStr();

private:
	std::string host_;	
	std::string ipstr_;	
	std::string query_string_;
	int port_;
	std::string host_;	
	unsigned ip_;
};

#endif
