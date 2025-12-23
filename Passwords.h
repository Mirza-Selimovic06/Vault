#pragma once
#include <iostream>
#include <string>
using namespace std;

class app {
    private:
        string appName;
        string password;
        string email;
        string userName;
    public:
        app() { 
            appName = "";
            password = "";
            email = "";
            userName = "";
        }
        //setters
        void setAppName(string name) { this->appName = name; }
        void setPassword(string pass) { this->password = pass; }
        void setEmail(string nEmail) { this->email = nEmail; }
        void setUserName(string nUserName) { this->userName = nUserName; }

        //getters
        string getAppName() { return this->appName;} 
        string getPassword() { return this->password;}
        string getEmail() { return this->email;}
        string getUserName() { return this->userName;}
};