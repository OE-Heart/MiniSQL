/*
 * @Author: Ou Yixin
 * @Date: 2021-06-10 10:04:37
 * @LastEditors: Ou Yixin
 * @LastEditTime: 2021-06-10 15:36:47
 * @Description: 
 * @FilePath: /MiniSQL/Interpreter/Interpreter.hpp
 */
#pragma once

#include <iostream>
#include <string>
#include <cstring>
#include <vector>
#include <deque>
// #include "MiniSQLcmd.hpp"

class Interpreter{
public:
    void mainLoop();
    Interpreter() = default;
private:
    static std::string getCmdString();
    std::vector<std::string> Tokenizer(const std::string &str);
    static void Parse(const std::vector<std::string> &strvec);
    static void parseCreateTable(const std::vector<std::string> &strvec);
    static void parseCreateIndex(const std::vector<std::string> &strvec);
    static void parseDropTable(const std::vector<std::string> &strvec);
    static void parseDropIndex(const std::vector<std::string> &strvec);
    static void parseInsert(const std::vector<std::string> &strvec);
    static void parseDelete(const std::vector<std::string> &strvec);
    static void parseSelect(const std::vector<std::string> &strvec);
};