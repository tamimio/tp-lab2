#include <iostream>
#include <windows.h>
#include <conio.h>

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
	Event() { hEvent=CreateEvent(NULL, TRUE, FALSE, NULL); }
	~Event() { Close(); }

	void Set() { SetEvent(hEvent); }
	void Reset(){ ResetEvent(hEvent); }

	void Wait (int ms) { WaitForSingleObject(hEvent, ms); }
	void Wait () { Wait(INFINITE); };

	void Close() { CloseHandle(hEvent); }
};

class Semaphore
{
	int maxCount;
	int Count;
	int c;
	int count_all;
	CS countCS;
	Event ev;
	Event sem;
	
public:
	Semaphore(int _maxCount, int count) : maxCount(_maxCount), Count(0), c(0), count_all(count)
	{
		countCS = CS();
		ev.Set();
		sem.Reset();
	}

	~Semaphore()
	{
		countCS.Delete();
		ev.Close();
	}	

	void Enter()
	{
		ev.Wait();
		countCS.Enter();
		if (c < maxCount)
		{
			std::cout << " thread =" << ++Count << "   ";
			c++;
			std::cout << "Max tread = " << maxCount << " enter: thread id: " << GetCurrentThreadId() << " total number of threads: " << c << std::endl;

			if (c < maxCount)
				ev.Set();

			countCS.Leave();
			Sleep(1500);
			countCS.Enter();

		}		
		Leave();
		countCS.Leave();
	}

	void Leave()
	{
		if (c == maxCount)
			ev.Set();

		c--;
		std::cout << "Leave: thread id: " << GetCurrentThreadId() << " total number of threads: " << c << std::endl;
	}

};