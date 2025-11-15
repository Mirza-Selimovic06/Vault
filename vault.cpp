#include <fstream>
#include <iostream>
#include <limits>
#include <string>
#include <vector>
#include "screens.h"
#include "user.h"
using namespace std;

int main() {
    //email and password variables
    string email, password;

    user currentUser;
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
            //Set user email
            cout << "----------------------------\n";   
            cout << "Enter email: ";
            cin >> email;
            currentUser.setEmail(email);
            //Ser user password
            cout << "--------------------\n";
            cout << "Enter password: ";
            cin >> password;
            currentUser.setPassword(password);
            cout << "--------------------\n";
            //TODO: account add function
        }
        else {
            cout << "Welcome back, user!\n";
            cout << "-------------------\n";
            cout << "Please sign in to continue.\n";
            cout << "--------------------\n";
            // get email and password
            cout << "Enter email: ";
            cin >> email;
            cout << "--------------------\n";
            cout << "Enter password: ";
            cin >> password;
            cout << "--------------------\n";
            //TODO: authentication function 
        }
        //if authentication is successful 
        options_screen();
        cin >> choice;

    } while(choice != 0);
}