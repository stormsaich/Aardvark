#include <SPI.h>
#include <Ethernet.h>

#include <SdFat.h>
#include <SdFatUtil.h>
#include <aJSON.h>

#include "WebServer.h"

WebServer aardvarkHttp = WebServer();

//networking info
byte mac[] = {
  0xDE, 0xAD, 0xBE, 0xEF, 0xFE, 0xED };// this should be fine 
byte ip[] = {
  192,168,10,190}; // chnage to your ipyou want the arduino to be
IPAddress gateway(192,168,10,254);//Change to your Gate way	
IPAddress subnet(255, 255, 255, 0);//change to your subnet 
EthernetServer server(80);// change this if you dont want to use the standard port


///////////////////////////////////////////////////////////////////////////////
//The setup Function
///////////////////////////////////////////////////////////////////////////////
void setup() {
  
  // set up serial comunication
  Serial.begin(115200);
  aardvarkHttp.initSD();

  // start the ethernet chip and server
  Ethernet.begin(mac);// everyting is auto set by your network
  //Ethernet.begin(mac, ip, gateway, subnet); //if you want manual set of parameters
  server.begin();
  Serial.println(Ethernet.localIP());
  //extra setup for other applications
  pinMode(13, OUTPUT);
}

///////////////////////////////////////////////////////////////////////////////
//The Loop Function
///////////////////////////////////////////////////////////////////////////////
void loop()
{
  aardvarkHttp.processRequest(server); 
  //place any extra code for the loop here 

}

///////////////////////////////////////////////////////////////////////////////
//place request action code here
///////////////////////////////////////////////////////////////////////////////

void requestAction( EthernetClient client, String request, String requestValue){

  client.print("{");

  if (request == "pin13"){
    if (requestValue == "HIGH"||requestValue == "high"){
      digitalWrite(13, HIGH);
      Serial.println("LED ON");
      client.print("'pin13','on'");
      client.print("}");
      return;

    }
    if (requestValue == "LOW"||requestValue == "low"){
      digitalWrite(13, LOW);
      Serial.println("LED OFF");
      client.print("'pin13','off'");
      client.print("}");
      return;
    }
  }

  else
    if (request == "delay"){
      char time[requestValue.length()+1];
      requestValue.toCharArray(time,requestValue.length()+1);
      Serial.print("delay ");
      Serial.println(atoi(time));
      client.print("'delay',");
      client.print(atoi(time));
      client.print("}");
      delay(atoi(time));

      return;
    }
  client.print("'notfound'");
}





