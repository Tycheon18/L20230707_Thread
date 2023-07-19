#define _WINSOCK_DEPRECATED_NO_WARNINGS

#include <stdio.h>
#include <iostream>
#include <winsock2.h>
#include "MySQLcommend.h"
#include <ws2tcpip.h>
#include <windows.h>
#include <process.h>
#include <vector>

using namespace std;

#pragma comment(lib, "ws2_32.lib")

typedef struct socket_INFO
{
	SOCKET ClientSocket;
	std::string User;
}SOCKET_INFO, Socket_INFO;

std::vector<SOCKET_INFO> Client_list;

DWORD WINAPI ProcessClient(LPVOID arg)
{
	SOCKET ClientSocket = (SOCKET)arg;
	int RecvByte = 0;
	int SendByte = 0;
	sockaddr_in ClientSockAddr;
	char addr[INET_ADDRSTRLEN];
	int addrlen;
	char Buffer[512] = { 0, };

	addrlen = sizeof(ClientSockAddr);
	getpeername(ClientSocket, (SOCKADDR*)&ClientSockAddr, &addrlen);
	inet_ntop(AF_INET, &ClientSockAddr.sin_addr, addr, sizeof(addr));

	while (1)
	{
		RecvByte = recv(ClientSocket, Buffer, 512, 0);

		//Buffer[RecvByte] = '\0'; //뭔데 얘 달려있으면 언리얼에서 통신이 안되지??
		printf("[TCP/%s:%d] %s\n", addr, ntohs(ClientSockAddr.sin_port), Buffer);

		//char sendstr[512] = { 0, };
		//for (int i = 0; i < strlen(Buffer); ++i)
		//{
		//	sendstr[i] = Buffer[i];
		//}

		SendByte = send(ClientSocket,Buffer, 512, 0);

	}

	closesocket(ClientSocket);
	printf("[TCP 서버] 클라이언트 종료 : IP 주소 = %s , 포트 번호 = %d\n", addr, ntohs(ClientSockAddr.sin_port));
	return 0;
}
//
//DWORD WINAPI cinTest(LPVOID arg)
//{
//	while(1)
//	{
//		std::string testText = "";
//		cin >> testText;
//	}
//
//	return 0;
//}

