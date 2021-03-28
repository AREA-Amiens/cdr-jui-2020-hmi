#include <Arduino.h>
#include <SPI.h>
#include <MsTimer2.h>
#include <U8g2lib.h>
#include <FlexCan.h>
#include <kinetis_flexcan.h>


#define LED_1       0
#define LED_2       1
#define CAN_TX      2
#define CAN_RX      3
#define ENCODEUR_B  6
#define ENCODEUR_A  7
#define ENCODEUR_BP 5
#define TIRETTE     8
#define BOUTON_2    9
#define BOUTON_3    10
#define LCD_MOSI    11
#define LCD_MISO    12
#define LCD_SCR     13

#define BUZZER      15

#define LED_RGB_R   20
#define LED_RGB_G   21
#define LED_RGB_B   22

byte ecrant_0();
byte ecrant_1();
byte ecrant_2();
byte ecrant_3();
byte ecrant_4();
byte ecrant_5();
byte ecrant_6();
byte ecrant_7();
void bpInterupEncodeur();
void tournInterupEncodeurRisingA();
void interpretationCan();
int comparChenCan(String comp);
void IntrerrupTimer();
