/*
 * @Author: Ou Yixin
 * @Date: 2021-06-10 10:04:37
 * @LastEditors: Ou Yixin
 * @LastEditTime: 2021-06-10 14:55:16
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
};