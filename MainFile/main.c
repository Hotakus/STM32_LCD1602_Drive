#include "stm32f10x_it.h"
#include "LCD1602.h"

unsigned char tem[] = {0x1C,0x14,0x1F,0x09,0x08,0x08,0x09,0x0F};	//���϶ȷ��š��桱����ģ

int main(){
	
	LCD_INIT();		//LCD1602��ʼ��
	WUserImg( 0, tem );	//д���û��Զ����ַ�
	
	LCD_WRITE_StrDATA( "HELLO",0,5 );		//����Ļ��һ�е��������HELLO
	LCD_WRITE_StrDATA( "W O R L D !",1,3 ); //����Ļ��һ�е��������W O R L D !
	
	LCD_WRITE_CMD( 0x80 );				//ָ����Ļ��һ�е�һ�����
	LCD_WRITE_ByteDATA( 0x00 );			//�����һ���û��Զ����ַ�
	
}
