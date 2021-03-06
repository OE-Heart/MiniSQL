/*
 * @Author: Ou Yixin
 * @Date: 2021-06-09 23:19:04
 * @LastEditors: Yinwhe
 * @LastEditTime: 2021-06-25 19:40:00
 * @Description: 
 * @FilePath: /MiniSQL/Interpreter/Interpreter.cpp
 */
#include "Interpreter.hpp"
#include "../API/API.hpp"
#include "../CatalogManager/Column.hpp"
#include "Condition.hpp"
extern int IOCnt;
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
    while (true)
    {
        try
        {
            std::string str = getCmdString();
            std::vector<std::string> cmd = Tokenizer(str);
            Parse(cmd);
        }
        catch (std::exception &error)
        {
            std::cerr << error.what() << std::endl;
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
    else if (strvec.at(0) == "execfile") parseExec(strvec);
    else if (strvec.at(0) == "quit" || strvec.at(0) == "exit")
    {
        std::cout << "Bye\n";
        API::endLoop();
    }
    else
    {
        std::cerr << "ERROR : You have an error in your SQL syntax; check the manual that corresponds to your MiniSQL server version for the right syntax to use near '" << strvec.at(0) << "'.\n";
        // throw("ERROR : You have an error in your SQL syntax; check the manual that corresponds to your MiniSQL server version for the right syntax to use near '" + strvec.at(0) + "'.\n");
    }
}

void Interpreter::parseCreateTable(const std::vector<std::string> &strvec)
{
    std::string tableName = strvec.at(2);
    std::vector<Column> Columns;
    std::string primaryKey;
    bool hasPrimaryKey = false;
    int vecSize = strvec.size();

    try
    {
        if (strvec.at(vecSize-7) == "primary" && strvec.at(vecSize-6) == "key")
        {
            primaryKey = strvec.at(vecSize-4);
        }
        else
        {
            std::cerr << "ERROR : You have an error in your SQL syntax; the primary key must be set when create table.\n";
            //throw("ERROR : You have an error in your SQL syntax; the primary key must be set when create table.\n");
        }
        
        for (int i = 4; i < vecSize - 7; )
        {
            Column column;
            column.columnName = strvec.at(i++);

            if (column.columnName == primaryKey)
            {
                hasPrimaryKey = true;
                column.isPrimaryKey = true;
                column.isUnique = true;
            }

            if (strvec.at(i) == "int")
            {
                column.field = Field::INT;
                i++;
            }
            else if (strvec.at(i) == "float")
            {
                column.field = Field::FLOAT;
                i++;
            }
            else if (strvec.at(i) == "char")
            {
                column.field = Field::CHAR;
                i += 2;
                column.charSize = std::stoi(strvec.at(i));
                i += 2;
            }
            else
            {
                std::cerr << "ERROR : You have an error in your SQL syntax; the type "<<strvec.at(i)<<" is not defined.\n";
                // throw("ERROR : You have an error in your SQL syntax; the type "+strvec.at(i)+" is not defined.\n");
                return ;
            }

            if (strvec.at(i) == "unique")
            {
                column.isUnique = true;
                i++;
            }
            i++;
            Columns.push_back(column);
        }

        if (!hasPrimaryKey)
        {
            std::cerr << "ERROR : You have an error in your SQL syntax; the primary key must be set when create table.\n";
            // throw("ERROR : You have an error in your SQL syntax; the primary key must be set when create table.\n");
        }

        auto start = std::chrono::high_resolution_clock::now();
        API::createTable(tableName, Columns, primaryKey);
        auto end = std::chrono::high_resolution_clock::now();
        auto duration = std::chrono::duration_cast<std::chrono::microseconds>(end - start);
        std::cout << "Command was successfully executed and took " << double(duration.count()) * std::chrono::microseconds::period::num / std::chrono::microseconds::period::den << "s.\n";
    }
    catch (std::exception &error)
    {
        std::cerr << error.what() << std::endl;
    }
}

void Interpreter::parseCreateIndex(const std::vector<std::string> &strvec)
{
    if (strvec.at(3) != "on" || strvec.at(5) != "(" || strvec.at(7) != ")")
    {
        std::cerr << "ERROR : You have an error in your SQL syntax; you can use 'create index INDEXNAME on TABLENAME(COLUMNNAME);' to create index.\n";
        // throw("ERROR : You have an error in your SQL syntax; you can use 'create index INDEXNAME on TABLENAME(COLUMNNAME);' to create index.\n");
    }

    std::string indexName = strvec.at(2);
    std::string tableName = strvec.at(4);
    std::string columnName = strvec.at(6);
    try
    {
        auto start = std::chrono::high_resolution_clock::now();
        API::createIndex(indexName, tableName, columnName);
        auto end = std::chrono::high_resolution_clock::now();
        auto duration = std::chrono::duration_cast<std::chrono::microseconds>(end - start);
        std::cout << "Command was successfully executed and took " << double(duration.count()) * std::chrono::microseconds::period::num / std::chrono::microseconds::period::den << "s.\n";
    }
    catch (std::exception &error)
    {
        std::cerr << error.what() << std::endl;
    }
}

void Interpreter::parseDropTable(const std::vector<std::string> &strvec)
{
    std::string tableName = strvec.at(2);
    try
    {
        auto start = std::chrono::high_resolution_clock::now();
        API::dropTable(tableName);
        auto end = std::chrono::high_resolution_clock::now();
        auto duration = std::chrono::duration_cast<std::chrono::microseconds>(end - start);
        std::cout << "Command was successfully executed and took " << double(duration.count()) * std::chrono::microseconds::period::num / std::chrono::microseconds::period::den << "s.\n";
    }
    catch (std::exception &error)
    {
        std::cerr << error.what() << std::endl;
    }
}

void Interpreter::parseDropIndex(const std::vector<std::string> &strvec)
{
    std::string indexName = strvec.at(2);
    try
    {
        auto start = std::chrono::high_resolution_clock::now();
        API::dropIndex(indexName);
        auto end = std::chrono::high_resolution_clock::now();
        auto duration = std::chrono::duration_cast<std::chrono::microseconds>(end - start);
        std::cout << "Command was successfully executed and took " << double(duration.count()) * std::chrono::microseconds::period::num / std::chrono::microseconds::period::den << "s.\n";
    }
    catch (std::exception &error)
    {
        std::cerr << error.what() << std::endl;
    }
}

void Interpreter::parseInsert(const std::vector<std::string> &strvec)
{
    if (strvec.at(1) != "into" || strvec.at(3) != "values" || strvec.at(4) != "(")
    {
        std::cerr << "ERROR : You have an error in your SQL syntax; you can use 'insert into TABLENAME values(VALUE1, VALUE2, ...);' to insert.\n";
        // throw("ERROR : You have an error in your SQL syntax; you can use 'insert into TABLENAME values(VALUE1, VALUE2, ...);' to insert.\n");
    }

    std::string tableName = strvec.at(2);
    std::vector<Value> valueList;
    int vecSize = strvec.size();

    try
    {
        CatalogManager *cm = API::getCatalogManager();
        if (!cm->existTable(tableName))
        {
            std::cerr << "ERROR : You have an error in your SQL syntax; tabel named " << tableName << " doesn't exist.\n";
            // throw("ERROR : You have an error in your SQL syntax; tabel named " + tableName + " doesn't exist.\n");
        }
        Table table = cm->getTable(tableName);

        for (int i = 5, j = 0; i < vecSize-1; i += 2, j++)
        {
            Value v;
            try
            {
                Column column = table.columns.at(j);
                switch (column.field)
                {
                    case Field::INT:
                        v = std::stoi(strvec.at(i));
                        break;
                    case Field::FLOAT:
                        v = std::stof(strvec.at(i));
                        break;
                    case Field::CHAR:
                        v = strvec.at(i).substr(1, strvec.at(i).length() - 2); // remove 2 "'"
                        break;
                }
            }
            catch(...)
            {
                std::cerr << "ERROR : You have an error in your SQL syntax; value type is not defined.\n";
                // throw("ERROR : You have an error in your SQL syntax; value type is not defined.\n");
            }
            valueList.push_back(v);
        }

        auto start = std::chrono::high_resolution_clock::now();
        API::insertOn(tableName, valueList);
        auto end = std::chrono::high_resolution_clock::now();
        auto duration = std::chrono::duration_cast<std::chrono::microseconds>(end - start);
        std::cout << "Command was successfully executed and took " << double(duration.count()) * std::chrono::microseconds::period::num / std::chrono::microseconds::period::den << "s.\n";
    }
    catch (std::exception &error)
    {
        std::cerr << error.what() << std::endl;
    }
}

void Interpreter::parseDelete(const std::vector<std::string> &strvec)
{

    if (strvec.at(1) != "from")
    {
        std::cerr << "ERROR : You have an error in your SQL syntax; you can use 'delete from TABLENAME;' or 'delete from TABLENAME where (CON1 and CON2 ...);' to delete.\n";
        // throw("ERROR : You have an error in your SQL syntax; you can use 'delete from TABLENAME;' or 'delete from TABLENAME where (CON1 and CON2 ...);' to delete.\n");
    }

    std::string tableName = strvec.at(2);
    std::vector<Condition> conditions;
    int vecSize = strvec.size();

    if (strvec.at(3) == ";")
    {
        try
        {
            auto start = std::chrono::high_resolution_clock::now();
            API::deleteAll(tableName);
            auto end = std::chrono::high_resolution_clock::now();
            auto duration = std::chrono::duration_cast<std::chrono::microseconds>(end - start);
            std::cout << "Command was successfully executed and took " << double(duration.count()) * std::chrono::microseconds::period::num / std::chrono::microseconds::period::den << "s.\n";
            return ;
        }
        catch (std::exception &error)
        {
            std::cerr << error.what() << std::endl;
        }
    }

    if (strvec.at(3) != "where")
    {
        std::cerr << "ERROR : You have an error in your SQL syntax; you can use 'delete from TABLENAME;' or 'delete from TABLENAME where (CON1 and CON2 ...);' to delete.\n";
        // throw("ERROR : You have an error in your SQL syntax; you can use 'delete from TABLENAME;' or 'delete from TABLENAME where (CON1 and CON2 ...);' to delete.\n");
    }

    try
    {
        CatalogManager *cm = API::getCatalogManager();
        if (!cm->existTable(tableName))
        {
            std::cerr << "ERROR : You have an error in your SQL syntax; tabel named " << tableName << " doesn't exist.\n";
            // throw("ERROR : You have an error in your SQL syntax; tabel named " + tableName + " doesn't exist.\n");
        }
        Table table = cm->getTable(tableName);

        for (int i = 4; i < vecSize; )
        {
            std::string columnName = strvec.at(i++);
            OP op;
            if (strvec.at(i) == "=")
            {
                op = OP::EQ;
            }
            else if (strvec.at(i) == "<" && strvec.at(i+1) == ">")
            {
                op = OP::NEQ;
                i++;
            }
            else if (strvec.at(i) == "<")
            {
                if (strvec.at(i+1) == ">")
                {
                    op = OP::NEQ;
                    i++;
                }
                else if (strvec.at(i+1) == "=")
                {
                    op = OP::LEQ;
                    i++;
                }
                else
                {
                    op = OP::L;
                }
            }
            else if (strvec.at(i) == ">")
            {
                if (strvec.at(i+1) == "=")
                {
                    op = OP::GEQ;
                    i++;
                }
                else
                {
                    op = OP::G;
                }
            }
            else
            {
                std::cerr << "ERROR : You have an error in your SQL syntax; operator is not defined.\n";
                // throw("ERROR : You have an error in your SQL syntax; operator is not defined.\n");
            }
            i++;
            
            Value v;
            try
            {
                int j;
                for (j = 0; j < table.columns.size(); j++)
                {
                    if (table.columns.at(j).columnName == columnName) break;
                }
                if (j == table.columns.size())
                {
                    std::cerr << "ERROR : You have an error in your SQL syntax; column named " << columnName << " doesn't exist.\n";
                    // throw("ERROR : You have an error in your SQL syntax; column named " + columnName + " doesn't exist.\n");
                }
                Column column = table.columns.at(j);
                switch (column.field)
                {
                    case Field::INT:
                        v = std::stoi(strvec.at(i));
                        break;
                    case Field::FLOAT:
                        v = std::stof(strvec.at(i));
                        break;
                    case Field::CHAR:
                        v = strvec.at(i).substr(1, strvec.at(i).length() - 2); // remove 2 "'"
                        break;
                }
            }
            catch(...)
            {
                std::cerr << "ERROR : You have an error in your SQL syntax; value type is not defined.\n";
                // throw("ERROR : You have an error in your SQL syntax; value type is not defined.\n");
            }
            Condition condition;
            condition.columnName = columnName;
            condition.op = op;
            condition.value = v;
            conditions.push_back(condition);
            i += 2; // pass "and"
        }

        auto start = std::chrono::high_resolution_clock::now();
        API::deleteFrom(tableName, conditions);
        auto end = std::chrono::high_resolution_clock::now();
        auto duration = std::chrono::duration_cast<std::chrono::microseconds>(end - start);
        std::cout << "Command was successfully executed and took " << double(duration.count()) * std::chrono::microseconds::period::num / std::chrono::microseconds::period::den << "s.\n";
    }
    catch (std::exception &error)
    {
        std::cerr << error.what() << std::endl;
    }
}

void Interpreter::parseSelect(const std::vector<std::string> &strvec)
{
    if (strvec.at(1) != "*" || strvec.at(2) != "from")
    {
        std::cerr << "ERROR : You have an error in your SQL syntax; you can use 'select * from TABLENAME;' or 'select * from TABLENAME where (CON1 and CON2 ...);'.\n";
        // throw("ERROR : You have an error in your SQL syntax; you can use 'select * from TABLENAME;' or 'select * from TABLENAME where (CON1 and CON2 ...);'.\n");
    }

    std::string tableName = strvec.at(3);
    std::vector<Condition> conditions;
    int vecSize = strvec.size();

    if (strvec.at(4) == ";")
    {
        try
        {
            auto start = std::chrono::high_resolution_clock::now();
            API::selectAll(tableName);
            auto end = std::chrono::high_resolution_clock::now();
            auto duration = std::chrono::duration_cast<std::chrono::microseconds>(end - start);
            std::cout << "Command was successfully executed and took " << double(duration.count()) * std::chrono::microseconds::period::num / std::chrono::microseconds::period::den << "s.\n";
            return ;
        }
        catch (std::exception &error)
        {
            std::cerr << error.what() << std::endl;
        }
    }

    if (strvec.at(4) != "where")
    {
        std::cerr << "ERROR : You have an error in your SQL syntax; you can use 'select * from TABLENAME;' or 'select * from TABLENAME where (CON1 and CON2 ...);'.\n";
        // throw("ERROR : You have an error in your SQL syntax; you can use 'select * from TABLENAME;' or 'select * from TABLENAME where (CON1 and CON2 ...);'.\n");
    }

    try
    {
        CatalogManager *cm = API::getCatalogManager();
        Table table = cm->getTable(tableName);

        for (int i = 5; i < vecSize; )
        {
            std::string columnName = strvec.at(i++);
            OP op;
            if (strvec.at(i) == "=")
            {
                op = OP::EQ;
            }
            else if (strvec.at(i) == "<" && strvec.at(i+1) == ">")
            {
                op = OP::NEQ;
                i++;
            }
            else if (strvec.at(i) == "<")
            {
                if (strvec.at(i+1) == ">")
                {
                    op = OP::NEQ;
                    i++;
                }
                else if (strvec.at(i+1) == "=")
                {
                    op = OP::LEQ;
                    i++;
                }
                else
                {
                    op = OP::L;
                }
            }
            else if (strvec.at(i) == ">")
            {
                if (strvec.at(i+1) == "=")
                {
                    op = OP::GEQ;
                    i++;
                }
                else
                {
                    op = OP::G;
                }
            }
            else
            {
                std::cerr << "ERROR : You have an error in your SQL syntax; operator is not defined.\n";
                // throw("ERROR : You have an error in your SQL syntax; operator is not defined.\n");
            }
            i++;

            Value v;
            try
            {
                int j;
                for (j = 0; j < table.columns.size(); j++)
                {
                    if (table.columns.at(j).columnName == columnName) break;
                }
                if (j == table.columns.size())
                {
                    std::cerr << "ERROR : You have an error in your SQL syntax; column named " << columnName << " doesn't exist.\n";
                    // throw("ERROR : You have an error in your SQL syntax; column named " + columnName + " doesn't exist.\n");
                }
                Column column = table.columns.at(j);

                // std::cout << strvec.at(i) <<"\n";

                switch (column.field)
                {
                    case Field::INT:
                        v = std::stoi(strvec.at(i));
                        break;
                    case Field::FLOAT:
                        v = std::stof(strvec.at(i));
                        break;
                    case Field::CHAR:
                        v = strvec.at(i).substr(1, strvec.at(i).length() - 2); // remove 2 "'"
                        break;
                }
            }
            catch(...)
            {
                std::cerr << "ERROR : You have an error in your SQL syntax; value type is not defined.\n";
                // throw("ERROR : You have an error in your SQL syntax; value type is not defined.\n");
            }
            Condition condition;
            condition.columnName = columnName;
            condition.op = op;
            condition.value = v;
            conditions.push_back(condition);
            i += 2; // pass "and"
        }
        auto start = std::chrono::high_resolution_clock::now();
        API::select(tableName, conditions);
        auto end = std::chrono::high_resolution_clock::now();
        auto duration = std::chrono::duration_cast<std::chrono::microseconds>(end - start);
        std::cout << "Command was successfully executed and took " << double(duration.count()) * std::chrono::microseconds::period::num / std::chrono::microseconds::period::den << "s.\n";
    }
    catch (std::exception &error)
    {
        std::cerr << error.what() << std::endl;
    }
}

void Interpreter::parseExec(const std::vector<std::string> &strvec)
{
    if (strvec.at(2) != ";")
    {
        std::cerr << "ERROR : You have an error in your SQL syntax; you can use 'execfile FILENAME;' to execute.\n";
        // throw("ERROR : You have an error in your SQL syntax; you can use 'execfile FILENAME;' to execute.\n");
    }

    std::string fileName = "test/"+strvec.at(1);
    std::ifstream infile(fileName);

    if (!infile)
    {
        std::cerr << "ERROR : You have an error in your SQL syntax; file named " << fileName << " doesn't exist.\n";
        // throw("ERROR : You have an error in your SQL syntax; file named " + fileName + " doesn't exist.\n");
    }
    
    auto start = std::chrono::high_resolution_clock::now();
    std::string str;
    std::string line;
    int cmdNum = 0;
    while (std::getline(infile, line))
    {
        for (int i = 0; i < line.length(); i++) 
        {
            if (line[i] == '*' || line[i] == '=' || line[i] == ',' || line[i] == '(' || line[i] == ')' || line[i] == '<' || line[i] == '>' || line[i] == ';') 
                {
                    if (line[i-1] != ' ') line.insert(i++, " ");
                    if (line[i+1] != ' ') line.insert(++i, " ");
                }
        }

        str += line;

        while (!line.empty() && isspace(line.back()))  
            line.pop_back();
        if (line.back() == ';')
        {
            cmdNum++;
            std::cout << "Command " << cmdNum << ": \n";
            std::vector<std::string> cmd = Tokenizer(str);
            Parse(cmd);
            str.clear();
            continue;
        }
        str.push_back(' ');
    }
    auto end = std::chrono::high_resolution_clock::now();
    auto duration = std::chrono::duration_cast<std::chrono::microseconds>(end - start);
    std::cout << cmdNum << " commands were successfully executed and took " << double(duration.count()) *std::chrono::microseconds::period::num / std::chrono::microseconds::period::den << "s.\n";
    std::cout << "IO operation times: " << IOCnt << std::endl;
    IOCnt = 0;
}