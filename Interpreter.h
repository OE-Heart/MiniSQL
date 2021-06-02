//////////////////////////////////////////////////////////
///----------------------------------------------------///
///       Module: Interpreter                          ///
///       Produced by: XXX                         ///
///       Description: Read user's input and change it ///
///                    to another format that can be   ///
///                    recognized by Module API        ///
///       date: 2004/12/8                              ///
///----------------------------------------------------///
//////////////////////////////////////////////////////////
#if !defined(_INTERPRETER_H_)
#define _INTERPRETER_H_

#include"MiniSQL.h"
//获取用户输入，并对输入作有效性检查，若正确，返回语句的内部表示形式
CString Interpreter(CString statement);
//读取用户输入
CString read_input();
//验证create语句是否有效
CString create_clause(CString SQL,int start);
//验证create database语句是否有效
CString create_database(CString SQL,int start);
//验证create table语句是否有效
CString create_table(CString SQL,int start);
//获得属性
CString get_attribute(CString temp,CString sql);
//验证create index语句是否有效
CString create_index(CString SQL,int start);
//验证create index on语句是否有效
CString create_index_on(CString SQL,int start,CString sql);
//验证drop语句是否有效
CString drop_clause(CString SQL,int start);
//验证drop database语句是否有效
CString drop_database(CString SQL,int start);
//验证drop table语句是否有效
CString drop_table(CString SQl,int start);
//验证drop index语句是否有效
CString drop_index(CString SQL,int start);
//验证select 语句是否有效
CString select_clause(CString SQL,int start);
//获得属性组或文件组的每一项
CString get_part(CString temp,CString sql,CString kind);
//验证insert 语句是否有效
CString insert_clause(CString SQL,int start);
//验证insert into values语句是否有效
CString insert_into_values(CString SQL,int start,CString sql);
//验证delete语句是否有效
CString delete_clause(CString SQL,int start);
//验证 delete from where 语句是否有效
CString delete_from_where(CString SQL,int start,CString sql);
//将表达式转化为内部形式
CString get_expression(CString temp,CString sql);
//获取表达式组的每个表达式
CString get_each(CString T,CString sql,CString condition);
//验证use语句是否有效
CString use_clause(CString SQL,int start);
//验证execfile语句是否有效
CString execfile_clause(CString SQL,int start);
//验证quit语句是否有效
CString quit_clause(CString SQL,int start);

#endif