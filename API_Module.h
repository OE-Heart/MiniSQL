//////////////////////////////////////////////////////////
///----------------------------------------------------///
///       Module: API_Module                           ///
///       Produced by: XXX                          ///
///       Description: Produced to deal with the       ///
///                    functions between Interpreter,  ///
///                    Record_Manager and Index_Manager///
///       date: 2004/12/29                             ///
///----------------------------------------------------///
//////////////////////////////////////////////////////////

#if !defined(_API_MODULE_H_)
#define _API_MODULE_H_

#include"MiniSQL.h"

void  API_Module(CString SQL);
//显示帮助信息
void Help();
//插入索引节点
void Insert_Index(CString DB_Name,CString Table_Name,index_info & index);
//删除索引节点
void Delete_Index(CString DB_Name,CString Table_Name,index_info & index);
//查找索引节点
void Find_Index(CString DB_Name,CString Table_Name,index_info & index);
//获取条件的索引信息
void Get_Index(CString DB_Name,CString Table_Name,int & start,int & end,int type,index_info & index);

#endif
