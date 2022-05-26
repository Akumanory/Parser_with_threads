#include "Replacer.h"

#include <fstream>
#include <iostream>
#include <filesystem>
namespace fs = std::filesystem;


Replacer::Replacer(Config& config)
{
    m_instance_config = &config;
};

void Replacer::Run(ThreadPool& td)
{
    for (fs::recursive_directory_iterator i(m_instance_config->GetRootFolder()), end; i != end; ++i)
    if (!is_directory(i->path()))
    {
        auto str = i->path().string();
        
        td.Submit([this, str]() {
            ReplaceByTemplate(str);
        });
    }
}

void Replacer::ReplaceByTemplate(std::string path)
{
    std::ifstream replace_file_read(path);

    std::vector<std::string> file;
    std::string line;

    if (replace_file_read.is_open())
    {
        while (!replace_file_read.eof())
        {
            getline(replace_file_read, line);
            FindTemplateSubstr(line);
            file.push_back(line);
        }
    }
    else
    {
        throw std::runtime_error("Unable to open file to replace text");
    }
    replace_file_read.close();

    std::ofstream replace_file_write(path);
    if (replace_file_write.is_open())
    {
        for (auto& line : file)
        {
            replace_file_write << line << std::endl;
        }

    }
    else
    {
        throw std::runtime_error("Unable to open file to replace text");
    }
    replace_file_write.close();
}


void Replacer::FindTemplateSubstr(std::string& line)
{
    for (auto& item : m_instance_config->GetTemplatesAndValues())
    {
        size_t index = 0;
        while ((index = line.find(item.m_template, index)) != std::string::npos)
        {
            line.replace(index, item.m_template.length(), item.m_value);
            index += item.m_template.length();
        }
    }
}