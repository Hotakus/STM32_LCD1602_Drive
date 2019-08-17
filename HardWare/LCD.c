#include "LCD1602.h"
#include "stm32f10x_rcc.h"

void delay_us(unsigned int us){
	unsigned int  i;
	
	do{
		i = 10;
		while(i--) __nop();
	} while (--us);

}
/***********************************GPIO��ʼ��********************************************/
void GPIO_INIT(void){		//GPIO��ʼ��
	GPIO_InitTypeDef PB;
	GPIO_InitTypeDef PA;
	
	GPIO_PinRemapConfig(GPIO_Remap_SWJ_JTAGDisable, ENABLE);	//����jtag����Ȼд�����ͳ���ִ�ж�����Ӱ��
	RCC_APB2PeriphClockCmd( RCC_APB2Periph_GPIOA, ENABLE );
	RCC_APB2PeriphClockCmd( RCC_APB2Periph_GPIOB, ENABLE );		//��GPIOA~C
	RCC_APB2PeriphClockCmd( RCC_APB2Periph_GPIOC, ENABLE );
	
	PB.GPIO_Pin = EN|RW|RS;
	PB.GPIO_Mode = GPIO_Mode_Out_PP;
	PB.GPIO_Speed = GPIO_Speed_50MHz;
	GPIO_Init(GPIOB, &PB);
	
	PA.GPIO_Pin = GPIO_Pin_0|GPIO_Pin_1|GPIO_Pin_2|
				  GPIO_Pin_3|GPIO_Pin_4|GPIO_Pin_5|
				  GPIO_Pin_6|GPIO_Pin_7;
	PA.GPIO_Mode = GPIO_Mode_Out_PP;
	PA.GPIO_Speed = GPIO_Speed_50MHz;
	GPIO_Init(GPIOA, &PA);
	
}
/***********************************GPIO��ʼ��********************************************/

/***********************************LCD��ʼ��********************************************/
void LCD_INIT(void){	//��ʼ��
	GPIO_INIT();	//GPIO�ĳ�ʼ������LCD_INIT������ʱ�Զ�����
	
	GPIO_Write( GPIOA, 0x0000 );		//��ʼ��GPIOA��GOIOB��ֵΪ0x0000�����Բ��ã�����ϰ��
	GPIO_Write( GPIOB, 0x0000 );
	
	delay_us(500);
	
	LCD_WRITE_CMD( 0x38 );
	LCD_WRITE_CMD( 0x0d );	//����������˸
	LCD_WRITE_CMD( 0x06 );
	LCD_WRITE_CMD( 0x01 );
}
/***********************************LCD��ʼ��********************************************/

/***********************************д�������********************************************/
void LCD_WRITE_CMD( unsigned char CMD ){		//д�������
	ReadBusy();
	GPIO_ResetBits( GPIOB, RS );
	GPIO_ResetBits( GPIOB, RW );
	GPIO_ResetBits( GPIOB, EN );
	GPIO_Write( GPIOA, CMD );		//
	GPIO_SetBits( GPIOB, EN );
	GPIO_ResetBits( GPIOB, EN );
}
/***********************************д�������********************************************/

/***********************************д�뵥��Byte����********************************************/
void LCD_WRITE_ByteDATA( unsigned char ByteData ){	//д�뵥��Byte����
	ReadBusy();
	GPIO_SetBits( GPIOB, RS );
	GPIO_ResetBits( GPIOB, RW );
	GPIO_ResetBits( GPIOB, EN );
	GPIO_Write( GPIOA, ByteData );
	GPIO_SetBits( GPIOB, EN );
	GPIO_ResetBits( GPIOB, EN );
}

/***********************************д�뵥��Byte����********************************************/

/***********************************д���ַ�������********************************************/

void LCD_WRITE_StrDATA( unsigned char *StrData, unsigned char row, unsigned char col ){//д���ַ���
	unsigned char baseAddr = 0x00;			//����256λ��ַ
	if ( row ){
		baseAddr = 0xc0;
	}else{
		baseAddr = 0x80;																				   
	} 	//rowΪ1�û�ѡ��ڶ���
		//rowΪ0�û�ѡ���һ��
	baseAddr += col;

	while ( *StrData != '\0' ){

		LCD_WRITE_CMD( baseAddr );
		LCD_WRITE_ByteDATA( *StrData );
	
		baseAddr++;			   //ÿ��ѭ����ַ��һ������ָ���һ
		StrData++;
	}
}
/***********************************д���ַ�������********************************************/

/***********************************��æ����********************************************/
void ReadBusy(void){		//��æ��������æ֮ǰ�ǵø������ŵĹ�����ʽ��������ΪSTM32��IO����׼˫��IO
	GPIO_Write( GPIOA, 0x00ff );	
	
	GPIO_InitTypeDef p;
	p.GPIO_Pin = GPIO_Pin_7;		//ѡ��GPIOA�ĵ���Pin
	p.GPIO_Mode = GPIO_Mode_IN_FLOATING;	//����Pin�Ĺ�����ʽΪ��������ģʽ�����ڼ��LCD1602��æ״̬
	p.GPIO_Speed = GPIO_Speed_50MHz;
	GPIO_Init( GPIOA, &p );
	
	GPIO_ResetBits( GPIOB, RS );//RS����
	GPIO_SetBits( GPIOB, RW );//RW����
	
	GPIO_SetBits( GPIOB, EN );	//ʹ�ܿ�
	while( GPIO_ReadInputDataBit( GPIOA, GPIO_Pin_7 ) );	//������Pin״̬�����һֱΪ1��ѭ���ȴ�
	GPIO_ResetBits( GPIOB, EN );//ʹ�ܹ�
	/*do{
		GPIO_ResetBits( GPIOB, EN );
		GPIO_SetBits( GPIOB, EN );
		Flag = GPIO_ReadInputDataBit( GPIOA, GPIO_Pin_7 ) & BUSY;
		GPIO_ResetBits( GPIOB, EN );
	}while( Flag );*/
	
	p.GPIO_Pin = GPIO_Pin_0|GPIO_Pin_1|GPIO_Pin_2|
				  GPIO_Pin_3|GPIO_Pin_4|GPIO_Pin_5|
				  GPIO_Pin_6|GPIO_Pin_7;		//ʹGPIOA��״̬��ԭ������ģʽ
	p.GPIO_Mode = GPIO_Mode_Out_PP;
	p.GPIO_Speed = GPIO_Speed_50MHz;
	GPIO_Init( GPIOA, &p  );
}
/***********************************��æ����********************************************/

/***********************************д���û��Զ���ͼ��********************************************/
void WUserImg(unsigned char pos,unsigned char *ImgInfo){ //д���û��Զ���ͼ��
	unsigned char cgramAddr;			//CGRAM���û��Զ����ַ�λ
	
	if( pos <= 1 ) cgramAddr = 0x40;		// 
	if( pos > 1 && pos <= 3 ) cgramAddr = 0x50;
	if( pos > 3 && pos <= 5 ) cgramAddr = 0x60;
	if( pos > 5 && pos <= 7 ) cgramAddr = 0x70;

	LCD_WRITE_CMD( (cgramAddr + (pos%2) * 8) );	//ָ����ģд��ĵ�ַ��һ���0x40��ʼ��0x78����
	
	while( *ImgInfo != '\0' ){		//ѭ��д��tem����,���û�ȡģ������
		LCD_WRITE_ByteDATA( *ImgInfo );
		ImgInfo++;
	}
}
/***********************************д���û��Զ���ͼ��********************************************/
