#pragma once
#include <iostream>
#include <cstdarg>

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

	/*
	* TODO: This is a pretty unsafe function with no compile time, or runt time, checks for correct parameter count and types
	*/
	static void log_internal(const std::string &category, log_severity severity, const char* fmt...)
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

		va_list args;
		va_start(args, fmt);

		const std::vector<char> types = { 'd', 'c', 'f', 's' };

		int i = 0;
		while (fmt[i] != '\0')
		{
			if (fmt[i] == '%' && fmt[i + 1] != '\0')
			{
				if (std::find(types.begin(), types.end(), fmt[i + 1]) != types.end())
				{
					++i;
					if (fmt[i] == 'd')
					{
						int v = va_arg(args, int);
						std::cout << v;
					}
					else if (fmt[i] == 'c')
					{
						int c = va_arg(args, int); // note automatic conversion to integral type
						std::cout << static_cast<char>(c);
					}
					else if (fmt[i] == 'f')
					{
						double d = va_arg(args, double);
						std::cout << d;
					}
					else if (fmt[i] == 's')
					{
						char* s = va_arg(args, char*);
						std::cout << s;
					}

					++i;
					continue;
				}
			}

			std::cout << fmt[i];
			++i;
		}

		std::cout << "\n";

		va_end(args);
	}


public:
	template <typename ... Args>
	static void info(const std::string &log_category, const std::string& fmt, const Args& ... args)
	{
		log_internal(log_category, log_severity::info, fmt.c_str(), args...);
	}

	template <typename ... Args>
	static void warning(const std::string& log_category, const std::string& fmt, const Args& ... args)
	{
		log_internal(log_category, log_severity::warning, fmt.c_str(), args...);
	}

	template <typename ... Args>
	static void error(const std::string& log_category, const std::string& fmt, const Args& ... args)
	{
		log_internal(log_category, log_severity::error, fmt.c_str(), args...);
	}
};