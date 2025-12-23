#pragma once
#include <iostream>
#include <string>
#include <sqlite3.h>
#include <botan/passhash9.h>
#include <botan/system_rng.h>
#include <vector>
#include "user.h"
#include "Passwords.h"
using namespace std;
using namespace Botan;

/*
The hashPass function hashes the inputed password from the user when creating an account.
I decided to use Botans passhash9 as it was simpler for me to use compared to the 
Windows BCRYPT API and the Botan repo is always up to date. System_RNG is a random number generator
which randomly genorates a salt.
*/
string hashPass(const string &password) {
    System_RNG rng; //From the Botan namespace
    return generate_passhash9(password, rng); //From the Botan namespace
}

/*
I don't know why I really decided to do a checkPassword function but it came to mined. It returns
a boolean which is from the check_passhash9 function within Botan which sees the orginal passwords
salt and salts and hashes the stored password (aka the stored variable) the same way as the orginal/master.
*/
bool checkPassword(const string &password, string &stored) {
    return check_passhash9(password, stored);
}

//callback function for sqlite3_exec
static int callback(void *data, int argc, char **argv, char **azColName) {
    int i;
    cout << (const char*)data << endl;

    for (i = 0; i < argc; i++) {
        cout << azColName[i] << ": " << (argv[i] ? argv[i] : "NULL") << endl;
    }

    cout << endl;
    return 0;
}


//Function to add user to DB
void addUserToDB(const user &newUser) { 
    user nUser = newUser;
    sqlite3 *db;
    char* zErrMsg = 0;
    int rc;

    //hash password from user
    string userPassword = hashPass(nUser.getPassword());

    //opens the vault database
    rc = sqlite3_open("vault.db", &db);

    //error handling
    if (rc) {
        cerr << "Can't open database: " << sqlite3_errmsg(db) << endl;
        return;
    }
    else {
        cout << "Opened database successfully" << endl;
    }

    //insert user into database, email, and hashed password
    string sql = "INSERT INTO users (email, password) VALUES ('" + nUser.getEmail() + "', '" + userPassword + "');";
    int exec = sqlite3_exec(db, sql.c_str(), callback, 0, &zErrMsg);
    
    
    //error handling for insertion
    if (exec != SQLITE_OK) {
        cerr << "SQL error: " << zErrMsg << endl;
        sqlite3_free(zErrMsg);
    } else {
        cout << "User added successfully" << endl;
    }
    //closes the database
    sqlite3_close(db);
}

/*
The CheckUserInDB function simply checks the entered master password from the user and with the 
hashed password in the database by using the checkPassword function from above. 
checkPassword function details will be in a block comment above checkPassword.
*/
bool checkUserInDB(user *existingUser) {
    string password = existingUser->getPassword();
    sqlite3 *db;
    char* zErrMsg = 0;
    int rc;
    sqlite3_stmt *stmt = nullptr; 
    //cout << password << endl;
    string passwordHash;
    //opens the vault database
    rc = sqlite3_open("vault.db", &db);

    //error handling
    if (rc) {
        cerr << "Can't open database: " << sqlite3_errmsg(db) << endl;
    }
    else {
        cout << "Opened database successfully" << endl;
    }

    string sql = "SELECT email, password FROM users WHERE email = ? LIMIT 1;";

    rc = sqlite3_prepare_v2(db, sql.c_str(), -1, &stmt, nullptr);
    if (rc != SQLITE_OK) {
        cout << "Failed to prepare SQL" << endl;
        return false;
    }
    sqlite3_bind_text(stmt, 1, existingUser->getEmail().c_str(), -1, SQLITE_TRANSIENT);
    rc = sqlite3_step(stmt);

    if(rc == SQLITE_ROW) {
        const unsigned char* stored = sqlite3_column_text(stmt, 1);
        passwordHash = reinterpret_cast<const char*>(stored);
    }
    else if(rc == SQLITE_DONE) {
        cout << "Email does not exist\n";
        sqlite3_finalize(stmt);
        return false;
    }
    else {
        cout << "Sqlite execution error\n";
        sqlite3_finalize(stmt);
        return false;
    }

        //closes the database   
        sqlite3_close(db);
        sqlite3_finalize(stmt);
        return checkPassword(password, passwordHash);
}

