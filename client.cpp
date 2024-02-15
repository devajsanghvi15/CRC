#include <stdio.h>
#include <conio.h>
#include <winsock2.h>
#include <time.h>
#include <iostream>
using namespace std;
#pragma comment(lib, "ws2_32.lib")

string xorfun(string encoded, string crc) // Bitwise XOR operation
{
    int crclen = crc.length();

    for (int i = 0; i <= (encoded.length() - crclen);) // performing bitwise xor operation
    {                                                  // " 0 xor 0 = 0"     " 1 xor 1 = 0 "
        for (int j = 0; j < crclen; j++)               // " 0 xor 1 = 1 "    " 1 xor 0 = 1"
        {
            encoded[i + j] = encoded[i + j] == crc[j] ? '0' : '1'; // if encoded bit and crc bit are same , then replace it with zero
        }
        for (; i < encoded.length() && encoded[i] != '1'; i++)
            ;
    }

    return encoded;
}

void timer(int);

int main()
{
    WSADATA wsa;
    SOCKET sck;
    struct sockaddr_in ip_info;
    char msg[2000];
    int recv_size;
    string crc = "1101";

    printf("Installing");
    for (int cnt = 1; cnt <= 3; cnt++)
    {
        timer(1);
        printf(".");
    }

    if (WSAStartup(MAKEWORD(2, 2), &wsa) != 0)
        printf("WSA error\n");
    else if ((sck = socket(AF_INET, SOCK_STREAM, 0)) == INVALID_SOCKET)
        printf("Socket error\n");
    else
    {
        ip_info.sin_addr.s_addr = inet_addr("127.0.0.1");
        ip_info.sin_family = AF_INET;
        ip_info.sin_port = htons(8888);
    }

    if (connect(sck, (struct sockaddr *)&ip_info, sizeof(ip_info)) < 0)
        printf("\nNot connected\n");
    else
        printf("\nSession is up\n");

    while (1)
    {
        if ((recv_size = recv(sck, msg, sizeof(msg), 0)) == SOCKET_ERROR)
        {
            puts("recv failed");
            break;
        }
        else if (recv_size == 0)
        {
            puts("Connection closed by server");
            break;
        }
        // else
        // {
        //     msg[recv_size] = '\0';
        //     puts("Code word received: ");
        //     puts(msg);
        //     //string(msg);
        //     string strmsg(msg);
        //     //cout<<endl<<msg<<endl;
        //     strmsg = xorfun(strmsg, crc);
        //     cout << "Modified Code word after XOR: " << strmsg << endl;

        //     for (char i : strmsg.substr(strmsg.length() - crc.length() + 1)) // after performing xor , if the last few bits are zero then there's no error in transmission
        //         if (i != '0')
        //         {
        //             cout << "Error in communication " << endl; // if bits not zero ; ERROR IN TRANSMISSION
        //             return 0;
        //         }

        //     cout << "No Error !" << endl; // else NO ERROR
        //     return 0;
        // }
        else
        {

            msg[recv_size] = '\0';
            //puts("Code word received: ");
            //puts(msg);
            string strmsg(msg);

            int choice;
            printf("\nEnter your choice:\n");
            printf("1. Successful case\n");
            printf("2. Error case\n");
            cin >> choice;

            switch (choice)
            {
            case 1:
                puts("Code word received: ");
                cout<<strmsg;
                strmsg = xorfun(strmsg, crc);
                cout << "\nModified Code word after XOR: " << strmsg << endl;

                for (char i : strmsg.substr(strmsg.length() - crc.length() + 1)) // after performing xor , if the last few bits are zero then there's no error in transmission
                    if (i != '0')
                    {
                        cout << "Error in communication " << endl; // if bits not zero ; ERROR IN TRANSMISSION
                        return 0;
                    }

                cout << "No Error !" << endl; // else NO ERROR
                return 0;

            case 2:
                puts("Code word received: ");
                swap(strmsg[strmsg.length() - 2], strmsg[strmsg.length() - 1]);
                cout<<strmsg;
                strmsg = xorfun(strmsg, crc);
                cout << "\nModified Code word after XOR: " << strmsg << endl;

                for (char i : strmsg.substr(strmsg.length() - crc.length() + 1)) // after performing xor , if the last few bits are zero then there's no error in transmission
                    if (i != '0')
                    {
                        cout << "Error in communication " << endl; // if bits not zero ; ERROR IN TRANSMISSION
                        return 0;
                    }

                cout << "No Error !" << endl; // else NO ERROR
                return 0;
            }
        }
    }

    closesocket(sck);
    WSACleanup();
    getch();

    return 0;
}

// Timer function:
void timer(int sec)
{
    clock_t end;
    end = clock() + sec * CLOCKS_PER_SEC;
    while (clock() <= end)
        ;
}