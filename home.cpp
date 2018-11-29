#include<iostream>
#include<string>
#include<fstream>
#include <vector>
#include "user.cpp"

using namespace std;

namespace home {
    class Home
    {
    public:
        //default size of the home
        static const int HOME_SIZE = 15;

        //path of the data file
        const string DATA_FILE= "home.dat";

        const string USER_FILE = "users.dat";

        //array to store all users
        User users[HOME_SIZE];

        //current user logged into the home
        int currentUser;

        //--status of the house
        string name;
        bool isOpened;
        int temperature;

        //LIGHTS
        vector<bool> lights;


        //default constructor
        Home() {
            name = "none";
            isOpened = false;
            temperature = 75;
        }


        //--------------------------------------------------
        //create a new User
        bool createUser(string name, string password)
        {
            //loop through the users array
            for (int i = 0; i < HOME_SIZE; i++)
            {
                //on the first instance of a notActive user create a new user.
                if (!users[i].isActive)
                {
                    users[i] = User(name, password);
                    //return true since there's no need of keep looping
                    return true;
                }
            }
            //return false if there is no space for a new user
            return false;
        }

        //overloaded function create user
        bool createUser(User u)
        {
            //loop through the users array
            for (int i = 0; i < HOME_SIZE; i++)
            {
                //on the first instance of a notActive user create a new user.
                if (!users[i].isActive)
                {
                    users[i] = User(u.username, u.code);
                    users[i].access = u.access;
                    //return true since there's no need of keep looping
                    return true;
                }
            }
            //return false if there is no space for a new user
            return false;
        }

        //--------------------------------------------------
        //validate an user
        int validateUser(string username)
        {
            //user not active to return in case there is not user with that username
            User no_user;
            for (int i = 0; i < HOME_SIZE; i++)
            {
                //check only active users
                if (users[i].isActive)
                {
                    //if the username matches...
                    if(users[i].username == username)
                    {
                        //validate the user (by calling a User method named validateUser(username);)
                        if(users[i].validateUser(username))
                            //if the returned value is true return the valid user
                            return i;
                    }
                }
            }
            //return the not active user
            return -1;
        }

        //--------------------------------------------------
        //Print the users list
        void printUserList()
        {
            for (int i = 0; i < HOME_SIZE; i++)
            {
                if (users[i].isActive)
                {
                    cout << users[i].username << endl;
                }
            }
        }

        //--------------------------------------------------
        //open home from file
        bool open()
        {
            //open home
            ifstream dataFile;

            dataFile.open(DATA_FILE);
            
            if (!dataFile)
                return false;

            if(dataFile)
            {
                dataFile >> name;
                dataFile >> isOpened;
                dataFile >> temperature;

                int i = 0;

                int status;


                while (dataFile >> status)
                {
                    lights.insert(lights.end(), status);
                    i++;
                }
            }

            dataFile.close();
            //open users
            ifstream userFile;
            //open the data file
            userFile.open(USER_FILE);

            //if success when opening the file
            if (userFile)
            {
                //variable to store username
                User input_user;

                //set the DATA_FILE to path, since that's the new location of the data file
                while(userFile >> input_user.username) //bring the username
                {
                    //bring the password for the user, since it would be the line following the username
                    userFile >> input_user.code;
                    userFile >> input_user.access;

                    //try to create a new user
                    if(!createUser(input_user))
                    {
                        cout << "Home is full" << endl;
                        userFile.close();
                        return false;
                    }
                }
                //close the data file
                userFile.close();


                //return true, since the operation was completed with success
                return true;
            }

            userFile.close();

            //return false since no file was found
            return false;
        }

        //save home to file
        void save()
        {
            ofstream userFile;

            //open file to save info
            userFile.open(USER_FILE);

            //loop through all users in the user array
            for (int i = 0; i < HOME_SIZE; i++)
            {
                if (users[i].isActive)
                {
                    //save the username and password (and any other info)
                    userFile << users[i].username << endl;
                    userFile << users[i].code << endl;
                    userFile << users[i].access << endl;
                }
            }

            userFile.close();

            ofstream dataFile;
            dataFile.open(DATA_FILE);

            dataFile << name << endl;
            dataFile << isOpened << endl;
            dataFile << temperature << endl;

            for (bool l : lights)
            {
                dataFile << l << endl;
            }
        }

        //--------------------------------------------------
        //##--------User action functions---------

        //user login
        bool userLogin()
        {
            string username;
            cout << "Username: ";
            cin >> username;

            int current_user;
            current_user = validateUser(username);

            currentUser = current_user;

            if(users[current_user].isActive)
                return true;
            return false;
        }

        void userLogoff()
        {
            User emptyUser;
            currentUser = -1;
        }

        //house status
        void printHouseStatus()
        {
            string opened = "closed";
            if (isOpened)
                opened = "opened";

            cout << "\nWelcome to " << name << endl;
            cout << "\nTemperature is " << temperature << " F." << endl;
            cout << "\nThe home's door is " << opened << endl << endl;

            int room = 1;

            for (int light : lights)
            {
                if (light)
                    cout << "Lights in room " << room++ << " are on." << endl;
                else
                    cout << "Lights in room " << room++ << " are off." << endl;
            }
        }

        //Turn light on/off
        string turnLight (int light)
        {
            string command = "";
            lights[light-1] ? lights[light-1] = false : lights[light-1] = true;
            command = "#L" + to_string(light) + to_string(lights[light-1]);

            cout << "Light " << light << " is ";
            lights[light-1] ? cout << "on." : cout << "off.";
            cout << endl << endl;

            save();

            return command;
        }
        //temperature
        string setTemperature(int F)
        {
            string command;
            temperature = F;

            command = "#T" + to_string(temperature);
            cout << "Temperature set to " << temperature << " F." << endl << endl;

            save();
            return command;
        }
        //access
        string access()
        {
            string command;

            isOpened ? isOpened = false : isOpened = true;
            command = "#A" + to_string(isOpened);

            cout << "Your home is ";
            isOpened ? cout << "opened." : cout << "closed.";
            cout << endl << endl;

            save();

            return command;
        }



    private:

        void clearInput()
        {
            cin.clear();
            cin.ignore(10000, '\n');
        }



    };
}
