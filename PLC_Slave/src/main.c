#include "MCAL_DIO_Header.h"
#include "MGIE_header.h"
#include "MEXTI_header.h"
#include "MCAL_UART_Header.h"
#include "util/delay.h"


void TryReceive(void);				// main receiving Fn 
void SendToMaster(U8 data);
void ISR_INT0(void);
///// LED Indication Functions //////
void BlinkYellow(void);
void BlinkGreen(void);
void DisplayData(U8 data);		 
///////////////////////////////////////

U8 MasterAddr = 0x0F; // go to addr
U8 AckData = 0; 	// ack to send to transmitter
U8 FrameToSend = 0;    // byte sent to transmitter for ACK
U8 Tx_Byte =0;

U8 MyAddr = 0x01; 	// my addr (change from module to module) 
U8 Rx_Buffer = 0;  	// received byte Buffer 
U8 ReceivedData = 0;		//store receive data		

int main ()
{
	

	Void_SetPinDir(PORTA,PIN0,OUTPUT); //Green led
	Void_SetPinDir(PORTC,PIN0,OUTPUT); //Red led


	/////// ReceivedData received indications ////////////
	Void_SetPinDir(PORTA,PIN4,OUTPUT);
	Void_SetPinDir(PORTA,PIN5,OUTPUT); 
	Void_SetPinDir(PORTA,PIN6,OUTPUT); 
	Void_SetPinDir(PORTA,PIN7,OUTPUT); 
	//////////////////////////////////////////////



	////////////////// INT 0 for Button For Tx ////////////////////////
	SetGlobalInteruputEnableBit(MGIE_ON);
	SetEXINTTriggState(INT0,MEXTI_FALLING_EDGE);
	SetEXINTFunction(INT0,&ISR_INT0);	
    ////////////////////////////////////////////////////////////

	Void_SetPinDir(PORTD,PIN1,OUTPUT); // Tx pin
    Void_UARTConfig(UART_BR_9600BPS,UART_ASYNCHRONOUS,UART_8BIT_MODE,UART_ONE_STOP_BIT,UART_NO_PARITY);
    // Void_UARTSetMode(UART_Transceiver_Mode);
	//Void_SetPinValue(PORTD,PIN1,HIGH); //Tx

	
	

	while (1)
	{
		Void_SetPinValue(PORTA,PIN0,LOW);  //Green Led
		Void_SetPinValue(PORTC,PIN0,HIGH); //Red led
		TryReceive();
	}
	return 0;
}

void ISR_INT0(void)
{
	Void_UARTSetMode(UART_Transmitter_Mode);
	SetGlobalInteruputEnableBit(MGIE_OFF);	
	Void_SetPinValue(PORTA,PIN0,HIGH);       // Green LED
	Void_SetPinValue(PORTC,PIN0,LOW); 		 //Red led

	DisplayData(0);
	//Void_UARTFlushBuffer();
	SendToMaster(MyAddr);
	
	//Void_UARTFlushBuffer();	
	Void_SetPinValue(PORTA,PIN0,LOW); 		//Green Led
	Void_SetPinValue(PORTC,PIN0,HIGH); 		//Green Led
	Void_UARTSetMode(UART_Receiver_Mode);
	SetGlobalInteruputEnableBit(MGIE_ON);
}



void TryReceive(void)
{	
	Void_UARTSetMode(UART_Receiver_Mode);
	
	Rx_Buffer = U16_UARTReadFrame(); 		// receiving Frame		

	if( MyAddr == ((Rx_Buffer >> 4)& 0x0F))  
	{
		ReceivedData = Rx_Buffer & 0x0F;		// extracting data 
		DisplayData(ReceivedData); 			// displaying received data on leds 
		AckData = ReceivedData;
		BlinkGreen();						// indicates that frame is arrived correctly  
		Void_UARTSetMode(UART_Transmitter_Mode);
		SendToMaster(AckData);
	}

	return ;
}

void BlinkYellow(void)
{
	Void_SetPinValue(PORTA,PIN0,HIGH); //Green led
	Void_SetPinValue(PORTC,PIN0,HIGH); //Red led
	_delay_ms(50);
	Void_SetPinValue(PORTA,PIN0,LOW); //Green led
	Void_SetPinValue(PORTC,PIN0,LOW); //Red led
	_delay_ms(50);
return;
}


void BlinkGreen(void)
{
	Void_SetPinValue(PORTA,PIN0,HIGH); //Green led
	Void_SetPinValue(PORTC,PIN0,LOW); //Red led
	_delay_ms(50);
	Void_SetPinValue(PORTA,PIN0,LOW); //Green led
	Void_SetPinValue(PORTC,PIN0,LOW); //Red led
	_delay_ms(50);
return;
}


void SendToMaster(U8 data)
{
	//Void_UARTFlushBuffer();
    //Void_UARTSetMode(UART_Transmitter_Mode);
	FrameToSend = (MasterAddr<<4) | data;
	Void_UARTWriteFrame(FrameToSend);
	return;
}


void DisplayData(U8 data)
{
	U8 Data0 = (1 & (data >> 0));
	U8 Data1 = (1 & (data >> 1));
	U8 Data2 = (1 & (data >> 2));
	U8 Data3 = (1 & (data >> 3));

	
	switch (Data0)
	{ 
	 	case 0:
			switch (Data1)
			{
				case 0:
					switch (Data2)
					{
						case 0:
							switch (Data3)  
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
							switch (Data3)
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
				
					switch (Data2)
					{
						case 0:
							switch (Data3)
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
							switch (Data3)
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
			switch (Data1)
			{
			 	case 0:
					switch (Data2)
					{
						case 0:
							switch (Data3)
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
							switch (Data3)
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
					switch (Data2)
					{
						case 0:
							switch (Data3)
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
							switch (Data3)
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

return;
}