/*
addApp adds an app and gets data from the Passwords class in Passwords.h
we then add the id from the users table to user_id into the website table where the current users
email matches any email from the users table.
Then we insert id, app password, app email, app username, and the app name/url
(P.S this was originally inteded for websites only but I decided app names can work as well)
*/
void addApp(app &App, user *existingUser) {
    sqlite3 *db;
    sqlite3_stmt *stmt = nullptr;
    char *zErrMsg = 0;
    int rc;
    int id = -1;

    string pass = App.getPassword();
    string email = App.getEmail();
    string username = App.getUserName();
    string appName = App.getAppName();

    rc = sqlite3_open("vault.db", &db);

    //error handling
    if (rc) {
        cerr << "Can't open database: " << sqlite3_errmsg(db) << endl;
        return;
    }
    else {
        cout << "Opened database successfully" << endl;
    }

    string sql = "SELECT id FROM users WHERE email = ? LIMIT 1;";
    rc = sqlite3_prepare_v2(db, sql.c_str(), -1, &stmt, nullptr);

    if (rc != SQLITE_OK) {
        cout << "Failed to prepare SQL" << endl;
    }

    sqlite3_bind_text(stmt, 1, existingUser->getEmail().c_str(), -1, SQLITE_TRANSIENT);
    rc = sqlite3_step(stmt);

    if(rc == SQLITE_ROW) {
        id = sqlite3_column_int(stmt, 0);
    }
    else if(rc == SQLITE_DONE) {
        cout << "Email does not exist\n";
        sqlite3_finalize(stmt);
    }
    else {
        cout << "Sqlite execution error\n";
        sqlite3_finalize(stmt);
    }

    //finalize stmt and set it to a null pointer for next query
    sqlite3_finalize(stmt);
    stmt = nullptr;

    //insert user into database, email, and hashed password
    sql = "INSERT INTO Website (user_id, url, email, username, password) VALUES(?, ?, ?, ?, ?);";
    rc = sqlite3_prepare_v2(db, sql.c_str(), -1, &stmt, nullptr);

    if (rc != SQLITE_OK) {
        cerr << "SQL error: " << zErrMsg << endl;
        sqlite3_close(db);
    }

    //binds these sql strings to the INSERT query above
    sqlite3_bind_int(stmt, 1, id);
    sqlite3_bind_text(stmt, 2, appName.c_str(), -1, SQLITE_TRANSIENT);
    sqlite3_bind_text(stmt, 3, email.c_str(), -1, SQLITE_TRANSIENT);
    sqlite3_bind_text(stmt, 4, username.c_str(), -1, SQLITE_TRANSIENT);
    sqlite3_bind_text(stmt, 5, pass.c_str(), -1, SQLITE_TRANSIENT);

    rc = sqlite3_step(stmt);

    //error handling for insertion
    if (rc != SQLITE_DONE) {
        cerr << "SQL error: " << zErrMsg << endl;
        sqlite3_free(zErrMsg);
    } else {
        cout << appName << " App added successfully" << endl;
    }
    //closes the database
    sqlite3_finalize(stmt);
    sqlite3_close(db);
}

