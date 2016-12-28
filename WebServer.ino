/*
  WebServer by Dominik Bogacki
  1. comunication by local host server -
  2. increased reading speed from SD card by buffor
  3. send file which browser wants (auto file directory localization)
  4.



  // karta musi być w formacie FAT ustawienie 32KB
*/

#include <SPI.h>
#include "SdFat.h"
#include <SDFatUtil.h>
#include <Ethernet.h>

#define REQ_BUF_SZ   50

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
  client = server.available(); // może to wyżej??? trzeba sprawdzić czy to tworzy co chwila nowego?
  if (client) {
    Serial.println("new client");
    boolean currentLineIsBlank = true;
    long initTime = millis();

    //                  char c;
    //  char get[] = {0};
    long timerS1;
    long timerK1;

    //if new client connected

    char c;
    timerS1 = micros();
    char url[30] = {0};
    boolean readAll = false;
    
    boolean firstSpace = false; // uzyskiwanie adresu i typu 
      byte  newLine=0;      //uzyskiwanie adresu i typu
      
    while (client.connected() ) {
//      && client.available()
//      Serial.println(" ");
//      Serial.print("HTTP_reqURL is: ");
//      Serial.println(HTTP_reqURL);
//      Serial.print("HTTP_reqTEXT is: ");
//      Serial.println(HTTP_reqTEXT);
//      Serial.println(" ");
//      if (client.available()) {
      if(!readAll){
        c= client.read();
        if(firstSpace == true && newLine==0 && c!=' '){
          HTTP_reqURL[reqURL_index] = c; 
          reqURL_index++;
        }
        else if(firstSpace ==true && newLine==6 && c!=','){
          HTTP_reqTEXT[reqTEXT_index]=c;
          reqTEXT_index++;
        }
        if(firstSpace == false && c==' ' && (newLine==0 || newLine==6)){
          firstSpace = true;
        }
        else if(c== ' ' && firstSpace == true && newLine==0){
          firstSpace = false;
        }
        else if(c==' ' && firstSpace == false && newLine==0){
          firstSpace =true;
        }
        else if(c==',' && newLine==6 || !client.available()){
          readAll=true;
          firstSpace = false;
        }

        if(c=='\n'){
          newLine++;
          readAll=true;
        }
//        Serial.print("ilosc nowych lini");
//        Serial.println(newLine);

         
//        if (reqURL_index < (REQ_BUF_SZ - 1)) {
//         HTTP_reqURL[reqURL_index] = c;           // save HTTP request character
//          reqURL_index++;
//                    continue;
//
//        }
//        else{
//          readAll=true;
//        }
//       if(!client.available()){
//          readAll=true;
//          Serial.println("klient juz nie dostepny");
//        }
    
//        Serial.write(c);
      }
      else if(readAll){
//        else{
          Serial.print("file: ");
          Serial.println(HTTP_reqURL);
          Serial.print("text type: ");
          Serial.println(HTTP_reqTEXT);
     
        //        Serial.print(url);

        // if you've gotten to the end of the line (received a newline
        // character) and the line is blank, the http request has ended,
        // so you can send a reply
//        if (c == '\n' && currentLineIsBlank) {
          // open requested web page file
//               HttpReqURL(url, HTTP_reqURL);
          //                    Serial.println(" \n test programu: ");
          //                    for(int i=0; i<30;i++){
          //                      Serial.print(url[i]);
          //                    }
        
         long initTimeSTR=micros();
        Serial.println(strstr(HTTP_reqURL, "/ ")? "TAK html" : "NIE html");
        Serial.println(strstr(HTTP_reqURL,".css") ? "css file" : "not css file");
        Serial.println(strstr(HTTP_reqURL, ".js") ? "js file" : "not js file");
        Serial.println(strstr(HTTP_reqURL, ".htm") ? "htm file" : "not htm file");
        Serial.println(strstr(HTTP_reqURL, "image") ? "image folder" : "not image folder");
        Serial.println(strstr(HTTP_reqURL, ".ipg") ? "jpg file" : "not ipg file");
        Serial.println(strstr(HTTP_reqURL, ".tiff") ? "tiff file" : "not tiff file");
        Serial.print("\n Time for strstr");
        Serial.println(micros()-initTimeSTR); 
          if ((strstr(HTTP_reqURL, "/")
              || strstr(HTTP_reqURL, "GET /index.html"))&& strlen(HTTP_reqURL)==1) {
            
              client.println("HTTP/1.1 200 OK");
              client.println("Content-Type: text/html");
              client.println("Connnection: close");
              client.println();
              webFile = SD.open("index.html");        // open web page file
            }
//            else if(strstr(HTTP_reqURL, ".ttf") || strstr(HTTP_reqURL, ".woff")){
//              Serial.print("czcionka woff, tiff itd");
//              client.println("HTTP/1.1 200 OK");
////              client.println("Content-Type: text/css");
//              client.println("Content-Type: */*");
//              client.println("Connnection: close");
//              client.println();
//              webFile = SD.open(HTTP_reqURL);
//            }
            else {
              Serial.print("inne opcje");
              client.println("HTTP/1.1 200 OK");
              client.println("Content-Type: text/css");
//              client.println("Content-Type: */*");
              client.println("Connnection: close");
              client.println();
              webFile = SD.open(HTTP_reqURL);
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
              long initTimeFile = micros();
              long timeFile;

              uint8_t b;
              initTimeFile = micros();
              while (webFile.available())
              {
                //
                webFile.read(clientBuf, number);
                //                clientBuf[clientCount] = webFile.read(&clientBuf, 2);
                //                clientCount++;

                //                if(clientCount > number-1)
                //                {
                //
                client.write(clientBuf, number);
                //                  clientCount = 0;
                //                }
              }
              timeFile = micros() - initTimeFile;

              Serial.print("\nczas wysłania danych " );
              Serial.println(timeFile);

              if (clientCount > 0) client.write(clientBuf, clientCount);
              webFile.close();

            }


            // reset buffer index and all buffer elements to 0
            reqTEXT_index = 0;
            reqURL_index=0;
            StrClear(HTTP_reqURL, REQ_BUF_SZ);
            StrClear(HTTP_reqTEXT, REQ_BUF_SZ);
            break;
//          }
//
//          if (c == '\n') {
//            // you're starting a new line
//            currentLineIsBlank = true;
//          }
//          else if (c != '\r') {
//            // you've gotten a character on the current line
//            currentLineIsBlank = false;
//          }

        }

      }
      timerK1 = micros();
      Serial.print("\n czas na odebranie danych ");
      Serial.println(timerK1 - timerS1);

      long stopTime = millis();
      Serial.print("\nCzas petli to: ");
      Serial.println(stopTime - initTime);
      // give the web browser time to receive the data
      delay(1);
      // close the connection:
      client.stop();
      Serial.println("client disconnected");
    }

    //  for(int i=0; i<REQ_BUF_SZ - 1;i++){
    //    Serial.print(HTTP_req[i]);
    //  }
  
}
// sets every element of str to 0 (clears array)
void StrClear(char *str, char length) {
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

void HttpReqURL(char *url, char *str) {
  boolean firstSpace = false;

  //  boolean secondSpace = false;
  byte len = strlen(str);
  byte j = 0;
  for (byte i = 0; i <= len; i++) {

    if (firstSpace == true && str[i]!=' ') {
      url[j] = str[i];
      j++;
    }
    if (str[i] == ' ' && firstSpace == false) {
      firstSpace = true;
    }
    else if (str[i] == ' ' && firstSpace == true) {
      firstSpace = false;
      break;
    }


  }

  if(strlen(url)==1){
    url="/index.html";
  }

}


void HttpRequest (char *sign, char *url, char *type){
  boolean firstSpace =false;
  byte newLine =0;
  
  if(firstSpace = true){
    
  }
}

