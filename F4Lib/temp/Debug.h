#ifndef _DEBUG_H_
#define _DEBUG_H_



#include "sys.h"


class Debug
{
public:
	virtual ~Debug(){};
	virtual void message(const char* str,bool newline = true)const = 0;
	virtual void warning(const char* str,bool newline = true)const = 0;
	virtual void error(const char* str,bool stop = true,bool newline = true) const= 0;
	virtual void success(const char* str,bool newline = true)const = 0;
};



class Debug_Empty:public Debug
{
public:
	Debug_Empty(){};

	virtual void message(const char* str,bool newline = true)const{};
	virtual void warning(const char* str,bool newline = true)const{};
	virtual void error(const char* str,bool stop = true,bool newline = true)const{};
	virtual void success(const char* str,bool newline = true)const{};

};


class Debug_Usart:public Debug
{
public:
	Debug_Usart(){};

	virtual void message(const char* str,bool newline = true)const;
	virtual void warning(const char* str,bool newline = true)const;
	virtual void error(const char* str,bool stop = true,bool newline = true)const;
	virtual void success(const char* str,bool newline = true)const;

};






extern const Debug &debug;

//Debug &debug = debug_empty;



#endif

