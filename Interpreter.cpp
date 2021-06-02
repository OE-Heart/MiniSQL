//////////////////////////////////////////////////////////
///----------------------------------------------------///
///       Module: Interpreter_Module                           ///
///       Produced by: ZZZ                          ///
///       Description: Produced to deal with SQL parse ///
///       date: 2004/12/25                             ///
///----------------------------------------------------///
//////////////////////////////////////////////////////////

#include"Interpreter.h"

/////////////////////////////////////////////////////////////////////////////////////////////
//读取用户输入
CString read_input()
{
	CString SQL;
	CString temp;
	char str[100];
	bool finish=false;
	while(!finish)
	{
		cin>>str;
		temp=str;
		SQL=SQL+" "+temp;
		if(SQL.GetAt(SQL.GetLength()-1)==';')
		{
			SQL.SetAt(SQL.GetLength()-1,' ');
			SQL+=";";
			finish=true;
		}
	}
	//将输入大写转化为小写
	SQL.MakeLower();
	//返回用户输入
	return SQL;
}

/////////////////////////////////////////////////////////////////////////////////////////////
//验证create database语句是否有效
CString create_database(CString SQL,int start)
{
	CString temp;
	int index,end;
	//获取第三个单词
	while(SQL.GetAt(start)==' ')
		start++;
	index=start;
	end=SQL.Find(' ',start);
	temp=SQL.Mid(start,end-start);
	start=end+1;
	//若无，打印出错信息
	if(temp.IsEmpty())
	{
		cout<<"error: database name has not been specified!"<<endl;
		SQL="99";
	}
	else
	{	
		while(SQL.GetAt(start)==' ')
			start++;
		//若为非法信息，打印出错信息
		if(SQL.GetAt(start)!=';'||start!=SQL.GetLength()-1)
		{
			cout<<"error12:"<<SQL.Mid(index,SQL.GetLength()-index-2)<<"---is not a valid database name!"<<endl;
			SQL="99";
		}
		//返回drop database语句的内部形式
		else
			SQL="00"+temp;
	}
	return SQL;
}

/////////////////////////////////////////////////////////////////////////////////////////////
//获得属性
CString get_attribute(CString temp,CString sql)
{
	int start=0,end,index;
	CString T,C;
	temp+=" ";
	//获得属性名
	end=temp.Find(' ',start);
	T=temp.Mid(start,end-start);
	start=end+1;
	sql+=T+" ";
	//获得数据类型
	while(temp.GetAt(start)==' ')
		start++;
	end=temp.Find(' ',start);
	T=temp.Mid(start,end-start);
	start=end+1;
	//若无，打印出错信息
	if(T.IsEmpty())
	{
		cout<<"error : error in create table statement!"<<endl;
		sql="99";
		return sql;
	}
	//若为int 
	else if(T=="int")
		sql+="+";
	//若为float
	else if(T=="float")
		sql+="-";
	//其他
	else
	{
		index=T.Find('(');
		C=T.Left(index);
		index++;
		//若有误，打印出错信息
		if(C.IsEmpty())
		{
			cout<<"error: "<<T<<"---is not a valid data type definition!"<<endl;
			sql="99";
			return sql;
		}
		//若为char
		else if(C=="char")
		{
			C=T.Mid(index,T.GetLength()-index-1);
			if(C.IsEmpty())
			{
				cout<<"error: the length of the data type char has not been specified!"<<endl;
				sql="99";
				return sql;
			}
			else
				sql+=C;
		}
		//若为非法信息，打印出错信息
		else
		{
			cout<<"error: "<<C<<"---is not a valid key word!"<<endl;
			sql="99";
			return sql;
		}
	}
	//是否有附加信息
	while(start<temp.GetLength()&&temp.GetAt(start)==' ')
		start++;
	if(start<temp.GetLength())
	{
		//若为unique定义，保存信息
		end=temp.Find(' ',start);
		T=temp.Mid(start,end-start);
		if(T=="unique")
		{
			sql+=" 1,";
		}
		//若为非法信息，打印出错信息
		else
		{
			cout<<"error: "<<temp<<"---is not a valid key word!"<<endl;
			sql="99";
			return sql;
		}
	}
	//若无附加信息
	else
		sql+=" 0,";
	return sql;
}

