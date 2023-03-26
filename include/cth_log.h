
#pragma once
#ifndef CTH_LOG_H_
#define CTH_LOG_H_

#if CTH_LOG_LINE_INFO
	// These print line info, so you can double click the error in the output window to go to the source location.
	#define log_success(_Format, ...)		log_printf_dbg(LOG_SUCCESS, __FILE__, __LINE__, _Format, ##__VA_ARGS__)
	#define log_info(_Format, ...)			log_printf_dbg(LOG_INFO, __FILE__, __LINE__, _Format, ##__VA_ARGS__)
	#define log_warning(_Format, ...)		log_printf_dbg(LOG_WARNING, __FILE__, __LINE__, _Format, ##__VA_ARGS__)
	#define log_error(_Format, ...)			log_printf_dbg(LOG_ERROR, __FILE__, __LINE__, _Format, ##__VA_ARGS__)
	#define log_critical(_Format, ...)		log_printf_dbg(LOG_CRITICAL, __FILE__, __LINE__, _Format, ##__VA_ARGS__)

	// These are the "no line info" versions, so you can still print messages without line info in debug mode if you wish.
	#define log_success_nli(_Format, ...)	log_printf(LOG_SUCCESS, _Format, ##__VA_ARGS__)
	#define log_info_nli(_Format, ...)		log_printf(LOG_INFO, _Format, ##__VA_ARGS__)
	#define log_warning_nli(_Format, ...)	log_printf(LOG_WARNING, _Format, ##__VA_ARGS__)
	#define log_error_nli(_Format, ...)		log_printf(LOG_ERROR, _Format, ##__VA_ARGS__)
	#define log_critical_nli(_Format, ...)	log_printf(LOG_CRITICAL, _Format, ##__VA_ARGS__)
#else
	#define log_success(_Format, ...)		log_printf(LOG_SUCCESS, _Format, ##__VA_ARGS__)
	#define log_info(_Format, ...)			log_printf(LOG_INFO, _Format, ##__VA_ARGS__)
	#define log_warning(_Format, ...)		log_printf(LOG_WARNING, _Format, ##__VA_ARGS__)
	#define log_error(_Format, ...)			log_printf(LOG_ERROR, _Format, ##__VA_ARGS__)
	#define log_critical(_Format, ...)		log_printf(LOG_CRITICAL, _Format, ##__VA_ARGS__)

	#define log_success_nli					log_success
	#define log_info_nli					log_info
	#define log_warning_nli					log_warning
	#define log_error_nli					log_error
	#define log_critical_nli				log_critical
#endif

typedef enum eLogErrorLevel
{
	LOG_SUCCESS,
	LOG_INFO,
	LOG_WARNING,
	LOG_ERROR,
	LOG_CRITICAL,

	LOG_LEVEL_COUNT
} eLogErrorLevel;

void log_printf(eLogErrorLevel errorLevel, const char* format, ...) STBSP__ATTRIBUTE_FORMAT(2, 3);
void log_printf_dbg(eLogErrorLevel errorLevel, const char* file, u32 line, const char* format, ...) STBSP__ATTRIBUTE_FORMAT(4, 5);
void log_printfv(eLogErrorLevel errorLevel, const char* format, va_list args);

#endif // CTH_LOG_H_

