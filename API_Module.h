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
//��ʾ������Ϣ
void Help();
//���������ڵ�
void Insert_Index(CString DB_Name,CString Table_Name,index_info & index);
//ɾ�������ڵ�
void Delete_Index(CString DB_Name,CString Table_Name,index_info & index);
//���������ڵ�
void Find_Index(CString DB_Name,CString Table_Name,index_info & index);
//��ȡ������������Ϣ
void Get_Index(CString DB_Name,CString Table_Name,int & start,int & end,int type,index_info & index);

#endif
