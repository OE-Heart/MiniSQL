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
//��ȡ�û����룬������������Ч�Լ�飬����ȷ�����������ڲ���ʾ��ʽ
CString Interpreter(CString statement);
//��ȡ�û�����
CString read_input();
//��֤create����Ƿ���Ч
CString create_clause(CString SQL,int start);
//��֤create database����Ƿ���Ч
CString create_database(CString SQL,int start);
//��֤create table����Ƿ���Ч
CString create_table(CString SQL,int start);
//�������
CString get_attribute(CString temp,CString sql);
//��֤create index����Ƿ���Ч
CString create_index(CString SQL,int start);
//��֤create index on����Ƿ���Ч
CString create_index_on(CString SQL,int start,CString sql);
//��֤drop����Ƿ���Ч
CString drop_clause(CString SQL,int start);
//��֤drop database����Ƿ���Ч
CString drop_database(CString SQL,int start);
//��֤drop table����Ƿ���Ч
CString drop_table(CString SQl,int start);
//��֤drop index����Ƿ���Ч
CString drop_index(CString SQL,int start);
//��֤select ����Ƿ���Ч
CString select_clause(CString SQL,int start);
//�����������ļ����ÿһ��
CString get_part(CString temp,CString sql,CString kind);
//��֤insert ����Ƿ���Ч
CString insert_clause(CString SQL,int start);
//��֤insert into values����Ƿ���Ч
CString insert_into_values(CString SQL,int start,CString sql);
//��֤delete����Ƿ���Ч
CString delete_clause(CString SQL,int start);
//��֤ delete from where ����Ƿ���Ч
CString delete_from_where(CString SQL,int start,CString sql);
//�����ʽת��Ϊ�ڲ���ʽ
CString get_expression(CString temp,CString sql);
//��ȡ���ʽ���ÿ�����ʽ
CString get_each(CString T,CString sql,CString condition);
//��֤use����Ƿ���Ч
CString use_clause(CString SQL,int start);
//��֤execfile����Ƿ���Ч
CString execfile_clause(CString SQL,int start);
//��֤quit����Ƿ���Ч
CString quit_clause(CString SQL,int start);

#endif