#ifndef _LOG_H_
#define _LOG_H_

#include <sys/stat.h>
#include <sys/types.h>
#include <sys/fcntl.h>
#include <unistd.h>
#include <stdio.h>
#include <stdlib.h>
#include <string>

// Each Line Can Be Written Less Than 1024 Bytes
#define LOG_MAX_LINE 1024
// Enable 128KB Cache
#define LOG_CACHE_SIZE (128*1024)

typedef enum {
	LOG_EMERG = 0,
	LOG_ALERT,
	LOG_CRIT,
	LOG_ERROR,
	LOG_WARNING,
	LOG_NOTICE,
	LOG_INFO,
	LOG_DEBUG,
} Loglevel_t;

typedef struct {
	Loglevel_t level;
	const char *name;
} LogLevel;

std::string today_str();
std::string now_str();

class Log {
public:
	~Log() {
		close(fd_);
		if (pbuff_ != NULL) {
			free(pbuff_);
		}
	}

	static Log* Instance();

	void SetPath(const std::string &path) {
		path_ = path;
	}

	size_t Record(Loglevel_t level, const char *file, int line, const char *func, const char *format, ...);
	
	// Flush Buffer
	void Flush();
	
private:
	// Private Constructor For Singletion
	Log(bool enable_buff);

	size_t WriteRecord(Loglevel_t level, const char *file, int line, const char *func, const char *format, va_list args);

	void FindExistingLog();
	
	// Update Today
	void Rotate();

private:
	int fd_;
	unsigned max_size_;
	unsigned written_bytes;
	unsigned files_counter_;
	Loglevel_t level_;

	std::string path_;		
	std::string prefix_;		
	std::string suffix_;		
	std::string current_file_;		
	std::string today_;		
	
	bool enable_buff_;
	char *pbuff_;
	size_t buff_offset_;
	static Log *plog_;
};

#endif 
