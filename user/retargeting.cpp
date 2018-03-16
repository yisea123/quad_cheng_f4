#include <stdio.h>
#include <stdlib.h>

#include "HAL_F4.h"
#pragma import(__use_no_semihosting)


namespace std {
  struct __FILE
  {
    int handle;
    /* Whatever you require here. If the only file you are using is */
    /* standard output using printf() for debugging, no file handling */
    /* is required. */
  };
  FILE __stdout;
  FILE __stdin;
  FILE __stderr;
  int fgetc(FILE *f)
  {
    /* Your implementation of fgetc(). */
    return 0;
  }
  int fputc(int c, FILE *stream)
  {
    /* Your implementation of fputc(). */
	  hal.console.putc(c);
	  return c;
  }
  int ferror(FILE *stream)
  {
    /* Your implementation of ferror(). */
	  return 0;
  }
  long int ftell(FILE *stream)
  {
    /* Your implementation of ftell(). */
	  return 0;
  }
  int fclose(FILE *f)
  {
    /* Your implementation of fclose(). */
    return 0;
  }
  int fseek(FILE *f, long nPos, int nMode)
  {
    /* Your implementation of fseek(). */
    return 0;
  }
  int fflush(FILE *f)
  {
    /* Your implementation of fflush(). */    
    return 1;
  }
  
  
  extern "C"  void  _sys_exit(int) {
        /* declared in <stdlib.h> */
        abort();
        while(1);
    }
  
  extern "C" void _ttywrch(int ch) {
		
		return ;
	}
}


//#pragma import(__use_no_semihosting)
////标准库需要的支持函数
//struct __FILE
//{
//	int handle;
//};
//FILE __stdout;
////定义_sys_exit()以避免使用半主机模式
//void _sys_exit(int x)
//{
//	x = x;
//}
////重定义 fputc 函数
//int fputc(int ch, FILE *f)
//{
//	while(USART_GetFlagStatus(USART1,USART_FLAG_TC)==RESET);
//	USART_SendData(USART1,(uint8_t)ch);
//	return ch;
//}
