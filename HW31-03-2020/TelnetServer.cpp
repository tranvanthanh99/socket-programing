// TelnetServer.cpp : This file contains the 'main' function. Program execution begins and ends there.
//


#include <stdio.h>
#include <winsock2.h>

#pragma comment(lib, "ws2_32")
#pragma warning(disable:4996)

DWORD WINAPI ClientThread(LPVOID);

CRITICAL_SECTION cs;

int main()
{
	WSADATA wsa;
	WSAStartup(MAKEWORD(2, 2), &wsa);

	InitializeCriticalSection(&cs);

	SOCKADDR_IN addr;
	addr.sin_family = AF_INET;
	addr.sin_addr.s_addr = htonl(INADDR_ANY);
	addr.sin_port = htons(9000);

	SOCKET listener = socket(AF_INET, SOCK_STREAM, IPPROTO_TCP);
	bind(listener, (SOCKADDR*)&addr, sizeof(addr));
	listen(listener, 5);

	// Chap nhan ket noi va truyen nhan du lieu
	while (1)
	{
		printf("waiting foe new client...\n");
		SOCKET client = accept(listener, NULL, NULL);
		printf("New client accepted: %s", client);

		CreateThread(0, 0, ClientThread, &client, 0, 0);
	}

	DeleteCriticalSection(&cs);
}

DWORD WINAPI ClientThread(LPVOID lpParam)
{
	SOCKET client = *(SOCKET*)lpParam;

	char buf[256];
	int ret;

	char user[32], pass[32], tmp[32], fbuf[64];

	// xac thuc id client
	while (1)
	{
		ret = recv(client, buf, sizeof(buf), 0);
		if (ret <= 0)
		{
			closesocket(client);
			return 0;
		}

		buf[ret] = 0;
		printf("Received: %s\n", buf);

		ret = sscanf(buf, "%s %s %s", user, pass, tmp);
		int found = 0;
		if (ret == 2)
		{
			sprintf(tmp, "%s %s", user, pass);
			FILE* f = fopen("D:\\LTU16\\lập trình mạng\\test\\user.txt", "r");
			while (!feof(f))
			{
				fgets(fbuf, sizeof(fbuf), f);
				if (strncmp(tmp, fbuf, strlen(tmp)) == 0)
				{
					found = 1;
					break;
				}
			}
			fclose(f);
			if (found == 1)
			{
				const char* msg = "dang nhap thanh cong. Hay gui lenh\n";
				send(client, msg, strlen(msg), 0);
			}
			else
			{
				const char* msg = "dang nhap that bai. Hay gui lai username va password\n";
				send(client, msg, strlen(msg), 0);
			}
		}
		else
		{

		}
	}

	// Forwarding messages
	while (1)
	{
		ret = recv(client, buf, sizeof(buf), 0);
		if (ret <= 0)
		{
			closesocket(client);
			return 0;
		}

		buf[ret] = 0;
		printf("receved: %s\n", buf);

		if (buf[ret - 1] == '\n')
			buf[ret - 1] == 0;

		EnterCriticalSection(&cs);

		char cmdBuf[64];
		sprintf(cmdBuf, "%s > D:\\LTU16\\lập trình mạng\\test\\output.txt", buf);
		system(cmdBuf);


		FILE* f = fopen("D:\\LTU16\\lập trình mạng\\test\\output.txt", "rb");
		while (!feof(f))
		{
			ret = fread(buf, 1, sizeof(buf), f);
			if (ret <= 0) break;
			send(client, buf, ret, 0);
		}
		fclose(f);
		LeaveCriticalSection(&cs);
	}

	return 0;
}


// Run program: Ctrl + F5 or Debug > Start Without Debugging menu
// Debug program: F5 or Debug > Start Debugging menu

// Tips for Getting Started: 
//   1. Use the Solution Explorer window to add/manage files
//   2. Use the Team Explorer window to connect to source control
//   3. Use the Output window to see build output and other messages
//   4. Use the Error List window to view errors
//   5. Go to Project > Add New Item to create new code files, or Project > Add Existing Item to add existing code files to the project
//   6. In the future, to open this project again, go to File > Open > Project and select the .sln file
