#ifndef WebServer_h
#define WebServer_h

#include <SdFat.h>
#include <SdFatUtil.h>
#include <Ethernet.h>


class WebServer
{
public:
  WebServer();
  void initSD();
  void processRequest(EthernetServer server);
  void log(const char* str);
  long time();
  
  
private:
  void error_P(const char* str);
  void readRequest(EthernetClient client, String request);
  void SendFile(EthernetClient client, char *filename);
};

#endif

