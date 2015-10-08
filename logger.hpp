/*
 * The MIT License (MIT)
 * 
 * Copyright (c) 2014 - 2015 Digital Operatives LLC
 *                           evan.teran@digitaloperatives.com
 *                           nathan.landon@digitaloperatives.com
 * 
 * Permission is hereby granted, free of charge, to any person obtaining a copy
 * of this software and associated documentation files (the "Software"), to deal
 * in the Software without restriction, including without limitation the rights
 * to use, copy, modify, merge, publish, distribute, sublicense, and/or sell
 * copies of the Software, and to permit persons to whom the Software is
 * furnished to do so, subject to the following conditions:
 * 
 * The above copyright notice and this permission notice shall be included in all
 * copies or substantial portions of the Software.
 * 
 * THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND, EXPRESS OR
 * IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF MERCHANTABILITY,
 * FITNESS FOR A PARTICULAR PURPOSE AND NONINFRINGEMENT. IN NO EVENT SHALL THE
 * AUTHORS OR COPYRIGHT HOLDERS BE LIABLE FOR ANY CLAIM, DAMAGES OR OTHER
 * LIABILITY, WHETHER IN AN ACTION OF CONTRACT, TORT OR OTHERWISE, ARISING FROM,
 * OUT OF OR IN CONNECTION WITH THE SOFTWARE OR THE USE OR OTHER DEALINGS IN THE
 * SOFTWARE.
 */

#ifndef LOGGER_HPP_
#define LOGGER_HPP_

#include <algorithm>
#include <cctype>
#include <cstdarg>
#include <cstdio>
#include <cstdlib>
#include <cstring>
#include <ctime>
#include <iostream>
#include <mutex>
#include <sstream>

