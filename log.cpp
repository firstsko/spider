#include <sys/time.h>
#include <unistd.h>
#include <dirent.h>
#include <stdio.h>

#include "log.h"

Log::Log():fd_(-1), level_(LOG_DEBUG), path_("./log"), suffix_(".log") {
	// Check Existence, If Not, Create It
	if (access(path_.c_str(), F_OK) != 0) {
		mkdir(path_.c_str(), R_OK | W_OK | X_OK);
	}
	// Maxsum File Size 50MB As Default
	max_size_ = 50 * 1024 * 1024;

	// Today
	time_t now;
	struct tm pnow = {0};
	time(&now);
	localtime_r(&now, &pnow);
	
	char buf[64];
	snprintf(buf, sizeof(buf) - 1, "%d:%02d:%02d", 1900 + pnow.tm_year, 1 + pnow.tm_mon, pnow.tm_mday);
	today_ = buf;
	//current_file = prefix_ + today_ + "0" + suffix_;
	DIR *dir = opendir(path_.c_str());

	closedir(dir);
}


