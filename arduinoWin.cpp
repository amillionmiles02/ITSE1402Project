#include <windows.h>
#include <iostream>
#include <string>

using namespace std;

namespace arduino
{
    class Board
    {
        public:
        DCB dcb;
        DWORD byteswritten;
        HANDLE arduino;

        bool isConnected;
        int arduinoPort;

        Board()
        {
           isConnected = false;
           arduinoPort = 0;


        }

        bool Connect(int port)
        {
            arduinoPort = port;
            string port_string = "\\\\.\\COM" + to_string(port);

            arduino = CreateFile(port_string.c_str(), GENERIC_WRITE, 0, NULL, OPEN_EXISTING, 0, NULL);

            if (GetCommState(arduino, &dcb))
            {
                isConnected = true;

                dcb.BaudRate = CBR_9600;
                dcb.ByteSize = 8;
                dcb.Parity = NOPARITY;
                dcb.StopBits = ONESTOPBIT;

                SetCommState(arduino, &dcb);

                return true;
            }
            return false;
        }

        void close()
        {
            CloseHandle(arduino);
            isConnected = false;
        }

        bool Send(string command)
        {
            if (isConnected && command != "exit" && command != "")
            {
               return WriteFile(arduino, command.c_str(), 1, &byteswritten, NULL);
            }
            return false;
        }
    };
}