int main()
{
	MySQLcommend SQL;

	WSAData wsaData;
	WSAStartup(MAKEWORD(2, 2), &wsaData);

	SOCKET ServerSocket = socket(AF_INET, SOCK_STREAM, 0);

	SOCKADDR_IN ServerSockAddr;
	memset(&ServerSockAddr, 0, sizeof(ServerSockAddr));
	ServerSockAddr.sin_family = PF_INET;
	ServerSockAddr.sin_addr.s_addr = INADDR_ANY; //for Test
	ServerSockAddr.sin_port = htons(3001);

	bind(ServerSocket, (SOCKADDR*)&ServerSockAddr, sizeof(ServerSockAddr));

	listen(ServerSocket, SOMAXCONN);

	fd_set ReadSockets;
	fd_set CopyReadSockets;
	FD_ZERO(&ReadSockets);
	FD_SET(ServerSocket, &ReadSockets);

	TIMEVAL TimeOut;
	TimeOut.tv_sec = 0;
	TimeOut.tv_usec = 100;

	SOCKADDR_IN ClientSockAddr;
	memset(&ClientSockAddr, 0, sizeof(ClientSockAddr));
	int ClientSockAddrLength;

	SOCKET ClientSocket;
	HANDLE hThread;
	//HANDLE tThread = CreateThread(NULL, 0, cinTest, 0, 0, NULL);
	while (1)
	{
		ClientSockAddrLength =  sizeof(ClientSockAddr);
		ClientSocket = accept(ServerSocket, (SOCKADDR*)&ClientSockAddr, &ClientSockAddrLength);

		char addr[INET_ADDRSTRLEN];
		inet_ntop(AF_INET, &ClientSockAddr.sin_addr, addr, sizeof(addr));
		printf("\n[TCP 서버] 클라이언트 접속 : IP 주소 = %s , 포트 번호 = %d\n", addr, ntohs(ClientSockAddr.sin_port));

		hThread = CreateThread(NULL, 0, ProcessClient, (LPVOID)ClientSocket, 0, NULL);
		if (hThread == NULL) { closesocket(ClientSocket); }
		else { CloseHandle(hThread); }
	}

	//while(1)
	//{
	//	CopyReadSockets = ReadSockets;
	//	int ChangeCount = select(0, &CopyReadSockets, 0, 0, &TimeOut);
	//	if (ChangeCount == 0) continue;
	//	else
	//	{
	//		for(int i = 0; i < (int)ReadSockets.fd_count; ++i)
	//		{
	//			SOCKADDR_IN ClientSockAddr;
	//			memset(&ClientSockAddr, 0, sizeof(ClientSockAddr));
	//			int ClientSockAddrLength = sizeof(ClientSockAddr);

	//			SOCKET ClientSocket = accept(ServerSocket, (SOCKADDR*)&ClientSockAddr, &ClientSockAddrLength);

	//			char ip[128] = { 0, };

	//			string ClientIP = inet_ntop(AF_INET, &ClientSockAddr.sin_addr, ip, INET_ADDRSTRLEN);

	//			cout << ClientIP << endl;
	//		}
	//	}

	//	string Text = "";
	//	cin >> Text;

	//}

	//while(1)
	//{
	//	char Buffer[512] = { 0, };
	//	
	//	int RecvByte = 0;
	//	int SendByte = 0;

	//	RecvByte = recv(ClientSocket, Buffer, 512, 0);
	//	
	//	cout << Buffer << endl;
	//	cout << "Buffer[0]은 " << Buffer[0] << "입니다." << endl;
	//	cout << "Buffer의 길이는 " << strlen(Buffer) << "입니다." << endl;

	//	if (Buffer[0] == '1' && strlen(Buffer) == 1)
	//	{
	//		char sendnum[] = "1";
	//		SendByte = send(ClientSocket, sendnum, strlen(sendnum) + 1, 0);

	//		char sendstr[] = "Join : Please input Your ID and PASSWORD.";
	//		SendByte = send(ClientSocket, sendstr, strlen(sendstr) + 1, 0);

	//		//cout << "회원가입 절차 시작" << endl;

	//		char IDBuf[512] = { 0, };
	//		char PWBuf[512] = { 0, };
	//		string ID;
	//		string PW;	

	//		RecvByte = recv(ClientSocket, IDBuf, 512, 0);
	//		RecvByte = recv(ClientSocket, PWBuf, 512, 0);

	//		for (int i = 0; i < strlen(IDBuf); ++i)
	//		{
	//			ID += IDBuf[i];
	//		}
	//		for (int i = 0; i < strlen(PWBuf); ++i)
	//		{
	//			PW += PWBuf[i];
	//		}
	//		
	//		cout << ID << ' ' << PW;

	//		char num = SQL.Join(ID, PW);

	//		switch (num)
	//		{
	//		case '0':
	//		{
	//			char result[] = "Join Success!";
	//			SendByte = send(ClientSocket, result, strlen(result) + 1, 0);
	//			break;
	//		}
	//		case '1':
	//		{
	//			char result[] = "This ID already exists.";
	//			SendByte = send(ClientSocket, result, strlen(result) + 1, 0);
	//			break;
	//		}
	//		case '2':
	//		{
	//			char result[] = "DB Error.";
	//			SendByte = send(ClientSocket, result, strlen(result) + 1, 0);
	//			break;
	//		}
	//		}

	//	}
	//	else if (Buffer[0] == '2' && strlen(Buffer) == 1)
	//	{
	//		char sendnum[] = "2";
	//		SendByte = send(ClientSocket, sendnum, strlen(sendnum) + 1, 0);

	//		char sendstr[] = "Login : Please input Your ID and PASSWORD.";
	//		SendByte = send(ClientSocket, sendstr, strlen(sendstr) + 1, 0);

	//		char IDBuf[512] = { 0, };
	//		char PWBuf[512] = { 0, };
	//		string ID;
	//		string PW;

	//		RecvByte = recv(ClientSocket, IDBuf, 512, 0);
	//		RecvByte = recv(ClientSocket, PWBuf, 512, 0);

	//		for (int i = 0; i < strlen(IDBuf); ++i)
	//		{
	//			ID += IDBuf[i];
	//		}
	//		for (int i = 0; i < strlen(PWBuf); ++i)
	//		{
	//			PW += PWBuf[i];
	//		}

	//		cout << "ID is : " << ID << "/ PW is : " << PW << endl;

	//		char num = SQL.Login(ID, PW);

	//		switch (num)
	//		{
	//		case '0':
	//		{
	//			char result[] = "Login Success!";
	//			SendByte = send(ClientSocket, result, strlen(result) + 1, 0);
	//			break;
	//		}
	//		case '1':
	//		{
	//			char result[] = "PASSWORD is Incorrect.";
	//			SendByte = send(ClientSocket, result, strlen(result) + 1, 0);
	//			break;
	//		}
	//		case '2':
	//		{
	//			char result[] = "ID does not exist.";
	//			SendByte = send(ClientSocket, result, strlen(result) + 1, 0);
	//			break;
	//		}
	//		case '3':
	//		{
	//			char result[] = "DB Error.";
	//			SendByte = send(ClientSocket, result, strlen(result) + 1, 0);
	//			break;
	//		}
	//		}

	///*		switch ()
	//		{
	//		case '0':
	//			cout << "로그인 성공!" << endl;
	//			break;
	//		case '1':
	//			cout << "비밀번호가 틀렸습니다." << endl;
	//			break;
	//		case '2':
	//			cout << "존재하지 않는 ID입니다." << endl;
	//			break;
	//		case '3':
	//			cout << "DB 오류 발생." << endl;
	//			break;
	//		}*/
	//	}
	//	else
	//	{
	//		char sendstr[] = "abcdefgh1234";
	//		SendByte = send(ClientSocket, sendstr, strlen(sendstr)+1, 0);
	//	}

	//	
	//}

	//closesocket(ClientSocket);
	closesocket(ServerSocket);

	WSACleanup();

	return 0;
}