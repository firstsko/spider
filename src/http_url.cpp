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

	// Get Protocol

	// Get Hostname

	// Get Port

	// Get Path

	// Get QueryString

    return 0;  
}
