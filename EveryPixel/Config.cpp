#include <fstream>

#include "Config.h"

Config::Config(std::string&& config_path)
{
    Initialize(config_path);
}

void Config::Initialize(std::string& config_path)
{
    std::ifstream config_file (config_path);
    if (config_file.is_open())
    {
        ParseConfig(config_file);
    }
    else
    {
        throw std::runtime_error("Unable to open file configuration file");
    }
    config_file.close();
}

void Config::ParseConfig(std::ifstream& config_file)
{
    std::string line;
    getline(config_file,line);
    if (line == "THREADS")
    {
        getline(config_file,line);
        m_num_threads = std::stoi(line);
    }
    else
    {
        throw std::runtime_error("No THREADS header in configuration file");
    }
    getline(config_file,line);
    if (line == "DIR")
    {
        getline(config_file,line);
        m_root_folder = line;
    }
    else
    {
        throw std::runtime_error("No DIR header in configuration file");
    }
    getline(config_file,line);
    if (line == "TEMPLATES")
    {
        std::string delimiter = "-";
        while (config_file.good())
        {
            getline(config_file,line);
            size_t delimiter_pos = line.find(delimiter);
            std::string token = line.substr(0, delimiter_pos);
            line.erase(0, delimiter_pos + delimiter.length());
            m_template_value.push_back(TemplateValue(token, line));
        }
    }
    else
    {
        throw std::runtime_error("No TEMPLATES header in configuration file");
    }
}
