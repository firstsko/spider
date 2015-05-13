#include <regex.h> 
#include "http_url.h"

using namespace std;

const string proto_regex = "^(https?:\\/\\/)?";
const string hostname_regex = "([0-9a-zA-Z-]+\\.)+[0-9a-zA-Z]{2,6}";
const string port_regex = "(:[0-9]{1,5})?";
const string path_regex = "(\\/([[:alnum:]_-]+\\/)*([[:alnum:]_-]+\\.?[[:alnum:]_-])+)?";
const string querystring_regex = "(\\?((([[:alnum:]%_-]+=[[:alnum:]%\\._-]+)&)*([[:alnum:]%_-]+=[[:alnum:]%\\._-]+)))?$";

int HttpUrl::Parse(const string &source) {  
	int ret = 0;
	if ( (ret = UrlValid(source)) != 0) {
		return ret;
	}
	
	GetProtocol(source);
	GetHostName(source);
	GetPath(source);
	GetQueryString(source);
	ParseQueryString(querystring_);

	return ret;
}

int HttpUrl::UrlValid(const string &source) {  
	// protocol + hostname + port + path + querystring = url
	string valid_http_url = proto_regex + hostname_regex + port_regex + path_regex + querystring_regex;

	regex_t reg;
	int ret = 0;
	char buf[1024] = {0};
	if ( (ret = regcomp(&reg, valid_http_url.c_str(), REG_EXTENDED)) != 0) {
		regerror(ret, &reg, buf, sizeof(buf));
		ERROR("Failed to Compile Regular Expression, %s\n", buf);
		regfree(&reg);
		return ret;
	}

	// Not A Http Url
	if ( (ret = regexec(&reg, source.c_str(), 0, NULL, 0)) != 0) {
		regerror(ret, &reg, buf, sizeof(buf));
		ERROR("Failed to Compile Regular Expression, %s\n", buf);
		regfree(&reg);
		return ret;
	}
	
	regfree(&reg);

	return ret;
}

int HttpUrl::GetProtocol(const string &source) {  
	int ret = 0;
	regmatch_t store[1];
	char buf[1024] = {0};
	regex_t reg;

	regfree(&reg);
	if ( (ret = regcomp(&reg, proto_regex.c_str(), REG_EXTENDED)) != 0) {
		regerror(ret, &reg, buf, sizeof(buf));
		ERROR("Failed to Compile Regular Expression, %s\n", buf);
		regfree(&reg);
		return ret;
	}

	if ( (ret = regexec(&reg, source.c_str(), 1, store, 0)) != 0) {
		regerror(ret, &reg, buf, sizeof(buf));
		ERROR("Failed to Compile Regular Expression, %s\n", buf);
		regfree(&reg);
		return ret;
	}

	memcpy(buf, source.c_str() + store[0].rm_so, store[0].rm_eo - store[0].rm_so);
	protocol_ = buf;
	if (protocol_.empty()) {
		ret = NOTEXIST;
	}

	regfree(&reg);
	return ret;
}

int HttpUrl::GetHostName(const string &source) {  
	int ret = 0;
	regmatch_t store[1];
	char buf[1024] = {0};
	regex_t reg;

	regfree(&reg);
	if ( (ret = regcomp(&reg, hostname_regex.c_str(), REG_EXTENDED)) != 0) {
		regerror(ret, &reg, buf, sizeof(buf));
		ERROR("Failed to Compile Regular Expression, %s\n", buf);
		regfree(&reg);
		return ret;
	}

	if ( (ret = regexec(&reg, source.c_str(), 1, store, 0)) != 0) {
		regerror(ret, &reg, buf, sizeof(buf));
		ERROR("Failed to Compile Regular Expression, %s\n", buf);
		regfree(&reg);
		return ret;
	}

	memcpy(buf, source.c_str() + store[0].rm_so, store[0].rm_eo - store[0].rm_so);
	hostname_ = buf;
	if (hostname_.empty()) {
		ret = NOTEXIST;
	}

	regfree(&reg);
	return ret;
}

