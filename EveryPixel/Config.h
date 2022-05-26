#pragma once

#include <string>
#include <vector>

struct TemplateValue
{
    TemplateValue(std::string t, std::string v):
    m_template(t),
    m_value(v)
    {};

    std::string m_template;
    std::string m_value;
};


class Config
{
public:
    Config(std::string&& config_filename);

    Config() = delete;
    Config(const Config&) = delete;
    Config& operator=(const Config&) = delete;
    Config(Config&&) = delete;
    Config& operator=(Config&&) = delete;


    const size_t& GetNumThreads() const
    {
        return m_num_threads;
    }
    const std::string& GetRootFolder() const
    {
        return m_root_folder;
    }
    const std::vector<TemplateValue>& GetTemplatesAndValues() const
    {
        return m_template_value;
    }

private:
    void Initialize(std::string& config_filename);
    void ParseConfig(std::ifstream& config_file);
private:
    size_t m_num_threads = 0;
    std::string m_root_folder = "";
    std::vector<TemplateValue> m_template_value = {};
};