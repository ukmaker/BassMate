#ifndef AD5204_H
#define AD5204_H

#include <Arduino.h>

class AD5204 {

    public:
    
    AD5204(uint8_t cs , uint8_t sck, uint8_t sdi) : _cs(cs), _sck(sck), _sdi(sdi) {
        pinMode(_cs, OUTPUT);
        pinMode(_sck, OUTPUT);
        pinMode(_sdi, OUTPUT);
        digitalWrite(_cs, HIGH);
        digitalWrite(_sck, LOW);
        digitalWrite(_sdi, LOW);
        digitalPotWrite(0,255);
        digitalPotWrite(1,255);
        digitalPotWrite(2,255);
        digitalPotWrite(3,255);
    }

void digitalPotWrite(int address, int value) {
 digitalWrite(_cs,LOW);
 // shift out the address
 for(int i=7; i>=0; i--) {
    if(address & (1 << i)) {
      digitalWrite(_sdi,HIGH);
    } else {
      digitalWrite(_sdi,LOW);
    }
    delayMicroseconds(2);
    digitalWrite(_sck,LOW);
    delayMicroseconds(2);
    digitalWrite(_sck,HIGH); 
    delayMicroseconds(2);
 }
 // shift out the data
 for(int i=7; i>=0; i--) {
    if(value & (1 << i)) {
      digitalWrite(_sdi,HIGH);
    } else {
      digitalWrite(_sdi,LOW);
    }
    
    delayMicroseconds(2);
    digitalWrite(_sck,LOW);
    delayMicroseconds(2);
    digitalWrite(_sck,HIGH); 
    delayMicroseconds(2);
 }
  digitalWrite(_cs,HIGH);
}
    protected:
     uint8_t _cs, _sck, _sdi;

};

#endif