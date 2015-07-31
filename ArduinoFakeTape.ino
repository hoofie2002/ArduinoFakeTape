/*

Microprofessor Tape Player

Dumps a compiled Z80 program into a MicroProfessor

 */

int PIN_TAPE = 8;

long leader_time=4000;
long mid_sync_time = 2000;
long tail_sync_time = 2000;

word fileName = 0x0102;
word startAddress = 0x1800;

byte chkSum = 0x00;

byte programData[] = {0x3E, 0x05, 0x06, 0x04, 0x80, 0x32, 0x30, 0x18, 0x76};
int programLength = 9;

word endAddress = startAddress + programLength - 1;

#include "tape.h"

void setup() {

    play_leader();
    
    sendWord(fileName);
    sendWord(startAddress);
    sendWord(endAddress);
    sendByte(chkSum);
    
    play_mid_sync();
    
    for (int i = 0; i < programLength; i++) {
      sendByte(programData[i]);
    }
    
    play_tail_sync();

}

void generateSquareWave2K(long numberOfPulses) {
  pinMode(8, OUTPUT);
  int period = 250;
  
  for (int i =0; i < numberOfPulses; i++) {
    digitalWrite(8, HIGH);
    delayMicroseconds(period);
    digitalWrite(8, LOW);
    delayMicroseconds(period);
  }
}

void generateSquareWave1K(long numberOfPulses) {
  pinMode(8, OUTPUT);
  int period = 500;
  
  for (int i =0; i < numberOfPulses; i++) {
    digitalWrite(8, HIGH);
    delayMicroseconds(period);
    digitalWrite(8, LOW);
    delayMicroseconds(period);
  }
}
  
  
void sendWord(word data) {
  sendByte(data);
  sendByte(data >> 8);
}
  
void sendByte(byte data) {
  play_bit_0();

  
    for (byte mask = 00000001; mask>0; mask <<= 1) { //iterate through bit mask
    if (data & mask){ // if bitwise AND resolves to true
      play_bit_0();
    }
    else{ //if bitwise and resolves to false
      play_bit_1();
    }

  }
  
  play_bit_1();
 }




void play_bit_1() {
  generateSquareWave2K(8);
  generateSquareWave1K(2);
}

void play_bit_0() {
  generateSquareWave2K(4);
  generateSquareWave1K(4);
}

void play_leader() {
    tone(PIN_TAPE, TONE_1K);
    
    delay(leader_time);
    
    noTone(PIN_TAPE);
}

void play_mid_sync() {
    tone(PIN_TAPE, TONE_2K);
    delay(mid_sync_time);
    noTone(PIN_TAPE);
}

void play_tail_sync() {
    tone(PIN_TAPE, TONE_2K);
    delay(tail_sync_time);
    noTone(PIN_TAPE);
}



void loop() {

}
