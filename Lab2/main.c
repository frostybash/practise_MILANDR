#include <MDR32F9Qx_port.h>
#include <MDR32F9Qx_rst_clk.h>
#include <MDR32F9Qx_timer.h>

void initLED (void);
void buttonINIT (void);
void readButton_LED (void);
void NextLED (void);
void TimerInitialise (void);
void Timer1_IRQHandler (void);

#define delay(T) for (uint32_t i = T; i > 0; i--)

uint8_t cur_i;

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

void TimerInitialise (void)
{
	TIMER_CntInitTypeDef tim1INIT;
	RST_CLK_PCLKcmd (RST_CLK_PCLK_TIMER1, ENABLE);
	TIMER_BRGInit (MDR_TIMER1, TIMER_HCLKdiv1);
	TIMER_CntStructInit (&tim1INIT);
	tim1INIT.TIMER_Prescaler = 5000;
	tim1INIT.TIMER_Period = 200;
	TIMER_CntInit (MDR_TIMER1, &tim1INIT);
	
	NVIC_EnableIRQ (Timer1_IRQn);
	NVIC_SetPriority (Timer1_IRQn, 0);
	
	TIMER_ITConfig (MDR_TIMER1, TIMER_STATUS_CNT_ZERO, ENABLE);
	
	TIMER_Cmd (MDR_TIMER1, ENABLE);
}

void NextLED()
{
		switch(cur_i++ % 2) 
		{
			case 0:
				PORT_ResetBits(MDR_PORTC, PORT_Pin_1);
				PORT_SetBits(MDR_PORTC, PORT_Pin_0);
			break;
			case 1:
				PORT_ResetBits(MDR_PORTC, PORT_Pin_0);
				PORT_SetBits(MDR_PORTC, PORT_Pin_1);
			break;
		}
}

void Timer1_IRQHandler()
{
	if (TIMER_GetITStatus( MDR_TIMER1, TIMER_STATUS_CNT_ZERO))
	{
		NextLED();
		TIMER_ClearITPendingBit (MDR_TIMER1, TIMER_STATUS_CNT_ZERO);
		
	}
}



int main (void)
{
		initLED();
		TimerInitialise();
		cur_i=0;

	while (1)
	{
		//readButton_LED();
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
