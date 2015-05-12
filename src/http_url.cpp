#include <regex.h> 
#include "http_url.h"

using namespace std;

int HttpUrl::Parse(const string &source) {  
	// protocol + hostname + port + path + querystring = url
	const string proto_regex = "^(https?:\\/\\/)?";
	const string hostname_regex = "([0-9a-zA-Z-]+\\.)+[0-9a-zA-Z]{2,6}";
	const string port_regex = "(:[0-9]{1,5})?";
	const string path_regex = "(\\/([\\w]+\\/)*([\\w]+\\.?[\\w])+)?";
	const string querystring_regex = "(\\?((([\\w%]+=[\\w%\\.]+)&)*([\\w%]+=[\\w%\\.]+)))?$";
	
	string valid_http_url = proto_regex + hostname_regex + port_regex + path_regex + querystring_regex;

	regex_t reg;
	if (regcomp(&reg, valid_http_url.c_str(), REG_EXTENDED) != 0) {
		printf ("Failed to Compile Regular Expression\n");
		regfree(&reg);
		return -1;
	}

	// Not A Http Url
	if (regexec(&reg, source.c_str(), 0, NULL, 0) != 0) {
		regfree(&reg);
		return -1;
	}

	regmatch_t store[1];
	char buf[1024] = {0};
	// Get Protocol
	if (regexec(&reg, proto_regex.c_str(), 1, store, 0) != 0) {
		regfree(&reg);
		return -1;
	}

	memcpy(buf, source.c_str()+store[0].rm_so, source.c_str() + store[0].rm_eo);
	protocol_ = buf;
	memset(buf, 0, 1024);
	// Get Hostname
	if (regexec(&reg, hostname_regex.c_str(), 1, store, 0) != 0) {
		regfree(&reg);
		return -1;
	}

	memcpy(buf, source.c_str()+store[0].rm_so, source.c_str() + store[0].rm_eo);
	hostname_ = buf;
	memset(buf, 0, 1024);
	memset(store, 0, sizeof(regmatch_t));

	// Get Port
	if (regexec(&reg, hostname_regex.c_str(), 1, store, 0) != 0) {
		regfree(&reg);
		return -1;
	}

	memcpy(buf, source.c_str()+store[0].rm_so, source.c_str() + store[0].rm_eo);
	port_ = atoi(buf);
	memset(buf, 0, 1024);
	memset(store, 0, sizeof(regmatch_t));

	// Get Path
	if (regexec(&reg, path_regex.c_str(), 1, store, 0) != 0) {
		regfree(&reg);
		return -1;
	}

	memcpy(buf, source.c_str()+store[0].rm_so, source.c_str() + store[0].rm_eo);
	path_ = buf;
	memset(buf, 0, 1024);
	memset(store, 0, sizeof(regmatch_t));

	// Get QueryString
	if (regexec(&reg, querystring_regex.c_str(), 1, store, 0) != 0) {
		regfree(&reg);
		return -1;
	}

	memcpy(buf, source.c_str()+store[0].rm_so, source.c_str() + store[0].rm_eo);
	querystring_ = buf;
	memset(buf, 0, 1024);
	memset(store, 0, sizeof(regmatch_t));

	regfree(&reg);
    return 0;  
}