namespace logger {

enum Level {
	EMERG  = 0, // Emergency     - A "panic" condition usually affecting multiple apps/servers/sites. At this level it would usually notify all tech staff on call.
	ALERT  = 1, // Alert         - Should be corrected immediately, therefore notify staff who can fix the problem. An example would be the loss of a primary ISP connection.
	CRIT   = 2, // Critical      - Should be corrected immediately, but indicates failure in a secondary system, an example is a loss of a backup ISP connection.
	ERROR  = 3, // Error         - Non-urgent failures, these should be relayed to developers or admins; each item must be resolved within a given time.
	WARN   = 4, // Warning       - Warning messages, not an error, but indication that an error will occur if action is not taken, e.g. file system 85% full - each item must be resolved within a given time.
	NOTICE = 5, // Notice        - Events that are unusual but not error conditions - might be summarized in an email to developers or admins to spot potential problems - no immediate action required.
	INFO   = 6, // Informational - Normal operational messages - may be harvested for reporting, measuring throughput, etc. - no action required.
	DEBUG  = 7  // Debug         - Info useful to developers for debugging the application, not useful during operations.
};

namespace detail {

static const int buffer_size = 1024;

//------------------------------------------------------------------------------
// Name: get_minimum_from_env
//------------------------------------------------------------------------------
inline Level get_minimum_from_env() {

	if(const char *const e = getenv("LOG_LEVEL")) {

		if(e[0] != '\0' && e[1] == '\0') {
			switch(e[0]) {
			case '7': return DEBUG;
			case '6': return INFO;
			case '5': return NOTICE;
			case '4': return WARN;
			case '3': return ERROR;
			case '2': return CRIT;
			case '1': return ALERT;
			case '0': return EMERG;
			}		
		}

		if(strcmp(e, "DEBUG") == 0) {
			return DEBUG;
		} else if(strcmp(e, "INFO") == 0) {
			return INFO;
		} else if(strcmp(e, "NOTICE") == 0) {
			return NOTICE;
		} else if(strcmp(e, "WARN") == 0) {
			return WARN;
		} else if(strcmp(e, "ERROR") == 0) {
			return ERROR;
		} else if(strcmp(e, "CRIT") == 0) {
			return CRIT;
		} else if(strcmp(e, "ALERT") == 0) {
			return ALERT;
		} else if(strcmp(e, "EMERG") == 0) {
			return EMERG;
		}
	}

	return NOTICE;
}

//------------------------------------------------------------------------------
// Name: get_colorize_from_env
//------------------------------------------------------------------------------
inline bool get_colorize_from_env() {

	if(const char *const e = getenv("LOG_COLOR")) {
		if(strcmp(e, "1") == 0) {
			return true;
		} else if(strcmp(e, "TRUE") == 0) {
			return true;
		} else {
			return false;
		}
	}

	return true;
}

//------------------------------------------------------------------------------
// Name: get_timedate_from_env()
//------------------------------------------------------------------------------
inline bool get_timedate_from_env() {

	if(const char *const e = getenv("LOG_TIME")) {
		if(strcmp(e, "1") == 0) {
			return true;
		} else if(strcmp(e, "TRUE") == 0) {
			return true;
		} else {
			return false;
		}
	}

	return true;
}
//------------------------------------------------------------------------------
// Name: minimum_level
//------------------------------------------------------------------------------
inline Level minimum_level() {
	static const Level level = get_minimum_from_env();
	return level;
}

//------------------------------------------------------------------------------
// Name: colorize
//------------------------------------------------------------------------------
inline bool colorize() {
	static const bool c = get_colorize_from_env();
	return c;
}

//------------------------------------------------------------------------------
// Name: timedate
//------------------------------------------------------------------------------
inline bool timedate() {
	static const bool t = get_timedate_from_env();
	return t;
}

//------------------------------------------------------------------------------
// Name: log_mutex
//------------------------------------------------------------------------------
inline std::mutex &log_mutex() {
	static std::mutex m;
	return m;
}

class log_impl {
public:
	log_impl(Level level) noexcept : level_(level)  {

		if(level_ <= minimum_level()) {

			if(timedate()){
				char buf[64];
				const time_t t = time(nullptr);
			
				strftime(buf, sizeof(buf), "%Y-%m-%dT%H:%M:%SZ +0000 ", gmtime(&t));
				ss_ << buf;
			}

			if(colorize()) {

				static const char *const ansi_clear  = "\x1B[0m";
				static const char *const ansi_emerg  = "\x1B[1;5;91m";
				static const char *const ansi_alert  = "\x1B[1;91m";
				static const char *const ansi_crit   = "\x1B[91m";
				static const char *const ansi_error  = "\x1B[93m";
				static const char *const ansi_warn   = "\x1B[96m";
				static const char *const ansi_notice = "\x1B[94m";
				static const char *const ansi_info   = "\x1B[92m";
				static const char *const ansi_debug  = "\x1B[95m";

				switch(level) {
				case EMERG : ss_ << '[' << ansi_emerg  << "EMERG" << ansi_clear << "  ]: "; break;
				case ALERT : ss_ << '[' << ansi_alert  << "ALERT" << ansi_clear << "  ]: "; break;
				case CRIT  : ss_ << '[' << ansi_crit   << "CRIT" << ansi_clear << "   ]: "; break;
				case ERROR : ss_ << '[' << ansi_error  << "ERROR" << ansi_clear << "  ]: "; break;
				case WARN  : ss_ << '[' << ansi_warn   << "WARN" << ansi_clear << "   ]: "; break;
				case NOTICE: ss_ << '[' << ansi_notice << "NOTICE" << ansi_clear << " ]: "; break;
				case INFO  : ss_ << '[' << ansi_info   << "INFO" << ansi_clear << "   ]: "; break;
				case DEBUG : ss_ << '[' << ansi_debug  << "DEBUG" << ansi_clear << "  ]: "; break;
				}
			} else {
				switch(level) {
				case EMERG : ss_ << "[EMERG  ]: "; break;
				case ALERT : ss_ << "[ALERT  ]: "; break;
				case CRIT  : ss_ << "[CRIT   ]: "; break;
				case ERROR : ss_ << "[ERROR  ]: "; break;
				case WARN  : ss_ << "[WARN   ]: "; break;
				case NOTICE: ss_ << "[NOTICE ]: "; break;
				case INFO  : ss_ << "[INFO   ]: "; break;
				case DEBUG : ss_ << "[DEBUG  ]: "; break;
				}
			}
		}
	}

