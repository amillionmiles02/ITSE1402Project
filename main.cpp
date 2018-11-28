#include "home.cpp"
#include "arduinoWin.cpp"

using namespace std;
using namespace home;

ostream& operator<<(ostream&, User);

void clearInput();
void initialize(arduino::Board&, Home&);

int main()
{
    //create new object Home
    Home myhome;

    //open the home
    myhome.open();

    arduino::Board myboard;

    if (myboard.Connect(10))
    {
        cout << "SUCCESS!" << endl;
    }
    else
        cout << "ERROR" << endl;

    return 0;
}

void clearInput()
{
    cin.clear();
    cin.ignore(10000, '\n');
}

void initialize(arduino::Board& board, Home& home)
{
    for (int i = 0; i < home.lights.size(); i++)
    {
        if (home.lights[i])
            board.Send("#L" + to_string(i + 1) + "1");
    }
    board.Send("#T" + to_string(home.temperature));
    if (home.isOpened)
        board.Send("#A1");
}
