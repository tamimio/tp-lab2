#include <iostream>
#include <windows.h>
#include <conio.h>

using namespace std;

class CS
{
	CRITICAL_SECTION cs;
public:
	CS() { InitializeCriticalSection(&cs); }
	void Enter() { EnterCriticalSection(&cs); }
	void Leave() { LeaveCriticalSection(&cs); }
};

class Event
{
	HANDLE hEvent;
public:
	Event();
	~Event();
	void Set();
	void Reset();
	DWORD Wait (int ms);
	DWORD Wait () { Wait(INFINITE); };
};

class Semaphor
{
	int maxCount;
	int Count;
	CS countCS;
	Event ev;
public:
	Semaphor(int _maxCount) : maxCount(_maxCount), Count (0) {}
	~Semaphor(){};

	void Enter()
	{
		countCS.Enter();
		if(Count++<maxCount)
		{
			countCS.Leave();
			return;
		}
		countCS.Leave();
		ev.Wait();
	}

	void Leave()
	{
		countCS.Enter();
		if(--Count>=maxCount)
		{
			ev.Set();
		}
		countCS.Leave();
	}
};
