/*
 * INPUT_CAPTURE.c
 *
 * Created: 8/9/2023 1:28:51 AM
 * Author : Govind
 */ 

#include <avr/io.h>
#include <util/delay.h>
#include <avr/interrupt.h>


void lcd_command(unsigned char cmd)
{
	PORTA = cmd;
	PORTB &= ~(1<<PB0);
	PORTB &= ~(1<<PB1);
	PORTB |=  (1<<PB2);
	_delay_ms(10);
	PORTB &= ~(1<<PB2);
}

void lcd_data(unsigned char data)
{
	PORTA = data;
	PORTB |=  (1<<PB0);
	PORTB &= ~(1<<PB1);
	PORTB |= (1<<PB2);
	_delay_ms(10);
	PORTB &= ~(1<<PB2);
}

void lcd_string(unsigned char *str,unsigned char length)
{
	for(char i=0;i<length;i++)
	{
		lcd_data(str[i]);
	}
}
	
void lcd_intialise()
{
	lcd_command(0x38);
	lcd_command(0x01);
	lcd_command(0x06);
	lcd_command(0x0C);
}

unsigned int a,b,c,d,e,f,first_digit,second_digit,third_digit,fourth_digit,fifth_digit;
unsigned int capture;

int main(void)
{
    DDRA = 0XFF;
	DDRB |= (1<<DDB0)|(1<<DDB1)|(1<<DDB2);
    lcd_intialise();	
    
	sei();  //enable global interrupt bit
	TIMSK |= (1<<TICIE1); //ENABLE REQUIRED INPUT INTERRUPT
	TCCR1A &= ~(1<<WGM10) & ~(1<<WGM11);
	TCCR1B &= ~(1<<WGM12) & ~(1<<WGM13);
	TCCR1B |= (1<<CS10)|(1<<CS12);  //SET PRESCALER 
	TCCR1B &= ~(1<<CS11);
	TCNT1 = 0;  //SET THE TIMER WITH INTIAL VALUE
	TCCR1B |= (1<<ICES1);  //SET EDGE OF THE CAPTURE
	
	
	lcd_intialise();
	
    while (1) 
    {
		lcd_command(0x80);
		lcd_string("INPUT CAPTURE :",16);
		lcd_command(0xC5);

		a = capture/10;
		fifth_digit = capture % 10;
		b = a/10;
		fourth_digit = a % 10;
		c = b/10;
		third_digit = b % 10;
		d = c/10;
		second_digit = c % 10;
		e = d/10;
		first_digit = d % 10;
		f = e/10;
		
		lcd_data(first_digit + 0x30);
		lcd_data(second_digit + 0x30);
		lcd_data(third_digit + 0x30);
                lcd_data(fourth_digit + 0x30);
                lcd_data(fifth_digit + 0x30);
		lcd_data(first_digit + 0x30);
    }
}


ISR(TIMER1_CAPT_vect)
{
	capture = ICR1;
	
}
