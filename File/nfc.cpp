#include "nfc.h"

unsigned char old_id[5];

unsigned char receive_ACK[25];//Command receiving buffer



int VerifId(unsigned char * id, unsigned char knownId[][5],int* NbID){

  int val =0;
  Serial.print("NBID:");
  Serial.println(*NbID);

  for(int i=0;i<5;i++)
    Serial.print(id[i],HEX);
  Serial.println();

  for(int j=0;j<(*NbID);j++){
    val=0;
    for (int i=0;i<5;i++) {
      if (id[i] == knownId[j][i])
        val++;    
      if(val==5) {
        Serial.println(j);
        return j;
      }
    }
  }
  return 0;

}

void PrintId(unsigned char* id){
  for(int i=0;i<5;i++) 
    UART_Send_Byte(id[i]);
  Serial.println();
}

char ReadBadge(unsigned char* id){
  char newVal=0;
  send_tag(); 
  read_ACK(25);
  delay(100);

  if (!cmp_id ()) {
    if (test_ACK ()) {
      display (25);
      newVal=1;
      delay (100);
    }
  }
  copy_id ();
  for(int i=0;i<5;i++)
    id[i]=old_id[i];
  return newVal;
}

void copy_id (void) {//save old id
  int ai, oi;
  for (oi=0, ai=19; oi<5; oi++,ai++) {
    old_id[oi] = receive_ACK[ai];
  }
}
 
char cmp_id (void){//return true if find id is old
  int ai, oi;
  for (oi=0,ai=19; oi<5; oi++,ai++) {
    if (old_id[oi] != receive_ACK[ai])
      return 0;
  }
  return 1;
}

int test_ACK (void) {// return true if receive_ACK accord with std_ACK
  int i;
  for (i=0; i<19; i++) {
    if (receive_ACK[i] != std_ACK[i])
      return 0;
  }
  return 1;
}

void send_id (void) {//send id to PC
  int i;
  Serial.print ("ID: ");
  for (i=19; i<= 23; i++) {
    Serial.print (receive_ACK[i], HEX);
    Serial.print (" ");
  }
  Serial.println ();
}

void UART1_Send_Byte(unsigned char command_data){//send byte to device
  print1Byte(command_data);
#if defined(ARDUINO) && ARDUINO >= 100
  Serial1.flush();// complete the transmission of outgoing serial data 
#endif
} 

void UART_Send_Byte(unsigned char command_data){//send byte to PC
  Serial.print(command_data,HEX);
  Serial.print(" ");
} 

void read_ACK(unsigned char temp){//read ACK into reveive_ACK[]
  unsigned char i;
  for(i=0;i<temp;i++) {
    receive_ACK[i]= Serial1.read();
  }
}

void wake_card(void){//send wake[] to device
  unsigned char i;
  for(i=0;i<24;i++) //send command
    UART1_Send_Byte(wake[i]);
}

void firmware_version(void){//send fireware[] to device
  unsigned char i;
  for(i=0;i<9;i++) //send command
    UART1_Send_Byte(firmware[i]);
}

void send_tag(void){//send tag[] to device
  unsigned char i;
  for(i=0;i<11;i++) //send command
    UART1_Send_Byte(tag[i]);
}

void display(unsigned char tem){//send receive_ACK[] to PC
  unsigned char i;
  for(i=0;i<tem;i++) //send command
    UART_Send_Byte(receive_ACK[i]);
  Serial.println();
}
