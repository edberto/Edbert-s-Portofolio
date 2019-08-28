#include "FUTABA_SBUS.h"
#include <Servo.h>
#define ail 1
#define wheel 5
#define pitch 0
#define aux_W 6

FUTABA_SBUS sBus;

/*Input Channels*/
int led = PC13;

/* Output pins */
int pinRotorKiri = PA8;
int pinRotorKanan = PB6;

//ArmCheck
//bool armCheck;

/* Input yang diperlukan */
int inputAil = 1000;
int inputPitch = 1000;
int inputWheel = 1000;
int inputAux_W = 1000;
uint32_t keKiri = 1000;
uint32_t keKanan = 1000;
uint32_t heartbeat1 = 0;
uint32_t heartbeat2 = 0;

/* Rotor as Servos */
Servo rotorKiri;
Servo rotorKanan;

void setup() {
  // begin the SBUS communication
  sBus.begin();
  Serial.begin(115200);
  //cek n ricek
  pinMode(led, OUTPUT);
  digitalWrite(led, LOW);
  /*Rotor punya */
  rotorKiri.attach(pinRotorKiri);
  rotorKanan.attach(pinRotorKanan);
  rotorKiri.writeMicroseconds(1000);
  rotorKanan.writeMicroseconds(1000);
  //armCheck = false;
}

void loop() {
  sBus.FeedLine();
  if (sBus.toChannels == 1){
    sBus.UpdateChannels();
    sBus.toChannels = 0;
    //checkArmStatus();
    
    //Normalize 
    inputAux_W = map(sBus.channels[aux_W],172,1811,1000,2000); 
    inputAil = map(sBus.channels[ail],172,1811,1000,2000);
    inputPitch = map(sBus.channels[pitch],172,1811,1000,2000);
    inputWheel = map(sBus.channels[wheel],172,1811,1000,2000);

    //Tulis ke pin
    if((inputAux_W>=1000) && (inputAux_W<=1500)) {
    keKiri = 1500 - ((1500-inputWheel)/2)+((1500-inputAil)/2);
    keKanan = 1500 - ((1500-inputWheel)/2)-((1500-inputAil)/2);
    }
    else {
    keKiri =  1500 - ((1500-inputWheel)/3);
    keKanan =  1500 - ((1500-inputWheel)/3);
    }
    rotorKiri.writeMicroseconds(keKiri);
    rotorKanan.writeMicroseconds(keKanan);
    
  }
  delay(1);
  Serial.print("Ailoooo :");
  Serial.print(inputAil);
  Serial.print("\t\t output :");
  Serial.println(keKiri);
  Serial.flush();
  /*Serial.print("Wheel :");
  Serial.println(inputWheel);
  Serial.print("Rotor Kiri :");
  Serial.println(inputWheel-((1500-inputAil)/2));
  Serial.print("Rotor Kanan :");
  Serial.println(inputWheel+((1500-inputAil)/2));*/
}

/*void checkArmStatus() {
  if ((inputAux_1>=1000) && (inputAux_1<=1500)){
    armCheck = true;
  }
  else {
    armCheck = false;
  }
}*/
