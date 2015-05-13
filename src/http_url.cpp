#include <regex.h> 
#include "http_url.h"
#include "log.h"

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

	memcpy(buf, source.c_str()+store[0].rm_so, source.c_str() + store[0].rm_eo);
	protocol_ = buf;

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

	memcpy(buf, source.c_str()+store[0].rm_so, source.c_str() + store[0].rm_eo);
	hostname_ = buf;

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

	memcpy(buf, source.c_str()+store[0].rm_so, source.c_str() + store[0].rm_eo);
	path_ = buf;

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

	memcpy(buf, source.c_str()+store[0].rm_so, source.c_str() + store[0].rm_eo);
	querystring_ = buf;

	regfree(&reg);
	return ret;
}

int HttpUrl::GetPort(const string &source) {  

	return 80;
}

unsigned HttpUrl::GetIp() {

	return 0;
}

string HttpUrl::GetIpStr() {

	return "";
}
