#ifndef _LOG_H_
#define _LOG_H_

#include <sys/stat.h>
#include <string>

typedef enum {
	LOG_EMERG = 0,
	LOG_ALERT,
	LOG_CRIT,
	LOG_ERR,
	LOG_WARNING,
	LOG_NOTICE,
	LOG_INFO,
	LOG_DEBUG,
} Loglevel_t;

class Log {
public:
	Log():path("./log") {
		// Check Directory Exist, If Not, Create It
		if (access(path_.c_str(), F_OK) != 0) {
			mkdir(path.c_str(), R_OK | W_OK | X_OK);
		}

	}

	void SetPath(const std::string &path) {
		path_ = path;
	}
	




private:
	int fd_;
	unsigned max_size_;

	Loglevel_t level_;
	std::string path_;		
};

#endif 
