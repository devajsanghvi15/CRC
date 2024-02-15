#include <stdio.h>
#include <winsock2.h>
#include <iostream>
#include <time.h>
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

void timer(int sec)
{
    clock_t end;
    end = clock() + sec * CLOCKS_PER_SEC;
    while (clock() <= end);
}

int main()
{
    WSADATA wsa;
    SOCKET s, new_socket;
    struct sockaddr_in server, client;
    int c, recv_size;
    string data, crc, encoded, msg = "";

    printf("\nInitialising Winsock...");
    if (WSAStartup(MAKEWORD(2, 2), &wsa) != 0)
    {
        printf("Failed. Error Code : %d", WSAGetLastError());
        return 1;
    }

    printf("Initialised.\n");

    if ((s = socket(AF_INET, SOCK_STREAM, 0)) == INVALID_SOCKET)
    {
        printf("Could not create socket : %d", WSAGetLastError());
        return 1;
    }

    printf("Socket created.\n");
    server.sin_family = AF_INET;
    server.sin_addr.s_addr = INADDR_ANY;
    server.sin_port = htons(8888);

    if (bind(s, (struct sockaddr *)&server, sizeof(server)) == SOCKET_ERROR)
    {
        printf("Bind failed with error code : %d", WSAGetLastError());
        return 1;
    }

    puts("Bind done");

    listen(s, 3);

    puts("Waiting for incoming connections...");

    c = sizeof(struct sockaddr_in);
    new_socket = accept(s, (struct sockaddr *)&client, &c);
    if (new_socket == INVALID_SOCKET)
    {
        printf("accept failed with error code : %d", WSAGetLastError());
        return 1;
    }

    puts("Connection accepted");

    while (1)
    {
        printf("\nEnter data bits: ");
        cin >> data;
        printf("\nEnter Generator: ");
        cin >> crc;

        encoded += data; // encoded bits are initialized to data bits

        int datalen = data.length();
        int crclen = crc.length();

        for (int i = 1; i <= (crclen - 1); i++)
            encoded += '0'; // appending length of (generator polinomial -1) number of zeros to encoded bits

        encoded = xorfun(encoded, crc); // performing bitwise xor to obtain

        cout << "Checksum generated is: ";
        cout << encoded.substr(encoded.length() - crclen + 1) << endl
             << endl; // data bits + checksum bit is what going to be sent to reciever
        cout << "Message to be Transmitted over network: ";
        cout << data + encoded.substr(encoded.length() - crclen + 1); // this is the message going to be sent to the Reciever
        msg = data + encoded.substr(encoded.length() - crclen + 1);

        printf("\nSending code word to receiver");
        for (int cnt = 1; cnt <= 3; cnt++)
        {
            timer(1);
            printf(".");
        }
        send(new_socket, msg.c_str(), strlen(msg.c_str()), 0);
        printf("\nCode word sent");
        for (int cnt = 1; cnt <= 15; cnt++)
        {
            timer(1);
        }
        break;
    }

    closesocket(s);
    closesocket(new_socket);
    WSACleanup();

    return 0;
}
