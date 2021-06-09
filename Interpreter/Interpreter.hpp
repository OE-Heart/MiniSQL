#pragma once

#include <iostream>
#include <string>
#include <vector>
#include <deque>

class Interpreter{
public:
    void mainLoop();
    Interpreter()=default;
private:
    std::deque<std::string> commandQueue;
    static void parse(const std::vector<std::string> &args);
    static std::string getLower(std::string s);
    static void parseSelect(const std::vector<std::string> &args);
    static void parseDelete(const std::vector<std::string> &args);
    static void parseDrop(const std::vector<std::string> &args);
    static void parseCreateIndex(const std::vector<std::string> &args);
    static void parseCreateTable(const std::vector<std::string> &args);
    static void parseInsert(const std::vector<std::string> &args);
    static void parseFile(const std::string &fileName);
    static std::string getCmdString();
    bool inputLine(std::string line);
    void clearCommandQueue();
};