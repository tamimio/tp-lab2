#include <iostream>
#include <windows.h>
#include "Semaphore.h"

Semaphore *sem;

DWORD WINAPI foo(PVOID)
{
	int a=2,b=0;
	sem->Enter();
	for (int i = 0; i < 5; ++i)
	{
		b += a;
		Sleep(10);
	}	
	return 0;
}

int main()
{
	int maxCount, num;
	std::cout << "Input maxCount for Semaphore -> ";
	std::cin >> maxCount;
	std::cout << "Input number of threads -> ";
	std::cin >> num;

	sem = new Semaphore(maxCount, num);
	HANDLE * thread = new HANDLE [num];

	for(int i=0; i<num; ++i)
        thread[i]=CreateThread(NULL, 0, foo, NULL, 0, NULL);

	WaitForMultipleObjects(num, thread, TRUE, INFINITE);

	for(int i=0; i<num; ++i)
		CloseHandle(thread[i]);

	std::cout<<"Done ";
	return 0;
}


