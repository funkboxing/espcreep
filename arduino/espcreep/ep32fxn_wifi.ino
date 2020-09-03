//========================================
//==============WIFI FXNS=================
//========================================
#include <Preferences.h>

//===EEPROM VALS (hardcoded for testing, will be replaced with EEPROM read\writable vals)
String wifissid = "NCC-74656";
String wifipass = "alphaeureka";
String wifihost = "esp32cambot";

void WiFiConnectToAP(){
  WiFi.disconnect(true);
  delay(1000);
  WiFi.onEvent(WiFiEvent);
  WiFi.mode(WIFI_STA);  //---SUPPOSEDLY PREVENTS WIFI POWERSAVE SLEEP, BETTER RESPONSE, USES MORE POWER
  WiFi.setSleep(false); //---ALSO POWER RELATED
  dbo("---CONNECTING TO WIFI: "); dbo(wifissid); dbo("|"); dbo(wifipass); dbo("\n"); delay(5);  
  while (WiFi.status() != WL_CONNECTED){
    WiFi.begin(wifissid.c_str(), wifipass.c_str());
    delay(1000);
  }
}

void WiFiEvent(WiFiEvent_t event){
    switch(event) {
    case SYSTEM_EVENT_STA_GOT_IP:
      blinkInteger(4, 300);
      break;
    case SYSTEM_EVENT_STA_DISCONNECTED:
      blinkInteger(6, 150);    
      break;
    case SYSTEM_EVENT_STA_START:
      dbo("   WiFiEVENT: SYSTEM_EVENT_STA_START\n");
      WiFi.setHostname(wifihost.c_str());
      break;        
    }
}
