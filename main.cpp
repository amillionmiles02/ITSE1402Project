#include "home.cpp"
#include "arduinoWin.cpp"
#include <dos.h>

using namespace std;
using namespace home;

//GLOBALS
const int MAX_LIGHTS = 9;

const int MIN_TEMP = 30;
const int MAX_TEMP = 130;

arduino::Board myboard;

home::Home myhome;

//prototype
void clearInput();
//clear the input buffer

void connectArduino();
//connect to arduino

void disconnectArduino();
//reset arduino to defults and close connection

void initializeArduino();
//initialize the arduino

void loginMenu();
//login menu

void adminArea();
//admin area code

void mainMenu();
//main menu area

void printUserList();
//prints the list of all users

void roomsSetup();
//for setting up lights

int main()
{
    if(!myhome.open())
    {
        string new_password;
        //program setup
        cout << "Welcome to smart home setup" << endl << endl;
        cout << "Enter a new administration password: ";
        cin >> new_password;
        clearInput();

        myhome.name = new_password;
        myhome.temperature = 75;

        myhome.save();

        cout << "Setup completed! Please login to the administration area" << endl;
        cout << "to start configuring your smart home. Press Enter to continue..." << endl << endl;
        cin.get();
    }
    char choice;

    cout << "--- WELCOME TO SMART HOME ---" << endl << endl;
    cout << "Would you like to connect a smart home device? (Y/N)" << endl;
    cout << "Your choice: ";
    cin >> choice;
    clearInput();

    if (choice == 'y' || choice == 'Y')
        //connect arduino
        connectArduino();

    if (myboard.isConnected)
        initializeArduino();

    //login

    int selection;

    do
    {
        loginMenu();
        cout << "Your selection: ";
        cin >> selection;
        clearInput();

        switch (selection)
        {
            case 1:
                //sign in
                if (myhome.userLogin())
                {
                    //success
                    cout << "Login successful! Press Enter to continue..." << endl;
                    clearInput();
                    cin.get();
                    cout << endl;

                    mainMenu();
                }
                else
                {
                    cout << "Authentication error. Press Enter to continue..." << endl << endl;
                    clearInput();
                    cin.get();
                }
                break;
            case 2:
                //admin area
                adminArea();
                break;
            case 3:
                //exit
                cout << "Good bye! Thanks for using our awesome program." << endl;
                disconnectArduino();
                break;
            default:
                cout << "Bad input. Press Enter to continue..." << endl << endl;
                cin.get();
                break;
        }
    }
    while (selection != 3);

    return 0;
}

//clear inut buffer
void clearInput()
{
    cin.clear();
    cin.ignore(10000, '\n');
}

//connect to arduino
void connectArduino()
{
    bool connected = false;

    cout << "\nLooking for device..." << endl;

    char choice;

    while (true)
    {
        for (int i = 1; i < 15; i++)
        {

            if (myboard.Connect(i))
            {
                cout << "\nDevice found on port " << i << "..." << endl;
                Sleep(4000);
                myboard.Send("#A1");
                cout << "Did your smart home turn on? (Y/N)" << endl;
                cout << "Your choice: ";
                cin >> choice;
                clearInput();

                if(choice == 'y' || choice == 'Y')
                {
                    connected = true;
                    cout << "Connection established!" << endl;
                    myboard.Send("#A0");
                    break;
                }
                else
                {
                    connected = false;
                    myboard.close();
                }
            }
        }

        if (connected)
            break;
        else
        {
            cout << "\nDevice not found" << endl;
            cout << "Would you like to try again? (y/n)" << endl;
            cout << "Your choice: ";
            cin >> choice;
            clearInput();

            if (choice != 'y' && choice != 'Y')
                break;
        }
    }
}

//disconnect arduino
void disconnectArduino()
{
    myboard.Send("#A0");
    myboard.Send("#T30");
    for (unsigned int i = 0; i < myhome.lights.size(); i++)
    {
        myboard.Send("#L" + to_string(i+1) + "0");
    }
    myboard.close();
}

//initialized the arduino
void initializeArduino()
{
    if (myhome.isOpened)
        myboard.Send("#A1");

    string temperature = "#T" + to_string(myhome.temperature);
    myboard.Send(temperature);

    for (unsigned int i = 0; i < myhome.lights.size(); i++)
    {
        if (myhome.lights[i])
            myboard.Send("#L" + to_string(i+1) + "1");
    }
}

//login menu
void loginMenu()
{
    cout << endl;
    cout << "--- MENU ---" << endl << endl;
    cout << "1 - Sign in" << endl;
    cout << "2 - Administration area" << endl;
    cout << "3 - Exit" << endl << endl;
}

