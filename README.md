# Contact Management API
This is a simple contact management API built with C++ using the Crow framework and libpqxx for PostgreSQL, enabling creation, reading, updating, and deletion of contacts.

## Features
- **Add a Contact**: Insert a new contact into the database.
- **List Contacts**: Retrieve a list of all contacts.
- **Update a Contact**: Modify an existing contact's details.
- **Delete a Contact**: Remove a contact from the database.

## Technologies Used 
- C++
- Crow
- PostgreSQL
- libpqxx

## Database Setup
Create the database and table:
  CREATE DATABASE your_database_name;

CREATE TABLE contacts (
    id SERIAL PRIMARY KEY,
    name VARCHAR(100),
    surname VARCHAR(100),
    phone_number VARCHAR(15),
    email VARCHAR(100),
    notes TEXT
);

