
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

    // PLACE YOUR COOL PAYLOAD HERE XD *wink
    Keyboard.print("https://www.youtube.com/watch?v=dQw4w9WgXcQ");

    delay(1000);
    Keyboard.releaseAll();
    Keyboard.press(KEY_RETURN);
    
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
