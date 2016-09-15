/**
* \author       Robert Widziszewski https://github.com/softhairspring
* \copyright    https://opensource.org/licenses/MIT License
*/
#ifndef _LOGGER_CPP
#define _LOGGER_CPP
#include "rwLogger.h"

//#ifdef DEBUG
#define _LOG_DBG_
#endif

//ofstream Log::m_stream;
int rwLog::akapit;

rwLog::rwLog() {
#ifdef _LOG_DBG_

	m_stream.open("db.rwdb");
	m_stream.precision(5);
	m_count =0;
	akapit=0;
#endif
}

void rwLog::Write(char* logline){
#ifdef _LOG_DBG_	
	if(akapit < 0)
		akapit =0;
	for(int i=akapit;i!=0;--i)
		m_stream << "\t";
	m_stream << logline << endl;
	if(*logline == '<')
		++akapit;
	if(*logline == '>')
		--akapit;
#endif
}

void rwLog::Write(double digit){
#ifdef _LOG_DBG_	
	for(int i=akapit;i!=0;--i)
		m_stream << "\t";
	m_stream << digit << endl;

#endif
}
void rwLog::Write(int digit){
#ifdef _LOG_DBG_	
	for(int i=akapit;i!=0;--i)
		m_stream << "\t";
	m_stream << digit << endl;
#endif
}
void rwLog::count(){
	m_count++;
}

void rwLog::WriteCount(){
#ifdef _LOG_DBG_
	for(int i=akapit;i!=0;--i)
		m_stream << "\t";
	m_stream << m_count << endl;
	m_count=0;
#endif
}

rwLog::~rwLog(){
  m_stream.close();
}

//#endif