/////////////////////////////////////////////////////////////////////////////////////////////
//验证create table语句是否有效
CString create_table(CString SQL,int start)
{
	CString temp,sql,T;
	int index,end,length;
	//获取表名
	while(SQL.GetAt(start)==' ')
		start++;
	index=start;
	if((end=SQL.Find('(',start))==-1)
	{
		cout<<"error: missing ( in the statement!"<<endl;
		SQL="99";
		return SQL;
	}
	temp=SQL.Mid(start,end-start);
	start=end+1;
	if(!temp.IsEmpty())
	{
		while(SQL.GetAt(start)==' ')
			start++;
		length=temp.GetLength()-1;
		while(temp.GetAt(length)==' ')
			length--;
		temp=temp.Left(length+1);
	}
	//若无，打印出错信息
	if(temp.IsEmpty())
	{
		cout<<"error: error in create table statement!"<<endl;
		SQL="99";
		return SQL;
	}
	//若为非法信息，打印出错信息
	else if(temp.Find(' ')!=-1)
	{
		cout<<"error: "<<temp<<"---is not a valid table name!"<<endl;
		SQL="99";
		return SQL;
	}
	else 
	{
		sql=temp+",";
		//获取每个属性
		while((end=SQL.Find(',',start))!=-1)
		{
			temp=SQL.Mid(start,end-start);
			start=end+1;
			//若有空属性，打印出错信息
			if(temp.IsEmpty())
			{
				cout<<"error: error in create table statement!"<<endl;
				SQL="99";
				return SQL;
			}
			//保存属性
			else
			{
				sql=get_attribute(temp,sql);
				if(sql=="99")
					return sql;
			}
			while(SQL.GetAt(start)==' ')
				start++;
		}
		//获得最后属性
		temp=SQL.Mid(start,SQL.GetLength()-start-1);
		length=temp.GetLength()-1;
		while(temp.GetAt(length)!=')'&&length>=0)
			length--;
		//若无，打印出错信息
		if(length<1)
		{
			cout<<"error: error in create table statement!"<<endl;
			SQL="99";
			return SQL;
		}
		//存储属性
		else
		{
			temp=temp.Left(length);
			end=SQL.Find(' ',start);
			T=SQL.Mid(start,end-start);
			start=end+1;
			//若为主键定义
			if(T=="primary")
			{
				//判断是否有关键字key
				temp+=")";
				while(SQL.GetAt(start)==' ')
					start++;
				end=SQL.Find('(',start);
				T=SQL.Mid(start,end-start);
				start=end+1;
				length=T.GetLength()-1;
				while(T.GetAt(length)==' ')
					length--;
				T=T.Left(length+1);
				//若为空，打印出错信息
				if(T.IsEmpty())
				{
					cout<<"syntax error: syntax error in create table statement!"<<endl;
					cout<<"\t missing key word key!"<<endl;
					SQL="99";
					return SQL;
				}
				//若有，继续验证
				else if(T=="key")
				{
					//获取主键属性名
					while(SQL.GetAt(start)==' ')
						start++;
					end=SQL.Find(')',start);
					T=SQL.Mid(start,end-start);
					length=T.GetLength()-1;
					while(T.GetAt(length)==' ')
						length--;
					T=T.Left(length+1);
					//若无，打印出错信息
					if(T.IsEmpty())
					{
						cout<<"error : missing primary key attribute name!"<<endl;
						SQL="99";
						return SQL;
					}
					//若为非法信息，打印出错信息
					else if(T.Find(' ')!=-1)
					{
						cout<<"error : "<<T<<"---is not a valid primary key attribute name!"<<endl;
						SQL="99";
						return SQL;
					}
					//保存主键
					else
					{
						sql+=T+" #,";
						SQL="01"+sql;
					}
				}
				//若为非法信息，打印出错信息
				else
				{
					cout<<"error : "<<T<<"---is not a valid key word!"<<endl;
					SQL="99";
					return SQL;
				}				
			}
			//若为一般属性
			else
			{
				sql=get_attribute(temp,sql);
				if(sql=="99")
				{
					SQL="99";
					return SQL;
				}
				else
					SQL="01"+sql;
			}
		}
	}
	return SQL;
}

