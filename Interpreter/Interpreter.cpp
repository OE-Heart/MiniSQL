#include "Interpreter.hpp"
#include <string>

void Interpreter::mainLoop()
{
    std::cout << R"( __  __ _       _ ____   ___  _     )" << std::endl;
    std::cout << R"(|  \/  (_)_ __ (_) ___| / _ \| |    )" << std::endl;
    std::cout << R"(| |\/| | | '_ \| \___ \| | | | |    )" << std::endl;
    std::cout << R"(| |  | | | | | | |___) | |_| | |___ )" << std::endl;
    std::cout << R"(|_|  |_|_|_| |_|_|____/ \__\_\_____|)" << std::endl;
    std::cout << std::endl;
    std::cout << "Welcome to the MiniSQL monitor. Commands end with ;." << std::endl;
    std::cout << std::endl;
    std::cout << "Copyright (c) 2021, 2021, OE-Heart, TLBZero and Chenxi Gao."<< std::endl;
    std::cout << std::endl;
    std::string str;
    std::string input;
    bool exitState = false;
    while (!exitState)
    {
        try
        {
            std::string str = getCmdString();
            // std::cout << str << std::endl;
            //TODO: parse
        }
        catch (std::runtime_error &error)
        {
            std::cout << "[Error] " << error.what() << std::endl;
        }
    }
}

std::string Interpreter::getCmdString()
{
    std::string cmd;
    std::cout << "MiniSQL> ";
    while (true)
    {
        std::string line;
        std::getline(std::cin, line);
        cmd += line;
        while (!line.empty() && isspace(line.back()))  
            line.pop_back();
        if (line.back() == ';') return cmd;
        std::cout << "       > ";
    }
}