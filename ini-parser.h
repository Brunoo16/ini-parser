#ifndef _INI_PARSER_H_
#define _INI_PARSER_H_

#include <iostream>
#include <algorithm>
#include <fstream>
#include <vector>
#include <map>
#include <string>

class IniParser
{
	private:
		struct data
		{
			std::string field;
			std::string value;
		};

		std::map<std::string, std::vector<data>> parsed_data;
		std::string ini_filename;

	public:
		explicit IniParser(const std::string &file_name);

		std::string ReadValueAsString(const std::string &section, const std::string &field, const std::string &default_value = "None") const;
		int ReadValueAsInteger(const std::string &section, const std::string &field, const int default_value = 0) const;
		float ReadValueAsFloat(const std::string &section, const std::string &field, const float default_value = 0.0) const;
		
		void WriteValue(const std::string &section, const std::string &field, const std::string &value);
		void SaveFile();
};

#endif