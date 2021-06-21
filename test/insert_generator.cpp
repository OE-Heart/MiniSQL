#include <iostream>
#include <fstream>
#include <cstdlib>
#include <ctime>
#include <string>

const int MAXN=999999999;

int main()
{
    std::cout << "Please input the fileName:\n";
	std::string fileName;
	std::cin >> fileName;
    fileName = fileName+".sql";
    std::cout << "Please input the tableName:\n";
    std::string tableName;
    std::cin >> tableName;
    std::cout << "Please input the recordNum:\n";
    int N;
    std::cin >> N;

    std::fstream outfile;
	outfile.open(fileName, std::ios::out);
    outfile << "create table " << tableName << "(id int ,name char(20) unique,age int ,salary float,primary key(id));\n";

    int ID;
    char name[20];
    int age;
	srand(time(NULL));
	for (int i = 0; i < N; i++)
	{
		int randomNum = rand()%MAXN+1;
		ID = randomNum;

        int lj = rand() % 9 + 1;
        int j;
		for (j = 0; j < lj; j++)
		{
			
			name[j] = rand() % 26 + 97;
		}
        name[j] = '\0';
        age = rand()%200;
        
        outfile << "insert into " << tableName << " values(" << ID << ",'" << name << "'," << age << ","<< ID/100000 << "." << age <<");\n";
	}

    outfile << "select * from " << tableName << ";\n";

    outfile.close();

    return 0;
}