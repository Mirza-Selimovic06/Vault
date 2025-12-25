# Overview
#### Video Demo: [CS50 Final Video](https://youtu.be/K5dbqZ02LZc)

#### Description
Vault.cpp is a security focused, cli password manager written in C++, designed to explore secure credential storage, authentication workflows, and database backed systems programming.

The project emphasizes cryptographic practice, secure programming, and low level systems control.

## Security Model
- no plaintext passwords stored
- Passwords are salted and hashed using Botan 
- Authentication is preformed via hash verification, not hash comparison
- SQLite3 used for local, file-based persistence

## Tech Stack
- Language: C++
- Database: SQLite3
- Cryptography: Botan
- Platform Linux (WSL Compatible)
- Build: g++

## Core features
- User registration and log-in
- Secure password hashing and verification
- Error handling and defense checks

## In Progress/Planned 
- Adding passwords into vault and viewing
- Search functionality for viewing
- Editing/Deleting passwords
- Deleting account

## Build and run
> g++ vault.cpp -o vault -lsqlite3 $(pkg-config --cflags --libs botan-2)

> ./vault

## Learning Objectives
This project was built to gain hands on experience with:
- Password hashing vs encryption
- Secure authentication flows
- SQLite C API usage
- CLI Application Design
- Secure programming in C++

## Why C++
At SNHU we use C++ for majority of our Computer Science Classes so I decided to use it as a sumplement to learn some C++ topics and get better practice with C++ such as more OOP practice, learn about lambda functions, and how to use an API. I also wanted to see how different C++ interacts with SQLite compared to Python. The SQLite API was fairly simple to use. Functions such as sqlite3_prepare_v2, sqlite3_step, sqlite3_bind_text, and sqlite3_bind_int were very helpul in making my SQL queries safe from SQL Injections compared to sqlite3_exec(). Sqlite3_exec is not safe when it comes to if user input is included if not (like table creation) its safe to use.

## Why SQLite3 and how it worked
SQLite3 was used because of its simplicity. It was very easy to integrate because it is a file based database rather than a server based database which means connecting it to the userRepository.h file was very easy. All I had to do was #include <sqlite3.h> and a sqlite3.c file into my folder. I created my tables in the terminal (I used WSL Ubuntu) by running sqlite3 database.db. I also used .schema to refer to my database to make sure that the I was Selecting, Inserting, Updating, and Deleting, from the right table as well as the right names for my columns. In the beginning I used sqlite3_exec with a callback function that I got from [tutorials point](https://www.tutorialspoint.com/sqlite/sqlite_c_cpp.htm) in order to Select and Insert. Then in later functions I started using sqlite3_prepare_v2(), and sqlite3_bind_text or sqlite3_bind_int in order to run my SQL queries. How sqlite3_exec is used is it takes a SQL string ex: string sql = "SELECT * FROM users WHERE name = '" + name + "';";
This makes it prone to SQL injections because it allows for injecting user input within the string. With sqlite3_prepare_v2 the string looks like: 
string sql = "SELECT * FROM users WHERE name = ?". the ? placeholders are treated as data slots, not code. sqlite3_bind_text or slqite3_bind_int sends user input as data only, SQLite escapes input (it prevents special characters in user input from being interpreted as SQL syntax) and types it internally. SQLite3_step() runs the compiled statement (The sql query is compiled from sqlite3_prepare_v2) step by step and returns SQLITE_ROW which is the result data.

## Why Botan
I used Botan's passhash9 because of its simplicity. I used generate_passhash9(password, rng) and check_passhash9(passwored, stored) and that was it for password hashing. I was originally trying to use the windows bcrypt API but it wasn't working for me and it was harder for me to understand. Using Botan's passhash9 minimized the amount of lines I had to code as well as made my code secure by using an rng to salt my password and checking using the same perameters as the hash which check_passhash9 does by reading them from the stored hash string and uses those parameters and the same hash function on the password you entered and then verifies the two strings. 

## Why I Create Vault
I created vault as a way for people who constantly use the CLI and "live in the terminal" to store their passwords safely within the command line rather than using windows/macos password savers or even their browsers password savers.

## Disclaimer
This product is not intended for professional use as it is not fully done

## Planned features
Turn it into a CLI command rather than compiling and running
Remember users instead of prompting users for email and password again everytime and only prompt users for their vault passwords in order to add, view, delete passwords, and delete account.
When signing up and inputting a users vault password make it so that it doesn't appear when typing (either make it invisible like when you are entering your password to sudo run a command or replace each character with an asterisk)

