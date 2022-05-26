#include <iostream>
#include <string>

#include "Replacer.h"

void test_function(std::string str)
{
    std::cout << str << std::endl;
}

int main()
{
    try
    {
        Config config("config.txt");
        Replacer replacer(config);
        ThreadPool td(config.GetNumThreads());
        replacer.Run(td);

        std::cout << "Task Done" << std::endl;
    }
    catch (const std::exception& e)
    {
        std::cerr << "Error: " << e.what() << std::endl;
    }

    return 0;
}