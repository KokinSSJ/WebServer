/*
  WebServer by Dominik Bogacki
  1. comunication by local host server -
  2. increased reading speed from SD card by buffor
  3. send file which browser wants (auto file directory localization)
  4. increased sending proces - over 20times!



  // karta musi być w formacie FAT ustawienie 32KB
*/

#include <SPI.h>
#include "SdFat.h"
#include <SDFatUtil.h>
#include <Ethernet.h>

#define REQ_BUF_SZ   55

SdFat SD;
File webFile;

char *plik = "index.html"; //initial file - check if server has something to show!

byte mac[] = {0xDE, 0xAD, 0xBE, 0xEF, 0xFE, 0xED}; // mac of arduino server
IPAddress ip(192, 168, 1, 105); //local server address
EthernetServer server(12345); //local port
EthernetClient client;

char HTTP_reqURL[REQ_BUF_SZ] = {0}; // buffered HTTP request stored as null terminated string
int reqURL_index = 0;              // index into HTTP_req buffer
int reqTEXT_index =0 ;
char HTTP_reqTEXT[REQ_BUF_SZ]={0};
void setup()
{
  Serial.begin(115200);
  Serial.print("Initializing SD card...");
  pinMode(10, OUTPUT);

  // check if card is OK
  if (!SD.begin(4)) {
    Serial.println("initialization failed!");
    return;
  }
  Serial.println("initialization done.");

  //check if SD card has some site to show!
  if (!SD.exists(plik)) {
    Serial.println("Nie znaleziono pliku HTML (HTM) :( " + String(plik));
    return;  // can't find index file
  }
  Serial.println("Plik strony HTML " + String(plik) + " istnieje :) Serwer za chwile bedzie gotowy :) ");

  Ethernet.begin(mac, ip);
  server.begin();
  Serial.print("Serwer parcuje na adresie IP: ");
  Serial.println(Ethernet.localIP());



}

void loop()
{
  client = server.available(); 
  if (client) {
    Serial.println("new client");

long timerS1;
long timerK1;

    char c;
timerS1 = micros();
    boolean readAll = false;
    
    boolean firstSpace = false; // uzyskiwanie adresu i typu 

    
      /*check what new client wants */
    while (client.connected() && client.available() ) {

      if(!readAll){
        c= client.read();
        if(firstSpace == true){
          HTTP_reqURL[reqURL_index] = c; 
          reqURL_index++;
        }

        if(firstSpace == false && c==' '){
          firstSpace = true;
        }
        else if(firstSpace == true && c== ' '){
          firstSpace = false;
        }

        if(c=='\n'){
          readAll=true;
        }
      }
      /* send necessary file */
      else if(readAll){
//Serial.print("\nData reading time - from browser = client: ");
//Serial.println(micros()-timerS1);
Serial.print("file: ");
Serial.println(HTTP_reqURL);

//long initTimeSTR = micros();
               
          //first message to client = browser
          client.println("HTTP/1.1 200 OK");
        
          if (strstr(HTTP_reqURL, "/ ") || strstr(HTTP_reqURL, "/index.html")) {
              Serial.print("\nindex.html sending...");
              client.println("Content-Type: text/html");
              webFile = SD.open("index.html");        // open web page file
            }
            else if(strstr(HTTP_reqURL, ".css ")){
              Serial.print("style file (css) sending");
              client.println("Content-Type: text/css");
              webFile = SD.open(HTTP_reqURL);        // open web page file
            }
            else if(strstr(HTTP_reqURL, ".html ")){
               Serial.print("other .html sending");
              client.println("Content-Type: text/html");
              webFile = SD.open(HTTP_reqURL);        // open web page file
            }
            else {
              Serial.print("another type of file sending");
//              client.println("Content-Type: text/css");
              client.println("Content-Type: */*");
              webFile = SD.open(HTTP_reqURL);        // open web page file

            }

              client.println("Connnection: close");
              client.println();

//            Serial.print("\nAnswer to browser, time: ");
//            Serial.println(micros()-initTimeSTR);

            /* read from SD card and send to browser = client */
            if (webFile) {
              int number = 512;


              byte clientBuf[number];
//long initTimeFile = micros();
              
               //webFile return int with info how many chars clientBuff consists
              while (webFile.available()){
                client.write(clientBuf, webFile.read(clientBuf, number));
              }

//Serial.print("\nczas wysłania danych " );
//Serial.println(micros() - initTimeFile);

              webFile.close();
            }

            // reset buffer index and all buffer elements to 0
            reqURL_index=0;
            StrClear(HTTP_reqURL, REQ_BUF_SZ);
            break;
        }

      }
timerK1 = micros();
Serial.print("\n czas na odebranie danych ");
Serial.println(timerK1 - timerS1);

      // give the web browser time to receive the data
      delay(1);
      client.stop();
      Serial.println("client disconnected");
    } 
}


// sets every element of str to 0 (clears array) //~30us microseconds 
void StrClear(char *str, char length) {
  for (int i = 0; i < length; i++) {
    str[i] = 0;
  }
}

