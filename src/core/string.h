#pragma once
#include <iostream>
#include <cstdarg>
#include <vector>
#include <string>

struct yete_str
{
private:
	yete_str() = delete;
	yete_str(const yete_str&) = delete;
	yete_str(yete_str&&) = delete;

public:

	/* Case insensitive string compare */
	static bool iequals(const std::string& a, const std::string& b)
	{
		const auto sz = a.size();
		if (b.size() != sz)
			return false;
		for (size_t i = 0; i < sz; ++i)
			if (tolower(a[i]) != tolower(b[i]))
				return false;
		return true;
	}

	static std::string format_string(const char* fmt...)
	{
		std::string out_str;

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
						out_str += std::to_string(v);
					}
					else if (fmt[i] == 'c')
					{
						int c = va_arg(args, int); // note automatic conversion to integral type
						out_str += static_cast<char>(c);
					}
					else if (fmt[i] == 'f')
					{
						double d = va_arg(args, double);
						out_str += std::to_string(d);
					}
					else if (fmt[i] == 's')
					{
						char* s = va_arg(args, char*);
						out_str += s;
					}

					++i;
					continue;
				}
			}

			out_str += fmt[i];
			++i;
		}

		va_end(args);

		return out_str;
	}
};
