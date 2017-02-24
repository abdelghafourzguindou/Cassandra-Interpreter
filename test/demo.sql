------------------------------------------- TEST CASSANDRA  -------------------------------------------

-------------- KEYSPACE 

** CREATE  :

execute(test/createkeyspaces.cql)

Create keyspace test with replication = test ;


** DROP   :

Drop Keyspace test;

Drop Keyspace test;


-------------- TABLE   

** CREATE 

execute(test/createtables.cql)

use ensias;

Create table test_x ( test int primary key);
Create table test_x ( test int primary key);

Create table test (age int , age varchar);

** DROP

Drop table test;
Drop table test;


** ALTER 

Create table test (age int primary key, sexe varchar primary key);

desc test
ALTER table test add t1 int , t2 varchar;
desc test

ALTER table test alter t1 type varchar;

ALTER table test drop t2;


-------------- USER 

** CREATE 

Create user root with password 'root' superuser;
Create user root with password 'root' superuser;

** ALTER
ALTER user root with password 'root_ensias';

ALTER user root nosuperuser;

** GRANT

grant select on All keyspaces to root;

** DROP
drop user root;
drop user root;


-------------- INSERT 

** INSERT STANDART

use ensias;
execute(test/insert.cql)


** INSERT PERSONALISE

insert into prof (id,cin,nom) values(500,'QAC','ISSAM');
select * from prof where id = 500;

insert into prof(nom,cin,id) values('ZGN','HSH',5823);
select * from prof where id = 5823;

 --->  Erreur Semantique
Deppassement d argument :
insert into prof (id) values(5987,'QAC');

Incompatible Type
insert into prof  values('QAC',500,'QAC','ISSAM',5000);

Insifisant arguments
insert into prof  values(502);

Duplication Cle Primaire
insert into prof  values(1,'CIN1','QAC','ISSAM',5000);

ça Passe :
insert into prof  values(1,'QYAT','QAC','ISSAM',5000);
insert into prof  values(5000,'CIN1','QAC','ISSAM',5000);


-------------- SELECT

-- Ca Passe :
select * from prof ;

select nom,prenom,salaire from prof;

select * from prof where id=50;

select * from prof where CIN='CIN5';

select * from prof where salaire >= 1000 and salaire <= 5000;

select nom,prenom,salaire from prof where salaire >= 20000;

select count(*) from prof where salaire >= 20000;

select * from prof where cin IN ('cin30', 'cin45');

select * from prof where id > 50 and salaire < 10000;

-- Ca Passe Pas:

select * from profs ;

select sexe from prof ;

select * from prof where id= 'issam';

select * from prof where id IN (55,56,'Abdel');

select * from prof where salaire > 'Mohamed';


-------------- UPDATE 

update prof set salaire = 0 where salaire >= 20000;
select * from prof where salaire = 0;

update prof set salaire = 'a' where salaire >= 20000;


-------------- DELETE

delete from prof where salaire = 0;

select count(*) from prof where id > 10 and id <30 ;  
delete from prof where id > 10 and id <30 ;  
select count(*) from prof where id > 10 and id <30 ;  


------------- TRUNCUTE
select count(*) from prof;

TRUNCATE table prof;
TRUNCATE prof;

select count(*) from prof;

TRUNCATE mytable;






























