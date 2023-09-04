#include <msp430.h>

#define LED1 BIT0
#define LED2 BIT1
#define LED3 BIT2
#define BUTTON BIT7

#define MIN_PRESSED_AMOUNT 7

int main(void)
{
    // отключаем сторожевой таймер
    WDTCTL = WDTPW | WDTHOLD;

    // устанавливаем пины LED1, LED2 и LED3 как выходы
    P1DIR |= LED1;
    P8DIR |= (LED2 | LED3);
    P8OUT &= ~(LED2 | LED3);

    // выключаем все светодиоды
    P1OUT &= ~LED1;
    P8OUT &= ~(LED2 | LED3);

    // устанавливаем пин BUTTON как вход
    P1DIR &= ~BUTTON;
    // включаем подтягивающий резистор на пине BUTTON
    P1REN |= BUTTON;
    // устанавливаем подтягивающий резистор на пине BUTTON
    P1OUT |= BUTTON;

    unsigned int led_num = 0;
    unsigned int button_check = 0;
    unsigned int i = 0;

    while(1)
    {
    	// цикл проверки на корректность нажатия
    	for (i = 0; i < 10; i++) {
    		// если кнопка нажата
    		if (!(P1IN & BUTTON)) {
    			button_check++;
    		}
    	}

        if (button_check >= MIN_PRESSED_AMOUNT)  // если кнопка нажата
        {
        	__delay_cycles(100000);
        	// если кнопка все еще нажата
            if (!(P1IN & BUTTON))
            {
            	// зажигаем LED1
                if (led_num == 0)
                {
                    P1OUT |= LED1;
                    led_num++;
                }
                // зажигаем LED2
                else if(led_num == 1)
                {
                    P8OUT |= LED2;
                    led_num++;
                }
                // зажигаем LED3
                else if(led_num == 2)
                {
                    P8OUT |= LED3;
                    led_num++;
                }
                // сбрасываем счетчик и гасим все лампы
                else if(led_num == 3)
                {
                    P1OUT &= ~LED1;
                    P8OUT &= ~(LED2 | LED3);
                    led_num = 0;
                }
            }
            // ждем, пока кнопка не отпущена
            while(!(P1IN & BUTTON));
            // обнуляем счетчики
            i = button_check = 0;
        }
    }
}