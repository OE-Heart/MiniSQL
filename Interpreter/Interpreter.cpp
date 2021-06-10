/*
 * @Author: Ou Yixin
 * @Date: 2021-06-09 23:19:04
 * @LastEditors: Ou Yixin
 * @LastEditTime: 2021-06-10 15:00:13
 * @Description: 
 * @FilePath: /MiniSQL/Interpreter/Interpreter.cpp
 */
#include "Interpreter.hpp"

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
            std::vector<std::string> m = Tokenizer(str);
            // for (int i = 0; i < m.size(); i++)
            // {
            //     std::cout << m.at(i) << std::endl;
            // }
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
    bool first = true;
    while (true)
    {
        std::string line;
        std::getline(std::cin, line);

        if (first && !line.empty() && (line == "quit" || line == "quit;" || line == "exit" || line == "exit;"))
        {
            std::cout << "Bye" << std::endl;
            exit(0);
        }
        else first = false;

        for (int i = 0; i < line.length(); i++) 
        {
            if (line[i] == '*' || line[i] == '=' || line[i] == ',' || line[i] == '(' || line[i] == ')' || line[i] == '<' || line[i] == '>' || line[i] == ';') 
                {
                    if (line[i-1] != ' ') line.insert(i++, " ");
                    if (line[i+1] != ' ') line.insert(++i, " ");
                }
        }

        cmd += line;

        while (!line.empty() && isspace(line.back()))  
            line.pop_back();
        if (line.back() == ';') return cmd;
        cmd.push_back(' ');
        std::cout << "       > ";
    }
}

std::vector<std::string> Interpreter::Tokenizer(const std::string &str)
{
    std::vector<std::string> res;
	if("" == str) return res;
	//先将要切割的字符串从string类型转换为char*类型
	char * strs = new char[str.length() + 1];
	strcpy(strs, str.c_str()); 
 
	char *p = strtok(strs, " ");
	while(p) 
    {
		std::string s = p; //分割得到的字符串转换为string类型
		res.push_back(s); //存入结果数组
		p = strtok(NULL, " ");
	}
 
	return res;
}