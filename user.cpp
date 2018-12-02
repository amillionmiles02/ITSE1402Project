#include<iostream>
#include<string>

using namespace std;

namespace home {
    
    class User
    {
    public:
        static const int MAX_ACCESS_LEVEL = 5;
        
        string username;    //username
        string code;        //user password
        int access;         //access level
        bool isActive;      //flag to know if current user is valid
        
        //constructors
        
        //default (required because in an array of Users all User variables must be initialize by default,
        //else, each element in an User array must be individuallt initialize)
        User()
        {
            username = "";      //default username is blank
            code = "";          //default password is blank
            access = 1;         //default access level is always 1
            isActive = false;   //set flag to false since this is not a real user (every thing is blank)
        }
        
        //providing name and password
        User(string name, string password)
        {
            username = name; //initialize username
            code = password; //initialize code
            isActive = true; //set flag to true
            access = 1;      //default access level is 1
        }
        
        //copy constructor (for assigning a User variable to another User variable)
        //Ex.code:
        // User my_user("test", "mycode");
        // User new_user = my_user; //this line will copy my_user to new_user
        User(User& u)
        {
            username = u.username; //copy username from u
            code = u.code;         //copy code from u
            isActive = u.isActive; //copy flag state from u
            access = u.access;     //copy access level from u
        }
        
        //Validate an username by asking for password and return a bool
        bool validateUser(string name)
        {
            //validate username
            if(name == username)
            {
                //ask user to enter password
                string password;
                cout << "Enter password for " << name << ": ";
                cin >> password;
                
                //validate password
                if(password == code)
                    //return true if that is the correct password
                    return true;
            }
            //return false if there any error
            return false;
        }
        
        //assign new access level
        bool assignAccess(int new_access)
        {
            //validate access level
            if(new_access >= 0 && new_access <= MAX_ACCESS_LEVEL)
            {
                //assign new access level
                access = new_access;
                //return true since operation was successful
                return true;
            }
            //return false since there was a validation error
            return false;
            
        }
        
        //operators
        bool operator== (User u)
        {
            if (u.username == username)
                return true;
            return false;
        }
        
        
    private:
        
    };
}

