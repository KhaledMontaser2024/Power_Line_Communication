#include "MCAL_DIO_Header.h"
#include "MGIE_header.h"
#include "MEXTI_header.h"
#include "MCAL_UART_Header.h"
#include "util/delay.h"

/*
dip switsh 1  from A0 to A3
LEDs 1 		  from A4 to A7

dip switch 2  from C7 to C4
LEDs 2 		  from C3 to C0
RGB 		  D7 D6
*/

U8 Tx_Byte = 0;

///// dip switches variables /////////
U8 Addr1 = 0; 
U8 Addr2 = 0;
U8 Addr3 = 0; 
U8 Addr4 = 0;

U8 Data1=0;
U8 Data2=0;
U8 Data3=0;
U8 Data4=0;
//////////////////////////////////////

U8 AddrBuffer = 0;		// to store Dip switches addr values 
U8 DataBuffer = 0;		// to store Dip switches data values
U8 MyAddress = 0x0F; 	// fixed address of tx
U8 AckData = 0; 		// my data
U8 Rx_Buffer = 0;
/////////////////


void TryReceive(void);
void BlinkYellow(void);
void BlinkGreen(void);
void DisplayData(U8 data);
void ISR_INT0(void);

int main ()
{
	// initializing ports directions
	Void_SetPinDir(PORTD,PIN6,OUTPUT); // Green led 
	Void_SetPinDir(PORTD,PIN7,OUTPUT); // Red Led
	Void_SetPinDir(PORTD,PIN1,OUTPUT); // Tx pn
	
    ///////////////////// pins of dip switch /////////////////////////////
	Void_SetPinDir(PORTA,PIN0,INPUT);
	Void_SetPinDir(PORTA,PIN1,INPUT); 
	Void_SetPinDir(PORTA,PIN2,INPUT); 
	Void_SetPinDir(PORTA,PIN3,INPUT);

	Void_SetPinDir(PORTC,PIN7,INPUT);
	Void_SetPinDir(PORTC,PIN6,INPUT); 
	Void_SetPinDir(PORTC,PIN5,INPUT); 
	Void_SetPinDir(PORTC,PIN4,INPUT);
    /////////////////////////////////////////////////////////////////////

	//////////////////////// dip switches Leds /////////////////////////
	Void_SetPinDir(PORTA,PIN4,OUTPUT);
	Void_SetPinDir(PORTA,PIN5,OUTPUT);
	Void_SetPinDir(PORTA,PIN6,OUTPUT);
	Void_SetPinDir(PORTA,PIN7,OUTPUT);

	Void_SetPinDir(PORTC,PIN3,OUTPUT);
	Void_SetPinDir(PORTC,PIN2,OUTPUT);
	Void_SetPinDir(PORTC,PIN1,OUTPUT);
	Void_SetPinDir(PORTC,PIN0,OUTPUT);
	///////////////////////////////////////////////////////////////////


	////////////////// INT 0 for Button For Tx ////////////////////////
	SetGlobalInteruputEnableBit(MGIE_ON);
	SetEXINTTriggState(INT0,MEXTI_FALLING_EDGE);
	SetEXINTFunction(INT0,&ISR_INT0);	
    ////////////////////////////////////////////////////////////

	Void_UARTConfig(UART_BR_9600BPS,UART_ASYNCHRONOUS,UART_8BIT_MODE,UART_ONE_STOP_BIT,UART_NO_PARITY);
	Void_UARTSetMode(UART_Transceiver_Mode);

	while (1)
	{	
		TryReceive();	
	}
	
	return 0;
}


