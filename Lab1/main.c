#include <MDR32F9Qx_port.h>
#include <MDR32F9Qx_rst_clk.h>

void initLED (void);
void buttonINIT (void);
void readButton_LED (void);

#define delay(T) for (uint32_t i = T; i > 0; i--)


void initLED (void)
{
		PORT_InitTypeDef initialisePORTC;
		RST_CLK_PCLKcmd(RST_CLK_PCLK_PORTC, ENABLE);
		initialisePORTC.PORT_Pin = PORT_Pin_0 | PORT_Pin_1;
		initialisePORTC.PORT_OE = PORT_OE_OUT;
		initialisePORTC.PORT_PULL_UP = PORT_PULL_UP_ON;
		initialisePORTC.PORT_FUNC = PORT_FUNC_PORT; 
		initialisePORTC.PORT_MODE = PORT_MODE_DIGITAL; 
		initialisePORTC.PORT_SPEED = PORT_SPEED_SLOW;
		PORT_Init(MDR_PORTC, &initialisePORTC);
}

void buttonINIT (void)
{
		PORT_InitTypeDef initialisePORTB;
		RST_CLK_PCLKcmd(RST_CLK_PCLK_PORTB, ENABLE);
		initialisePORTB.PORT_Pin = PORT_Pin_5;
		initialisePORTB.PORT_OE = PORT_OE_IN;
		initialisePORTB.PORT_PULL_UP = PORT_PULL_UP_ON;
		initialisePORTB.PORT_FUNC = PORT_FUNC_PORT; 
		initialisePORTB.PORT_MODE = PORT_MODE_DIGITAL; 
		initialisePORTB.PORT_SPEED = PORT_SPEED_SLOW;
		PORT_Init(MDR_PORTB, &initialisePORTB);
}

void readButton_LED (void)
{

		if ( ! PORT_ReadInputDataBit ( MDR_PORTB, PORT_Pin_5))
		{	
			PORT_SetBits(MDR_PORTC, PORT_Pin_0);
			while ( ! PORT_ReadInputDataBit ( MDR_PORTB, PORT_Pin_5)){}
		}
		else PORT_ResetBits(MDR_PORTC, PORT_Pin_0);
}


int main (void)
{
		buttonINIT();
		initLED();
	while (1)
	{
		readButton_LED();
	}
	/*while (1)
	{
		PORT_SetBits(MDR_PORTC, PORT_Pin_0); 
		delay(50000);
		PORT_ResetBits(MDR_PORTC, PORT_Pin_0);
		PORT_SetBits(MDR_PORTC, PORT_Pin_1);                 Uncomment this for  working LEDs
		delay(50000);
		PORT_ResetBits(MDR_PORTC, PORT_Pin_1);
		delay(50000);
	}*/

}
