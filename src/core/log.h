#pragma once
#include <iostream>
#include <cstdarg>
#include "string.h"

struct log
{
public:
	enum class log_severity
	{
		info,
		warning,
		error
	};

private:
	log() = delete;
	log(const log&) = delete;
	log(log&&) = delete;

	template <typename ... Args>
	static void log_internal(const std::string &category, log_severity severity, const std::string& fmt, const Args& ... args)
	{
		const std::string log_severity_str = [&]() {
			switch (severity)
			{
			case log::log_severity::info:
				return "info";
			case log::log_severity::warning:
				return "warning";
			case log::log_severity::error:
				return "error";
			}
			return "UNKNOWN LOG CATEGORY";
		}();

		std::cout << "[" << category << "] - " << log_severity_str << ": ";

		std::cout << yete_str::format_string(fmt.c_str(), args...) << "\n";
	}


public:
	template <typename ... Args>
	static void info(const std::string &log_category, const std::string& fmt, const Args& ... args)
	{
		log_internal(log_category, log_severity::info, fmt, args...);
	}

	template <typename ... Args>
	static void warning(const std::string& log_category, const std::string& fmt, const Args& ... args)
	{
		log_internal(log_category, log_severity::warning, fmt, args...);
	}

	template <typename ... Args>
	static void error(const std::string& log_category, const std::string& fmt, const Args& ... args)
	{
		log_internal(log_category, log_severity::error, fmt, args...);
	}
};