/*
View Passwords function views user passwords based of if the user table id is equivalent to the 
website user_id where the user email is equivalent to the current user email 
(we get current user email from the user class)
*/
void ViewPasswords(user *User) {
    sqlite3 *db = nullptr;
    sqlite3_stmt *stmt = nullptr;
    char *zErrMsg = 0;
    int rc;

    if(!User) {
        cerr << "User pointer null";
        return;
    }

    rc = sqlite3_open("vault.db", &db);

    if (rc) {
        cerr << "Can't open database: " << sqlite3_errmsg(db) << endl;
        sqlite3_close(db);
        return;
    }
    else {
        cout << "Opened database successfully" << endl;
    }

    string sql = "SELECT w.url, w.username, w.email, w.password FROM Website w JOIN users u ON w.user_id = u.id WHERE u.email = ?;";
    rc = sqlite3_prepare_v2(db, sql.c_str(), -1, &stmt, nullptr);
    if(rc != SQLITE_OK) {
        cerr << "Prepare failed: " << sqlite3_errmsg(db) << endl;
        sqlite3_close(db);
        return;
    }

    rc = sqlite3_bind_text(stmt, 1, User->getEmail().c_str(), -1, SQLITE_TRANSIENT);

    if (rc != SQLITE_OK) {
        cerr << "Bind failed" << sqlite3_errmsg(db);
        sqlite3_close(db);
        return;
    }
    //rc = sqlite3_step(stmt);

    auto colText = [&](int col) -> std::string { 
            const unsigned char *t = sqlite3_column_text(stmt, col);
            return t ? reinterpret_cast<const char*>(t) : "";
        };
    
    bool hasPasswords = false;
    cout << "-------------------------------\n";

    while((rc = sqlite3_step(stmt)) == SQLITE_ROW) {
        hasPasswords = true;

        string appName = colText(0);
        string userName = colText(1);
        string userEmail = colText(2);
        string userPass = colText(3);

        cout << "App Name: " << appName << endl;
        cout << "Username: " << userName << endl;
        cout << "Email: " << userEmail << endl;
        cout << "Password: " << userPass << endl;
        cout << "-------------------------------\n";
    }
    
    if(!hasPasswords) {
        cout << "No accounts saved to the vault please add a password to the vault\n";
    }
    else if(rc != SQLITE_DONE){ 
        cout << "Sqlite execution error" << sqlite3_errmsg(db) << endl;
    }
    
    sqlite3_finalize(stmt);
    sqlite3_close(db);
}

/*
This is the EditPassword function. It opens the database then
prompts the user for the app name they want to update then asks to update the password.
*/
void EditPassword() {
    sqlite3 *db = nullptr;
    sqlite3_stmt *stmt = nullptr;
    int rc;

    rc = sqlite3_open("vault.db", &db);

    if (rc != SQLITE_OK) {
        cerr << "Error opening database: " << sqlite3_errmsg(db) << endl;
        sqlite3_close(db);
        return;
    }

    string appName;
    string password;
    string confirm;
    cout << "Which apps password do you want to edit?\n";
    cout << "Enter App Name: "; cin >> appName;

    cout << "Please enter your new password: "; cin >> password;
    cout << "Confirm password: "; cin >> confirm;

    if(strcmp(password.c_str(), confirm.c_str()) != 0) {
        cerr << "Error: Password confirmation failed(try again)\n";
        return;
    }
    string sql = "UPDATE Website SET password = ? WHERE url = ?";
    rc = sqlite3_prepare_v2(db, sql.c_str(), -1, &stmt, nullptr);

    if (rc != SQLITE_OK) {
        cerr << "Prepare statement error: " << sqlite3_errmsg(db) << endl;
        sqlite3_close(db);
        return;
    }

    rc = sqlite3_bind_text(stmt, 1, password.c_str(), -1, SQLITE_TRANSIENT);
    rc = sqlite3_bind_text(stmt, 2, appName.c_str(), -1, SQLITE_TRANSIENT);
    
    if (rc != SQLITE_OK) {
        cerr << "Bind failed" << sqlite3_errmsg(db);
        sqlite3_close(db);
        return;
    }

    rc = sqlite3_step(stmt);

    //error handling for insertion
    if (rc != SQLITE_DONE) {
        cerr << "SQL error: " << sqlite3_errmsg(db) << endl;
    } else {
        cout << appName << " App added successfully" << endl;
    }

    //closes the database
    sqlite3_finalize(stmt);
    sqlite3_close(db);
}

