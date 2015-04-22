#ifndef _SERVER_H
#define _SERVER_H

#include <string.h>
#include <errno.h>

#include "log.h"

// File Name, Line Number And Function Name Will Be Printed
#define DEBUG(format, vargs...) 		Log::Instance()->Record(LOG_DEBUG, __FILE__, __LINE__, __func__, format, ##vargs) 
#define INFO(format, vargs...) 			Log::Instance()->Record(LOG_INFO, __FILE__, __LINE__, __func__, format, ##vargs) 
#define NOTICE(format, vargs...) 		Log::Instance()->Record(LOG_NOTICE, __FILE__, __LINE__, __func__, format, ##vargs) 
#define WARNING(format, vargs...) 		Log::Instance()->Record(LOG_WARNING, __FILE__, __LINE__, __func__, format, ##vargs) 
#define ERROR(format, vargs...) 		Log::Instance()->Record(LOG_ERROR, __FILE__, __LINE__, __func__, format, ##vargs) 
#define CRIT(format, vargs...) 			Log::Instance()->Record(LOG_CRIT, __FILE__, __LINE__, __func__, format, ##vargs) 
#define ALERT(format, vargs...) 		Log::Instance()->Record(LOG_ALERT, __FILE__, __LINE__, __func__, format, ##vargs) 
#define EMERG(format, vargs...) 		Log::Instance()->Record(LOG_EMERG, __FILE__, __LINE__, __func__, format, ##vargs) 

#endif
