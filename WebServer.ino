/*
  SD card read/write

 This example shows how to read and write data to and from an SD card file
 The circuit:
 * SD card attached to SPI bus as follows:
 ** MOSI - pin 11
 ** MISO - pin 12
 ** CLK - pin 13
 ** CS - pin 4

 created   Nov 2010
 by David A. Mellis
 modified 9 Apr 2012
 by Tom Igoe

 This example code is in the public domain.


// karta musi być w formacie FAT ustawienie 32KB
 */
//#define SD_CS_PIN SS
#include <SPI.h>
//#include <SD.h>
#include "SdFat.h"
SdFat SD;
#include <SDFatUtil.h>


#include <Ethernet.h>
#define REQ_BUF_SZ   30

File webFile;
char *plik = "index.html";

byte mac[] = {
  0xDE, 0xAD, 0xBE, 0xEF, 0xFE, 0xED
};
IPAddress ip(192, 168, 1, 105);

// Initialize the Ethernet server library
// with the IP address and port you want to use
// (port 80 is default for HTTP):
EthernetServer server(12345);
EthernetClient client;


char HTTP_req[REQ_BUF_SZ] = {0}; // buffered HTTP request stored as null terminated string
char req_index = 0;              // index into HTTP_req buffer

void setup()
{
  // Open serial communications and wait for port to open:
  Serial.begin(115200);

  Serial.print("Initializing SD card...");
  // On the Ethernet Shield, CS is pin 4. It's set as an output by default.
  // Note that even if it's not used as the CS pin, the hardware SS pin
  // (10 on most Arduino boards, 53 on the Mega) must be left as an output
  // or the SD library functions will not work.
  pinMode(10, OUTPUT);
//  SD.init(SPI_HALF_SPEED, 4);
  if (!SD.begin(4)) {
    Serial.println("initialization failed!");
    return;
  }
  Serial.println("initialization done.");
  if (!SD.exists(plik)) {
        Serial.println("Nie znaleziono pliku HTML (HTM) :( " + String(plik));
        return;  // can't find index file
    }
   Serial.println("Plik strony HTML " + String(plik) + " istnieje :) Serwer za chwile bedzie gotowy :) ");


  // open the file. note that only one file can be open at a time,
  // so you have to close this one before opening another.
//  myFile = SD.open("example.txt", FILE_WRITE);

  // if the file opened okay, write to it:
//  if (myFile) {
//    Serial.print("Writing to test.txt...");
//    myFile.println("testing 1, 2, 3.");
//    // close the file:
//    myFile.close();
//    Serial.println("done.");
//  } else {
//    // if the file didn't open, print an error:
//    Serial.println("error opening test.txt");
//  }

  // re-open the file for reading:
//  myFile = SD.open(plik);
//  if (myFile) {
//    Serial.println("test.txt:");
//
//    // read from the file until there's nothing else in it:
//    while (myFile.available()) {
//      Serial.write(myFile.read());
//    }
//    // close the file:
//    myFile.close();
//  } else {
//    // if the file didn't open, print an error:
//    Serial.println("error opening test.txt");
//  }

  Ethernet.begin(mac, ip);
  server.begin();
  Serial.print("Serwer parcuje na adresie IP: ");
  Serial.println(Ethernet.localIP());


}