//admin area
void adminArea()
{
    string password;
    cout << "Please enter the administration password: ";
    cin >> password;
    clearInput();

    if (password == myhome.name)
    {
        //success
        cout << "\n--- WELCOME TO THE ADMINISTRATION AREA ---" << endl << endl;
        int selection;

        //input variables
        string text_input;
        string text_input_2;
        char char_input;

        bool found;


        do
        {
            cout << "--- MENU ---" << endl << endl;
            cout << "1 - Room settings" << endl;
            cout << "2 - User list" << endl;
            cout << "3 - Create user" << endl;
            cout << "4 - Delete user" << endl;
            cout << "5 - Change user password" << endl;
            cout << "6 - Change administration password" << endl;
            cout << "7 - Exit" << endl << endl;

            cout << "Your choice: ";
            cin >> selection;
            clearInput();

            switch (selection)
            {
            case 1:
                //rooms
                roomsSetup();
                break;

            case 2:
                //user list
                printUserList();
                cout << "Press Enter to go back to menu..." << endl;
                cin.get();
                break;


            case 3:
                //new user
                cout << "Enter new username: ";
                cin >> text_input;
                clearInput();

                found = false;

                for (User u : myhome.users)
                    if (u.isActive && u.username == text_input)
                        found = true;

                if (found)
                    cout << "User already exists. Press Enter to go back to menu..." << endl;
                else
                {
                    cout << "Enter a new password for " << text_input << " :";
                    cin >> text_input_2;
                    clearInput();

                    if(myhome.createUser(text_input, text_input_2))
                    {
                        cout << "New user created. Press Enter to go back to menu..." << endl;
                        myhome.save();
                    }
                    else
                    {
                        cout << "\nSmart Home supports up to 15 users. Eliminate an user to create a new one." << endl;
                        cout << "\nPress Enter to go back to menu..." << endl;
                    }

                }
                cin.get();
                break;


            case 4:
                //delete user
                cout << "Press Enter to display list of users and then enter the username to delete..." << endl;
                cin.get();
                printUserList();

                cout << "Enter username: ";
                cin >> text_input;
                clearInput();

                found = false;

                for (int i = 0; i < myhome.HOME_SIZE; i++)
                {
                    if (myhome.users[i].isActive && myhome.users[i].username == text_input)
                    {
                        found = true;

                        cout << "Are you sure you want to delete user " << myhome.users[i].username << "? (Y/N)" << endl;
                        cout << "Your choice: ";
                        cin >> char_input;
                        clearInput();

                        if (char_input == 'y' || char_input == 'Y')
                        {
                            myhome.users[i].isActive = false;
                            myhome.save();

                            cout << "User deleted. ";
                        }

                        cout << "Press Enter to go back to menu..." << endl;
                    }


                }

                if(!found)
                    cout << "Invalid username. Press Enter to go to menu..." << endl;
                cin.get();

                break;


            case 5:
                //change user password
                cout << "Press Enter to display list of users and then enter the username to change the password..." << endl;
                cin.get();
                printUserList();

                cout << "Enter username: ";
                cin >> text_input;
                clearInput();

                found = false;

                for (int i = 0; i < myhome.HOME_SIZE; i++)
                {
                    if (myhome.users[i].isActive && myhome.users[i].username == text_input)
                    {
                        cout << "Enter a new password: ";
                        cin >> text_input;
                        clearInput();

                        myhome.users[i].code = text_input;
                        myhome.save();

                        found = true;

                        cout << "Password changed. Press Enter to go back to menu..." << endl;
                    }
                }
                if (!found)
                    cout << "Invalid user. Press Enter to go back..." << endl;
                cin.get();

                break;


            case 6:
                //change admin password
                cout << "Please, enter the current administration password: ";
                cin >> text_input;
                clearInput();

                if (text_input == myhome.name)
                {
                    cout << "Please enter a new password: ";
                    cin >> text_input;
                    clearInput();

                    myhome.name = text_input;
                    myhome.save();

                    cout << "Password successfully changed. Press Enter to continue..." << endl;
                }
                else
                    cout << "Wrong password. Press Enter to go back to the menu..." << endl;
                cin.get();

                break;


            case 7:
                cout << "Good bye! Press Enter to go to initial menu..." << endl;
                cin.get();
                break;


            default:
                cout << "Invalid input. Press Enter to try again..." << endl;
                cin.get();
                break;
            }
        }
        while (selection != 7);

    }
    else
    {
        cout << "Wrong password. Press Enter to go back..." << endl;
        cin.get();
    }
}

void printUserList()
{
    cout << "\nShowing all users:" << endl << endl;

    int i = 1;
    for(User u : myhome.users)
    {
        if (u.isActive)
        {
            cout << i++ << " - " << u.username << endl;
        }
    }
    cout << endl;
}

//lights setup
void roomsSetup()
{
    myhome.lights.clear();

    myhome.rooms.clear();

    int num_lights;

    string room_name;

    cout << "How many rooms does your house have?" << endl;

    do
    {
        cout << "Enter a number (1 - " << MAX_LIGHTS << "): ";
        cin >> num_lights;
        clearInput();

        if (num_lights < 1 || num_lights > MAX_LIGHTS)
            cout << "Bad input. ";
        else
        {
            cout << endl;
            for (int i = 1; i <= num_lights; i++)
            {
                myhome.lights.push_back(false);
                cout << "Enter a name for room " << i << ": ";
                cin >> room_name;
                clearInput();
                myhome.rooms.push_back(room_name);
                cout << endl;
            }

            myhome.save();

            for (unsigned int i = 0; i < myhome.lights.size(); i++)
            {
                myboard.Send("#L" + to_string(i + 1) + "0");
            }
            initializeArduino();

            cout << "Rooms setup completed. Press Enter to go back to menu..." << endl;
            cin.get();
        }
    }
    while (num_lights < 1 || num_lights > MAX_LIGHTS);


}