	~log_impl() noexcept {
		if(level_ <= minimum_level()) {
			std::unique_lock<std::mutex> lock(log_mutex());
			std::clog << ss_.str() << std::endl;
		}
	}

private:
	log_impl(const log_impl &);
	log_impl &operator=(const log_impl &);

public:
	template <class T>
	log_impl &operator<<(const T &x) noexcept {
		if(level_ <= minimum_level()) {
			ss_ << x;
		}
		return *this;
	}

private:
	std::ostringstream ss_;
	const Level        level_;
};

class log_ref {
private:
	explicit log_ref(log_impl *p) noexcept : ptr_(p) {
	}

public:
	explicit log_ref(Level level) noexcept : ptr_(new log_impl(level)) {
	}

	~log_ref() noexcept {
		delete ptr_;
	}

	log_ref(log_ref &&other) noexcept : ptr_(other.ptr_) {
		other.ptr_ = nullptr;
	}

	log_ref &operator=(log_ref &&rhs) noexcept {
		if(this != &rhs) {
			ptr_ = rhs.ptr_;
			rhs.ptr_ = nullptr;
		}
		return *this;
	}
public:
	template <class T>
	log_impl &operator<<(const T &x) noexcept {
		*ptr_ << x;
		return *ptr_;
	}
private:
	log_impl *ptr_;
};

}

//------------------------------------------------------------------------------
// Name: message
//------------------------------------------------------------------------------
inline detail::log_ref message(Level level) {
	return detail::log_ref(level);
}

//------------------------------------------------------------------------------
// Name: debug
//------------------------------------------------------------------------------
inline detail::log_ref debug() {
	return message(DEBUG);
}

//------------------------------------------------------------------------------
// Name: info
//------------------------------------------------------------------------------
inline detail::log_ref info() {
	return message(INFO);
}

//------------------------------------------------------------------------------
// Name: notice
//------------------------------------------------------------------------------
inline detail::log_ref notice() {
	return message(NOTICE);
}

//------------------------------------------------------------------------------
// Name: warning
//------------------------------------------------------------------------------
inline detail::log_ref warning() {
	return message(WARN);
}

//------------------------------------------------------------------------------
// Name: error
//------------------------------------------------------------------------------
inline detail::log_ref error() {
	return message(ERROR);
}

//------------------------------------------------------------------------------
// Name: critical
//------------------------------------------------------------------------------
inline detail::log_ref critical() {
	return message(CRIT);
}

//------------------------------------------------------------------------------
// Name: alert
//------------------------------------------------------------------------------
inline detail::log_ref alert() {
	return message(ALERT);
}

//------------------------------------------------------------------------------
// Name: emergency
//------------------------------------------------------------------------------
inline detail::log_ref emergency() {
	return message(EMERG);
}

//------------------------------------------------------------------------------
// Name: message
//------------------------------------------------------------------------------
inline void message(Level level, const char *fmt, ...) noexcept {
	char buf[detail::buffer_size];
	va_list ap;
	va_start(ap, fmt);
	vsnprintf(buf, sizeof(buf), fmt, ap);
	va_end(ap);
	detail::log_impl(level) << buf;
}

//------------------------------------------------------------------------------
// Name: debug
//------------------------------------------------------------------------------
inline void debug(const char *fmt, ...) noexcept {
	char buf[detail::buffer_size];
	va_list ap;
	va_start(ap, fmt);
	vsnprintf(buf, sizeof(buf), fmt, ap);
	va_end(ap);
	debug() << buf;
}

//------------------------------------------------------------------------------
// Name: info
//------------------------------------------------------------------------------
inline void info(const char *fmt, ...) noexcept {
	char buf[detail::buffer_size];
	va_list ap;
	va_start(ap, fmt);
	vsnprintf(buf, sizeof(buf), fmt, ap);
	va_end(ap);
	info() << buf;
}

//------------------------------------------------------------------------------
// Name: notice
//------------------------------------------------------------------------------
inline void notice(const char *fmt, ...) noexcept {
	char buf[detail::buffer_size];
	va_list ap;
	va_start(ap, fmt);
	vsnprintf(buf, sizeof(buf), fmt, ap);
	va_end(ap);
	notice() << buf;
}

//------------------------------------------------------------------------------
// Name: warning
//------------------------------------------------------------------------------
inline void warning(const char *fmt, ...) noexcept {
	char buf[detail::buffer_size];
	va_list ap;
	va_start(ap, fmt);
	vsnprintf(buf, sizeof(buf), fmt, ap);
	va_end(ap);
	warning() << buf;
}

//------------------------------------------------------------------------------
// Name: error
//------------------------------------------------------------------------------
inline void error(const char *fmt, ...) noexcept {
	char buf[detail::buffer_size];
	va_list ap;
	va_start(ap, fmt);
	vsnprintf(buf, sizeof(buf), fmt, ap);
	va_end(ap);
	error() << buf;
}

//------------------------------------------------------------------------------
// Name: critical
//------------------------------------------------------------------------------
inline void critical(const char *fmt, ...) noexcept {
	char buf[detail::buffer_size];
	va_list ap;
	va_start(ap, fmt);
	vsnprintf(buf, sizeof(buf), fmt, ap);
	va_end(ap);
	critical() << buf;
}

//------------------------------------------------------------------------------
// Name: alert
//------------------------------------------------------------------------------
inline void alert(const char *fmt, ...) noexcept {
	char buf[detail::buffer_size];
	va_list ap;
	va_start(ap, fmt);
	vsnprintf(buf, sizeof(buf), fmt, ap);
	va_end(ap);
	alert() << buf;
}

//------------------------------------------------------------------------------
// Name: emergency
//------------------------------------------------------------------------------
inline void emergency(const char *fmt, ...) noexcept {
	char buf[detail::buffer_size];
	va_list ap;
	va_start(ap, fmt);
	vsnprintf(buf, sizeof(buf), fmt, ap);
	va_end(ap);
	emergency() << buf;
}

//------------------------------------------------------------------------------
// Name: basename
//------------------------------------------------------------------------------
inline std::string basename(const std::string &path) {
	std::string::size_type n = path.rfind('/');
	if(n == std::string::npos) {
		return path;
	} else {
		return path.substr(n + 1);
	}
}

// A class which will easily handle redirecting std::clog to the
// ostream of your choosing, and properly restore it before shutdown
// to avoid errors
class redirector {
public:
	redirector(std::ostream &os) {
		rdbuf_ = std::clog.rdbuf();
		std::clog.rdbuf(os.rdbuf());
	}

	~redirector() {
		std::clog.rdbuf(rdbuf_);
	}

private:
	std::streambuf *rdbuf_;
};

}

#define LOG_DEBUG()     logger::debug()     << '[' << logger::basename(__FILE__) << ':' << __LINE__ << "] "
#define LOG_INFO()      logger::info()      << '[' << logger::basename(__FILE__) << ':' << __LINE__ << "] "
#define LOG_NOTICE()    logger::notice()    << '[' << logger::basename(__FILE__) << ':' << __LINE__ << "] "
#define LOG_WARNING()   logger::warning()   << '[' << logger::basename(__FILE__) << ':' << __LINE__ << "] "
#define LOG_ERROR()     logger::error()     << '[' << logger::basename(__FILE__) << ':' << __LINE__ << "] "
#define LOG_CRITICAL()  logger::critical()  << '[' << logger::basename(__FILE__) << ':' << __LINE__ << "] "
#define LOG_ALERT()     logger::alert()     << '[' << logger::basename(__FILE__) << ':' << __LINE__ << "] "
#define LOG_EMERGENCY() logger::emergency() << '[' << logger::basename(__FILE__) << ':' << __LINE__ << "] "

#endif