/////////////////////////////////////////////////////////////////////////////////////////////
//验证create index on语句是否有效
CString create_index_on(CString SQL,int start,CString sql)
{
	CString temp;
	int end,length;
	//获取表名
	while(SQL.GetAt(start)==' ')
		start++;
	end=SQL.Find('(',start);
	temp=SQL.Mid(start,end-start);
	start=end+1;
	//若无，打印出错信息
	if(temp.IsEmpty())
	{
		cout<<"syntax error: syntax error for create index statement!"<<endl;
		cout<<"\t missing ( !"<<endl;
		SQL="99";
		return SQL;
	}
	else
	{
		//检验是否为有效文件名
		length=temp.GetLength()-1;
		while(temp.GetAt(length)==' ')
			length--;
		temp=temp.Left(length+1);
		//有效
		if(temp.Find(' ')==-1)
		{
			sql+=" "+temp;
			//获取属性名
			while(SQL.GetAt(start)==' ')
				start++;
			end=SQL.Find(')',start);
			temp=SQL.Mid(start,end-start);
			start=end+1;
			//若无，打印出错信息
			if(temp.IsEmpty())
			{
				cout<<"syntax error: syntax error for create index statement!"<<endl;
				cout<<"\t missing ) !"<<endl;
				SQL="99";
				return SQL;
			}
			else
			{
				//检验是否为有效属性名
				length=temp.GetLength()-1;
				while(temp.GetAt(length)==' ')
					length--;
				temp=temp.Left(length+1);
				//有效
				if(temp.Find(' ')==-1)
				{
					sql+=" "+temp;
					while(SQL.GetAt(start)==' ')
						start++;
					if(SQL.GetAt(start)!=';'||start!=SQL.GetLength()-1)
					{
						cout<<"syntax error: syntax error for quit!"<<endl;
						SQL="99";
						return SQL;
					}
					//返回create index语句的内部形式
					else
						SQL="02"+sql;
				}
				//无效,打印出错信息
				else
				{
					cout<<"error:"<<" "<<temp<<"---is not a valid attribute name!"<<endl;
					SQL="99";
					return SQL;
				}
			}
		}
		//无效,打印出错信息
		else
		{
			cout<<"error:"<<" "<<temp<<"---is not a valid table name!"<<endl;
			SQL="99";
			return SQL;
		}
	}
	return SQL;
}

/////////////////////////////////////////////////////////////////////////////////////////////
//验证create index语句是否有效
CString create_index(CString SQL,int start)
{
	CString temp,sql;
	int end;
	//获取第三个单词
	while(SQL.GetAt(start)==' ')
		start++;
	end=SQL.Find(' ',start);
	temp=SQL.Mid(start,end-start);
	start=end+1;
	//若无，打印出错信息
	if(temp.IsEmpty())
	{
		cout<<"syntax error: syntax error for create index statement!"<<endl;
		SQL="99";
	}
	else
	{
		sql=temp;
		//获取第四个单词
		while(SQL.GetAt(start)==' ')
			start++;
		end=SQL.Find(' ',start);
		temp=SQL.Mid(start,end-start);
		start=end+1;
		//若无，打印出错信息
		if(temp.IsEmpty())
		{
			cout<<"syntax error: syntax error for create index statement!"<<endl;
			SQL="99";
		}
		//若为on,继续验证
		else if(temp=="on")
			SQL=create_index_on(SQL,start,sql);
		//若为非法信息，打印非法信息
		else
		{
			cout<<"syntax error:"<<" "<<temp<<"---is not a valid key word!"<<endl;
			SQL="99";
		}			
	}
	return SQL;
}

/////////////////////////////////////////////////////////////////////////////////////////////
//验证create语句是否有效
CString create_clause(CString SQL,int start)
{
	CString temp;
	int end;
	//获取第二个单词
	while(SQL.GetAt(start)==' ')
		start++;
	end=SQL.Find(' ',start);
	temp=SQL.Mid(start,end-start);
	start=end+1;
	//若无，打印出错信息
	if(temp.IsEmpty())
	{
		cout<<"syntax error: syntax error for create statement!"<<endl;
		SQL="99";
	}
	//若为database,继续验证
	else if(temp=="database")
		SQL=create_database(SQL,start);
	//若为table,继续验证
	else if(temp=="table")
		SQL=create_table(SQL,start);
	//若为index,继续验证
	else if(temp=="index")
		SQL=create_index(SQL,start);
	//若为错误信息，打印出错信息
	else
	{
		cout<<"syntax error:"<<" "<<temp<<"---is not a valid key word!"<<endl;
		SQL="99";
	}
	//返回create语句的内部形式
	return SQL;
}