void RemovePassword() {
    sqlite3 *db = nullptr;
    sqlite3_stmt *stmt = nullptr;
    int rc;

    rc = sqlite3_open("vault.db", &db);

    if (rc != SQLITE_OK) {
        cerr << "Error opening database: " << sqlite3_errmsg(db) << endl;
        sqlite3_close(db);
        return;
    }

    string appName;
    char confirm;
    cout << "Which app do you want to delete?\n";
    cout << "Enter App Name: "; cin >> appName;

    cout << "Are you sure you want to delete " << appName << " from your vault? [ENTER Y OR N]: ";  cin >> confirm;
    if(tolower(confirm) == 'n') {
        sqlite3_close(db);
        return;
    }

    
    string sql = "DELETE FROM Website WHERE url = ?";
    rc = sqlite3_prepare_v2(db, sql.c_str(), -1, &stmt, nullptr);

    if (rc != SQLITE_OK) {
        cerr << "Prepare statement error: " << sqlite3_errmsg(db) << endl;
        sqlite3_close(db);
        return;
    }

    rc = sqlite3_bind_text(stmt, 1, appName.c_str(), -1, SQLITE_TRANSIENT);
    
    if (rc != SQLITE_OK) {
        cerr << "Bind failed" << sqlite3_errmsg(db);
        sqlite3_close(db);
        return;
    }

    rc = sqlite3_step(stmt);

    //error handling for insertion
    if (rc != SQLITE_DONE) {
        cerr << "SQL error: " << sqlite3_errmsg(db) << endl;
    } else {
        cout << appName << " App deleted successfully" << endl;
    }

    //closes the database and frees stmt pointer
    sqlite3_finalize(stmt);
    sqlite3_close(db);
}

bool checkPasswordForDelete(string email, string password) {
    sqlite3 *db;
    char* zErrMsg = 0;
    int rc;
    sqlite3_stmt *stmt = nullptr; 
    //cout << password << endl;
    string passwordHash;
    //opens the vault database
    rc = sqlite3_open("vault.db", &db);

    //error handling
    if (rc) {
        cerr << "Can't open database: " << sqlite3_errmsg(db) << endl;
    }
    else {
        cout << "Opened database successfully" << endl;
    }

    string sql = "SELECT email, password FROM users WHERE email = ? LIMIT 1;";

    rc = sqlite3_prepare_v2(db, sql.c_str(), -1, &stmt, nullptr);
    if (rc != SQLITE_OK) {
        cout << "Failed to prepare SQL" << endl;
        return false;
    }
    sqlite3_bind_text(stmt, 1, email.c_str(), -1, SQLITE_TRANSIENT);
    rc = sqlite3_step(stmt);

    if(rc == SQLITE_ROW) {
        const unsigned char* stored = sqlite3_column_text(stmt, 1);
        passwordHash = reinterpret_cast<const char*>(stored);
    }
    else if(rc == SQLITE_DONE) {
        cout << "Email does not exist\n";
        sqlite3_finalize(stmt);
        return false;
    }
    else {
        cout << "Sqlite execution error\n";
        sqlite3_finalize(stmt);
        return false;
    }
        //closes the database   
        sqlite3_close(db);
        sqlite3_finalize(stmt);
        return checkPassword(password, passwordHash);
}

void deleteUser(user *User) {
    sqlite3 *db = nullptr;
    sqlite3_stmt *stmt = nullptr;
    int rc;
    rc = sqlite3_open("vault.db", &db);
    if(rc != SQLITE_OK) {
        cerr << "Error: " << sqlite3_errmsg(db);
        return;
    }
    string sql = "DELETE FROM users WHERE email = ?";
    rc = sqlite3_prepare_v2(db, sql.c_str(), -1, &stmt, nullptr);

    if (rc != SQLITE_OK) {
        cerr << "Prepare statement error: " << sqlite3_errmsg(db) << endl;
        sqlite3_close(db);
        return;
    }

    rc = sqlite3_bind_text(stmt, 1, User->getEmail().c_str(), -1, SQLITE_TRANSIENT);

    if (rc != SQLITE_OK) {
        cerr << "Bind failed" << sqlite3_errmsg(db);
        sqlite3_close(db);
        return;
    }

    rc = sqlite3_step(stmt);

    //error handling for insertion
    if (rc != SQLITE_DONE) {
        cerr << "SQL error: " << sqlite3_errmsg(db) << endl;
    } else {
        cout << User->getEmail() << " account deleted successfully" << endl;
    }

    //closes the database and frees stmt pointer
    sqlite3_finalize(stmt);
    sqlite3_close(db);
}