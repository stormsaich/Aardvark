#include "Arduino.h"
#include "WebServer.h"

//Memory Buffers
uint8_t bufindex;
const uint8_t maxbyte=100;
uint8_t buf[maxbyte];

// sd Card
Sd2Card sys_card;
SdVolume sys_volume;
SdFile sys_root;
SdFile sys_file;


// buffer size for reading requests
#define BUFSIZ 110
#define error(s) error_P(PSTR(s))

WebServer::WebServer(){
  Serial.println("Http Server Started");
}

void WebServer::processRequest(EthernetServer server){
  char rootFileName[] = "index.htm";
  char clientline[BUFSIZ];
  char *requestString;

  int index = 0;
  EthernetClient client = server.available();
  if (client) {
    boolean current_line_is_blank = true;
    index = 0;
    while (client.connected()) {
      if (client.available()) {
        char c = client.read();
        if (c != '\n' && c != '\r') {
          clientline[index] = c;
          index++;
          if (index >= BUFSIZ)
            index = BUFSIZ -1;
          continue;
        }
        clientline[index] = 0;
        requestString = 0;
        Serial.println(clientline);
        if (strstr(clientline, "GET / ") != 0) {
          requestString = rootFileName;
          SendFile(client, requestString);
        }
        else 
          if (strstr(clientline, "GET /") != 0) {

          //if(clientline[5] == '?'){
          if(strstr(clientline,"?")){

            //ignore any page request just look at the GET
            requestString = strstr(clientline,"?") + 1;
            (strstr(clientline, " HTTP"))[0] = 0;
            //Serial.println(requestString);
            readRequest(client, requestString);
          } 
          else {
            requestString = clientline + 5;
            (strstr(clientline, " HTTP"))[0] = 0;
            //Serial.println(requestString);
            SendFile(client, requestString);
          } 

        }

        break;
      }
    }
    delay(1);
    client.stop();
  }
}

void WebServer::initSD(){
  //free ram
  PgmPrint("Free RAM: ");
  Serial.println(FreeRam());
  //set pin mode to allow sd card to work 
  pinMode(10, OUTPUT);
  digitalWrite(10, HIGH);
  //initalise the sd card
  if (!sys_card.init(SPI_FULL_SPEED, 4)) error("card.init failed!");
  if (!sys_volume.init(&sys_card)) error("vol.init failed!");
  //SD card stats
  PgmPrint("Volume is FAT");
  Serial.println(sys_volume.fatType(),DEC);
  Serial.println();
  if (!sys_root.openRoot(&sys_volume)) error("openRoot failed");
  PgmPrintln("Files found in root:");
  sys_root.ls(LS_DATE | LS_SIZE);
  Serial.println();
  // list all files
  //PgmPrintln("Files found in all dirs:");
  //root.ls(LS_R);
  Serial.println();
  PgmPrintln("Done");

}

//log to the server log file
void WebServer::log(const char* str){

}

long WebServer::time(){
  
}




/*
* Private function implementation
*
*/


//
void WebServer::error_P(const char* str) {
  PgmPrint("error: ");
  SerialPrintln_P(str);
  if (sys_card.errorCode()) {
    PgmPrint("SD error: ");
    Serial.print(sys_card.errorCode(), HEX);
    Serial.print(',');
    Serial.println(sys_card.errorData(), HEX);
  }
  while(1);
}

void WebServer::readRequest(EthernetClient client, String request){
  boolean reading = true;
  int bitBuffer = 0;
  client.println("HTTP/1.1 200 OK");
  client.println("Content-Type: application/json");
  client.println();
  client.println("[");

  while (reading){
    String requestId;
    String requestValue;
    while(request.charAt(bitBuffer) != '='){
      requestId += request.charAt(bitBuffer);
      bitBuffer++;
    }
    //add one to the buffer to skip the '&'
    bitBuffer++;
    while(request.charAt(bitBuffer) != '&'){
      requestValue += request.charAt(bitBuffer);

      if (request.length() == bitBuffer){
        Serial.println("end of request");
        reading = false;
        break;
      }
      else{
        bitBuffer++;
      }
    }
    //client.print("{");
    //requestAction(client,requestId,requestValue);
    //client.print("}");
    if (request.length() != bitBuffer){
      client.println(",");
      //add one to the buffer to skip the '='
      bitBuffer++;
    }
  }
  client.print("]");
}


void WebServer::SendFile(EthernetClient client, char *filename){
  char error404File[] = "404error.htm";
  if (! sys_file.open(&sys_root, filename, O_READ)) {
    // set the file name to the error file
    filename = error404File;
    Serial.println("Can not find file: 404!");
    client.println("HTTP/1.1 404 Not Found");
    if (!sys_file.open(&sys_root, filename, O_READ)){
      Serial.println("No 404 file found, is there one?");
      client.println();
      client.println("<h1>404! No 404 error page found!</h1>");
      return;
    }
  }
  else{
    Serial.println("file founded opening!");
    client.println("HTTP/1.1 200 OK");
  }
  if (strstr(filename, ".htm") != 0)
    client.println("Content-Type: text/html");
  else if (strstr(filename, ".html") != 0)
    client.println("Content-Type: text/css");
  else if (strstr(filename, ".css") != 0)
    client.println("Content-Type: text/css");
  else if (strstr(filename, ".png") != 0)
    client.println("Content-Type: image/png");
  else if (strstr(filename, ".jpg") != 0)
    client.println("Content-Type: image/jpeg");
  else if (strstr(filename, ".gif") != 0)
    client.println("Content-Type: image/gif");
  else if (strstr(filename, ".3gp") != 0)
    client.println("Content-Type: video/mpeg");
  else if (strstr(filename, ".pdf") != 0)
    client.println("Content-Type: application/pdf");
  else if (strstr(filename, ".jso") != 0)
    client.println("Content-Type: application/json");
  else if (strstr(filename, ".js") != 0)
    client.println("Content-Type: application/x-javascript");
  else if (strstr(filename, ".xml") != 0)
    client.println("Content-Type: application/xml");

  else
    client.println("Content-Type: text");

  client.println();
  int16_t c;
  bufindex=0;
  while ((c = sys_file.read()) >= 0) {
    buf[bufindex++]=((char)c);
    if(bufindex==maxbyte)
    {
      client.write(buf, maxbyte);
      bufindex=0;
    }
  }
  sys_file.close();
  if(bufindex>0)
  {
    client.write(buf, bufindex);
  }
  bufindex=0;
}