void ISR_INT0(void)
{
	SetGlobalInteruputEnableBit(MGIE_OFF);	
	Void_SetPinValue(PORTD,PIN6,HIGH);       // Green LED 
	//Void_UARTFlushBuffer();
	Void_UARTSetMode(UART_Transmitter_Mode);
	
	Addr1 = U8_ReadPinValue(PORTA,PIN0);
	Addr2 = U8_ReadPinValue(PORTA,PIN1);
	Addr3 = U8_ReadPinValue(PORTA,PIN2);
	Addr4 = U8_ReadPinValue(PORTA,PIN3);

	Data1 = U8_ReadPinValue(PORTC,PIN7);
	Data2 = U8_ReadPinValue(PORTC,PIN6);
	Data3 = U8_ReadPinValue(PORTC,PIN5);
	Data4 = U8_ReadPinValue(PORTC,PIN4);


	switch (Addr1)
	{ 
	 	case 0:
			switch (Addr2)
			{
				case 0:
					switch (Addr3)
					{
						case 0:
							switch (Addr4)  
							{
								case 0:		//0b 0000
									Void_SetPinValue(PORTA,PIN4,LOW);
									Void_SetPinValue(PORTA,PIN5,LOW);
									Void_SetPinValue(PORTA,PIN6,LOW);
									Void_SetPinValue(PORTA,PIN7,LOW);
								break;

								case 1:		//0b 1000
									Void_SetPinValue(PORTA,PIN4,LOW);
									Void_SetPinValue(PORTA,PIN5,LOW);
									Void_SetPinValue(PORTA,PIN6,LOW);
									Void_SetPinValue(PORTA,PIN7,HIGH);
								break;

								default:
								break;
							
							}

						break;	

						case 1:
							switch (Addr4)
							{
								case 0:		//0b 0100
									Void_SetPinValue(PORTA,PIN4,LOW);
									Void_SetPinValue(PORTA,PIN5,LOW);
									Void_SetPinValue(PORTA,PIN6,HIGH);
									Void_SetPinValue(PORTA,PIN7,LOW);
								break;

								case 1:		//0b 1100
									Void_SetPinValue(PORTA,PIN4,LOW);
									Void_SetPinValue(PORTA,PIN5,LOW);
									Void_SetPinValue(PORTA,PIN6,HIGH);
									Void_SetPinValue(PORTA,PIN7,HIGH);
								break;

								default:
								break;
							}

						break;

						default:
						break;			   
					
					}

				break;
				
				case 1:
				
					switch (Addr3)
					{
						case 0:
							switch (Addr4)
							{
								case 0:			//0b 0010
									Void_SetPinValue(PORTA,PIN4,LOW);
									Void_SetPinValue(PORTA,PIN5,HIGH);
									Void_SetPinValue(PORTA,PIN6,LOW);
									Void_SetPinValue(PORTA,PIN7,LOW);
								break;

								case 1:			//0b 1010
									Void_SetPinValue(PORTA,PIN4,LOW);
									Void_SetPinValue(PORTA,PIN5,HIGH);
									Void_SetPinValue(PORTA,PIN6,LOW);
									Void_SetPinValue(PORTA,PIN7,HIGH);
								break;

								default:
								break;
							}

						break;		

						case 1:
							switch (Addr4)
							{
								case 0:		//0b 0110
									Void_SetPinValue(PORTA,PIN4,LOW);
									Void_SetPinValue(PORTA,PIN5,HIGH);
									Void_SetPinValue(PORTA,PIN6,HIGH);
									Void_SetPinValue(PORTA,PIN7,LOW);
								break;

								case 1:		//0b 1110
									Void_SetPinValue(PORTA,PIN4,LOW);
									Void_SetPinValue(PORTA,PIN5,HIGH);
									Void_SetPinValue(PORTA,PIN6,HIGH);
									Void_SetPinValue(PORTA,PIN7,HIGH);
								break;

								default:
								break;
							}

						break;
					
						default:
						break;			   
					
					}				
				break;
				
				
				
				default:
				break;
					
			}
		break;


		case 1:
			switch (Addr2)
			{
			 	case 0:
					switch (Addr3)
					{
						case 0:
							switch (Addr4)
							{
								case 0:		//0b 0001
									Void_SetPinValue(PORTA,PIN4,HIGH);
									Void_SetPinValue(PORTA,PIN5,LOW);
									Void_SetPinValue(PORTA,PIN6,LOW);
									Void_SetPinValue(PORTA,PIN7,LOW);
								break;

								case 1:		//0b 1001
									Void_SetPinValue(PORTA,PIN4,HIGH);
									Void_SetPinValue(PORTA,PIN5,LOW);
									Void_SetPinValue(PORTA,PIN6,LOW);
									Void_SetPinValue(PORTA,PIN7,HIGH);
								break;

								default:
								break;
							}

						break;	

						case 1:
							switch (Addr4)
							{
								case 0:		//0b 0101
									Void_SetPinValue(PORTA,PIN4,HIGH);
									Void_SetPinValue(PORTA,PIN5,LOW);
									Void_SetPinValue(PORTA,PIN6,HIGH);
									Void_SetPinValue(PORTA,PIN7,LOW);
								break;

								case 1:		//0b 1101
									Void_SetPinValue(PORTA,PIN4,HIGH);
									Void_SetPinValue(PORTA,PIN5,LOW);
									Void_SetPinValue(PORTA,PIN6,HIGH);
									Void_SetPinValue(PORTA,PIN7,HIGH);
								break;

								default:
								break;
							}
						break;

						default:
						break;			   
					
					}

				break;

			 	case 1:
					switch (Addr3)
					{
						case 0:
							switch (Addr4)
							{
								case 0:		//0b 0011
									Void_SetPinValue(PORTA,PIN4,HIGH);
									Void_SetPinValue(PORTA,PIN5,HIGH);
									Void_SetPinValue(PORTA,PIN6,LOW);
									Void_SetPinValue(PORTA,PIN7,LOW);
								break;

								case 1:		//0b 1011
									Void_SetPinValue(PORTA,PIN4,HIGH);
									Void_SetPinValue(PORTA,PIN5,HIGH);
									Void_SetPinValue(PORTA,PIN6,LOW);
									Void_SetPinValue(PORTA,PIN7,HIGH);
								break;

								default:
								break;
							}
						break;		

						case 1:
							switch (Addr4)
							{
								case 0:		//0b 0111
									Void_SetPinValue(PORTA,PIN4,HIGH);
									Void_SetPinValue(PORTA,PIN5,HIGH);
									Void_SetPinValue(PORTA,PIN6,HIGH);
									Void_SetPinValue(PORTA,PIN7,LOW);
								break;

								case 1:		//0b 1111
									Void_SetPinValue(PORTA,PIN4,HIGH);
									Void_SetPinValue(PORTA,PIN5,HIGH);
									Void_SetPinValue(PORTA,PIN6,HIGH);
									Void_SetPinValue(PORTA,PIN7,HIGH);
								break;

								default:
								break;
							}

						break;
					
						default:
						break;			   
					
					}				
				break;
			
				default:
				break;

			}
		break;
			
		default:
		break;

	}


	AddrBuffer = (Addr1<<0) | (Addr2<<1) | (Addr3<<2) | (Addr4<<3);
	DataBuffer = (Data1<<0) | (Data2<<1) | (Data3<<2) | (Data4<<3);
	Tx_Byte= (AddrBuffer << 4) | (DataBuffer);
	Void_UARTWriteFrame(Tx_Byte);	

	//Void_UARTFlushBuffer();	
	Void_UARTSetMode(UART_Receiver_Mode);
	Void_SetPinValue(PORTD,PIN6,LOW); 		//Green Led
	SetGlobalInteruputEnableBit(MGIE_ON);
}




