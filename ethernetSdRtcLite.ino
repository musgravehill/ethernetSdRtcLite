/********************************************************
 * 
 * This example shows how to connect a simple web server read that shows 
 * the value of the analog input pins. And then write the data to and read 
 * from an SD card file--"test.txt".
 * 
 * The W5200 circuit:
 * W5200 on Ethernet Shield V2.0 by seeed attached to SPI bus as follows:
 ** ----------------
 **| W5200 - Arduino|
 **| MOSI  - MOSI   |
 **| MISO  - MISO   |
 **| CLK   - SCK    |
 **| CS    - pin 10 |
 ** ----------------
 * 
 * The other Circuit:
 * SD card attached to SPI bus as follows:
 ** ----------------
 **| SD    - Arduino|
 **| MOSI  - MOSI   |
 **| MISO  - MISO   |
 **| CLK   - SCK    |
 **| CS    - pin 4  |
 ** ----------------
 * 
 * Analog inputs attached to pins A0 through A5 (optional).
 * 
 * Author: Frankie.Chu at Seeed Studio.
 * Date:   2012-11-20
 * 
 *******************************************************/

/*
digitalWrite(W5200_CS, HIGH); //W5200 sleep
 digitalWrite(W5200_CS, LOW); //W5200 ready
 
 digitalWrite(SDCARD_CS, HIGH); //SD sleep
 digitalWrite(SDCARD_CS, LOW); //SD ready
 
 delay(50);
 */

#include <SPI.h>
#include <EthernetV2_0.h>
#include <Wire.h>
#include <RTClib.h>
#include <SD.h>

RTC_DS1307 RTC;

#define W5200_CS  10
#define SDCARD_CS 4
// Enter a MAC address and IP address for your controller below.
// The IP address will be dependent on your local network:
byte mac[] = { 
  0xDE, 0xAD, 0xBE, 0xEF, 0xFE, 0xED };
IPAddress ip(192,168,0,111);

// Initialize the Ethernet server library
// with the IP address and port you want to use 
// (port 80 is default for HTTP):
EthernetServer server(80);

void setup()
{
  //Serial.begin(9600);  
  pinMode(W5200_CS, OUTPUT);      
  pinMode(SDCARD_CS,OUTPUT);

  digitalWrite(W5200_CS, HIGH); //W5200 sleep
  delay(50);
  SD.begin(SDCARD_CS); //it bang d10 and d4?   
  delay(50);  
  digitalWrite(SDCARD_CS, HIGH); //SD sleep
  delay(50);
  Ethernet.begin(mac, ip); //it bang d10 and d4?
  server.begin();  

  delay(50);
  //Wire.begin();
  RTC.begin();
  //RTC.adjust(DateTime(F(__DATE__), F(__TIME__)));
  delay(100);
}



void loop()
{   
  //listen for incoming clients
  EthernetClient client = server.available();
  if (client) {      
    boolean isCLientRequestEnded = false; // an http request ends with a blank line  
    char clientRequestCharPrev = ' ';
    char clientRequestCharCurr = ' ';      
    while (client.connected()) {
      if (client.available()) {
        char clientRequestChar = client.read();  
        clientRequestCharPrev = clientRequestCharCurr;
        clientRequestCharCurr = clientRequestChar;
        if(clientRequestCharPrev == '\n' && clientRequestCharCurr == '\r'){
          isCLientRequestEnded = true;
        }
        else{
          isCLientRequestEnded = false;
        }

        if(isCLientRequestEnded){                    
          processRequest(client);
          delay(1); // give the web browser time to receive the data       
          client.stop(); // close the connection  
          logRequest();            
        }
      }      
    }    

    //int freeRam = FreeRam();
    //String freeRamInfo = "freeRAM: 2048 - ";
    //freeRamInfo += freeRam;
    //freeRamInfo += " bytes";
    //Serial.println(freeRamInfo);     
  }   //endif client

}//loop

void processRequest(EthernetClient client){  
  client.println("HTTP/1.1 200 OK");
  client.println("Content-Type: text/html");
  client.println("Connnection: close");
  client.println();     //!!!! end of http headers  
  client.println("<!DOCTYPE html>");
  client.println("<html>");
  client.println("<head>");
  client.println("<meta charset='UTF-8'>");
  client.println("<title>ethernetW5200 RTC SD</title>");
  client.println("</head>");
  client.println("<body>");
  client.println("Hello!");
  client.println("</body>");
  client.println("</html>"); 
  
}

void logRequest(){ 
  DateTime now = RTC.now();  

  File logFile; 
  logFile = SD.open("LOG.TXT", FILE_WRITE);  
  if (logFile) {      
    char line[50];    
    sprintf(line, "%02d:%02d:%02d %02d-%02d-%04d",     
    now.hour(), now.minute(), now.second(),
    now.day(),now.month(), now.year()    
      );
    logFile.println(line);      
    logFile.close();  
  }
}
/*
int freeRam() {
 extern int __heap_start,*__brkval;
 int v;
 return (int)&v - (__brkval == 0 ? (int)&__heap_start : (int) __brkval);  
 }
 */




