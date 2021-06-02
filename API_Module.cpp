#include"API_Module.h"
#include"Catalog_Manager.h"
#include"Record_Manager.h"
#include"Index_Manager.h"

CString DB_Name;

void API_Module(CString SQL)
{
	CString Type,Attr,Index_Name,Table_Name,Attr_Name,Condition,index_name[32];
	int index,end,length,offset,type,count,num,record_Num,Count;
	index_info nodes[32];
	condition_info conds[10];
	attr_info print[32];
	char cond;
	bool ok;
	Type=SQL.Left(2);
	SQL=SQL.Mid(2,SQL.GetLength()-2);

	/////////////////////////////////////////////////////////////////////////////////////////
	//�������ݿ�
	if(Type=="00")
		Create_Database(SQL);
	
	/////////////////////////////////////////////////////////////////////////////////////////
	//�������ݱ�
	else if(Type=="01")
	{
		if(DB_Name.IsEmpty())
			cout<<"error: you have not specified the database to be used!"<<endl;
		else
		{
			index=SQL.Find(',');
			Table_Name=SQL.Left(index);
			Attr=SQL.Right(SQL.GetLength()-index-1);
			Attr_Name.Empty();
			//�������ݱ�
			Create_Table(Table_Name,Attr,DB_Name,Attr_Name);
			//�ж��Ƿ񴴽���������
			if(!Attr_Name.IsEmpty())
				Create_Index(Table_Name,Table_Name,Attr_Name,DB_Name,length,offset,type);
		}		
	}

	/////////////////////////////////////////////////////////////////////////////////////////
	//��������
	else if(Type=="02")
	{
		if(DB_Name.IsEmpty())
			cout<<"error: you have not specified the database to be used!"<<endl;
		else
		{
			index=SQL.Find(' ');
			//��ȡ������
			Index_Name=SQL.Left(index);
			index++;
			end=SQL.Find(' ',index);
			//��ȡ����
			Table_Name=SQL.Mid(index,end-index);
			//��ȡ������
			Attr=SQL.Right(SQL.GetLength()-end-1);
			Create_Index(Index_Name,Table_Name,Attr,DB_Name,length,offset,type);
			//�������������ڵ�
			if(length!=-1)
				Insert_Index_All(DB_Name,Table_Name,Index_Name,length,offset,type);			
		}
	}

	/////////////////////////////////////////////////////////////////////////////////////////
	//ɾ�����ݿ�
	else if(Type=="10")
	{
		if(SQL==DB_Name)
		{
			//�ͷ����ݿ���ڴ�
			Close_Database(DB_Name,false);
			DB_Name.Empty();
		}
		Drop_Database(SQL);
	}

	/////////////////////////////////////////////////////////////////////////////////////////
	//ɾ�����ݱ�
	else if(Type=="11")
	{
		if(DB_Name.IsEmpty())
			cout<<"error: you have not specified the database to be used!"<<endl;
		else
		{
			Table_Name=SQL;
			//�ͷű���ڴ�
			Close_File(DB_Name,Table_Name,0,false);
			//ɾ�����ļ�
			Drop_Table(Table_Name,DB_Name,index_name,count);
			//�ͷű��������������ڴ�
			for(index=0;index<count;index++)
				Close_File(DB_Name,index_name[index],1,false);
		}
	}

	/////////////////////////////////////////////////////////////////////////////////////////
	//ɾ������
	else if(Type=="12")
	{
		if(DB_Name.IsEmpty())
			cout<<"error: you have not specified the database to be used!"<<endl;
		else
		{
			Index_Name=SQL;
			//ɾ���������ڴ�
			Close_File(DB_Name,Index_Name,1,false);
			Drop_Index(Index_Name,DB_Name);
		}
	}

	/////////////////////////////////////////////////////////////////////////////////////////
	//ѡ�����(��where�Ӿ�)
	else if(Type=="20")
	{
		if(DB_Name.IsEmpty())
			cout<<"error: you have not specified the database to be used!"<<endl;
		else
		{
			//��ȡѡ��������
			index=SQL.Find(',');
			Attr=SQL.Left(index);
			index++;
			//��ȡ����
			Table_Name=SQL.Right(SQL.GetLength()-index);
			Table_Name=Table_Name.Left(Table_Name.GetLength()-1);
			if(Table_Name.Find(' ')!=-1)
				cout<<"error: can not select from more than one table!"<<endl;
			else
			{
				//��ȡ��ʾ��¼��ʽ
				if(Attr=="*")
					Get_Attr_Info_All(DB_Name,Table_Name,print,count);
				else
					Get_Attr_Info(DB_Name,Table_Name,print,count,Attr);	
				if(count!=0)
					Select_No_Where(DB_Name,Table_Name,print,count);
			}
		}		
	}

	/////////////////////////////////////////////////////////////////////////////////////////
	//ѡ�����(��where�Ӿ�)
	else if(Type=="21")
	{



	�������ɾ���ˡ�
