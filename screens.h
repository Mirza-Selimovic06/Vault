#pragma once
#include<iostream>
using namespace std;

//welcom screen
void welcome_screen(void) {
    cout << "--------------------\n";
    cout << "Welcome to the Vault\n";
    cout << "--------------------\n";
    cout << __DATE__ << " " << __TIME__ << endl;
    cout << "--------\n";
    cout << "Would you like to start the program? y/n: ";
}

//options screen
void options_screen(void) {
    cout << "1 - Add password\n";  
    cout << "2 - View passwords\n"; 
    cout << "3 - Edit passwords\n";
    cout << "4 - Delete password\n";
    cout << "5 - Delete account\n"; 
    cout << "0 - Exit\n";
    cout << "Enter your choice: ";
}

void exit_screen(void) {
    cout << "--------------------\n";
    cout << "Exiting the Vault\n";
    cout << "--------------------\n";
}

void editOptions(void) {
    cout << "1 - Edit email\n";
    cout << "2 - Edit username\n";
    cout << "3 - Edit password\n";
    cout << "0 - Return\n";
    cout << "Enter you choice: ";
}