/*
 * Displays con LM35.c
 *
 * Created: 20/04/2018 09:46:24 p. m.
 * Author : dx_ch
 */ 

#include <avr/io.h>
#include <avr/interrupt.h>

unsigned char TurnoDisplay =0;
unsigned char UM, Cent, Dec, Uni;
unsigned char DigitosDisplay [10]= {0xc0, 0xf9,0xa4,0xb0,0x99,0x92,0x82,0xf8,0x80,0x90};//,0x88,0x83,0xc6,0xa1,0x86,0x8e};
unsigned int temp, temp1;
unsigned char CalculaDigitos=0;
unsigned int ValorADC;
unsigned int Valor;


int main(void)
{
	DDRB= 0x0f;	//Habilita puerto b como salida para transistores de displays
	PORTB= 0x0f;//Apaga displays
	
	DDRD= 0xFF;//puerto d como salida para datos displays
	PORTD= 0xFF;//todos loa digitos apagados
	
	//Configuracion del timer1 cada 1ms con int de sobreflujo
	TCNT1 = 0xc180;//Contador del timer para intervalo de 1ms
	TCCR1B = 0x01;
	TIMSK1= 0x01;
	
	ADMUX=0;
	ADCSRA=0x8D;
	ADCSRA|=1<<ADSC;

	sei();
    /* Replace with your application code */
    while (1) 
    {
	
    }
}

ISR(TIMER1_OVF_vect)
{
	TCNT1=0xc180;	//Carga tcnt para que entre cada 1mSeg
	ADCSRA|=1<<ADSC;
}

ISR(ADC_vect)
{
	TurnoDisplay++;
	if (TurnoDisplay==2)
	TurnoDisplay=0;
	
	PORTD=0xff;	//Apagamos displays
	PORTB=0xff;
	
	switch(TurnoDisplay)
	{
		case 0:
		PORTB =~0x02;
		PORTD= DigitosDisplay[Dec];
		break;
		
		case 1:
		PORTB =~0x04;
		PORTD= DigitosDisplay[Cent];
		PORTD &=0x7f;
		break;	
	}
	CalculaDigitos=1;
	
	uint8_t parte_baja = ADCL;
	uint16_t completa = ADCH<<8 | parte_baja;
	uint16_t temperatura;
	temperatura= (5.0*completa*100.0)/1024;
		
	if(CalculaDigitos==1)
	{
		CalculaDigitos=0;
		Valor=temperatura;
		Cent=Valor/10;						temp1=Valor%10;	
		Dec= temp1;
	}
}