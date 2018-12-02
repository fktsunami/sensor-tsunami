#include <SPI.h>
#include <nRF24L01.h>
#include <RF24.h>
RF24 radio(A5, A4); // CE, CSN
const byte address[12] = "00001";

char data[150]="";
int data_index = 0;
int offset = 0;
bool data_completed = false;

void print_data() {
  for (int i=0; i<150; i++)
  {
    Serial.print(data[i]);    
  }
  Serial.println();
}

void setup() {
  Serial.begin(9600);
  radio.begin();
  radio.openReadingPipe(0, address);
  radio.setPALevel(RF24_PA_MAX);
  radio.startListening();
}
void loop() {  
  if (radio.available()) {
    char receive_buffer[50]="";
    radio.read(&receive_buffer, sizeof(receive_buffer));
//    Serial.println(receive_buffer);
    
    if (receive_buffer[0] == '@') {
//      Serial.println("Begin new package");
      data_index = 0;
      offset = 1;
    }
    else {
//      Serial.println("Continue old package");
      offset = 0;
    }
//    Serial.println("Before appending");
//    Serial.println(data);
    for(int i=0; i<50; i++)
    {            
      if(receive_buffer[i]=='$') {
        data_completed = true;
        data_index = 0;
//        Serial.println("Complete package");
        break;
      }
      else if(receive_buffer[i] =='\0') {        
        // Stop append the rest of buffer
//        Serial.println("Stop for loop");
        break;        
      }
      else {
//        Serial.println(receive_buffer[i]);
        data[data_index] = receive_buffer[i+offset];
        data_index++;
//        print_data();
      }            
    }
//    Serial.println("After appending");
//    Serial.println(data);
    if(data_completed){
//      Serial.println("Data completed:");
//      Serial.println(data);
      print_data();
      data_completed = false;        
    }
        
  }  
}
