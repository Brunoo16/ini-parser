#include "ini-parser.h"

IniParser::IniParser(const std::string &file_name) : ini_filename(file_name)
{
	std::fstream file_stream;
	file_stream.open(file_name, std::fstream::app | std::fstream::in); // If it doesn't exist, it will be created.

	std::string line, current_section, current_field, current_value;

	while (std::getline(file_stream, line))
	{
		if (!line.empty())
		{
			current_value.clear();
			line.erase(std::remove(line.begin(), line.end(), '\t'), line.end()); // Remove tabs if any.

			if (line.front() == '[' && line.back() == ']')
			{
				line.pop_back();
				line.erase(line.begin());

				if (!line.empty())
				{
					current_section = line;
				}
				else
				{
					current_section = "Default"; // If there is nothing between the brackets, let's give the section a name.
				}
			}
			else
			{
				size_t first_equal_sign = line.find_first_of('='), first_space = line.find_first_of(' ');

				if (first_equal_sign != std::string::npos && first_equal_sign != 0)
				{
					if (first_space != std::string::npos && first_space < first_equal_sign)
					{
						current_field = line.substr(0, first_space);
					}
					else
					{
						current_field = line.substr(0, first_equal_sign);
					}
				}

				bool space_before_value = true;

				for (size_t pos = first_equal_sign + 1; pos < line.size(); ++pos)
				{
					if (line.at(pos) != ' ' && space_before_value == true)
					{
						space_before_value = false;
					}

					if (!space_before_value)
					{
						current_value += line.at(pos);
					}
				}
				this->parsed_data[current_section].push_back({ current_field, current_value });
			}
		}
	}
	file_stream.close();
}

std::string IniParser::ReadValueAsString(const std::string &section, const std::string &field, const std::string &default_value) const
{
	if(this->parsed_data.find(section) == this->parsed_data.end())
	{
		return default_value; // Section wasn't found, so return the default value instead.
	}
	else
	{
		auto iter = std::find_if(this->parsed_data.at(section).begin(), this->parsed_data.at(section).end(), [field] (const data &entry) { return !entry.field.compare(field); });
		
		if (iter == this->parsed_data.at(section).end())
		{
			return default_value; // Field not found, so the default value is returned.
		}
		else
		{
			return iter->value;
		}
	}
}

int IniParser::ReadValueAsInteger(const std::string &section, const std::string &field, const int default_value) const
{
	if (this->parsed_data.find(section) == this->parsed_data.end())
	{
		return default_value; // Section wasn't found, so return the default value instead.
	}
	else
	{
		auto iter = std::find_if(this->parsed_data.at(section).begin(), this->parsed_data.at(section).end(), [field] (const data &entry) { return !entry.field.compare(field); });

		if (iter == this->parsed_data.at(section).end())
		{
			return default_value; // Field not found, so the default value is returned.
		}
		else
		{
			try
			{
				return std::stoi(iter->value);
			}

			catch (const std::invalid_argument &ia)
			{
				std::cerr << "ini-parser: " << ia.what() << std::endl;
				return default_value;
			}

			catch (const std::out_of_range &oor)
			{
				std::cerr << "ini-parser: " << oor.what() << std::endl;
				return default_value;
			}
		}
	}
}

float IniParser::ReadValueAsFloat(const std::string &section, const std::string &field, const float default_value) const
{
	if (this->parsed_data.find(section) == this->parsed_data.end())
	{
		return default_value; // Section wasn't found, so return the default value instead.
	}
	else
	{
		auto iter = std::find_if(this->parsed_data.at(section).begin(), this->parsed_data.at(section).end(), [field] (const data &entry) { return !entry.field.compare(field); });

		if (iter == this->parsed_data.at(section).end())
		{
			return default_value; // Field not found, so the default value is returned.
		}
		else
		{
			try
			{
				return std::stof(iter->value);
			}

			catch (const std::invalid_argument &ia)
			{
				std::cerr << "ini-parser: " << ia.what() << std::endl;
				return default_value;
			}

			catch (const std::out_of_range &oor)
			{
				std::cerr << "ini-parser: " << oor.what() << std::endl;
				return default_value;
			}
		}
	}
}

void IniParser::WriteValue(const std::string &section, const std::string &field, const std::string &value)
{
	auto iter = std::find_if(this->parsed_data[section].begin(), this->parsed_data[section].end(), [field] (const data &entry) { return !entry.field.compare(field); });

	if (iter != this->parsed_data[section].end())
	{
		iter->value = value;
	}
	else
	{
		this->parsed_data[section].push_back({ field, value });
	}
}

void IniParser::SaveFile()
{
	std::string current_section, output_string;
	std::ofstream output_file(this->ini_filename, std::ofstream::out);
	output_file.clear();

	for (auto it = this->parsed_data.begin(); it != this->parsed_data.end(); ++it)
	{
		if (current_section.compare(it->first))
		{
			if (current_section.empty())
			{
				output_string += '[' + it->first + "]\n";
			}
			else
			{
				output_string += "\n[" + it->first + "]\n";
			}

			current_section = it->first;

			for (auto it_vector = this->parsed_data.at(current_section).begin(); it_vector != this->parsed_data.at(current_section).end(); ++it_vector)
			{
				output_string += it_vector->field + " = " + it_vector->value + '\n';
			}
		}
	}

	output_file << output_string;
	output_file.close();
}