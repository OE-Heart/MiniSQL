/*
 * @Author: Yinwhe
 * @Date: 2021-06-20 13:54:07
 * @LastEditors: Yinwhe
 * @LastEditTime: 2021-06-21 18:57:12
 * @Description: file information
 * @Copyright: Copyright (c) 2021
 */

#include "IndexManager.hpp"
IndexManager::IndexManager(){
    int_im = new _index_manager<int>();
    float_im = new _index_manager<double>();
    char_im = new _index_manager<std::string>();
}

IndexManager::~IndexManager(){
    delete int_im;
    delete float_im;
    delete char_im;
}

void IndexManager::CreateIndex(const std::string index_name, Table & table, const std::string & column_name){
    int index = table.indexOfCol(column_name);
    auto &attr = table.columns[index];
    switch (attr.field){
        case Field::INT:    return int_im->create_index(index_name, table, column_name);
        case Field::FLOAT:  return float_im->create_index(index_name, table, column_name);
        case Field::CHAR:   return char_im->create_index(index_name, table, column_name);
    }
}

int IndexManager::FindIndex(const std::string index_name, Table & table, const std::string & column_name, const Value &val){
    int index = table.indexOfCol(column_name);
    auto &attr = table.columns[index];
    switch(attr.field){
        case Field::INT:    return int_im->find_index(index_name, table, std::get<int>(val));
        case Field::FLOAT:  return float_im->find_index(index_name, table, std::get<double>(val));
        case Field::CHAR:   return char_im->find_index(index_name, table, std::get<std::string>(val));
    }
}

void IndexManager::InsertIndex(const std::string index_name, Table & table, const std::string & column_name, const Value &val, int offset){
    int index = table.indexOfCol(column_name);
    auto &attr = table.columns[index];
    switch(attr.field){
        case Field::INT:    return int_im->insert_index(index_name, table, std::get<int>(val), offset);
        case Field::FLOAT:  return float_im->insert_index(index_name, table, std::get<double>(val), offset);
        case Field::CHAR:   return char_im->insert_index(index_name, table, std::get<std::string>(val), offset);
    }
}

void IndexManager::DeleteIndex(const std::string index_name, Table & table, const std::string & column_name, const Value &val){
    int index = table.indexOfCol(column_name);
    auto &attr = table.columns[index];
    switch(attr.field){
        case Field::INT:    return int_im->delete_index(index_name, table, std::get<int>(val));
        case Field::FLOAT:  return float_im->delete_index(index_name, table, std::get<double>(val));
        case Field::CHAR:   return char_im->delete_index(index_name, table, std::get<std::string>(val));
    }
}

void IndexManager::AlterIndex(const std::string index_name, Table & table, const std::string & column_name, const Value &val_before, const Value &val_after, int offset){
    int index = table.indexOfCol(column_name);
    auto &attr = table.columns[index];
    switch(attr.field){
        case Field::INT:    return int_im->alter_index(index_name, table, std::get<int>(val_before), std::get<int>(val_after), offset);
        case Field::FLOAT:  return float_im->alter_index(index_name, table, std::get<double>(val_before), std::get<double>(val_after), offset);
        case Field::CHAR:   return char_im->alter_index(index_name, table, std::get<std::string>(val_before), std::get<std::string>(val_after), offset);
    }
}

void IndexManager::DropIndex(const std::string index_name, Table & table, const std::string & column_name){
    int index = table.indexOfCol(column_name);
    auto &attr = table.columns[index];
    switch (attr.field){
        case Field::INT:    return int_im->drop_index(index_name, table);
        case Field::FLOAT:  return float_im->drop_index(index_name, table);
        case Field::CHAR:   return char_im->drop_index(index_name, table);
    }
}