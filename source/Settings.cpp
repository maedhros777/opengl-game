#include "Settings.h"

ERR_CODE Settings::load(const std::string &filename)
{
    FILE *file = file_open(filename, "r");

    if (!file)
	{
		badfile = filename;
        return FILE_NOT_FOUND;
	}

    char first[30];
    char second[30];

    while (!feof(file))
    {
        if (fscanf(file, "%s = %[^\n]\n", first, second) < 0)
        {
            fclose(file);
			badfile = filename;
            return CORRUPTED_FILE;
        }

        std::string second_str = second;
        int len = second_str.size();

        if (second_str[0] == '"' && second_str[len - 1] == '"')
            str_map[first] = second_str.substr(1, len - 2);
        else if (second_str[len - 1] == 'f')
            float_map[first] = atof(second);
        else
            int_map[first] = atoi(second);
    }

    fclose(file);
    return NONE;
}

std::string Settings::getStr(const std::string &name)
{
    if (str_map.find(name) == str_map.end())
        die("The configuration file did not contain the following required string: " + name);

    return str_map[name];
}

int Settings::getInt(const std::string &name)
{
    if (int_map.find(name) == int_map.end())
        die("The configuration file did not contain the following required constant: " + name);

    return int_map[name];
}

float Settings::getFloat(const std::string &name)
{
    if (float_map.find(name) == float_map.end())
        die("The configuration file did not contain the following required constant: " + name);

    return float_map[name];
}
