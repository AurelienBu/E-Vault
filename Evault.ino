
#include "File/CoffreFort.h"
#include "File/nfc.h"
//#include "donees.h"

#define LED 13
#define NB_BADGE_KNOWN 10

char add=0;






int test=0;

static int NbID = 2;


static unsigned char knownId[NB_BADGE_KNOWN][5] = {{0x52, 0xA3, 0xAA, 0x56, 0xD9},
                                                   {0x00, 0x33, 0x72, 0x28, 0x01}};
//static String NameId[10]={"NewID","Aurelien"};

static char KnownPassword[NB_BADGE_KNOWN][4]={{'1','1','1','1'},
                                              {'1','2','3','4'}};






void setup(){
  Serial.begin(9600); // open serial with PC
  Serial1.begin(115200);  //open serial1 with device
  
  
  myservo.write(90);
  lcd.begin(16, 2);
  
  wake_card();
  delay(100);
  read_ACK(15);
  delay(100);
  display(15);
  pinMode(LED, OUTPUT);
  digitalWrite(LED,LOW);


 

}


void loop(){



  if(!test){ 
    SaveToSD(KnownPassword,knownId,NbID); 
    //ReadSD(KnownPassword,knownId,&NbID); 
    test ++;
    Serial.println();
    Serial.println("Press # to open the volt");
    
    lcd.clear();
    lcd.setCursor(0, 0);
    lcd.print("Press # to open");

  }

  //Serial.println(NbID);

 //PrintAllID(knownId,NbID);
  char key=0;

  key = keypad.getKey();


    if (key){
      if(key=='#') {
        if(WaitToUnlock(KnownPassword,knownId,NbID))
          while(!WaitTolock(KnownPassword,knownId,NbID));
      }else if (key == 'A'){ //Pour ajouter un nouvel utilisateur placer le badge et appuyer sur #
        addNewId(KnownPassword,knownId,&NbID);
        Serial.println(NbID);
      }
    }


  
  
 // WaitTolock();
}



