/**
* \author       Robert Widziszewski https://github.com/softhairspring
* \copyright    https://opensource.org/licenses/MIT License
*/
#ifndef _LOGGER_H_
#define _LOGGER_H_

#include <fstream>
#include "singleton.h"

using namespace std;

/**
* Klasa zapisuj¹ca informacje do pliku
* no...
'<' przed zdaniem rozszerza akapit a
'>' zamyka akapit
*/
class rwLog : public Singleton <rwLog> {
  public:
    rwLog();
    ~rwLog();
	
    void Write(char*  logline);
	void Write(double digit);
	void Write(int    digit);

	void WriteCount();
	void count();

  private:
    ofstream m_stream;
	static int akapit;
	int m_count;
	#define grwLog rwLog::GetSingletonPtr()
};


#endif // _PRECISIONTIMER_H_