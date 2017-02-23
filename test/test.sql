/*

CREATE KEYSPACE myKeyspace1
  WITH REPLICATION = { 'class' : 'SimpleStrategy', 'replication_factor' : 3 };


CREATE KEYSPACE myKeyspace2
  WITH REPLICATION = { 'class' : 'SimpleStrategy', 'replication_factor' : 3 };

CREATE KEYSPACE myKeyspace3
  WITH REPLICATION = { 'class' : 'SimpleStrategy', 'replication_factor' : 3 };

CREATE KEYSPACE myKeyspace4
  WITH REPLICATION = { 'class' : 'SimpleStrategy', 'replication_factor' : 3 };

use MYKEYSPACE1;

CREATE TABLE employe (
  empID int,
  deptID int,
  first_name varchar,
  last_name varchar,
  PRIMARY KEY (empID, deptID)
);

CREATE TABLE Cats (
  block_id int,
  breed varchar,
  color varchar,
  PRIMARY KEY ((block_id, breed), color)
);

use MYKEYSPACE2;

CREATE TABLE usersG (
  userid text PRIMARY KEY,
  first_name text,
  last_name text,
  emails set<text>,
  top_scores list<int>,
  todo map<timestamp, text>
);

CREATE TABLE MonkeyTypes (
  block_id uuid,
  species text,
  alias text,
  population varint,
  PRIMARY KEY (block_id)
)
WITH comment='Important biological records'
AND read_repair_chance = 1.0;

CREATE TABLE DogTypes (
  block_id uuid,
  species text,
  alias text,
  population varint,
  PRIMARY KEY (block_id)
) WITH compression =
    { 'sstable_compression' : 'DeflateCompressor', 'chunk_length_kb' : 64 }
  AND compaction =
    { 'class' : 'SizeTieredCompactionStrategy', 'min_threshold' : 6 }; */


use MYKEYSPACE1;
/*
--DROP KEYSPACE MYKEYSPACE2;
*/
insert into EMPLOYE (deptid,last_name, first_name, empid)  values (8,'zguindou','Abdelghafour', 88);
insert into EMPLOYE (empid, deptid, first_name, last_name) values (2,57,'GHANOUCH','Issam');
insert into EMPLOYE (empid, deptid, first_name, last_name) values (3,15,'G1','I1');
insert into EMPLOYE (empid, deptid, first_name, last_name) values (4,5,'G2','I2');
insert into EMPLOYE (empid, deptid, first_name, last_name) values (5,5,'G3','I3');
insert into EMPLOYE (empid, deptid, first_name, last_name) values (6,5,'G4','I4');
insert into EMPLOYE (empid, deptid, first_name, last_name) values (7,5,'G5','I5');
insert into EMPLOYE (empid, deptid, first_name, last_name) values (8,6,'G6','I6');
insert into EMPLOYE (empid, deptid, first_name, last_name) values (9,6,'G7','I7');
insert into EMPLOYE (empid, deptid, first_name, last_name) values (10,7,'G8','I8');
insert into EMPLOYE (empid, deptid, first_name, last_name) values (11,88,'G9','I9');
insert into EMPLOYE (empid, deptid, first_name, last_name) values (12,1,'G10','I10');
insert into EMPLOYE (empid, deptid, first_name, last_name) values (13,54,'G11','I11');
  

--select * from employe;
--select * from employe where empid = 2;
--select * from employe where last_name = 'ZGUINDOU';
--select * from employe where deptid IN (8 , 57);
--select * from employe where empid > 10 AND deptID > 5  ;
--select * from employe where last_name = 'ZGUINDOU' and first_name = 'Abdelghafour';

--select * from employe;
--select * from employe where empid > 2;
--select * from employe where last_name = 'ZGUINDOU';
 --select count(*) from employe where deptid IN (199 , 200);
--select count(*) from employe where deptid = 5;

--select deptid, last_name, first_name from employe;
--select first_name, last_name, DEPTID, empid from employe where deptid IN (8 , 57);
--select deptid, first_name from employe where FIRST_NAME = 'issam' AND deptID > 5  ;
--select first_name, last_name from employe where empid > 2 and deptid < 57;

--DROP TABLE CATS;
--delete from employe; --N'exist pas f cassandra
--delete from employe where empid > 0 and deptid > 10;
--update employe set last_name = 'zguindou', FIRST_NAME = 'abdelghafour' where DEPTID > 0 and empid > 0 ;

--ALTER TABLE EMPLOYE ALTER last_name type int;
--ALTER TABLE EMPLOYE ADD sex varchar, age int, profession varchar;
--ALTER TABLE EMPLOYE DROP sex profession age;
--ALTER TABLE EMPLOYE DROP last55_name;

--TRUNCATE TABLE employe;

