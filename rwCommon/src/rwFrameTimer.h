/**
* \author       Robert Widziszewski https://github.com/softhairspring
* \copyright    https://opensource.org/licenses/MIT License
*/
#ifdef WIN32

#ifndef TIMER_H
#define TIMER_H
#include "windows.h"



/**
* klasa kontroluj¹ca wyœwietlanie klatek na sekunde
* liczy pozosta³y czas dla klatki
* posiada równie¿ licznik sekund
**/
class rwFrameTimer{

	bool ISFRAMEACTIVE;
	bool ISSECONDSCOUNTING;
	/// koniec klatki/start klatki/klatkaconst/roznicaostatniej klatki//sekunda/milisek.
	LARGE_INTEGER lStop, lStart,lFreq,lDiff,lSecond,lMilisec;
	LARGE_INTEGER lSecCountStart, lSecCountCurrent;
	int seconds;
	double dSleep;
	double out ;//temp

public:
	rwFrameTimer(){
		QueryPerformanceCounter(&lStop);
		QueryPerformanceCounter(&lStart);
		QueryPerformanceCounter(&lDiff);
		QueryPerformanceFrequency(&lSecond);
		lFreq.QuadPart = (LONGLONG)(lSecond.QuadPart * 0.16666);
		lMilisec.QuadPart = lSecond.QuadPart/1000;
		dSleep=0;
		ISFRAMEACTIVE=false;
		ISSECONDSCOUNTING=false;
	}

	///zaczyna liczyc czas nowej aktualnej klatki
	void start(){
		QueryPerformanceCounter(&lStart);
		ISFRAMEACTIVE=true;
	}
	///zatrzymuje licznik aktualnej klatki
	void stop(){
		QueryPerformanceCounter(&lStop);
		ISFRAMEACTIVE=false;
		lDiff.QuadPart = getFrameLONG();
	}

	///ustala czas jednej klatki FPS (podaj czas jednej klatki)
	void setFrequency(double second){
		lFreq.QuadPart = (LONGLONG)(second * lSecond.QuadPart);
	}
	double getFrequency(){
		return ((double)lFreq.QuadPart / (double)lSecond.QuadPart);
	}
	///pobiera czas wykonania jednej klatki przez program w sekundach
	double getFrameInSeconds(){
		if(ISFRAMEACTIVE)
			return 0;
		return (double(lStop.QuadPart - lStart.QuadPart) / lSecond.QuadPart);
	}
	///pobiera czas wykonania jednej klatki przez program w milisekundach
	double getFrameInMiliSeconds(){
		if(ISFRAMEACTIVE)
			return 0;
		return (double(lStop.QuadPart - lStart.QuadPart) / lMilisec.QuadPart);
	}
	double getLastFrameInSeconds(){
		return  ((double)lDiff.QuadPart/ (double)lSecond.QuadPart);
	}
	///pobiera czas wykonania jednej klatki przez program w formacie windowsowym
	double getFrame(){
		if(ISFRAMEACTIVE)
			return 0;
		return (double(lStop.QuadPart - lStart.QuadPart));
	}
	int getFrameLONG(){
		if(ISFRAMEACTIVE)
			return 0;
		return (int(lStop.QuadPart - lStart.QuadPart));
	}
	///pobier czas pozosta³y po wykonaniu klatki a przed rozpoczêciem nastêpnej w formacie windows
	double getFreeTime(){
		if(ISFRAMEACTIVE)
			return 0;
		return (double)(lFreq.QuadPart - (lStop.QuadPart - lStart.QuadPart));
	}
	///pobier czas pozosta³y po wykonaniu klatki a przed rozpoczêciem nastêpnej w sekundach
	double getFreeTimeInSeconds(){
		if(ISFRAMEACTIVE)
			return 0;
		return (double)(lFreq.QuadPart - (lStop.QuadPart - lStart.QuadPart)) / (double)lSecond.QuadPart ;
	}
	///odczekuje czas pozosta³y po wykonaniu klatki a przed rozpoczêciem nastêpnej
	bool waitDiff(){
		if(ISFRAMEACTIVE)
			return false;
		dSleep = (double)(lFreq.QuadPart - (lStop.QuadPart - lStart.QuadPart))/ (double)lSecond.QuadPart;
		if(dSleep < 0)
			return false;
		Sleep( (DWORD)dSleep);
		return true;
	}

	///resetuje licznik sekund
	void resetSecondsCounter(){
		lSecCountStart.QuadPart=0;
		lSecCountCurrent.QuadPart=0;
		seconds=0;
		ISSECONDSCOUNTING=false;
	}
	///rozpoczyna liczenie sekund
	void startSecondsCounting(){
		resetSecondsCounter();
		QueryPerformanceCounter(&lSecCountStart);
		QueryPerformanceCounter(&lSecCountCurrent);
		ISSECONDSCOUNTING=true;
	}
	///rozpoczyna liczenie sekund
	void startSecondsCountingWithoutReset(){
		QueryPerformanceCounter(&lSecCountStart);
		QueryPerformanceCounter(&lSecCountCurrent);
		ISSECONDSCOUNTING=true;
	}
	///zatrzymuje licznik sekund - raz zatrzymany licznik nie mozna kontynuowac(zostaje reset)
	void stopSecondsCounting(){
		ISSECONDSCOUNTING=false;
		seconds = getSecondsCount();
	}
	///pobiera liczbe sekund od rozpoczecia zliczania
	int getSecondsCount(){
		if(ISSECONDSCOUNTING)
			QueryPerformanceCounter(&lSecCountCurrent);
		out= ( ((double)lSecCountCurrent.QuadPart- (double)lSecCountStart.QuadPart)/lSecond.QuadPart);
		return (int)out;
	}
	///pobiera liczbe milisekund od rozpoczecia zliczania
	int getMiliSecondsCount(){
		if(ISSECONDSCOUNTING)
			QueryPerformanceCounter(&lSecCountCurrent);
		out= ( ((double)lSecCountCurrent.QuadPart- (double)lSecCountStart.QuadPart)/lMilisec.QuadPart);
		return (int)out;
	}
	///czy liczymy sekundy?
	bool isSecondsCounting(){
		return ISSECONDSCOUNTING;
	}

	bool isStopped(){
		return ! ISFRAMEACTIVE;
	}
};



#endif

#endif