#include <Arduino.h>
#include "audio.h"

int last_audio_val = 0;

unsigned int i = 0;
int n_zero = 0;
int last_read = 0;

ISR(TIMER4_COMPA_vect){
    //Pretty wonky setup to get some different results depending on type of sound, values depend on hz of isr on clock4
    int val = analogRead(A0);
    if (last_read == 0 && val == 0){
      n_zero ++;
    } else if (val !=0) {
       if (n_zero != 0){
        last_read = n_zero;
        last_audio_val = last_read;
       }
       n_zero = 0;

    }
    last_read = val;
  
//  int val = analogRead(A0);
//  //int val = 1023;
//  Serial.write(val & 0xFF);
//  Serial.write(val >> 8 & 0xFF);
//  Serial.write(val >> 16 & 0xFF);
//  Serial.write(val >> 24 & 0xFF);
}

void setup_clock4(){
  cli();
  
  // interrupt messes alot with the VGA output timing :/
  TCCR4A = 0;
  TCCR4B = 0;
  TCNT4  = 0;
  OCR4A = 50;
  // turn on CTC mode
  TCCR4B |= (1 << WGM12);
  // Set CS12 and CS10 bits for 1024 prescaler
  TCCR4B |= (1 << CS12) | (1 << CS10);  
  // enable timer compare interrupt
  TIMSK4 |= (1 << OCIE4A);

  sei();
}
