#include"MiniSQL.h"
#include"Interpreter.h"
#include"API_Module.h"


void main()
{
	CString SQL;
	//��ӡ�����Ϣ
	cout<<endl;
	cout<<"\t\t***********************************************"<<endl;
	cout<<"\t\t             ��ӭʹ�� MiniSQL !"<<endl;
	cout<<"\t\t               Version (1.0)  "<<endl;
	cout<<"\t\t            "<<endl;      
	cout<<"\t\t        copyright(2021) all right reserved !"<<endl;
	cout<<"\t\t***********************************************"<<endl;
	cout<<endl<<endl;
	while(1)
	{
		cout<<"MiniSQL-->> ";
		SQL.Empty();
		SQL=Interpreter(SQL);
		API_Module(SQL);
	}
}

