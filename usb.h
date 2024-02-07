
#include "USB.h"
#include "USBHIDKeyboard.h"
#include "SPI.h"
#include <time.h>

USBHIDKeyboard Keyboard;

void key_input()
{
  Keyboard.press(KEY_LEFT_GUI);
  Keyboard.press('r');
  Keyboard.releaseAll();
  delay(1000);

 if (SD.exists("/badpayload.txt")) {
    File payloadFile = SD.open("/badpayload.txt", "r");
    if (payloadFile) {
      DISP.setCursor(0, 40);
      DISP.println("from file!");
      String fileContent = "";
      while (payloadFile.available()) {
        fileContent += (char)payloadFile.read();
      }

      payloadFile.close();
      Keyboard.print(fileContent);
    }
  } else {
    // rick
    Serial.println("rick");
    DISP.setCursor(0, 40);
    DISP.println("rick");
    Keyboard.print("https://www.youtube.com/watch?v=dQw4w9WgXcQ");
  }

  delay(1000);
  Keyboard.releaseAll();
  Keyboard.press(KEY_RETURN);
  Keyboard.releaseAll();
    
}


void usb_setup()
{
  Serial.println("BadUSB begin");
  DISP.clear();
  DISP.setTextColor(WHITE, BGCOLOR);
  DISP.setCursor(0, 0);
  
  DISP.setTextSize(MEDIUM_TEXT);  
  DISP.println("Sending...");

  Keyboard.begin();
  USB.begin();

  delay(2000);
  key_input();
  DISP.setCursor(0, 0);
  DISP.setTextColor(GREEN, BGCOLOR);
  DISP.println("PAYLOAD SENT!");
  DISP.setTextColor(FGCOLOR, BGCOLOR);

}

void usb_loop()
{


}

/*

Now cardputer works as a USB Keyboard!

Keyboard functions 
Created by: edulk2, thankss

*/

void keyboard_setup() {
    DISP.clear();
    DISP.setRotation(1);
    DISP.setTextColor(GREEN);
    DISP.setTextDatum(middle_center);
    DISP.setTextSize(2);
    DISP.drawString("Keyboard Started",
                                   DISP.width() / 2,
                                   DISP.height() / 2);
    Keyboard.begin();
    USB.begin();
   DISP.setTextColor(FGCOLOR, BGCOLOR);

}

void keyboard_loop() {
    M5Cardputer.update();
    if (M5Cardputer.Keyboard.isChange()) {
        if (M5Cardputer.Keyboard.isPressed()) {
            Keyboard_Class::KeysState status = M5Cardputer.Keyboard.keysState();

            KeyReport report = {0};
            report.modifiers = status.modifiers;
            uint8_t index    = 0;
            for (auto i : status.hid_keys) {
                report.keys[index] = i;
                index++;
                if (index > 5) {
                    index = 5;
                }
            }
            Keyboard.sendReport(&report);
            Keyboard.releaseAll();

            // only text for display
            String keyStr = "";
            for (auto i : status.word) {
                if (keyStr != "") {
                    keyStr = keyStr + "+" + i;
                } else {
                    keyStr += i;
                }
            }

            if (keyStr.length() > 0) {
                DISP.clear();
                DISP.drawString("Pressed: " + keyStr,
                                           DISP.width() / 2,
                                           DISP.height() / 2);

            }

        }
    }
}

