// TCPServer.cpp : This file contains the 'main' function. Program execution begins and ends there.
//

#include <stdio.h>

#include <winsock2.h>

#pragma comment(lib, "ws2_32")
#pragma warning(disable:4996)

int main(int argc, char* argv[])
{
	if (argc != 4)
	{
		printf("Sai so luong tham so! Nhap vao theo dinh dang TCPServer.exe <cong> <file1> <file2>");
		return 1;
	}

	WSADATA wsa;
	WSAStartup(MAKEWORD(2, 2), &wsa);

	SOCKET listener = socket(AF_INET, SOCK_STREAM, IPPROTO_TCP);

	SOCKADDR_IN addr;
	addr.sin_family = AF_INET;
	addr.sin_addr.s_addr = htonl(INADDR_ANY);
	addr.sin_port = htons(atoi(argv[1]));

	bind(listener, (SOCKADDR*)&addr, sizeof(addr));
	listen(listener, 5);

	SOCKET client = accept(listener, NULL, NULL);

	FILE* f = fopen(argv[2], "r");
	char fbuf[256];
	fgets(fbuf, sizeof(fbuf), f);
	fclose(f);

	send(client, fbuf, strlen(fbuf), 0);

	int ret = 0;
	f = fopen(argv[3], "w");
	while (1)
	{
		ret = recv(client, fbuf, sizeof(fbuf), 0);
		if (ret <= 0)
			break;

		fbuf[ret] = 0;
		fprintf(f, "%s\n", fbuf);
	}
	fclose(f);

	closesocket(client);
	closesocket(listener);
	WSACleanup();
}
