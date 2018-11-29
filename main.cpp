#include "home.cpp"
#include "arduinoWin.cpp"
#include <dos.h>

using namespace std;
using namespace home;

//GLOBALS
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
                    cout << "You are in!" << endl;
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
                cout << "\nTesting port " << i << "..." << endl;
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
    for (int i = 0; i < myhome.lights.size(); i++)
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
        cout << "SUCCESS" << endl;
    }
    else
    {
        cout << "Wrong password. Press Enter to go back..." << endl;
        cin.get();
    }
}

//main menu area
void mainMenu()
{

}
