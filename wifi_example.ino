#include <WiFi.h>


// connect to ap
int iStatus = WL_IDLE_STATUS;
char szSsid[] = "kerkerker";          //  network SSID (name) 
char szPass[] = "safesync";   // network password


// send a request
char szServername[]="www.google.com";  // remote server we will connect to
WiFiClient client;



// create webserver
WiFiServer server(80);

void setup(){
  Serial.begin(9600);
  fnConnectToAP();
//  fnSendRequest();
  fnStartServer();
}

void loop(){
  fnWaitingRequest();
//  fnReadData();
}

void fnSendRequest(){
  if (client.connect(szServername, 80)) {
    Serial.println("connected");
    
    // Send a HTTP request:
    client.println("GET /search?q=arduino HTTP/1.1");
    client.println("Host: www.google.com");
    client.println();
  }
    
}
void fnReadData(){
  while (client.available()) {
    char c = client.read();
    Serial.print(c);
  }
  
  // if the server's disconnected, stop the client:
  if (!client.connected()) {
    Serial.println();
    Serial.println("disconnecting from server.");
    client.stop();
  
    // do nothing forevermore:
    while(true);
  }  
}

void fnConnectToAP(){
  while ( iStatus != WL_CONNECTED) { 
    Serial.println("Connecting to network...");
    Serial.print("SSID: ");
    Serial.println(szSsid);
    delay(1000);
    iStatus = WiFi.begin(szSsid, szPass);
  }  
  
  Serial.print("Connected successfully. IP address:");
  IPAddress myAddress = WiFi.localIP();
  Serial.println(myAddress);  
}

void fnScanNetwork(){
  
  int iNum = WiFi.scanNetworks();
  Serial.print("=========number of available networks:");
  Serial.print(iNum);
  Serial.println("=========");
  
  // print the each SSID and Signal
  for (int i = 0; i<iNum; i++) {
    Serial.print(WiFi.SSID(i));  // pring SSID
    Serial.print("\tSignal: ");
    Serial.print(WiFi.RSSI(i));  // print Signal
    Serial.print(" dBm");
    Serial.print("\tEncryption: ");
    Serial.println(WiFi.encryptionType(i));  // print Encryption Type
  }  
  
}

void fnWaitingRequest(){
  WiFiClient client = server.available();
  
  if (client) {
    Serial.println("new client comming");
    
    boolean bCurrentLineIsBlank = true;
    while (client.connected()) {
      if (client.available()) {
        char c = client.read();
        Serial.write(c);
        // if you've gotten to the end of the line and the line is blank, the http request has ended,
        // so you can send a reply
        if (c == '\n' && bCurrentLineIsBlank) {
          // send a standard http response header
          client.println("HTTP/1.1 200 OK");
          client.println("Content-Type: text/html");
          client.println("Connection: close");  // the connection will be closed after completion of the response
          client.println();
          client.println("<!DOCTYPE HTML>");
          client.println("<html>");
          client.print("Hello World ");

          client.println("</html>");
          break;
        }
        if (c == '\n') {
          // you're starting a new line
          bCurrentLineIsBlank = true;
        } else if (c != '\r') {
          // you've gotten a character on the current line
          bCurrentLineIsBlank = false;
        }
      }
    }
    // give the web browser time to receive the data
    delay(1);
    
    // close the connection:
    client.stop();
    Serial.println("client disonnected");
  }  
  
}



void fnStartServer(){
  delay(1000);
  server.begin();
}





