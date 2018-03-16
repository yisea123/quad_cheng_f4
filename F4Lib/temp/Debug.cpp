#include "Debug.h"


#include <iostream>
using namespace std;


const Debug_Usart debug_usart;
const Debug_Usart debug_empty;
const Debug &debug = debug_usart;




void Debug_Usart::message(const char* str,bool newline)const
{

	cout<<str;
	if(newline)cout<<"\r\n";
	cout<<flush;	
}





void Debug_Usart::warning(const char* str,bool newline)const
{
	cout<<"warning:"<<str;
	if(newline)cout<<"\r\n";
	cout<<flush;	
}



void Debug_Usart::error(const char* str,bool stop,bool newline)const
{
	cout<<"error:"<<str;
	if(newline)cout<<"\r\n";
	cout<<flush;
	
	if(stop)
		while(1);
	
	

}


void Debug_Usart::success(const char* str,bool newline)const
{
	cout<<"success:"<<str;
	if(newline)cout<<"\r\n";
	cout<<flush;
	
}

