#include <regex.h>

#include "http_url.h"

using namespace std;

int HttpUrl::Parse(const string &source) {  
	// protocol + hostname + port + path + querystring = url
	size_t size = source.length();

	const string proto_regex = "^(https?:\\/\\/)?";
	const string hostname_regex = "([0-9a-zA-Z-]+\\.)+[0-9a-zA-Z]{2,6}";
	const string port_regex = "(:[0-9]{1,5})?";
	const string path_regex = "(\\/([\\w]+\\/)+[\\w])?";
	const string querystring_regex = "\\?((([\\w%]+=[\\w%]+)&)([\\w%]+=[\\w%]))?$";
	
	char buf[1024] = {0};
	snprintf(buf, sizeof(buf) - 1, "%s%s%s%s%s", proto_regex.c_str(), hostname_regex.c_str(), port_regex.c_str(), path_regex.c_str(), querystring_regex.c_str());
	string valid_http_url = buf;

	regex_t reg;
	if (regcomp(&reg, valid_http_url.c_str(), 0) != 0) {
		printf ("Failed to Compile Regular Expression\n");
		regfree(&reg);
		return -1;
	}

	// Not A Http Url
	if (regexec(&reg, source.c_str(), 0, NULL, 0) != 0) {
		regfree(&reg);
		return -1;
	}

    return 0;  
}

