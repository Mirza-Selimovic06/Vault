#pragma once
#include <iostream>
#include <string>
using namespace std;

class user {
    public:
        //default constructor
        user() {
            email = "";
            password = "";
        }
        
        //user constructor
        user(string &email, string &password) {
            this->email = email;
            this->password = password;
        }

        //get email
        string getEmail() {
            return email;
        }
        //get password
        string getPassword() {
            return password;
        }
        
        //set email
        void setEmail(const string &email) {
            this->email = email;
        }

        //set password
        void setPassword(const string &password) {
            this->password = password;
        }

    private:
        string email;
        string password;
};

