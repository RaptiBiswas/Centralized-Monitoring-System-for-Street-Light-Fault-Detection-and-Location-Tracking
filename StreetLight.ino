#include <WiFi.h>
#include <WiFiClient.h>
#include <WebServer.h>
#include <ESPmDNS.h>

const char *ssid = "Dream";
const char *password = "loveyourself22";
const int ldr1 = 33;
const int ldr2 = 32;
const int ldr3 = 35;

WebServer server(80);
void handleRoot();


String readLDR(int pin) {
  // Sensor readings may also be up to 2 seconds
  int t = analogRead(pin);
  if (isnan(t)) {    
    Serial.println("Failed to read from LDR sensor!");
    return "error";
  }
  else {
    Serial.println(t);
    if(t<1365){
      return "Faulty" ;
    }
    else if(1365<t && t<2730)
    {
      return "Dim";
    }
    else if(2730<t && t<4095)
    {
        return "Fine";
    }
  }
  
}

void handleRoot() {
  char msg[1500];

  snprintf(msg, 1500,
           "<html>\
  <head>\
    <meta http-equiv='refresh' content='2'/>\
    <meta name='viewport' content='width=device-width, initial-scale=1'>\
    <link rel='stylesheet' href='https://use.fontawesome.com/releases/v5.7.2/css/all.css' integrity='sha384-fnmOCqbTlWIlj8LyTjo7mOUStjsKC4pOpQbqyi7RrhN7udi9RwhKkMHpvLbHG9Sr' crossorigin='anonymous'>\
    <title>ESP32 LDR</title>\
    <style>\
    html { font-family: Arial; display: inline-block; margin: 0px auto; text-align: center;}\
    h2 { font-size: 3.0rem; }\
    p { font-size: 3.0rem; }\
    .ldr-labels{ font-size: 1.5rem; vertical-align:middle; padding-bottom: 15px;}\
    </style>\
  </head>\
  <body>\
      <h2>Street Light fault Detector</h2>\
      <p>\
        <i class='fas fa-lightbulb' style='color:#ca3517;'></i>\
        <span class='ldr-labels'>Street Light 1 Status:</span>\
        <span>%s</span>\
      </p>\
      <p>\
        <i class='fas fa-lightbulb' style='color:#ca3517;'></i>\
        <span class='ldr-labels'>Street Light 2 Status:</span>\
        <span>%s</span>\
      </p>\
      <p>\
        <i class='fas fa-lightbulb' style='color:#ca3517;'></i>\
        <span class='ldr-labels'>Street Light 3 Status:</span>\
        <span>%s</span>\
      </p>\
  </body>\
</html>",
           readLDR(ldr1), readLDR(ldr2), readLDR(ldr3)
          );
  server.send(200, "text/html", msg);
}

void setup(void) {
  Serial.begin(115200);
  WiFi.mode(WIFI_STA);
  WiFi.begin(ssid, password);
  Serial.println("");

  // Wait for connection
  while (WiFi.status() != WL_CONNECTED) {
    delay(500);
    Serial.print(".");
  }

  Serial.println("");
  Serial.print("Connected to ");
  Serial.println(ssid);
  Serial.print("IP address: ");
  Serial.println(WiFi.localIP());

  if (MDNS.begin("esp32")) {
    Serial.println("MDNS responder started");
  }
  server.on("/", handleRoot);

  server.begin();
  Serial.println("HTTP server started");
}

void loop(void) {
  server.handleClient();
  delay(2);//allow the cpu to switch to other tasks
}
