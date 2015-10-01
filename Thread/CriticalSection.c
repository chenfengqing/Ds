#include "stdafx.h"

#include <afxwin.h>

#define Number 20
#define myrand(n) (int)((n)*rand()/(RAND_MAX+1.0))

CRITICAL_SECTION cs;
HANDLE g_hThreadFull;
HANDLE g_hThreadEmpty;
int i = 0;
HANDLE g_hArrayFirstThread[Number];
HANDLE g_hArraySecondThread[Number];

UINT  FirstThread(LPVOID  pvParam){//生产者
	int id = 0;
	id = *(int*)pvParam;

	for (size_t l = 0; l < 100; l++)
	{
		WaitForSingleObject(g_hThreadFull, INFINITE);

		EnterCriticalSection(&cs);		
		printf("P[%d] 第%d次生产-->%d \n", id,l, ++i);		
		LeaveCriticalSection(&cs);

		ReleaseSemaphore(g_hThreadEmpty, 1, NULL);
		::Sleep(myrand(25000));
	}
	return 0;
}

UINT  SecondThread(LPVOID  pvParam){//消费者
	int id = 0;
	id = *(int*)pvParam;	

	for (size_t l = 0; l < 100; l++)
	{
		WaitForSingleObject(g_hThreadEmpty, INFINITE);

		EnterCriticalSection(&cs);		
		for (int j = 0; j < 40; j++){
		printf(" ");
		}
		printf("C[%d]  第%d次消费-->%d \n", id, l, i--);
		LeaveCriticalSection(&cs);

		ReleaseSemaphore(g_hThreadFull, 1, NULL);
		::Sleep(myrand(25000));
	}
	return 0;
}

void main()
{
	InitializeCriticalSection(&cs);
	g_hThreadFull = CreateSemaphore(NULL, 20, 20, NULL);
	g_hThreadEmpty = CreateSemaphore(NULL,0,20,NULL);
	
	for (int k = 0; k < Number; k++){
		g_hArrayFirstThread[k]=AfxBeginThread(FirstThread, &k);
		g_hArraySecondThread[k] = AfxBeginThread(SecondThread, &k);
	}


	
	for (int k = 0; k < Number; k++){
		WaitForSingleObject(g_hArrayFirstThread[k], INFINITE);		
		WaitForSingleObject(g_hArraySecondThread[k], INFINITE);		
	}
	WaitForSingleObject(g_hThreadFull, INFINITE);
	WaitForSingleObject(g_hThreadEmpty, INFINITE);
	//DeleteCriticalSection(&cs);
	
	scanf("&d",&i);
	return;
}