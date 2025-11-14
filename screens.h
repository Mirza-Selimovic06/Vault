#include<iostream>
using namespace std;

//welcom screen
void welcome_screen(void) {
    cout << "--------------------\n";
    cout << "Welcome to the Vault\n";
    cout << "--------------------\n";
    cout << __DATE__ << endl;
    cout << "--------\n";
    cout << "Would you like to start the program? y/n: ";
}

//options screen
void options_screen(void) {
    cout << "1\n";  
    cout << "2\n"; 
    cout << "3\n";
    cout << "4\n"; 
    cout << "0 - Exit\n";
    cout << "Enter your choice: ";
}



