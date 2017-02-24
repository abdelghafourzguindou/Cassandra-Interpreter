CREATE KEYSPACE ensias 		WITH REPLICATION = { 'class' : 'SimpleStrategy', 'replication_factor' : 3 };
CREATE KEYSPACE achat  		WITH REPLICATION = { 'class' : 'SimpleStrategy', 'replication_factor' : 3 };
CREATE KEYSPACE entreprise 	WITH REPLICATION = { 'class' : 'SimpleStrategy', 'replication_factor' : 3 };
CREATE KEYSPACE election 	WITH REPLICATION = { 'class' : 'SimpleStrategy', 'replication_factor' : 3 };