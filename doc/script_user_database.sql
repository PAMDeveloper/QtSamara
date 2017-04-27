CREATE USER user_samara WITH PASSWORD 'samarapassword';

CREATE DATABASE samara
WITH ENCODING = 'UTF8'
     TABLESPACE = pg_default
     CONNECTION LIMIT = -1;