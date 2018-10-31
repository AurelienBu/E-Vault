//CoffreFort_C
#include "CoffreFort.h"
#include "nfc.h"

//#include <Keypad.h>






char PrintAllID(unsigned char knownId[][5],int* NbID){



	Serial.println("-----------------IDs--------------");
	Serial.println();
	Serial.println(*NbID);
	delay(1000);
	for(int j=0;j<(*NbID);j++){
		Serial.print("NbID : ");
		Serial.println(*NbID);
		for(int i=0;i<5;i++)
    		Serial.print(knownId[j][i],HEX);
    	Serial.println();
	}
	Serial.println("---------------------------------");
	return 1;
}



char GetPassword(int nbNomber, char* password ){

	for(int i=0;i<4;i++)password[i]='F';
	//Keypad keypad = Keypad( makeKeymap(keys), rowPins, colPins, PAD_ROWS, PAD_COLS );
	char key=0;

	int i=0;
	for(i=0;((i<nbNomber));i++){
		 key = keypad.getKey();
		 if(key=='#')break;
		if(key){
			password[i] = key;
			Serial.print('*');
		}
		else i--;
	}
	while((key!='#')){
		key = keypad.getKey();
		if(key){
			password[i] = key;
			Serial.print('*');
			Serial.println();
		}

	}
	return 1;
}
char ComparePassword(int nbNomber, char* password,char IdSelected, char KnownPassword[][4],int* NbID){
	int val=0;
	Serial.println(IdSelected);
	for(int i;i<nbNomber;i++){
		Serial.print(KnownPassword[IdSelected][i]);
		if(password[i]==KnownPassword[IdSelected][i])val++;
	}
	if(val>=nbNomber)return 1;
	else return 0;

}

char addNewId(char KnownPassword[][4],unsigned char knownId[][5],int* NbID){

	lcd.clear();
    	lcd.setCursor(0, 0);
	Serial.println("Please badge a known badge to add new one");
	lcd.print("Use a known badge");
	//Serial.println("Please Badge with a know id");
	char NewId=0;
	unsigned char localid[5];
	char idOK=0;
	char password[4]={'F','F','F','F'};
	char key='F';
	int ok=0;
	int* pNbID = NbID;
	

 	while(ok==0){
		if(ReadBadge(localid)){  ///lecture badge connu
	    	idOK=VerifId(localid,knownId,pNbID); //verificatiuon du badge 
	    	Serial.println(idOK);
	    	for(int i=0;i<5;i++) localid[i];
	    	if(idOK){
	    		lcd.clear();
    	lcd.setCursor(0, 0);
	    		Serial.println("Please enter your passeword");
	    		lcd.print("Enter your pwd");
	      		if(GetPassword(4, password )){
	      			
	        		if(ComparePassword(4, password,idOK,KnownPassword,pNbID)){
	        			lcd.clear();
    	lcd.setCursor(0, 0);
	        			Serial.println("Please new badge");
	        			lcd.print("Please new badge");
	        			while(!ReadBadge(localid)){
	        				/*for(int i=0;i<5;i++){
	        					knownId[(*NbID)-1][i]=localid[i];
	        					//Serial.print(localid[i],HEX);	        				
	        				}
	        				//Serial.println("did it");*/
	        			}
	        			lcd.clear();
    	lcd.setCursor(0, 0);
	        			Serial.println("Please the new password");
	        			lcd.print("Enter the new pwd");
	        			if(GetPassword(4, password )){
	        				lcd.clear();
    	lcd.setCursor(0, 0);
	  	
	        				Serial.println("New ID added in the database !");
	        				Serial.print("New ID added in the database !");
	        				Serial.println(*NbID);
 							(*NbID)=(*NbID)+1;
 		        				Serial.println(*NbID);
 		        			for(int i=0;i<5;i++){
	        					KnownPassword[(*NbID)-1][i]=password[i];
	        				}
	  						Serial.print("knowId apres ajout : ");
	  						for(int i=0;i<5;i++){
	  								knownId[(*NbID)-1][i]=localid[i];

	  							Serial.print(knownId[(*NbID)-1][i],HEX);
	        				}

	        			}
	        		}
	        	}	        			
	  		Serial.println();
	  		Serial.print("localid apres ajout : ");
	  		for(int i=0;i<5;i++)
	  			Serial.print(localid[i],HEX);	

	  		ok=1;
	  		}else {
	  			lcd.clear();
    	lcd.setCursor(0, 0);
	  			Serial.println("Unknown ID");
	  			lcd.print("Unknown ID");
	  			
	  		}
	  	}
	  		
	}
	

	lcd.clear();
    lcd.setCursor(0, 0);
    lcd.print("Press # to open");

	return 1;
}








