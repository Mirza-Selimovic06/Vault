# Overview
Vault.cpp is a security focused, cli password manager written in C++, designed to explore secure credential storage, authentication workflows, and database backed systems programming.

The project emphasizes cryptographic practice, secure programming, and low level systems control.

> This project is still in progress

## Security Model
- no plaintext passwords stored
- Passwords are salted and hashed using Botan 
- Authentication is preformed via hash verification, not hash comparison
- SQLite3 used for local, file-based persistence

## Tech Stack
- Language: C++
- Database: SQLite3
- Cryptography: Botan
- Platform Linux (WSL Compatiable)
- Build: g++

## Core features
- User registration and log-in
- Secure password hashing and varification
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

## Disclaimer
This project is for educational purposes and not intended for production use
