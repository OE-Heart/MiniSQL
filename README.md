# MiniSQL

**任务目标：**

1. 设计并实现一个精简型单用户 SQL 引擎(DBMS) MiniSQL，允许用户通过字符界面输入 SQL 语句实现表的建立/删除；索引的建立/删除以及表记录的插入/删除/查找
2. 通过对 MiniSQL 的设计与实现,提高学生的系统编程能力,加深对数据库管理系统事先技术的理解

**系统需求:**

1. 数据类型：要求支持三种基本数据类型:integer,char(n),float
2. 表定义：一个表可以定义多达 32 个属性,各属性可以指定是否为 unique；支持单属性的主键定义
3. 索引定义：对于表的主属性自动建立 B+树索引,对于声明为 unique 的属性可以通过 SQL 语句由用户指定建立/删除 B+树索引(因此,所有的 B+树索引都是单属性单值的)
4. 数据操作：可以通过指定用 and 连接的多个条件进行查询,支持等值查询和区间