void TryReceive(void)
{	
	Void_SetPinValue(PORTD,PIN1,HIGH); //Tx	pin
	Rx_Buffer = U16_UARTReadFrame();
	BlinkGreen();
	
	AckData = Rx_Buffer & 0x0F;		
	if( MyAddress  == (Rx_Buffer >>4) )		
	{ 
		BlinkYellow();
		DisplayData(AckData);
		//Void_UARTFlushBuffer();	
		//Void_UARTSetMode(UART_Receiver_Mode);
	}
	return ;
}

void BlinkYellow(void)
{
	Void_SetPinValue(PORTD,PIN6,HIGH); //Green led
	Void_SetPinValue(PORTD,PIN7,HIGH); //Red led
	_delay_ms(50);
	Void_SetPinValue(PORTD,PIN6,LOW); //Green led
	Void_SetPinValue(PORTD,PIN7,LOW); //Red led
	_delay_ms(50);
return;
}

void BlinkGreen(void)
{
	Void_SetPinValue(PORTD,PIN6,HIGH); //Green led
	Void_SetPinValue(PORTD,PIN7,LOW); //Red led
	_delay_ms(50);
	Void_SetPinValue(PORTD,PIN6,LOW); //Green led
	Void_SetPinValue(PORTD,PIN7,LOW); //Red led
	_delay_ms(50);
return;
}

