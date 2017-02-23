/****************************/ SELECT DATA  /****************************/

select * from employe;
select * from employe where empid = 2;
select * from employe where last_name = 'ZGUINDOU';
select count(*) from employe where deptid IN (199 , 200);
select count(*) from employe where deptid = 5;
select deptid, first_name, last_name from employe;
select deptid, first_name from employe where FIRST_NAME = 'issam' AND deptID > 5  ;