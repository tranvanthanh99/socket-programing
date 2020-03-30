// TCPClient.cpp : This file contains the 'main' function. Program execution begins and ends there.
//

#include <stdio.h>

#define _WINSOCK_DEPRECATED_NO_WARNINGS
#include <winsock2.h>

#pragma comment(lib, "ws2_32")

int main(int argc, char* argv[])
{
    if (argc != 3)
    {
        printf("Sai so luong tham so! Nhap vao theo dinh dang TCPClient.exe <dia chi IP> <cong>");
        return 1;
    }

    WSADATA wsa;
    WSAStartup(MAKEWORD(2, 2), &wsa);

    SOCKET client = socket(AF_INET, SOCK_STREAM, IPPROTO_TCP);

    SOCKADDR_IN addr;
    addr.sin_family = AF_INET;
    addr.sin_addr.s_addr = inet_addr(argv[1]);
    addr.sin_port = htons(atoi(argv[2]));

    int ret = connect(client, (SOCKADDR*)&addr, sizeof(addr));
    if (ret == SOCKET_ERROR)
    {
        printf("connect() failed");
        return 1;
    }

    char buf[256];
    ret = recv(client, buf, sizeof(buf), 0);
    if (ret <= 0)
    {
        printf("recv() failed");
        return 1;
    }

    buf[ret] = 0;
    printf("Received: %s\n", buf);

    while (1)
    {
        printf("Enter a string: ");
        gets_s(buf);

        if (strcmp(buf, "exit") == 0)
            break;

        send(client, buf, strlen(buf), 0);
    }

    closesocket(client);
    WSACleanup();
}
