//CoffreFort_H

#ifndef COFFREFORT_H
#define COFFREFORT_H 1

#include "nfc.h"
#include <Keypad.h>
#include <SPI.h>
#include <SD.h>
#include <Servo.h>
#include <LiquidCrystal.h>
//#include "donees.h"

#define PAD_ROWS  4 //four rows
#define PAD_COLS  4 //three columns

/*typedef struct {
	char KnownPassword;
	unsigned char knownId[5];
}Ids;*/

static const char keys[PAD_ROWS][PAD_COLS] = {
  	{'1','2','3','A'},
    {'4','5','6','B'},
    {'7','8','9','C'},
    {'*','0','#','D'}
};
static byte rowPins[PAD_ROWS] = {31+2, 33+2, 35+2, 37+2}; //connect to the row pinouts of the keypad
static byte colPins[PAD_COLS] = {39+2, 41+2, 43+2, 45+2}; //connect to the column pinouts of the keypad
static Keypad keypad = Keypad( makeKeymap(keys), rowPins, colPins, PAD_ROWS, PAD_COLS );


static Servo myservo;
static LiquidCrystal lcd(22, 23, 24, 25, 26, 27);



char PrintAllID(unsigned char knownId[][5],int* NbID);
char GetPassword(int nbNomber, char* password);
char ComparePassword(int nbNomber, char* password,char IdSelected, char KnownPassword[][4],int* NbID);
char addNewId(char KnownPassword[][4],unsigned char knownId[][5],int* NbID);
int SaveToSD(char KnownPassword[][4],unsigned char knownId[][5],int NbID);
int ReadSD(char KnownPassword[][4],unsigned char knownId[][5],int* NbID);
int WaitTolock(const char KnownPassword[][4],const unsigned char knownId[][5],const int NbID);
int WaitToUnlock(const char KnownPassword[][4],const unsigned char knownId[][5],const int NbID);
void OpenDoor();
void CloseDoor();



#endif
