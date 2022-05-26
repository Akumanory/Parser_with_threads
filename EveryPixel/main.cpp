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
        ThreadPool td;

        td.Start();

        td.QueueJob([&](){
            test_function("1");
        });
        td.QueueJob([&](){
            test_function("2");
        });
        td.QueueJob([&](){
            test_function("3");
        });
        td.QueueJob([&](){
            test_function("4");
        });
        td.QueueJob([&](){
            test_function("5");
        });
        td.QueueJob([&](){
            test_function("6");
        });

        td.QueueJob([&](){
            test_function("After_start - 7");
        });



        td.Stop();

    }
    catch(const std::exception& e)
    {
        std::cerr << "Error: " << e.what() << std::endl;
    }

    // std::ifstream myfile ("config.txt");
    // if (myfile.is_open())
    // {
    //     while (myfile.good())
    //     {
    //         std::string line;
    //         getline (myfile,line);
    //         cout << line << endl;
    //     }
    //     myfile.close();
    // }
    // else
    // {
    //     cout << "Unable to open file";
    // }




    // итерирование по директории рекурсивно и поиск файлов
    // for (fs::recursive_directory_iterator i("Test_folder_root"), end; i != end; ++i)
    // if (!is_directory(i->path()))
    // {
    //     cout << i->path() << "\n";
    //     cout << i->path().filename() << "\n";
    // }

    return 0;
}