//main menu area
void mainMenu()
{
    int num_input;
    char char_input;
    string text_input;

    do
    {
       cout << "\n-- MAIN MENU ---" << endl << endl;
       cout << "1 - Entrance" << endl;
       cout << "2 - Lights" << endl;
       cout << "3 - Temperature" << endl;
       cout << "4 - Change password" << endl;
       cout << "5 - Exit" << endl << endl;

       cout << "Your selection: ";
       cin >> num_input;
       clearInput();

       switch (num_input)
       {
       case 1:
           //access
           cout << endl;
           if (myhome.isOpened)
           {
               cout << "-----------------------------" << endl;
               cout << "Your home's entrance is open" << endl;
               cout << "-----------------------------" << endl << endl;
               cout << "would you like to close it? (Y/N)" << endl;

               cout << "Your choice: ";
               cin >> char_input;
               clearInput();

               cout << endl;

               if (char_input == 'y' || char_input == 'Y')
               {
                   myboard.Send(myhome.access());
               }
           }
           else
           {
               cout << "-----------------------------" << endl;
               cout << "Your home's entrance is closed" << endl;
               cout << "-----------------------------" << endl << endl;
               cout << "would you like to open it? (Y/N)" << endl;

               cout << "Your choice: ";
               cin >> char_input;
               clearInput();

               cout << endl;

               if (char_input == 'y' || char_input == 'Y')
               {
                   myboard.Send(myhome.access());
               }
           }

           cout << "Press Enter to go to menu..." << endl;
           cin.get();
           break;

       case 2:
           //lights
           cout << "\nPlease select one of the following rooms: " << endl << endl;
           for (unsigned int i = 0; i < myhome.lights.size(); i++)
           {
               cout << i+1 << " - " << myhome.rooms[i] << endl;
           }

           cout << "\nEnter a room number: ";
           cin >> num_input;
           clearInput();

           if (num_input >= 1 && static_cast<unsigned int>(num_input) <= myhome.lights.size())
           {
               if (myhome.lights[num_input - 1])
               {
                   cout << endl;
                   cout << "-----------------------------" << endl;
                   cout << myhome.rooms[num_input - 1] << "'s light is on" << endl;
                   cout << "-----------------------------" << endl << endl;
                   cout << "Would you like to turn in off? (Y/N)" << endl;
                   cout << "Your choice: ";
                   cin >> char_input;
                   clearInput();

                   if (char_input == 'y' || char_input == 'Y')
                   {
                       myboard.Send(myhome.turnLight(num_input));
                   }
               }
               else
               {
                   cout << endl;
                   cout << "-----------------------------" << endl;
                   cout << myhome.rooms[num_input - 1] << "'s light is off" << endl;
                   cout << "-----------------------------" << endl << endl;
                   cout << "Would you like to turn in on? (Y/N)" << endl;
                   cout << "Your choice: ";
                   cin >> char_input;
                   clearInput();

                   if (char_input == 'y' || char_input == 'Y')
                   {
                       myboard.Send(myhome.turnLight(num_input));
                   }
               }
           }
           else
                cout << "Invalid room. ";
           cout << "Press Enter to go back to menu..." << endl;
           cin.get();
           break;

       case 3:
           //temperature
           cout << endl;
           cout << "-----------------------------" << endl;
           cout << "Temperature is " << myhome.temperature << " F." << endl;
           cout << "-----------------------------" << endl << endl;
           cout << "Enter a new temperature (" << MIN_TEMP << " - " << MAX_TEMP << " F): ";
           cin >> num_input;
           clearInput();

           if (num_input >= MIN_TEMP && num_input <= MAX_TEMP)
           {
               myboard.Send(myhome.setTemperature(num_input));
               cout << "Press Enter to go to continue..." << endl;
           }
           else
                cout << "Bad input. Press Enter to go back to manu..." << endl;
           cin.get();

           break;

       case 4:
           //change password
           cout << "Please enter your current password: ";
           cin >> text_input;
           clearInput();

           if (text_input == myhome.users[myhome.currentUser].code)
           {
               cout << "Enter a new password: ";
               cin >> text_input;
               clearInput();

               myhome.users[myhome.currentUser].code = text_input;
               myhome.save();

               cout << "Password changed. Press Enter to continue..." << endl;
           }
           else
               cout << "Wrong password. Press Enter to go back to menu..." << endl;
            cin.get();

           break;

       case 5:
           //exit
           cout << "Good bye! Press Enter to go to initial menu..." << endl;
           cin.get();
           break;

       default:
           //bad input
           cout << "Bad input. Press Enter to continue..." << endl;
           cin.get();
           break;
       }
    }
    while (num_input != 5);
}
