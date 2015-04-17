#include "ini.h"

static inline size_t min(size_t a, size_t b) {
    return a <= b ? a : b;
}

static bool check_blank (char *pointer) {
    if (*pointer == ' ' || *pointer == '\t' || *pointer == '\r' || *pointer == '\n' || *pointer == 0) {
        return true;
    }
    return false;
}

static size_t first_no_blank(char *str) {
    char *p = str;
    size_t count = 0;
    while (check_blank(p)) {
        p++;
        count++;
    }
    return count;
}

static size_t lastest_no_blank(char *str) {
    char *p = str - 1;
    size_t count = 0;
    while (check_blank(p)) {
        p--;
        count++;
    }
    return count;
}

static size_t last_line_blank(char *str, size_t len) {  
    char *p = str + len - 1;
    size_t count = 0;
    while ( check_blank(p) ) {
            p--;
            count++;
    }
    return len - count - 1;
}

static size_t start_comment(char *str) {
    char *p = str;
    size_t count = 0;
    while ( *p != ';' && *p != '#' ) {
        p++;
        count++;
    }
    return count;
}

int Ini::LoadFile(const string &filename) {  
	sections.clear();
    FILE *fp;
    fp = fopen(filename.c_str(), "r");

    if (fp == NULL) {
        return -1;
    }

    char line[MAXLINE] = {0};
    char section_name[MAXLINE] = {0};
    multimap <string, string> node;

    while (fgets(line, MAXLINE, fp) != NULL) {

        char *p, *section; 
        char key_name[MAXLINE] = {0};
        char value_name[MAXLINE] = {0};

        int  key_flag = 0;
        int  section_flag = 0;

        for (p = line; *p != 0; p++) {

            if (*p == '[') {
				if ((line + first_no_blank(line)) != p) {
					sections.clear();
                    return -1; // illegal ini input format
				}
                section = p;
				section_flag++;
            }

            if (*p == ']') { 
				section_flag--;

                if ((line + last_line_blank(line, strlen(line)) != p && *(p + 1 + first_no_blank(p + 1)) != '#')) {
					sections.clear();
                    return -1; // illegal ini input format
                }

				node.clear();	

				memset(section_name, 0, MAXLINE);
                memcpy(section_name, section + 1, p - section - 1);  
                if (sections.find(string(section_name)) != sections.end()) {
					sections.clear();
                    return -1 ; // illegal ini input format, section name already exists
                }

                sections.insert(make_pair(string(section_name), node));
                break;
            }

            if (*p == '=') {
                key_flag++;

                if (key_flag != 1 || section_flag != 0) {
					sections.clear();
                    return -1; // illegal ini input format
                }

                memcpy(key_name, line + first_no_blank(line), p - lastest_no_blank(p) - line);
                memcpy(value_name, p + 1 + first_no_blank(p + 1), min(line + last_line_blank(line, start_comment(line)), 
				line + last_line_blank(line, strlen(line))) - p - first_no_blank(p + 1));

                node.insert(make_pair(string(key_name), string(value_name)));
				sections[string(section_name)] = node;
            }

            if (*p == ';' || *p == '#') {
                break;
            }

			if (*p == '\n' && section_flag != 0) {
				sections.clear();
            	return -1; // illegal ini input format
			}
        }
    }

    fclose(fp);
    if_loaded = true;
    return 0;  
}

int Ini::GetIntKey(const string &section, const string &key, int &value) {
    if (!if_loaded) {
        return -4;
    }

    map <string, multimap <string, string> >::iterator it;   
	multimap <string, string>::iterator mul_it; 

	if ((it = sections.find(section)) != sections.end()) {
		if((mul_it = (it->second).find(key)) != (it->second).end()) {
			value = atoi(mul_it->second.c_str());
			return 0;
		}	
		return -2;
	}
	return -3;
}

int Ini::GetIntKey(const string &section, const string &key, vector <int> &values) {
    if (!if_loaded) {
        return -4;
    }

    map <string, multimap <string, string> >::iterator it;   
	multimap <string, string>::iterator mul_it; 
	pair < multimap <string, string>::iterator, multimap <string, string>::iterator > ret;
	
	if ((it = sections.find(section)) != sections.end()) {
		ret = (it->second).equal_range(key);
		values.clear();
		for (mul_it = ret.first; mul_it != ret.second; mul_it++) {
			values.push_back(atoi(mul_it->second.c_str()));
		}
		if (values.empty()) {
			return -2;
		}
		return 0;
	}
	return -3;
}

int Ini::GetFloatKey(const string &section, const string &key, double &value) {
    if (!if_loaded) {
        return -4;
    }

    map <string, multimap <string, string> >::iterator it;   
	multimap <string, string>::iterator mul_it; 

	if ((it = sections.find(section)) != sections.end()) {
		if((mul_it = (it->second).find(key)) != (it->second).end()) {
			value = atof(mul_it->second.c_str());
			return 0;
		}	
		return -2;
	}
	return -3;
}

