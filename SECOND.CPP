#include <iostream>
#include <cmath>
#include <fstream>
using namespace std;

void cube(float);
void rect(float, float);
void circ(float);

int loopCount = 0;
bool wantLoop = 0;
ofstream outfile;
int main() {

char type, areaType, yesNo;
 wantLoop = 0;

do {
    cout << "This program calculates either the area of a circle or rectangle, or the volume of a cube" << endl;
    cout << "Would you like to calculate area (A) or volume (V)?" << endl;
    cin >> type;

        if (type == 'a' || type == 'A') { //chosen area
            cout << "Do you want to calculate the area of a rectangle (R) or circle (C)?" << endl;
            cin >> areaType;
            if (areaType == 'c' || areaType == 'C') { //Chosen area of circle
                float diameter = 0;
                cout << "Enter diameter" << endl;
                cin >> diameter;
                circ(diameter);
                cout << endl << "Do you want to return to the start? (Y/N)" << endl;
                cin >> yesNo;
            if (yesNo == 'Y' || yesNo == 'y') {
                cout << "Now returning to the start..." << endl << endl;
            }
            else if (yesNo == 'N' || yesNo == 'n') {

                return 0;
        }

        }
            else if (areaType == 'r' || areaType == 'R') { //chosen area of rectangle
                float length = 0, width = 0;

                cout << "Enter length and width, separated by ENTER" << endl;
                cin >> length >> width;
                rect(length, width);
                cout << endl << "Do you want to return to the start? (Y/N)" << endl;
                cin >> yesNo;
            if (yesNo == 'Y' || yesNo == 'y') {
                cout << "Now returning to the start..." << endl << endl;
        }
            else if (yesNo == 'N' || yesNo == 'n') {
                return 0;
        }    }

    }
        else if (type == 'v' || type == 'V') { //Chosen Volume
            float sLength = 0;

            cout << "Enter the side length" << endl;
            cin >> sLength;
            cube(sLength);
            cout << endl << "Do you want to return to the start? (Y/N)" << endl;
            cin >> yesNo;
        if (yesNo == 'Y' || yesNo == 'y') {
                cout << "Now returning to the start..." << endl << endl;
            }
        else if (yesNo == 'N' || yesNo == 'n') {
            return 0;
        }}
    else {
        cout << "Now returning to the start..." << endl << endl;
        wantLoop = 1;

    }

} while (wantLoop < 1);

return 0;
}

void cube (float sLength) {
    double cubeVolume = pow(sLength, 3);
    cout << cubeVolume << endl;
    outfile.open("CubeVolume.txt");
    outfile << cubeVolume;
    outfile.close();


}

void rect (float length, float width) {
    double rectArea = length * width;
    cout << rectArea << endl;
    outfile.open("RectangleArea.txt");
    outfile << rectArea;
    outfile.close();


}

void circ (float diameter) {
    float radius = diameter / 2;
    double circleArea = 3.14 * (pow(radius, 2));
    cout << circleArea << endl;
    outfile.open("CircleArea.txt");
    outfile << circleArea;
    outfile.close();
}
