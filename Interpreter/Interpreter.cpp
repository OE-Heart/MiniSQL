/*
 * @Author: Ou Yixin
 * @Date: 2021-06-09 23:19:04
 * @LastEditors: Ou Yixin
 * @LastEditTime: 2021-06-14 17:36:22
 * @Description: 
 * @FilePath: /MiniSQL/Interpreter/Interpreter.cpp
 */
#include "Interpreter.hpp"
#include "../API/API.hpp"


void Interpreter::mainLoop()
{
    std::cout << R"( __  __ _       _ ____   ___  _     )" << "\n";
    std::cout << R"(|  \/  (_)_ __ (_) ___| / _ \| |    )" << "\n";
    std::cout << R"(| |\/| | | '_ \| \___ \| | | | |    )" << "\n";
    std::cout << R"(| |  | | | | | | |___) | |_| | |___ )" << "\n";
    std::cout << R"(|_|  |_|_|_| |_|_|____/ \__\_\_____|)" << "\n";
    std::cout << "\n";
    std::cout << "Welcome to the MiniSQL monitor. Commands end with ;.\n";
    std::cout << "\n";
    std::cout << "Copyright (c) 2021, 2021, OE-Heart, TLBZero and Chenxi Gao.\n";
    std::cout << "\n";
    std::string str;
    std::string input;
    bool exitState = false;
    while (!exitState)
    {
        try
        {
            std::string str = getCmdString();
            // std::cout << str << "\n";
            std::vector<std::string> cmd = Tokenizer(str);
            // for (int i = 0; i < cmd.size(); i++)
            // {
            //     std::cout << cmd.at(i) << "\n";
            // }
            Parse(cmd);
        }
        catch (std::runtime_error &error)
        {
            std::cout << "ERROR : " << error.what() << "\n";
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
        std::cout << "      —> ";
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

void Interpreter::Parse(const std::vector<std::string> &strvec)
{
    if (strvec.at(0) == "create")
    {
        if (strvec.at(1) == "table") parseCreateTable(strvec);
        else if (strvec.at(1) == "index") parseCreateIndex(strvec);
        else std::cout << "ERROR : You have an error in your SQL syntax; check the manual that corresponds to your MiniSQL server version for the right syntax to use near '" << strvec.at(1) << "'\n";
    }
    else if (strvec.at(0) == "drop")
    {
        if (strvec.at(1) == "table") parseDropTable(strvec);
        else if (strvec.at(1) == "index") parseDropIndex(strvec);
        else std::cout << "ERROR : You have an error in your SQL syntax; check the manual that corresponds to your MiniSQL server version for the right syntax to use near '" << strvec.at(1) << "'\n";
    }
    else if (strvec.at(0) == "insert") parseInsert(strvec);
    else if (strvec.at(0) == "delete") parseDelete(strvec);
    else if (strvec.at(0) == "select") parseSelect(strvec);
    else if (strvec.at(0) == "quit" || strvec.at(0) == "exit")
    {
        std::cout << "Bye\n";
        exit(0);
    }
    else std::cout << "ERROR : You have an error in your SQL syntax; check the manual that corresponds to your MiniSQL server version for the right syntax to use near '" << strvec.at(0) << "'\n";
}

void Interpreter::parseCreateTable(const std::vector<std::string> &strvec)
{
    
}

void Interpreter::parseCreateIndex(const std::vector<std::string> &strvec)
{
    if (strvec.at(3) != "on" || strvec.at(5) != "(" || strvec.at(7) != ")")
    {
        std::cout << ErrorMsg;
        return ;
    }

    std::string indexName = strvec.at(2);
    std::string tableName = strvec.at(4);
    std::string columnName = strvec.at(6);
    try
    {
        auto start_time = std::chrono::high_resolution_clock::now();
        API::createIndex(indexName, tableName, columnName);
        auto finish_time = std::chrono::high_resolution_clock::now();
    }
    catch (std::out_of_range)
    {
        throw std::runtime_error(ErrorMsg);
    }
}

void Interpreter::parseDropTable(const std::vector<std::string> &strvec)
{
    std::string tableName = strvec.at(2);
    try
    {
        auto start_time = std::chrono::high_resolution_clock::now();
        API::dropTable(tableName);
        auto finish_time = std::chrono::high_resolution_clock::now();
    }
    catch (std::out_of_range)
    {
        throw std::runtime_error(ErrorMsg);
    }
}

void Interpreter::parseDropIndex(const std::vector<std::string> &strvec)
{
    std::string indexName = strvec.at(2);
    try
    {
        auto start_time = std::chrono::high_resolution_clock::now();
        API::dropIndex(indexName);
        auto finish_time = std::chrono::high_resolution_clock::now();
    }
    catch (std::out_of_range)
    {
        throw std::runtime_error(ErrorMsg);
    }
}

void Interpreter::parseInsert(const std::vector<std::string> &strvec)
{
    if (strvec.at(1) != "into" || strvec.at(3) != "values"  || strvec.at(4) != "(")
    {
        std::cout << ErrorMsg;
        return ;
    }

    std::string tableName = strvec.at(2);

    
}

void Interpreter::parseDelete(const std::vector<std::string> &strvec)
{
    if (strvec.at(1) != "from")
    {
        std::cout << ErrorMsg;
        return ;
    }

    std::string tableName = strvec.at(2);

    if (strvec.at(3) == ";")
    {

    }
    else if (strvec.at(3) == "where")
    {

    }
    else
    {
        std::cout << ErrorMsg;
        return ;
    }
}

void Interpreter::parseSelect(const std::vector<std::string> &strvec)
{
    if (strvec.at(1) != "*" || strvec.at(2) != "from")
    {
        std::cout << ErrorMsg;
        return ;
    }

    std::string tableName = strvec.at(2);

    if (strvec.at(3) == ";")
    {

    }
    else if (strvec.at(3) == "where")
    {

    }
    else
    {
        std::cout << ErrorMsg;
        return ;
    }
}