#include "stm32f10x_it.h"
#include "LCD1602.h"

unsigned char tem[] = {0x1C,0x14,0x1F,0x09,0x08,0x08,0x09,0x0F};	//摄氏度符号“℃”的字模

int main(){
	
	LCD_INIT();		//LCD1602初始化
	WUserImg( 0, tem );	//写入用户自定义字符
	
	LCD_WRITE_StrDATA( "HELLO",0,5 );		//向屏幕第一行第五列输出HELLO
	LCD_WRITE_StrDATA( "W O R L D !",1,3 ); //向屏幕第一行第三列输出W O R L D !
	
	LCD_WRITE_CMD( 0x80 );				//指定屏幕第一行第一列输出
	LCD_WRITE_ByteDATA( 0x00 );			//输出第一个用户自定义字符
	
}
