/*
 * The MIT License (MIT)
 *
 * Copyright (c) 2021 Evan Teran
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
#include <cstring>
#include <ctime>
#include <iomanip>
#include <iostream>
#include <mutex>
#include <sstream>
#include <string_view>

#if defined __has_include
#if __has_include(<source_location>)
#include <source_location>
#elif __has_include(<experimental/source_location>)
#include <experimental/source_location>
#endif
#endif

namespace logger {

enum Level {
	Emerg  = 0, // Emergency	 - A "panic" condition usually affecting multiple apps/servers/sites. At this level it would usually notify all tech staff on call.
	Alert  = 1, // Alert		 - Should be corrected immediately, therefore notify staff who can fix the problem. An example would be the loss of a primary ISP connection.
	Crit   = 2, // Critical 	 - Should be corrected immediately, but indicates failure in a secondary system, an example is a loss of a backup ISP connection.
	Error  = 3, // Error		 - Non-urgent failures, these should be relayed to developers or admins; each item must be resolved within a given time.
	Warn   = 4, // Warning  	 - Warning messages, not an error, but indication that an error will occur if action is not taken, e.g. file system 85% full - each item must be resolved within a given time.
	Notice = 5, // Notice		 - Events that are unusual but not error conditions - might be summarized in an email to developers or admins to spot potential problems - no immediate action required.
	Info   = 6, // Informational - Normal operational messages - may be harvested for reporting, measuring throughput, etc. - no action required.
	Debug  = 7  // Debug		 - Info useful to developers for debugging the application, not useful during operations.
};

namespace detail {

#if defined __has_include
#if __has_include(<source_location>)
using sl = std::source_location;
#elif __has_include(<experimental/source_location>)
#include <experimental/source_location>
using sl = std::experimental::source_location;
#endif
#endif

//------------------------------------------------------------------------------
// Name: parse_bool_or_default
//------------------------------------------------------------------------------
inline bool parse_bool_or_default(const char *str, bool default_value) {
	if (!str) {
		return default_value;
	}

	return strcmp(str, "1") == 0 || strcmp(str, "Y") == 0 || strcmp(str, "TRUE") == 0;
}

//------------------------------------------------------------------------------
// Name: minimum_level
//------------------------------------------------------------------------------
inline Level minimum_level() {
	static const Level level = [] {
		if (const char *const e = getenv("LOG_LEVEL")) {

			if (e[0] != '\0' && e[1] == '\0') {
				switch (e[0]) {
				case '7':
					return Debug;
				case '6':
					return Info;
				case '5':
					return Notice;
				case '4':
					return Warn;
				case '3':
					return Error;
				case '2':
					return Crit;
				case '1':
					return Alert;
				case '0':
					return Emerg;
				}
			}

			if (strcmp(e, "DEBUG") == 0) {
				return Debug;
			} else if (strcmp(e, "INFO") == 0) {
				return Info;
			} else if (strcmp(e, "NOTICE") == 0) {
				return Notice;
			} else if (strcmp(e, "WARN") == 0) {
				return Warn;
			} else if (strcmp(e, "ERROR") == 0) {
				return Error;
			} else if (strcmp(e, "CRIT") == 0) {
				return Crit;
			} else if (strcmp(e, "ALERT") == 0) {
				return Alert;
			} else if (strcmp(e, "EMERG") == 0) {
				return Emerg;
			}
		}

		return Notice;
	}();

	return level;
}

//------------------------------------------------------------------------------
// Name: colorize
//------------------------------------------------------------------------------
inline bool colorize() {
	static const bool c = [] {
		return parse_bool_or_default(getenv("LOG_COLOR"), true);
	}();

	return c;
}

//------------------------------------------------------------------------------
// Name: timedate
//------------------------------------------------------------------------------
inline bool timedate() {
	static const bool t = [] {
		return parse_bool_or_default(getenv("LOG_TIME"), true);
	}();

	return t;
}

//------------------------------------------------------------------------------
// Name: timedate
//------------------------------------------------------------------------------
inline bool fullpath() {
	static const bool t = [] {
		return parse_bool_or_default(getenv("LOG_FULLPATH"), false);
	}();

	return t;
}

//------------------------------------------------------------------------------
// Name: basename
//------------------------------------------------------------------------------
inline std::string_view basename(std::string_view path) {

	if (fullpath()) {
		return path;
	}

	std::string_view::size_type n = path.rfind('/');
	if (n == std::string_view::npos) {
		return path;
	}

	return path.substr(n + 1);
}

}

//------------------------------------------------------------------------------
// Name: message
//------------------------------------------------------------------------------
inline void message(Level level, std::string_view msg, const detail::sl &location) {

	static std::mutex log_mutex;

	if (level <= detail::minimum_level()) {

		std::stringstream ss;

		if (detail::timedate()) {
			auto now       = std::chrono::system_clock::now();
			auto in_time_t = std::chrono::system_clock::to_time_t(now);

			ss << std::put_time(std::gmtime(&in_time_t), "%Y-%m-%dT%H:%M:%SZ") << ' ';
		}

		ss << '[' << detail::basename(location.file_name()) << ':' << location.line() << "] ";

		if (detail::colorize()) {

			static const char *const ansi_clear  = "\x1B[0m";
			static const char *const ansi_emerg  = "\x1B[7;91m";
			static const char *const ansi_alert  = "\x1B[3;91m";
			static const char *const ansi_crit   = "\x1B[1;91m";
			static const char *const ansi_warn   = "\x1B[92m";
			static const char *const ansi_error  = "\x1B[93m";
			static const char *const ansi_notice = "\x1B[94m";
			static const char *const ansi_info   = "\x1B[97m";
			static const char *const ansi_debug  = "\x1B[98m";

			switch (level) {
			case Emerg:
				ss << '[' << ansi_emerg << "EMERG" << ansi_clear << " ]: ";
				break;
			case Alert:
				ss << '[' << ansi_alert << "ALERT" << ansi_clear << " ]: ";
				break;
			case Crit:
				ss << '[' << ansi_crit << "CRIT" << ansi_clear << "  ]: ";
				break;
			case Error:
				ss << '[' << ansi_error << "ERROR" << ansi_clear << " ]: ";
				break;
			case Warn:
				ss << '[' << ansi_warn << "WARN" << ansi_clear << "  ]: ";
				break;
			case Notice:
				ss << '[' << ansi_notice << "NOTICE" << ansi_clear << "]: ";
				break;
			case Info:
				ss << '[' << ansi_info << "INFO" << ansi_clear << "  ]: ";
				break;
			case Debug:
				ss << '[' << ansi_debug << "DEBUG" << ansi_clear << " ]: ";
				break;
			}
		} else {
			switch (level) {
			case Emerg:
				ss << "[EMERG ]: ";
				break;
			case Alert:
				ss << "[ALERT ]: ";
				break;
			case Crit:
				ss << "[CRIT  ]: ";
				break;
			case Error:
				ss << "[ERROR ]: ";
				break;
			case Warn:
				ss << "[WARN  ]: ";
				break;
			case Notice:
				ss << "[NOTICE]: ";
				break;
			case Info:
				ss << "[INFO  ]: ";
				break;
			case Debug:
				ss << "[DEBUG  ]: ";
				break;
			}
		}

		ss << msg;

		// we render the whole message into the buffer above
		// so that the lock is held for the shortest amount of
		// time possible
		std::unique_lock<std::mutex> lock(log_mutex);
		std::clog << ss.str() << std::endl;
	}
}

//------------------------------------------------------------------------------
// Name: debug
//------------------------------------------------------------------------------
inline void debug(std::string_view msg, const detail::sl &location = detail::sl::current()) {
	message(Debug, msg, location);
}

//------------------------------------------------------------------------------
// Name: info
//------------------------------------------------------------------------------
inline void info(std::string_view msg, const detail::sl &location = detail::sl::current()) {
	message(Info, msg, location);
}

//------------------------------------------------------------------------------
// Name: notice
//------------------------------------------------------------------------------
inline void notice(std::string_view msg, const detail::sl &location = detail::sl::current()) {
	message(Notice, msg, location);
}

//------------------------------------------------------------------------------
// Name: warning
//------------------------------------------------------------------------------
inline void warning(std::string_view msg, const detail::sl &location = detail::sl::current()) {
	message(Warn, msg, location);
}

//------------------------------------------------------------------------------
// Name: error
//------------------------------------------------------------------------------
inline void error(std::string_view msg, const detail::sl &location = detail::sl::current()) {
	message(Error, msg, location);
}

//------------------------------------------------------------------------------
// Name: critical
//------------------------------------------------------------------------------
inline void critical(std::string_view msg, const detail::sl &location = detail::sl::current()) {
	message(Crit, msg, location);
}

//------------------------------------------------------------------------------
// Name: alert
//------------------------------------------------------------------------------
inline void alert(std::string_view msg, const detail::sl &location = detail::sl::current()) {
	message(Alert, msg, location);
}

//------------------------------------------------------------------------------
// Name: emergency
//------------------------------------------------------------------------------
inline void emergency(std::string_view msg, const detail::sl &location = detail::sl::current()) {
	message(Emerg, msg, location);
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

#endif
