#include "stdafx.h"

#include <afxwin.h>

CRITICAL_SECTION cs;
int i = 0;

UINT  FirstThread(LPVOID pvParam){
	
	for (int j = 0; j < 10;j++){
		EnterCriticalSection(&cs);
		printf("FirstThread ID:%d -->%d \n", cs.OwningThread,i++);		
		LeaveCriticalSection(&cs);
		::Sleep(rand()%2000);
	}
	
	return 0;
}

UINT  SecondThread(LPVOID pvParam){
	
	for (int j = 0; j < 10; j++){
		EnterCriticalSection(&cs);
		printf("SecondThread ID:%d -->%d \n", cs.OwningThread, i++);
		LeaveCriticalSection(&cs);
		::Sleep(rand() % 2000);
	}
	
	return 0;
}

void main()
{
	InitializeCriticalSection(&cs);
	AfxBeginThread(FirstThread, NULL);
	AfxBeginThread(SecondThread, NULL);
	
	for (int j = 0; j < 10; j++){
		EnterCriticalSection(&cs);
		printf("MainThread ID:%d -->%d \n", cs.OwningThread, i++);
		LeaveCriticalSection(&cs);
		::Sleep(rand() % 2000);
	}
	
	scanf("&d",&i);
	return;
}