void loop()
{
  String stringOne = "text dużo tekstu";
  // nothing happens after setup
    client = server.available();
  if (client) {
    Serial.println("new client");
    // an http request ends with a blank line
    boolean currentLineIsBlank = true;
    long initTime = millis();
    while (client.connected()) {
      char url[30]={0};
      if (client.available()) {
        char c = client.read();
        if (req_index < (REQ_BUF_SZ - 1)) {
                    HTTP_req[req_index] = c;          // save HTTP request character
                    req_index++;
                }
        Serial.write(c);
       
        // if you've gotten to the end of the line (received a newline
        // character) and the line is blank, the http request has ended,
        // so you can send a reply
        if (c == '\n' && currentLineIsBlank) {
                    // open requested web page file
                    HttpReqURL(url, HTTP_req);
//                    Serial.println(" \n test programu: ");
//                    for(int i=0; i<30;i++){
//                      Serial.print(url[i]);
//                    }
                  
                    if (strstr(HTTP_req, "GET / ")
                                 || strstr(HTTP_req, "GET /index.html")) {
                        client.println("HTTP/1.1 200 OK");
                        client.println("Content-Type: text/html");
                        client.println("Connnection: close");
                        client.println();
                        webFile = SD.open("index.html");        // open web page file
                    }
                    else{
                      Serial.print("inne opcje");
                      client.println("HTTP/1.1 200 OK");
                        client.println("Content-Type: text/css");
                        client.println("Connnection: close");
                        client.println();
                        webFile = SD.open(url); 
                    }
//                    else if (StrContains(HTTP_req, "GET /css/bootstrap.min.css")) {
//                      Serial.print("działa PETLA bootstrap");
//                        client.println("HTTP/1.1 200 OK");
//                        client.println("Content-Type: text/css");
//                        client.println("Connnection: close");
//                        client.println();
//                        webFile = SD.open("/css/bootstrap.min.css");        // open web page file
//                    }
//                    else if (StrContains(HTTP_req, "GET /css/styles.css")) {
//                      Serial.print("działa PETLA styles");
//                        client.println("HTTP/1.1 200 OK");
//                        client.println("Content-Type: text/css");
//                        client.println("Connnection: close");
//                        client.println();
//                        webFile = SD.open("/css/styles.css");        // open web page file
//                    }
//                    
//                    else if (StrContains(HTTP_req, "GET /pic.jpg")) {
//                        webFile = SD.open("pic.jpg");
//                        if (webFile) {
//                            client.println("HTTP/1.1 200 OK");
//                            client.println();
//                        }
//                    }
//                    if (webFile) {
//                        while(webFile.available()) {
//                            client.write(webFile.read()); // send web page to client
////                            
//                        }
////                        Serial.println(" tutaj cos jest?222222222222");
////                        byte buf[512];
////                        while (webFile) {
////                            int n = webFile.available();
////                            if (n == 0) break;
////                            if (n > 512) n = 512;
////                            webFile.read(buf, n);
////                            client.write(buf, n);
////                        }
//                        webFile.close();
//                    }
if (webFile) {
              int number = 1024;
              byte clientBuf[number];
           
              int clientCount = 0;              
            long initTimeFile= micros();
            long timeFile;
            
            uint8_t b;
            initTimeFile = micros();
              while (webFile.available()) 
              { 
//                
             webFile.read(&clientBuf, number);
//                clientBuf[clientCount] = webFile.read(&clientBuf, 2);
//                clientCount++;
        
//                if(clientCount > number-1)
//                {
//                  
                  client.write(clientBuf,number);
//                  clientCount = 0;
//                }                
              }
              timeFile = micros()-initTimeFile;
                
                Serial.print("\nczas " );
              Serial.println(timeFile);
              
              if(clientCount > 0) client.write(clientBuf,clientCount); 
              webFile.close();
              
            }
            
                    
                    // reset buffer index and all buffer elements to 0
                    req_index = 0;
                    StrClear(HTTP_req, REQ_BUF_SZ);
                    break;
                }

        if (c == '\n') {
          // you're starting a new line
          currentLineIsBlank = true;
        }
        else if (c != '\r') {
          // you've gotten a character on the current line
          currentLineIsBlank = false;
        }
      }
    }
    long stopTime = millis();
    Serial.print("\nCzas petli to: ");
    Serial.println(stopTime - initTime);
    // give the web browser time to receive the data
    delay(10);
    // close the connection:
    client.stop();
    Serial.println("client disconnected");
  }

//  for(int i=0; i<REQ_BUF_SZ - 1;i++){
//    Serial.print(HTTP_req[i]);
//  }
}
// sets every element of str to 0 (clears array)
void StrClear(char *str, char length)
{
    for (int i = 0; i < length; i++) {
        str[i] = 0;
    }
}

// searches for the string sfind in the string str
// returns 1 if string found
// returns 0 if string not found
//byte StrContains(char *str, char *sfind)
//{
//    char found = 0;
//    char index = 0;
//    char len;
//
//    len = strlen(str);
//    if (strlen(sfind) > len) {
//        return 0;
//    }
//    while (index < len) {
//        if (str[index] == sfind[found]) {
//            found++;
//            if (strlen(sfind) == found) {
//                return 1;
//            }
//        }
//        else {
//            found = 0;
//        }
//        index++;
//    }
//
//    return 0;
//}

void HttpReqURL(char *url, char *str){
  boolean firstSpace =false;

//  boolean secondSpace = false;
  byte len = strlen(str); 
  byte j=0;
  for(byte i=0; i<=len; i++){
    
   if(firstSpace==true){
      url[j]=str[i];
     j++;
     }
   if(str[i]==' ' && firstSpace ==false){
      firstSpace = true;
     }
   else if(str[i]==' ' && firstSpace ==true){
      firstSpace = false;
      break;
     }
     
     
  }

}

