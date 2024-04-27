#include <WireGuard-ESP32.h>

char private_key[45];
IPAddress local_ip;
char public_key[45];
char endpoint_address[16];
int endpoint_port = 31337;              

static constexpr const uint32_t UPDATE_INTERVAL_MS = 5000;

static WireGuard wg;

void parse_config_file(File configFile) {
  String line;

  while (configFile.available()) {
    line = configFile.readStringUntil('\n');
    Serial.println("==========PRINTING LINE");
    Serial.println(line);
    line.trim();

    if (line.startsWith("[Interface]") || line.isEmpty()) {
      // Skip [Interface] or empty lines
      continue;
    } else if (line.startsWith("PrivateKey")) {
      line.remove(0, line.indexOf('=') + 1);
      line.trim();
      Serial.println("Private Key: " + line);
      strncpy(private_key, line.c_str(), sizeof(private_key) - 1);
      private_key[sizeof(private_key) - 1] = '\0'; // Ensure null-terminated
    } else if (line.startsWith("Address")) {
      line.remove(0, line.indexOf('=') + 1);
      line.trim();
      Serial.println("Local IP: " + line);
      int slashIndex = line.indexOf('/');
      
      if (slashIndex != -1) {
        Serial.println("~~~~~~~~~~~~");
        Serial.println(line.substring(0, slashIndex));
        local_ip.fromString(line.substring(0, slashIndex));
      }

    } else if (line.startsWith("[Peer]")) {
      // Handle [Peer] section
    } else if (line.startsWith("PublicKey")) {
      line.remove(0, line.indexOf('=') + 1);
      line.trim();
      Serial.println("Public Key: " + line);
      strncpy(public_key, line.c_str(), sizeof(public_key) - 1);
      public_key[sizeof(public_key) - 1] = '\0'; // Ensure null-terminated
    } else if (line.startsWith("Endpoint")) {
      //Serial.println("~~~~~~~~~~~endpoint");
      //Serial.println(line);
      line.remove(0, line.indexOf('=') + 1);
      line.trim();
      int colonIndex = line.indexOf(':');

      if (colonIndex != -1) {
        //Serial.println("Endpoint Line: " + line);
        strncpy(endpoint_address, line.substring(0, colonIndex).c_str(), sizeof(endpoint_address) - 1);
        endpoint_address[sizeof(endpoint_address) - 1] = '\0'; // Ensure null-terminated
        Serial.println("Endpoint Address: " + String(endpoint_address));
        endpoint_port = line.substring(colonIndex + 1).toInt();
        Serial.println("Endpoint Port: " + String(endpoint_port));
      }
    }
  }

  Serial.println("Closing file!");
  configFile.close();
}



void read_and_parse_file() {
  if (!SD.begin(SS)) {
    Serial.println("Failed to initialize SD card");
    return;
  }

  File file = SD.open("/wg.conf");
  if (!file) {
        DISP.clear();
    DISP.setCursor(0, 0);

    DISP.setTextColor(RED, BGCOLOR);
    Serial.println("Failed to open file");
    DISP.println("No wg.conf file\nfound on\nthe SD");
    DISP.setTextColor(FGCOLOR, BGCOLOR);
    delay(60000);
    return;
  }

  Serial.println("Readed config file!");

  Serial.println("Found file!");
  parse_config_file(file);


}

void wg_setup()
{
    read_and_parse_file();

    Serial.println("Adjusting system time...");
    configTime(9 * 60 * 60, 0, "ntp.jst.mfeed.ad.jp", "ntp.nict.jp");
    DISP.clear();
    DISP.setCursor(0, 0);

    Serial.println("Connected. Initializing WireGuard...");
    DISP.println("Connecting to\nwireguard...");
    wg.begin(
        local_ip,
        private_key,
        endpoint_address,
        public_key,
        endpoint_port);
    Serial.println(local_ip);
    Serial.println(private_key);
    Serial.println(endpoint_address);
    Serial.println(public_key);
    Serial.println(endpoint_port);

    DISP.clear();
    DISP.setCursor(0, 0);

    DISP.setTextColor(GREEN, BGCOLOR);
    DISP.println("Connected!");
    DISP.setTextColor(FGCOLOR, BGCOLOR);
    DISP.println("IP on tunnel:");
    DISP.setTextColor(WHITE, BGCOLOR);
    DISP.println(local_ip);
    DISP.setTextColor(FGCOLOR, BGCOLOR);
    Serial.println(local_ip);

}

void wg_loop()
{

}