#include <sys/time.h>
#include <sys/stat.h>
#include <unistd.h>
#include <dirent.h>
#include <stdlib.h>
#include <regex.h>
#include <string.h>
#include <stdarg.h>

#include <set>

#include "log.h"

using namespace std;

// The Array Is Correctly Ordered
LogLevel Levelname[LOG_DEBUG + 1] = {
	{LOG_EMERG, "EMERG"},
	{LOG_ALERT, "ALERT"},
	{LOG_CRIT, "CRIT"},
	{LOG_ERROR, "ERROR"},
	{LOG_WARNING, "WARNING"},
	{LOG_NOTICE, "NOTICE"},
	{LOG_INFO, "INFO"},
	{LOG_DEBUG, "DEBUG"},
};

Log* Log::plog_ = NULL;

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

// 19:30:30 030 Hour Minute Second Millisecond 
string now_str() {
    char now_str[128];

    struct timeval now = {0};
    struct tm pnow;
    gettimeofday(&now, NULL);
    localtime_r(&now.tv_sec, &pnow);
	snprintf(now_str, sizeof(now_str) - 1, "%d:%02d:%02d %03ld", pnow.tm_hour, pnow.tm_min, pnow.tm_sec, now.tv_usec/1000);

    return string(now_str);
}

// Singleton, Not Multi-Thread Safe
Log* Log::Instance() {
	if (plog_ == NULL) {
		plog_ = new Log();
	}
	return plog_;
}

Log::Log(): fd_(-1), level_(LOG_DEBUG), path_("./log"), prefix_("undefined"), suffix_(".log") {
	// Check Existence, If Not, Create It
	if (access(path_.c_str(), F_OK) != 0) {
		mkdir(path_.c_str(), S_IRWXU | S_IRWXG | S_IRWXO);
	}
	// Maxsum File Size 50MB As Default
	max_size_ = 50 * 1024 * 1024;
	// Initialise current_file_ 
	FindExistingLog();
	// OpenFile, Append Write, Create If Not Exist, User Has RWX right
	fd_ = open(current_file_.c_str(), O_RDWR | O_APPEND | O_CREAT, S_IRUSR | S_IWUSR);
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

size_t Log::Record(Loglevel_t level, const char *file, int line, const char *func, const char *format, ...) {
	if (level_ < level) {
		return 0;
	}

	Rotate();
	size_t bytes = 0;
	va_list args;
	va_start(args, format);
	bytes = WriteRecord(level, file, line, func, format, args);
	va_end(args);
	
	return bytes;
}

// Each Line Can Be Written Less Than 1024 Bytes
size_t Log::WriteRecord(Loglevel_t level, const char *file, int line, const char *func, const char *format, va_list args) {
	char linebuffer[LOG_MAX_LINE];
	size_t offset = 0;

	// Print Header Stamp, [18:35:20 329][DEBUG] event_driver.cpp (113) bar 
	offset = snprintf(linebuffer + offset, LOG_MAX_LINE - 1, "[%s][%s] %s (%d) %s: ", now_str().c_str(), Levelname[level].name,
		file, line, func);

	// Print Log Infomation
	offset += vsnprintf(linebuffer + offset, LOG_MAX_LINE - 1 - offset, format, args);

	// I/O Action, Use '\n' to Speed The LineBuffer
	dprintf(fd_, "%s\n", linebuffer);

	return offset;
}

void Log::Rotate() {
	struct stat fd_stats;
	fstat(fd_, &fd_stats);

	if ((today_ != today_str()) || fd_stats.st_size >= max_size_) {
		close(fd_);
		FindExistingLog();	
		fd_ = open(current_file_.c_str(), O_RDWR | O_APPEND | O_CREAT, S_IRUSR | S_IWUSR);
	}
}
