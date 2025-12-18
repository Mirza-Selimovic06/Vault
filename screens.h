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
    cout << "1 add passwords\n";  
    cout << "2 view passwords\n"; 
    cout << "3 edit passwords\n";
    cout << "4 delete passwords\n";
    cout << "5 delete account\n"; 
    cout << "0 - Exit\n";
    cout << "Enter your choice: ";
}

void exit_screen(void) {
    cout << "--------------------\n";
    cout << "Exiting the Vault\n";
    cout << "--------------------\n";
}



