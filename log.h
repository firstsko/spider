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
	Log();

	void SetPath(const std::string &path) {
		path_ = path;
	}
	

private:
	int fd_;
	unsigned max_size_;
	unsigned files_counter_;
	Loglevel_t level_;

	std::string path_;		
	std::string suffix_;		
	std::string prefix_;		
	std::string current_file_;		
	std::string today_;		
};

#endif 
