#include <iostream>
#include <string>
#include <vector>
#include <fstream>
#include <limits>
#include "screens.h"
using namespace std;

int main() {
    //start and choice variable
    int choice;
    char start;
    char registered;
    bool auth = false;
    //welcome screen
    welcome_screen(); 
    cin >> start; 
    
    do {
        cout << "Are you a registered user? y/n: ";
        cin >> registered;

        //check registration status
        if (tolower(registered) == 'y') {
            auth = true;
        } else if (tolower(registered) == 'n') {
            auth = false;
        } else {
            cout << "Invalid input. Please enter 'y' or 'n'.\n";
            continue;
        }

        if(!auth){
            cout << "Please sign up to continue.\n";
            cout << "----------------------------\n";   
            cout << "Enter username: ";
            cout << "--------------------\n";
            cout << "Enter password: ";
            cout << "--------------------\n";
            //TODO: account function 
        }
        else {
            cout << "Welcome back, user!\n";
            cout << "-------------------\n";
            cout << "Please sign in to continue.\n";
            cout << "--------------------\n";
            cout << "Enter username: ";
            cout << "--------------------\n";
            cout << "Enter password: ";
            cout << "--------------------\n";
            //TODO: authentication function 
        }
        //if authentication is successful 
        options_screen();
        cin >> choice;

    } while(choice != 0);
}