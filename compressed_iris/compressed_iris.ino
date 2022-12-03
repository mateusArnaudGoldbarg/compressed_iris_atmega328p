#include "model.h"
#include "model_prune.h"
//#include "model_prune2.h"
//#include "model_quant.h"
#include "inputs.h"
//#include "inputs_quant.h"
#include <util/delay.h>


int t=0;
bool start= false;

int main(void){
  Serial.begin(9600);
  OCR1A = 14; //OCR1A = 16M/(N*ts)-1
  
  TCCR1A |= 0;
  //modo CTC com prescaler 1024
  TCCR1B |= 0b00001101;
  
  //Habilita COMPA
  //TIMSK1 |= 0b00000010;
  TIMSK1 |= (1<<OCIE1A);
  
  sei();

  DDRD |= 0b11100000;
  
  int res=0;

  
  while(1){
    
    start=true;
    t=0;
    for(int i= 0; i<12; i++){
      res = predict(X[i][0],X[i][1],X[i][2],X[i][3]);

      
      //Serial.print(X[i][0]);
      //Serial.print(" ");
      //Serial.print(X[i][1]);
      //Serial.print(" ");
      //Serial.print(X[i][2]);
      //Serial.print(" ");
      //Serial.print(X[i][3]);
      //Serial.print(" ");
      //Serial.println(res);
      
      if(res ==1){
        PORTD &=0;
        PORTD |=(1<<7);
      }
      if(res ==2){
        PORTD &=0;
        PORTD |=(1<<6);
      }
      else{
        PORTD &=0;
        PORTD |=(1<<5);
      }
    }
    start=false;
    //Serial.print("Tempo gasto: ");
    Serial.println(t);
    

    /*
    start=true;
    t=0;
    for(int i= 0; i<12; i++){
      res = predict_prune(X[i][0],X[i][1],X[i][2],X[i][3]);
      
      //Serial.print(X[i][0]);
      //Serial.print(" ");
      //Serial.print(X[i][1]);
      //Serial.print(" ");
      //Serial.print(X[i][2]);
      //Serial.print(" ");
      //Serial.print(X[i][3]);
      //Serial.print(" ");
      //Serial.println(res);
      
      if(res ==1){
        PORTD &=0;
        PORTD |=(1<<7);
      }
      if(res ==2){
        PORTD &=0;
        PORTD |=(1<<6);
      }
      else{
        PORTD &=0;
        PORTD |=(1<<5);
      }
    }
    start=false;
    //Serial.print("Tempo Gasto: ");
    Serial.println(t);
    */
    
    _delay_ms(500);
    
  }
  
}



ISR(TIMER1_COMPA_vect){
  if(start)
    t++;
}
