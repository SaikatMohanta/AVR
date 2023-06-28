const byte OUTPUT_PIN = 3;  // Timer 2 "B" output: OC2B

//uint16_t n;  // for example, 10 kHz

void setup() 
 {
  pinMode (OUTPUT_PIN, OUTPUT);
  ACD_init();
  
 }
void loop() 
{
  uint16_t sensorValue = adc_read(A0);
  //delay(20);
  uint16_t n = map(sensorValue, 0, 1023, 1, 250);
  
  TCCR2A = bit (WGM20) | bit (WGM21) | bit (COM2B1); // fast PWM, clear OC2A on compare

  //TImer 2 (8 Bit)Prescaler Values
  //#define T2_PS1      _BV(CS20)
  //#define T2_PS8      _BV(CS21)
  //#define T2_PS64     _BV(CS22)
  //#define T2_PS256    _BV(CS22) | _BV(CS21)
  //#define T2_PS1024   _BV(CS22) | _BV(CS21) | _BV(CS20)






  
  TCCR2B = bit (WGM22) | bit(CS22) | bit(CS21)| bit(CS20);         // fast PWM, prescaler of 64
  OCR2A =  n;                                // from table  
  OCR2B = ((n + 1) / 2) - 1;
}

void ACD_init()
{
  ADMUX = (1 << REFS0); //default Ch-0; Vref = 5V
  ADCSRA |= (1 << ADEN) | (0 << ADSC) | (0 << ADATE); //auto-trigger OFF
  ADCSRB = 0x00;
}


uint16_t adc_read(uint8_t ch)
{
  // select the corresponding channel 0~7
  // ANDing with '7′ will always keep the value
  // of 'ch' between 0 and 7
  //ch &= 0b00000111;  // AND operation with 7
  // ADMUX = (ADMUX & 0xF8)|ch; // clears the bottom 3 bits before ORing

  // start single convertion
  // write '1′ to ADSC
  ADCSRA |= (1 << ADSC);

  // wait for conversion to complete
  // ADSC becomes '0′ again
  // till then, run loop continuously
  while (ADCSRA & (1 << ADSC));
  ADC = (ADCL | (ADCH << 8));
  return (ADC);
}
