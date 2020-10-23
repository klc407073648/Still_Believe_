use mytestdata;
DROP TABLE IF EXISTS student;
CREATE TABLE student
  (
     id INT PRIMARY KEY NOT NULL,
     name VARCHAR(30),
     age INT,
     sex ENUM('man','woman')
  );

DROP TABLE IF EXISTS course;
CREATE TABLE course
  (
     cid INT PRIMARY KEY NOT NULL,
     cname VARCHAR(30),
     tid INT
  );

DROP TABLE IF EXISTS sc;
CREATE TABLE sc
  (
     sid INT NOT NULL,
     cid INT NOT NULL,
     score INT
  );

DROP TABLE IF EXISTS teacher;
CREATE TABLE teacher
  (
     tid INT PRIMARY KEY NOT NULL,
     tname VARCHAR(30)
  );
  
insert into student(id,name,age,sex) values(1001,"Jason",25,'man');
insert into student(id,name,age,sex) values(1002,"Helen",25,'woman');
insert into student(id,name,age,sex) values(1003,"Steve",27,'man');
insert into student(id,name,age,sex) values(1004,"Hanna",25,'woman');
insert into student(id,name,age,sex) values(1005,"Bob",26,'man');