void DisplayData(U8 data)
{
	U8 AckData0 = (1 & (data >> 0));
	U8 AckData1 = (1 & (data >> 1));
	U8 AckData2 = (1 & (data >> 2));
	U8 AckData3 = (1 & (data >> 3));

	
	switch (AckData0)
	{ 
	 	case 0:
			switch (AckData1)
			{
				case 0:
					switch (AckData2)
					{
						case 0:
							switch (AckData3)  
							{
								case 0:		//0b 0000
									Void_SetPinValue(PORTC,PIN3,LOW);
									Void_SetPinValue(PORTC,PIN2,LOW);
									Void_SetPinValue(PORTC,PIN1,LOW);
									Void_SetPinValue(PORTC,PIN0,LOW);
								break;

								case 1:		//0b 1000
									Void_SetPinValue(PORTC,PIN3,LOW);
									Void_SetPinValue(PORTC,PIN2,LOW);
									Void_SetPinValue(PORTC,PIN1,LOW);
									Void_SetPinValue(PORTC,PIN0,HIGH);
								break;

								default:
								break;
							
							}

						break;	

						case 1:
							switch (AckData3)
							{
								case 0:		//0b 0100
									Void_SetPinValue(PORTC,PIN3,LOW);
									Void_SetPinValue(PORTC,PIN2,LOW);
									Void_SetPinValue(PORTC,PIN1,HIGH);
									Void_SetPinValue(PORTC,PIN0,LOW);
								break;

								case 1:		//0b 1100
									Void_SetPinValue(PORTC,PIN3,LOW);
									Void_SetPinValue(PORTC,PIN2,LOW);
									Void_SetPinValue(PORTC,PIN1,HIGH);
									Void_SetPinValue(PORTC,PIN0,HIGH);
								break;

								default:
								break;
							}

						break;

						default:
						break;			   
					
					}

				break;
				
				case 1:
				
					switch (AckData2)
					{
						case 0:
							switch (AckData3)
							{
								case 0:			//0b 0010
									Void_SetPinValue(PORTC,PIN3,LOW);
									Void_SetPinValue(PORTC,PIN2,HIGH);
									Void_SetPinValue(PORTC,PIN1,LOW);
									Void_SetPinValue(PORTC,PIN0,LOW);
								break;

								case 1:			//0b 1010
									Void_SetPinValue(PORTC,PIN3,LOW);
									Void_SetPinValue(PORTC,PIN2,HIGH);
									Void_SetPinValue(PORTC,PIN1,LOW);
									Void_SetPinValue(PORTC,PIN0,HIGH);
								break;

								default:
								break;
							}

						break;		

						case 1:
							switch (AckData3)
							{
								case 0:		//0b 0110
									Void_SetPinValue(PORTC,PIN3,LOW);
									Void_SetPinValue(PORTC,PIN2,HIGH);
									Void_SetPinValue(PORTC,PIN1,HIGH);
									Void_SetPinValue(PORTC,PIN0,LOW);
								break;

								case 1:		//0b 1110
									Void_SetPinValue(PORTC,PIN3,LOW);
									Void_SetPinValue(PORTC,PIN2,HIGH);
									Void_SetPinValue(PORTC,PIN1,HIGH);
									Void_SetPinValue(PORTC,PIN0,HIGH);
								break;

								default:
								break;
							}

						break;
					
						default:
						break;			   
					
					}				
				break;
				
				
				
				default:
				break;
					
			}
		break;


		case 1:
			switch (AckData1)
			{
			 	case 0:
					switch (AckData2)
					{
						case 0:
							switch (AckData3)
							{
								case 0:		//0b 0001
									Void_SetPinValue(PORTC,PIN3,HIGH);
									Void_SetPinValue(PORTC,PIN2,LOW);
									Void_SetPinValue(PORTC,PIN1,LOW);
									Void_SetPinValue(PORTC,PIN0,LOW);
								break;

								case 1:		//0b 1001
									Void_SetPinValue(PORTC,PIN3,HIGH);
									Void_SetPinValue(PORTC,PIN2,LOW);
									Void_SetPinValue(PORTC,PIN1,LOW);
									Void_SetPinValue(PORTC,PIN0,HIGH);
								break;

								default:
								break;
							}

						break;	

						case 1:
							switch (AckData3)
							{
								case 0:		//0b 0101
									Void_SetPinValue(PORTC,PIN3,HIGH);
									Void_SetPinValue(PORTC,PIN2,LOW);
									Void_SetPinValue(PORTC,PIN1,HIGH);
									Void_SetPinValue(PORTC,PIN0,LOW);
								break;

								case 1:		//0b 1101
									Void_SetPinValue(PORTC,PIN3,HIGH);
									Void_SetPinValue(PORTC,PIN2,LOW);
									Void_SetPinValue(PORTC,PIN1,HIGH);
									Void_SetPinValue(PORTC,PIN0,HIGH);
								break;

								default:
								break;
							}
						break;

						default:
						break;			   
					
					}

				break;

			 	case 1:
					switch (AckData2)
					{
						case 0:
							switch (AckData3)
							{
								case 0:		//0b 0011
									Void_SetPinValue(PORTC,PIN3,HIGH);
									Void_SetPinValue(PORTC,PIN2,HIGH);
									Void_SetPinValue(PORTC,PIN1,LOW);
									Void_SetPinValue(PORTC,PIN0,LOW);
								break;

								case 1:		//0b 1011
									Void_SetPinValue(PORTC,PIN3,HIGH);
									Void_SetPinValue(PORTC,PIN2,HIGH);
									Void_SetPinValue(PORTC,PIN1,LOW);
									Void_SetPinValue(PORTC,PIN0,HIGH);
								break;

								default:
								break;
							}
						break;		

						case 1:
							switch (AckData3)
							{
								case 0:		//0b 0111
									Void_SetPinValue(PORTC,PIN3,HIGH);
									Void_SetPinValue(PORTC,PIN2,HIGH);
									Void_SetPinValue(PORTC,PIN1,HIGH);
									Void_SetPinValue(PORTC,PIN0,LOW);
								break;

								case 1:		//0b 1111
									Void_SetPinValue(PORTC,PIN3,HIGH);
									Void_SetPinValue(PORTC,PIN2,HIGH);
									Void_SetPinValue(PORTC,PIN1,HIGH);
									Void_SetPinValue(PORTC,PIN0,HIGH);
								break;

								default:
								break;
							}

						break;
					
						default:
						break;			   
					
					}				
				break;
			
				default:
				break;

			}
		break;
			
		default:
		break;

	}



return;
}