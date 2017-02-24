use entreprise;

CREATE TABLE employe (
  empID int,
  deptID int,
  first_name varchar,
  last_name varchar,
  PRIMARY KEY (empID, deptID)
);

CREATE TABLE departement (
  deptID int primary key,
  deptName varchar
);

use ensias;

CREATE TABLE etudiant (
  cen varchar PRIMARY KEY,
  first_name text,
  last_name text,
  filiere text,
);

CREATE TABLE prof (
  id int,
  cin varchar,
  nom text,
  prenom text,
  salaire float,
  primary key(id, cin)
)
WITH comment='Important biological records'
AND read_repair_chance = 1.0;

use election;

CREATE TABLE electeur (
  cin varchar,
  nom text,
  prenom text,
  profession text,
  adresse varchar,
  commune text,
  bv text,
  circonstripion text,
  primary key(commune, bv, circonstripion, cin)
);

CREATE TABLE BV (
  BV_ID int primary key,
  adresse varchar,
);

CREATE TABLE commune (
  com_id int primary key,
  adresse text,
);

use achat;

CREATE TABLE produit (
  produit_id int primary key,
  produit_name text,
  produit_categorie text,
  produit_prix float
);

CREATE TABLE client (
  client_id int primary key,
  first_name text,
  last_name text,
  adresse varchar
);

use ensias;

