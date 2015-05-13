#ifndef _HTTP_URL_H_
#define _HTTP_URL_H_

#include <cstdlib>
#include <stdio.h>
#include <vector>
#include <map>
#include <string>
#include <strings.h>
#include <string.h>
#include "log.h"
#include "server.h"

#define NOTEXIST 100 

class HttpUrl {
public:
    HttpUrl():port_(80) {
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

	// Please Check Return String Empty
	int GetParaKey(const std::string &key, std::string &result) {
		if (query_map_.find(key) != query_map_.end()) {
			result = query_map_[key];
			return 0;
		} else {	
			return NOTEXIST;
		}
	}

	int Port() {
		return port_;
	}

private:
	int UrlValid(const std::string &source);
	
	int GetHostName(const std::string &source);

	int GetProtocol(const std::string &source);

	int GetPort(const std::string &source);

	int GetPath(const std::string &source);

	int GetQueryString(const std::string &source);

	void ParseQueryString(const std::string &querystring);

private:
	std::string protocol_;	
	std::string hostname_;	
	std::string ipstr_;	
	std::string path_;	
	std::string querystring_;
	std::map<std::string, std::string> query_map_;
	int port_;
};

#endif