int SaveToSD(char KnownPassword[][4],unsigned char knownId[][5],int NbID){

	File KnownPasswordFILE;
	File KnownIdFILE;
	File NbIDFILE;


	Serial.print("Initializing SD card...");

	if (!SD.begin(53)) {
    	Serial.println("initialization failed!");
    	return -1;
  	}
 	Serial.println("initialization done.");
  	SD.remove("password.txt");
  	SD.remove("ids.txt");
  	SD.remove("nbid.txt");


//-----------password

  	KnownPasswordFILE = SD.open("password.txt", FILE_WRITE);

  	if (KnownPasswordFILE) {
    	Serial.print("Writing to password.txt...");
    	for(int j=0;j<NbID;j++){
	    	for(int i=0;i<4;i++){
	    	  	KnownPasswordFILE.print(KnownPassword[j][i]);
		      	KnownPasswordFILE.print(";");
		   	}
	    	KnownPasswordFILE.println();
	   	}

    	KnownPasswordFILE.close();
    	Serial.println("done.");
  	} else {
	   	Serial.println("error opening password.txt");
	   	return -1;
  	}
//--------------IDS


	KnownIdFILE = SD.open("ids.txt", FILE_WRITE);

	if (KnownIdFILE) {
		Serial.print("Writing to ids.txt...");
	    for(int j=0;j<NbID;j++){
		    for(int i=0;i<5;i++){
		      KnownIdFILE.print(knownId[j][i]);
		      //KnownIdFILE.print("*");
		      KnownIdFILE.print(";");
		    }
	    	KnownIdFILE.println();
	    }
	    KnownIdFILE.close();
	    Serial.println("done.");
	} else {
		Serial.println("error opening ids.txt");
		return -1;
	}

//--------------NbIds

	NbIDFILE = SD.open("nbid.txt", FILE_WRITE);

	if (NbIDFILE) {
	    Serial.print("Writing to nbid.txt...");
	    NbIDFILE.println(NbID);
	    NbIDFILE.close();
	    Serial.println("done.");
	} else {
	    Serial.println("error opening nbid.txt");
	    return -1;
	}
	return 1;
}

int ReadSD(char KnownPassword[][4],unsigned char knownId[][5],int* NbID){

	File KnownPasswordFILE;
	File KnownIdFILE;
	File NbIDFILE;
	unsigned char localids[5][5];
	int i=0,j=0;
	unsigned char read=0;

	Serial.print("Initializing SD card...");

/*	if (!SD.begin(53)) {
    	Serial.println("initialization failed!");
    	return -1;
  	}*/
 	Serial.println("initialization done.");

 	KnownIdFILE = SD.open("ids.txt", FILE_READ);

	if (KnownIdFILE) {

		while (KnownIdFILE.available()) {
			read=KnownIdFILE.read();
			//Serial.write(read);
			if(read==';') {
				i++;
			}
			else if (read==13){
				j++;
				i=0;
			}
			else localids[j][i]=read;    
			Serial.println();		
    	}
		
	    KnownIdFILE.close();
	    Serial.println("done.");
	} else {
		Serial.println("error opening ids.txt");
		return -1;
	}

	for(int i=0;i<5;i++){  								
		Serial.print(localids[0][i]);
	}
	Serial.println();


	return 1;
}



int WaitTolock(const char KnownPassword[][4],const unsigned char knownId[][5],const int NbID){
 char key=0;
 int val=0;
 lcd.clear();
    	lcd.setCursor(0, 0);
  Serial.println("Press # to lock agin");
  lcd.print("Press # to lock");
  while(!val){
    key = keypad.getKey();
    if (key){
      if(key=='#') {
        CloseDoor();
        val++;
      }
    }
  }
  return val;

}



int WaitToUnlock(const char KnownPassword[][4],const unsigned char knownId[][5],const int NbID){
    int newid=0;
    int lock=0;
    char password[4]={'F','F','F','F'};
    unsigned char id[5];

    lcd.clear();
    	lcd.setCursor(0, 0);
  Serial.println("Please badge to open the vault");
  lcd.print("Badge to open");
  while(!lock){
  
    if(ReadBadge(id)){  
      PrintId(id);
      newid=VerifId(id,knownId,&NbID);
      //Serial.println(newid);
      if(newid){
      	lcd.clear();
    	lcd.setCursor(0, 0);
        Serial.println("Please enter your passeword");
        lcd.print("Enter your pwd");
        if(GetPassword(4, password )){
          if(ComparePassword(4, password,newid,KnownPassword,&NbID)){
            
            lock++;//unlock
            OpenDoor();

          } else {
          		lcd.clear();
    			lcd.setCursor(0, 0);
              Serial.println("Wrong Password ! Badge again");
              lcd.print("Wrong Password!");
              return 0;
            }   
        }
      }else {
      	lcd.clear();
    	lcd.setCursor(0, 0);
        Serial.println("Unknown ID");
        lcd.print("Unknown ID");
        return 0;
      }
      for(int i=0;i<5;i++)
        id[i]=0xFF;
    }
  }
  return lock;
}





void OpenDoor(){
	lcd.clear();
    lcd.setCursor(0, 0);
	lcd.print("Welcome");

	myservo.attach(9);
	Serial.print("Welcome ");
	myservo.write(180); 
	lcd.setCursor(0, 1);
	Serial.println(" Door is open");
	lcd.print("Door is open");
	lcd.setCursor(0, 0);
}
void CloseDoor(){

	lcd.clear();
    lcd.setCursor(0, 0);

	myservo.attach(9);
	lcd.print("The Door is locked");
	Serial.println("The door is locked");
	myservo.write(90); 
	lcd.setCursor(0, 1);
	lcd.print("GoodBye");
	Serial.print("GoodBye");
	lcd.setCursor(0, 0);
	lcd.clear();
    lcd.setCursor(0, 0);
    lcd.print("Press # to open");
}






