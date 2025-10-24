#include "stm32f1xx.h"
#include <string.h>

void USART1_Init(void);
void USART1_SendChar(char c);
void USART1_SendString(char *str);
char USART1_GetChar(void);
void LED_Init(void);
void delay(int t);

int main(void)
{
    USART1_Init();
    LED_Init();

    USART1_SendString("UART Ready\r\n");

    while(1)
    {
        char c = USART1_GetChar();  // Receive char
        USART1_SendChar(c);         // Echo back

        if(c == '1')
        {
        	GPIOA->ODR |= (1 << 0);   // LED ON
        	USART1_SendString("\r\nLED is ON\r\n");      // Store received char
        }
        else if(c == '0')
        {
            GPIOA->ODR &= ~(1 << 0);  // LED OFF
            USART1_SendString("\r\nLED is OFF\r\n");
        }
    }
}

//---------------- USART ----------------
void USART1_Init(void)
{
    RCC->APB2ENR |= RCC_APB2ENR_IOPAEN | RCC_APB2ENR_USART1EN;

    // PA9 TX - AF Push Pull
    GPIOA->CRH &= ~(0xF << 4);
    GPIOA->CRH |=  (0xB << 4);

    // PA10 RX - Floating input
    GPIOA->CRH &= ~(0xF << 8);
    GPIOA->CRH |=  (0x4 << 8);

    USART1->BRR = 0x341;   // 9600 bps @ 8MHz HSI
    USART1->CR1 |= (1 << 13) | (1 << 3) | (1 << 2);
}

void USART1_SendChar(char c)
{
    while(!(USART1->SR & (1 << 7)));
    USART1->DR = c;
}

void USART1_SendString(char *str)
{
    while(*str) USART1_SendChar(*str++);
}

char USART1_GetChar(void)
{
    while(!(USART1->SR & (1 << 5)));
    return USART1->DR;
}

//---------------- LED Init ----------------
void LED_Init(void)
{
    RCC->APB2ENR |= RCC_APB2ENR_IOPAEN;  // GPIOA clock
    GPIOA->CRL &= ~(0xF << (0 * 4));     // Clear PA0
    GPIOA->CRL |=  (0x1 << (0 * 4));     // MODE=01 (Output 10MHz), CNF=00 (Push-Pull)
}

//---------------- Delay ----------------
void delay(int t)
{
    for(int i=0; i<800*t; i++){
    	__NOP();
    }
}
