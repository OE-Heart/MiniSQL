#if !defined(_MINISQL_H_)
#define _MINISQL_H_

#include<iostream.h>
#include<afx.h>
#include<stdlib.h>
#include<math.h>
#include<direct.h>

#define FILE_COUNTER 1
#define FILE_INFO_LENGTH 671
#define ATTR_INFO_LENGTH 20
#define FILE_HEAD_LENGTH 31

//�����������ڵ���Ϣ
struct index_info
{
	CString index_name;
	int length;
	char type;
	long offset;
	CString value;
};

//����delete ����������Ϣ
struct condition_info
{
	char left_type;
	int left_len;
	long left_offset;
	CString left_index_name;
	char right_type;
	int right_len;
	long right_offset;
	CString right_index_name;
	CString const_data;
	CString condition;
};	

//����select �����ʾ��������Ϣ
struct attr_info
{
	CString attr_name;
	int offset;
	int length;
	char type;
};

#endif