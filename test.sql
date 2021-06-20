create table t1(id int ,name char(20) unique,age int ,salary float,primary key(id));
insert into t1 values(1,'Jim',20,2000.35);
insert into t1 values(2,'Kate',24,1800.24);
insert into t1 values(3,'John',34,4000.17);
insert into t1 values(4,'Marry',20,3000.00);
insert into t1 values(5,'Tom',24,1850.00);
insert into t1 values(6,'Queen',28,24000.00);
insert into t1 values(7,'Porry',17,1000.00);
insert into t1 values(8,'Green',24,8000.00);
insert into t1 values(8,'Jim',20,4000.00);
insert into t1 values(9,'Green',22,5000.00);
select * from t1;

create index testindex1 on t1(id);
create index testindex2 on t1(salary);
select * from t1 where(id=1);