int HttpUrl::GetPath(const string &source) {  
	int ret = 0;
	regmatch_t store[1];
	char buf[1024] = {0};
	regex_t reg;

	regfree(&reg);
	if ( (ret = regcomp(&reg, path_regex.c_str(), REG_EXTENDED)) != 0) {
		regerror(ret, &reg, buf, sizeof(buf));
		ERROR("Failed to Compile Regular Expression, %s\n", buf);
		regfree(&reg);
		return ret;
	}

	if ( (ret = regexec(&reg, source.c_str(), 1, store, 0)) != 0) {
		regerror(ret, &reg, buf, sizeof(buf));
		ERROR("Failed to Compile Regular Expression, %s\n", buf);
		regfree(&reg);
		return ret;
	}

	memcpy(buf, source.c_str() + store[0].rm_so, store[0].rm_eo - store[0].rm_so);
	path_ = buf;

	if (path_.empty()) {
		ret = NOTEXIST;
	}

	regfree(&reg);
	return ret;
}

int HttpUrl::GetQueryString(const string &source) {  
	int ret = 0;
	regmatch_t store[1];
	char buf[1024] = {0};
	regex_t reg;

	regfree(&reg);
	if ( (ret = regcomp(&reg, querystring_regex.c_str(), REG_EXTENDED)) != 0) {
		regerror(ret, &reg, buf, sizeof(buf));
		ERROR("Failed to Compile Regular Expression, %s\n", buf);
		regfree(&reg);
		return ret;
	}

	if ( (ret = regexec(&reg, source.c_str(), 1, store, 0)) != 0) {
		regerror(ret, &reg, buf, sizeof(buf));
		ERROR("Failed to Compile Regular Expression, %s\n", buf);
		regfree(&reg);
		return ret;
	}

	memcpy(buf, source.c_str() + store[0].rm_so, store[0].rm_eo - store[0].rm_so);
	querystring_ = buf;
	if (querystring_.empty()) {
		ret = NOTEXIST;
	}

	regfree(&reg);
	return ret;
}

int HttpUrl::GetPort(const string &source) {  
	int ret = 0;
	regmatch_t store[1];
	char buf[1024] = {0};
	regex_t reg;
	string portstr;

	regfree(&reg);
	if ( (ret = regcomp(&reg, port_regex.c_str(), REG_EXTENDED)) != 0) {
		regerror(ret, &reg, buf, sizeof(buf));
		ERROR("Failed to Compile Regular Expression, %s\n", buf);
		regfree(&reg);
		return ret;
	}

	if ( (ret = regexec(&reg, source.c_str(), 1, store, 0)) != 0) {
		regerror(ret, &reg, buf, sizeof(buf));
		ERROR("Failed to Compile Regular Expression, %s\n", buf);
		regfree(&reg);
		return ret;
	}

	memcpy(buf, source.c_str() + store[0].rm_so, store[0].rm_eo - store[0].rm_so);
	portstr = buf;
	if (portstr.empty()) {
		port_ = 80;
		ret = NOTEXIST;
	} else {
		portstr = portstr.substr(1);
		port_ = atoi(portstr.c_str());
	}

	regfree(&reg);
	return ret;
}

void HttpUrl::ParseQueryString(const string &querystring) {
	if (querystring.empty()) {
		return;
	}

	string keys = querystring.substr(1);
	vector <string> store;

	char start[1024] = {0}; 
	strncpy(start, keys.c_str(), 1024);
	char *tokenptr = strtok(start, "&");	
	while (tokenptr != NULL) {
		string tmp = tokenptr;
		store.push_back(tmp);
		tokenptr = strtok(NULL, "&");
	}

	for (size_t i = 0; i < store.size(); i++) {
		string equation = store[i];
		char *l;
		char *r;
		char begin[1024];
		strncpy(begin, equation.c_str(), 1024);
		l = strtok(begin, "=");
		r = strtok(NULL, "=");
		string left = l;
		string right = r;
		query_map_.insert(make_pair(left, right));		
	}
	
	return;
}
