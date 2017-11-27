#include <iostream>
#include <windows.h>

// todo: need to show how much threads are in Semaphore (<=5) and how much are waiting to enter (<=20) 

class CS
{
	CRITICAL_SECTION cs;
public:
	CS() { InitializeCriticalSection(&cs); }
	void Enter() { EnterCriticalSection(&cs); }
	void Leave() { LeaveCriticalSection(&cs); }
	void Delete() { DeleteCriticalSection(&cs); }
};

class Event
{
	HANDLE hEvent;
public:
	Event() { CreateEvent(NULL, TRUE, FALSE, NULL); };
	~Event() { Close(); }

	void Set() { SetEvent(hEvent); }
	void Reset();

	/*DWORD*/ void Wait (int ms) { WaitForSingleObject(hEvent, ms); };
	/*DWORD*/ void Wait () { Wait(INFINITE); };

	void Close() { CloseHandle(hEvent); };
};

class Semaphore
{
	int maxCount;
	int Count;
	CS countCS;
	Event ev;
public:
	Semaphore(int _maxCount) : maxCount(_maxCount), Count (0)
	{
		countCS=CS();
		ev=Event();
	}
	~Semaphore()
	{
		countCS.Delete();
		ev.Close();
	}

	void Enter()
	{
		countCS.Enter();

        int id = GetCurrentThreadId();

		if(Count++<maxCount)
		{
			std::cout << "Enter: thread id: "<<id<<" total number of threads: " << Count << std::endl;
			countCS.Leave();
			return;
		}
		//std::cout << "Enter: CS is unlocked, total number of threads: " << Count << std::endl;
		std::cout << "Enter2: thread id: "<<id<<" total number of threads: " << Count << std::endl;
		countCS.Leave();
		ev.Wait();
	}

	void Leave()
	{
		countCS.Enter();
		if ((--Count)!=0)
             //if(--Count>=maxCount)
		{
			ev.Set();
			//std::cout << "Leave: CS is unlocked, total number of threads: " << Count << std::endl;

            int id = GetCurrentThreadId();
            std::cout << "Leave: thread id: "<<id<<" total number of threads: " << Count << std::endl;
		}
		countCS.Leave();
	}
};

Semaphore *sem;

DWORD WINAPI foo(PVOID)
{
	for (int i=0; i<5; ++i)
    {
		sem->Enter();
		Sleep(1000);
		sem->Leave();
		//Sleep(100);
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

	sem = new Semaphore(maxCount);
	HANDLE * thread = new HANDLE [num];

	for(int i=0; i<num; ++i)
    {
        thread[i]=CreateThread(NULL, 0, foo, NULL, 0, NULL);



    }

	WaitForMultipleObjects(num, thread, TRUE, INFINITE);

	for(int i=0; i<num; ++i)
		CloseHandle(thread[i]);

	std::cout<<"Done ";
	return 0;
}