/////////////////////////////////////////////////////////////////////////////////////////////
//验证drop database语句是否有效
CString drop_database(CString SQL,int start)
{
	CString temp;
	int index,end;
	//获取第三个单词
	while(SQL.GetAt(start)==' ')
		start++;
	index=start;
	end=SQL.Find(' ',start);
	temp=SQL.Mid(start,end-start);
	start=end+1;
	//若无，打印出错信息
	if(temp.IsEmpty())
	{
		cout<<"error: database name has not been specified!"<<endl;
		SQL="99";
	}
	else
	{
		while(SQL.GetAt(start)==' ')
			start++;
		//若为非法信息，打印出错信息
		if(SQL.GetAt(start)!=';'||start!=SQL.GetLength()-1)
		{
			cout<<"error:"<<SQL.Mid(index,SQL.GetLength()-index-2)<<"---is not a valid database name!"<<endl;
			SQL="99";
		}
		//返回drop database语句的内部形式
		else
			SQL="10"+temp;
	}
	return SQL;
}

/////////////////////////////////////////////////////////////////////////////////////////////
//验证drop table语句是否有效
CString drop_table(CString SQL,int start)
{
	CString temp;
	int index,end;
	//获取第三个单词
	while(SQL.GetAt(start)==' ')
		start++;
	index=start;
	end=SQL.Find(' ',start);
	temp=SQL.Mid(start,end-start);
	start=end+1;
	//若无，打印出错信息
	


…//这里删去一段程序
	
  return SQL;
}

/////////////////////////////////////////////////////////////////////////////////////////////
//验证drop index语句是否有效
CString drop_index(CString SQL,int start)
{
	CString temp;
	int index,end;
	//获取第三个单词
	while(SQL.GetAt(start)==' ')
		start++;
	index=start;
	end=SQL.Find(' ',start);
	temp=SQL.Mid(start,end-start);
	start=end+1;
	//若无，打印出错信息
	


…
这里删去了几十个函数！！！
。。。
/////////////////////////////////////////////////////////////////////////////////////////////
//获取用户输入，并对输入作有效性检查，若正确，返回语句的内部表示形式
CString Interpreter(CString statement)
{
	CString SQL;
	CString temp;
	CString sql;
	int start=0,end;
	if(statement.IsEmpty())
		//获取用户输入
		SQL=read_input();
	else
		SQL=statement;
	//获取输入的第一个单词
	while(SQL.GetAt(start)==' ')
		start++;
	end=SQL.Find(' ',start);
	temp=SQL.Mid(start,end-start);
	start=end+1;
	//若无输入，打印出错信息
	if(temp.IsEmpty())
	{
		cout<<"syntax error: empty statement!"<<endl;
		SQL="99";
	}
	//若为create语句
	else if(temp=="create")
		SQL=create_clause(SQL,start);
	//若为drop语句
	else if(temp=="drop")
		SQL=drop_clause(SQL,start);
	//若为select语句
	else if(temp=="select")
		SQL=select_clause(SQL,start);
	//若为insert语句
	else if(temp=="insert")
		SQL=insert_clause(SQL,start);
	//若为delete语句
	else if(temp=="delete")
		SQL=delete_clause(SQL,start);
	//若为use语句
	else if(temp=="use")
		SQL=use_clause(SQL,start);
	//若为execfile语句
	else if(temp=="execfile")
		SQL=execfile_clause(SQL,start);
	//若为quit语句
	else if(temp=="quit")
		SQL=quit_clause(SQL,start);
	//获取帮助
	else if(temp=="help")
		SQL="80";
	//若为非法语句
	else
	{
		cout<<"syntax error:"<<" "<<temp<<"---is not a valid key word!"<<endl;
		SQL="99";
	}
	//返回输入语句的内部形式
	return SQL;
}