int Ini::GetFloatKey(const string &section, const string &key, vector <double> &values) {
    if (!if_loaded) {
        return -4;
    }

    map <string, multimap <string, string> >::iterator it;   
	multimap <string, string>::iterator mul_it; 
	pair < multimap <string, string>::iterator, multimap <string, string>::iterator > ret;
	
	if ((it = sections.find(section)) != sections.end()) {
		ret = (it->second).equal_range(key);
		values.clear();
		for (mul_it = ret.first; mul_it != ret.second; mul_it++) {
			values.push_back(atof(mul_it->second.c_str()));
		}
		if (values.empty()) {
			return -2;
		}
		return 0;
	}
	return -3;
}

int Ini::GetStringKey(const string &section, const string &key, string &value) {
    if (!if_loaded) {
        return -4;
    }

    map <string, multimap <string, string> >::iterator it;   
	multimap <string, string>::iterator mul_it; 

	if ((it = sections.find(section)) != sections.end()) {
		if((mul_it = (it->second).find(key)) != (it->second).end()) {
			value = mul_it->second;
			return 0;
		}	
		return -2;
	}
	return -3;
}

int Ini::GetStringKey(const string &section, const string &key, vector <string> &values) {
    if (!if_loaded) {
        return -4;
    }

    map <string, multimap <string, string> >::iterator it;   
	multimap <string, string>::iterator mul_it; 
	pair < multimap <string, string>::iterator, multimap <string, string>::iterator > ret;
	
	if ((it = sections.find(section)) != sections.end()) {
		ret = (it->second).equal_range(key);
		values.clear();
		for (mul_it = ret.first; mul_it != ret.second; mul_it++) {
			values.push_back(mul_it->second);
		}
		if (values.empty()) {
			return -2;
		}
		return 0;
	}
	return -3;
}

int Ini::GetBoolKey(const string &section, const string &key, bool &value) {
    if (!if_loaded) {
        return -4;
    }

    map <string, multimap <string, string> >::iterator it;   
	multimap <string, string>::iterator mul_it; 

	if ((it = sections.find(section)) != sections.end()) {
		if((mul_it = (it->second).find(key)) != (it->second).end()) {
			if (strcasecmp(mul_it->second.c_str(), "true") == 0 || strcasecmp(mul_it->second.c_str(), "yes") == 0 ) {
				value = true;
				return 0; 
			} else if (strcasecmp(mul_it->second.c_str(), "false") == 0 || strcasecmp(mul_it->second.c_str(), "no") == 0 ) {
				value = false;
				return 0; 
			} else {
				return -1;	// illegal input
			}

		}	
		return -2;
	}
	return -3;
}

int Ini::GetBoolKey(const string &section, const string &key, vector <bool> &values) {
    if (!if_loaded) {
        return -4;
    }

    map <string, multimap <string, string> >::iterator it;   
	multimap <string, string>::iterator mul_it; 
	pair < multimap <string, string>::iterator, multimap <string, string>::iterator > ret;
	
	if ((it = sections.find(section)) != sections.end()) {
		ret = (it->second).equal_range(key);
		values.clear();
		for (mul_it = ret.first; mul_it != ret.second; mul_it++) {
			if (strcasecmp(mul_it->second.c_str(), "true") == 0 || strcasecmp(mul_it->second.c_str(), "yes") == 0 ) {
				values.push_back (true);
			} else if (strcasecmp(mul_it->second.c_str(), "false") == 0 || strcasecmp(mul_it->second.c_str(), "no") == 0 ) {
				values.push_back (false);
			}
		}

		if (values.empty()) {
			return -2;
		}
		return 0;
	}
	return -3;
}

size_t Ini::GetSectionSize(const string &section_name) {
    if (!if_loaded) {
        return 0;
    }

    map <string, multimap <string, string> >::iterator it;   
    if ((it = sections.find(section_name)) != sections.end()) {
        return it->second.size();
    }

    return 0;
}

void Ini::DumpAll(void) {
    if (!if_loaded) {
        return;
    }

    map <string, multimap <string, string> >::iterator it;   
	multimap <string, string>::iterator mul_it; 

	for (it = sections.begin(); it != sections.end(); it++) {
		printf("[%s]\n", it->first.c_str());
		for (mul_it = (it->second).begin(); mul_it != (it->second).end(); mul_it++) {
			printf("%s = %s\n", mul_it->first.c_str(), mul_it->second.c_str());
		}
		printf("\n");
	}
	return;
}

void Ini::ClearAll(void) {
    if_loaded = false;
	sections.clear();
	return;
}
