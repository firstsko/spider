#include <sys/time.h>
#include <sys/stat.h>
#include <unistd.h>
#include <dirent.h>
#include <stdlib.h>
#include <regex.h>
#include <string.h>

#include <set>

#include "log.h"

using namespace std;

// 20150417
string today_str() {
	time_t now;
	struct tm pnow = {0};
	time(&now);
	localtime_r(&now, &pnow);
	char buf[64];
	snprintf(buf, sizeof(buf) - 1, "%d%02d%02d", 1900 + pnow.tm_year, 1 + pnow.tm_mon, pnow.tm_mday);

	return std::string(buf);
}

// [19:30:30 030] Hour Minute Second Millisecond 
string now_str() {
    char now_str[128];

    struct timeval now = {0};
    struct tm pnow;
    gettimeofday(&now, NULL);
    localtime_r(&now.tv_sec, &pnow);
	snprintf(now_str, sizeof(now_str) - 1, "[%d:%02d:%02d %03ld]", pnow.tm_hour, pnow.tm_min, pnow.tm_sec, now.tv_usec/1000);

    return string(now_str);
}

Log::Log(): fp_(NULL), level_(LOG_DEBUG), path_("./log"), prefix_("undefined"), suffix_(".log") {
	// Check Existence, If Not, Create It
	if (access(path_.c_str(), F_OK) != 0) {
		mkdir(path_.c_str(), S_IRWXU | S_IRWXG | S_IRWXO);
	}
	// Maxsum File Size 50MB As Default
	max_size_ = 50 * 1024 * 1024;
	// Initialise current_file_ 
	FindExistingLog();
	// OpenFile
	fp_ = fopen(current_file_.c_str(), "a+");
}

// Format: server20150417001.log
void Log::FindExistingLog() {
	// Today
	today_ = today_str();

	// Use Regular Expression To Filt Files
	string logfile_reg = prefix_ + today_+ "[0-9]{3,}" + suffix_;
	string logindex_reg = "[0-9]{3,}";

	regex_t reg;

	if (regcomp(&reg, logfile_reg.c_str(), 0) != 0) {
		printf ("Failed to Compile Regular Expression\n");
		regfree(&reg);
		return;
	}
	
	DIR *dir = opendir(path_.c_str());

	set <string> files;
	set <string>::iterator it;
	struct dirent* ptr;
	while ((ptr = readdir(dir)) != NULL) {
		string file = ptr->d_name;
		if (regexec(&reg, file.c_str(), 0, NULL, 0) == 0) {
			files.insert(file);
		}
	}

	if (files.empty()) {
		current_file_ = path_ + "/" + prefix_ + today_ + "000" + suffix_;
		regfree(&reg);
		closedir(dir);
		return;
	}

	if (regcomp(&reg, logindex_reg.c_str(), 0) != 0) {
		printf ("Failed to Compile Regular Expression\n");
		regfree(&reg);
		closedir(dir);
		return;
	}

	// Find The Max Log Index File, Which Means The Lastest Written File
	int max_index = 0;
	regmatch_t match_set[1];
	char match[100] = {0};
	for (it = files.begin(); it != files.end(); it++) {
		string filename = *it;
		regexec(&reg, filename.c_str(), 1, match_set, 0);
		int len = match_set[0].rm_eo - match_set[0].rm_so;
		memset(match, '\0', sizeof(match));
		memcpy(match, filename.c_str() + match_set[0].rm_so, len);
		if (max_index < atoi(match)) {
			max_index = atoi(match);
			current_file_ = filename;
		}
	}

	// If current_file_ Oversized, Created A New Log File, Index++
	struct stat file_info;	
	stat(current_file_.c_str(), &file_info);
	if (file_info.st_size >= max_size_) {
		max_index++;
		char buf[256] = {0};
		snprintf(buf, sizeof(buf) - 1, "%s/%s%s%03d%s", path_.c_str(), prefix_.c_str(), today_.c_str(), max_index, suffix_.c_str());
		current_file_ = buf;
	}
	
	regfree(&reg);
	closedir(dir);
}
