#include <fstream>
#include <iostream>
#include <limits>
#include <string>
#include <vector>
#include "Passwords.h"
#include "screens.h"
#include "user.h"
#include "userRepository.h"
using namespace std;

int main() {
    //email and password variables
    string email, password;
    string aName, aUserName, aEmail, aPassword;
    //set up defautl constructors
    user *currentUser = new user();
    
    app *App = new app();
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
            currentUser->setEmail(email);
            //Set user password
            cout << "--------------------\n";
            cout << "Enter password: ";
            cin >> password;
            currentUser->setPassword(password);
            cout << "--------------------\n";
            //TODO: account add function
            addUserToDB(*currentUser);
        }
        else {
            cout << "Welcome back, user!\n";
            cout << "-------------------\n";
            cout << "Please sign in to continue.\n";
            cout << "--------------------\n";

            // get email and password
            cout << "Enter email: ";
            cin >> email;
            currentUser->setEmail(email);
            cout << "--------------------\n";


            cout << "Enter password: ";
            cin >> password;
            currentUser->setPassword(password);
            cout << "--------------------\n";

            //check if the user is in the vault database
            bool userIn = checkUserInDB(currentUser);
            if(!userIn){
                cout << "Authentication failed. Please check your email and password.\n";
                break;
            }
            else if(userIn){
                cout << "Authentication successful. Access granted to the vault.\n";
                cout << "--------------------\n";
                cout << "Accessing vault...\n";
            }
        }
        
        options_screen();
        cin >> choice;

        if (choice == 1) {
            //prompt user for app name, username, email, and password
            cout << "--------------------\n";
            cout << "Please enter App Name: ";
            cin >> aName;
            App->setAppName(aName);
            cout << "--------------------\n";
            
            cout << "Please enter the Username you use for the app: ";
            cin >> aUserName;
            App->setUserName(aUserName);
            cout << "--------------------\n";

            cout << "Please enter the email you use for the app: ";
            cin >> aEmail;
            App->setEmail(aEmail);
            cout << "--------------------\n";

            cout << "Please enter the password you use for the app: ";
            cin >> aPassword;
            App->setPassword(aPassword);
            cout << "--------------------\n";

            addApp(*App);
        }
        else if (choice == 2) { 

        }
        else if (choice == 3) {

        }
        else if (choice == 4) {

        }
        else if (choice == 5) {

        }

    } while(choice != 0);
}