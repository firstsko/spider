#include <regex.h> 
#include <stdio.h> 
#include <string> 
#include <string.h> 

using namespace std;

int main(int argc, char **argv) {

	char *source = argv[1];

	const string proto_regex = "^(https?:\\/\\/)?";
	const string hostname_regex = "([0-9a-zA-Z-]+\\.)+[0-9a-zA-Z]{2,6}";
	const string port_regex = "(:[0-9]{1,5})?";
	const string path_regex = "(\\/([[:alnum:]_-]+\\/)*([[:alnum:]_-]+\\.?[[:alnum:]_-])+)?";
	const string querystring_regex = "(\\?((([[:alnum:]%_-]+=[[:alnum:]%\\._-]+)&)*([[:alnum:]%_-]+=[[:alnum:]%\\._-]+)))?$";
	
	string valid_http_url = proto_regex + hostname_regex + port_regex + path_regex + querystring_regex;

	regex_t reg;
	char buf[1024] = {0};
	int ret = 0;

	if ((ret = regcomp(&reg, proto_regex.c_str(), REG_EXTENDED)) != 0) {
		printf ("Failed to Compile Regular Expression\n");
		regerror(ret, &reg, buf, sizeof(buf));
      	printf("%s\n", buf);
		regfree(&reg);
		return -1;
	}

	printf("Regular Expression: %s\n", proto_regex.c_str());
	printf("Source: %s\n", source);

	regmatch_t store[1];
	// Get Protocol
	if ((ret = regexec(&reg, source, 1, store, 0)) != 0) {
		printf("Failed to Exec Regular Expression, ret %d\n", ret);
		regerror(ret, &reg, buf, sizeof(buf));
      	printf("%s\n", buf);
		regfree(&reg);
		return -1;
	}

	memcpy(buf, source + store[0].rm_so, store[0].rm_eo - store[0].rm_so);
	
	string outcome = buf;
	printf("Outcome: %s\n", outcome.c_str());

	regfree(&reg);
	return 0;
}
