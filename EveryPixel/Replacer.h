#pragma once

#include "Config.h"
#include "ThreadPool.h"

class Replacer
{
public:
    Replacer(Config& config);

    void Run(ThreadPool& td);

    Replacer() = delete;
    Replacer(const Replacer&) = delete;
    Replacer& operator=(const Replacer&) = delete;
    Replacer(Replacer&&) = delete;
    Replacer& operator=(Replacer&&) = delete;
private:
    void ReplaceByTemplate(std::string path);
    void FindTemplateSubstr(std::string& line);
private:
    Config* m_instance_config;
};