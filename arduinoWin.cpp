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
        HANDLE arduinoBoard;

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

            arduinoBoard = CreateFile(port_string.c_str(), GENERIC_WRITE, 0, NULL, OPEN_EXISTING, 0, NULL);

            if (GetCommState(arduinoBoard, &dcb))
            {
                isConnected = true;

                dcb.BaudRate = CBR_9600;
                dcb.ByteSize = 8;
                dcb.Parity = NOPARITY;
                dcb.StopBits = ONESTOPBIT;
                dcb.fDtrControl = DTR_CONTROL_ENABLE;

                SetCommState(arduinoBoard, &dcb);


                return true;
            }
            return false;
        }

        void close()
        {
            CloseHandle(arduinoBoard);
            isConnected = false;
        }

        bool Send(string command)
        {
            string cmd = command + "\n";
            if (isConnected && command != "exit" && command != "")
            {
               return WriteFile(arduinoBoard, cmd.c_str(), cmd.size(), &byteswritten, NULL);
            }
            return false;
        }
    };
}
