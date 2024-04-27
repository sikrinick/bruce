# M5-Bruce
Firmware for offensive pranks to Cardputer

![Bruce](https://github.com/pr3y/m5-bruce/blob/main/media/pic4.png) ~thx Luidiblu for the image

## Name and Background
Forked project, same intentions B)
NEMO started a personal project to help me learn more about ESP32 development with the Arduino IDE. I decided to replicate a few common, trending pranks that were getting a lot of attention in the tech community, as a challenge to myself, and to also better understand these attacks.  
NEMO is named after the small, clever and stubborn fish in Finding Nemo. This project stands in contrast to another high-tech gadget that's associated with certain sea-dwelling creatures. I did want to prove that there are a lot of things you can do with a small development kit and some curiosity. I have no delusions of superseding the capabilities of any similar device with this project. It's just for fun, and my own education.  

Hello, my name is Bruce.

![M5Cardputer](https://github.com/pr3y/m5-bruce/blob/main/media/pic1.png)

![M5Cardputer Main Screen](https://github.com/pr3y/m5-bruce/blob/main/media/pic2.png)

![M5Cardputer Wifi Attacks](https://github.com/pr3y/m5-bruce/blob/main/media/pic3.png)

## Features
* [TV B-Gone](http://www.righto.com/2010/11/improved-arduino-tv-b-gone.html) port (thanks to MrArm's [HAKRWATCH](https://github.com/MrARM/hakrwatch)) to shut off many infrared-controlled TVs, projectors and other devices
* [AppleJuice](https://github.com/ECTO-1A/AppleJuice) iOS Bluetooth device pairing spam
* Bluetooth device notification spamming for SwiftPair (Windows) and Android
* WiFi EVIL Portal - A captive portal that tries to social engineer email credentials - saves usernames and passwords to SD Card (if inserted into a supported reader)
* WiFi SSID Scanner - Display 2.4 GHz SSIDs nearby and get information about them
* User-adjustable 24 Hour digital clock backed by the M5 Stick RTC so it holds relatively stable time even in deep sleep and low battery mode
* EEPROM-backed Settings for rotation, brightness and, automatic dimming
* Battery level and credits in settings menu
* SSH Client
* TELNET Client
* DPWO-ESP32
* Raw Sniffer - Saves PCAP to SD
* BadUSB - Reads payloads on SD card and interprets DuckyScript
* Wireguard Tunneling - Reads config file on SD card /wg.conf
* Keyboard - Use as a keyboard USB input
* ARP Scan - Make it better on next version and print as a menu
* Openhaystack - Track your device like an airtag (thx to viniciuspereiras 4 testing)

## User Interface
There are three main controls:
* Home - Stops the current process and returns you to the menu from almost anywhere in BRUCE
* Next - Moves the cursor to the next menu option. In function modes, this usually stops the process and returns you to the previous menu.
* Select - Activates the currently-selected menu option, and wakes up the dimmed screen in function modes  

* StickC and StickC-Plus
  * Power: Long-press the power button for 6 seconds to turn off the unit
  * Home: Tap the power button (closest to the USB port)
  * Next: Tap the side button
  * Select: Tap the M5 button on the front of the unit  

* Cardputer
  * Home: Tap the Esc/~/` key or the Left-Arrow/, key
  * Next/Prev: Tap the Down-Arrow/. key and Up-Arrow/; keys to navigate
  * Select: Tap the OK/Enter key or Right-Arrow/? key  

## EVIL Portal
In EVIL Portal mode, BRUCE reads the keyboard input for the SSID and activates a open WiFi, with DNS, DHCP and Web servers activated. 
* EVIL Portal serves a fake login page that claims to provide internet access if you log in.
* This is a social engineering attack, and will log the username and passwords entered on the page. 
* You can view these credentials by connecting to the portal from your own device and browsing to http://172.0.0.1/creds
* If your device has an SD Card reader with a FAT filesystem formatted card inserted, the usernames and passwords will be logged to nemo-portal-creds.txt on the SD Card for you to peruse later. 
* SD Card support is only enabled by default on the M5Stack Cardputer platform. It can be enabled on M5Stick devices but an SD Card reader must be built and attached to the front panel pin header.
* EVIL Portal is only for use on professional engagements with a valid scope of work, educational or demonstration purposes. Storage, sale, or use of personal information without consent is against the law. 🤓

## BadUSB
**NOW BRUCE PARSE PAYLOADS LIKE FLIPPER!!**

Now only DuckyScript payloads are supported!! for more info on creating your own DuckyScripts [read here](https://docs.hak5.org/hak5-usb-rubber-ducky/ducky-script-basics/hello-world)

To choose a payload for the BadUSB on Cardputer instead of getting rickrolled, you need to create a file on the SD card root directory ending with ".txt".
You can then select which payload that will be sent when the Cardputer is connected via USB cable.

## Wireguard
To be able to connect to a wireguard tunnel with your cardputer easily, you need to have your .conf file and place on the SD card root directory called "wg.conf".

## Openhaystack
Now its possible to use the cardputer as a AirTag! thanks to [Openhaystack](https://github.com/seemoo-lab/openhaystack).

To make it work, you should get your Public key encoded with base64 and save it on a file on the SD root called "pub.key".
To create pub.key file you should run this in bash:
```sh
base64 -d <<< "your_base64_public_key"|tee pub.key
```
## NFC / RFID
It is now possible to use the M5Stack RFID2 (WS1850S) to read and write Mifare Classic cards and tags. From what we investigated, this device does not allow emulation, so we recommend that you use an adhesive NFC tag on your Cardputer.
New functions involving NFC will be developed in the coming seasons.

To use, simply access the NFC / RFID menu, approach the card to which it will be read, press "ENTER" and approach the card to which it will be written. To exit, press "ESC".
Languages ​​in English and Brazilian Portuguese and some error messages were inserted.

By: @IncursioHack

![NFC](https://github.com/pr3y/m5-bruce/blob/main/media/nfc.gif)

## Install from M5Burner
This is the absolute easiest way to get BRUCE
* [M5Stick C Plus Quick Start](https://docs.m5stack.com/en/quick_start/m5stickc_plus/uiflow) has links to the M5Burner app for Linux, MacOS and Windows. This is the official tool to install UIFlow and other official firmware. I provide up-to-date binaries for BRUCE there.
* Launch M5Burner
* Select "STAMPS3" from the menu on the left (or StampS3 for Cardputer)
* Use the search at the top of the app to look for "BRUCE". My official builds will be uploaded by "owner" and have photos.
* Click Download
* Click Burn

## Install .bin files manually with esptool.py
* Install ESP-IDF tools per the [Espressif Getting Started Guide](https://docs.espressif.com/projects/esp-idf/en/latest/esp32/get-started/)
* Open the esp-idf CMD tool (on Windows) - on Mac or Linux, esp-idf.py and esptool.py should be in the system path.
* esptool.py --port COMPORT -b 115200 write_flash -z 0x0 bruce-VERSION.bin
  * port may be a COM port e.g. COM4, COM11 on Windows. On Mac and Linux it will usually be in /dev such as /dev/ttyUSB0, /dev/ttyACM0 or /dev/cu.usbserial-3
  * bruce-VERSION.bin should be a version you downloaded from a GitHub Release -- preferably the latest available.

## Building from Source
If you want to customize BRUCE or contribute to the project, you should be familiar with building BRUCE from source.
* Install VS Code + PlatformIO plugin.
* Un-comment the appropriate `#define` line near the top for your platform (STICK_C, STICK_C_PLUS or CARDPUTER)
* Probably, you would need to add boards and libraries with `pio install`
* Configuration
  * The code should compile cleanly and work on an M5Stick C Plus out of the box from the master branch or a release tag.
  * Uncomment only the one appropriate `#define` option or compiler errors will occur. 
  * If for some reason the screen jumps from very dim at level 0 to almost fully bright at level 1 and further brightness levels don't affect anything, set the pct_brightness variable to false.
  * You cannot send the deauth packet by default, so you have to [follow this instructions to get the deauth prerequisites](https://github.com/bmorcelli/m5stick-nemo/tree/main/deauth_prerequisites) and patch your platform.txt
* Compile and upload the project

## Troubleshooting
* Several features output debugging information to the serial monitor. Use the Serial Monitor feature in Arduino IDE, PlatformIO or M5Burner to gather this information. It may have useful hints. When filing a bug report, it often helps to include serial monitor output.
* Reset the EEPROM. On models with EEPROM settings support, use "Clear Settings" from the settings menu, or hold the "Next" button (Side key on StickC models, Tab or Down Arrow on Cardputer) while powering on. 
* TV-B-Gone's IR LED can be observed through a smart phone camera, emitting a pale purple beam of light. If it seems to be on constantly, or if it never flashes at all during TV-B-Gone operations, something is wrong. Report a bug. There's a known issue with TVBG not working after using Bluetooth spam or random wifi spam.
* Try viewing wifi lists from several different devices if you suspect wifi spam isn't working. Sometimes, Linux network manager can see networks that smart phones cannot. Please include the results of this testing if reporting wifi spam problems.
* Apple has patched a lot of Bluetooth stuff since summer 2023. If testing AppleJuice, try some of the AppleTV device types, as they tend to be more reliable due to apple not filtering out weaker bluetooth signals for that platform.
* While compiling, you can have the error ```fatal error: USBHIDKeyboard.h: No such file or directory```, if thats the case, [import this library as .zip](https://github.com/espressif/arduino-esp32/releases/tag/2.0.14) and recompile.
## Reporting Bugs
Please report bugs via GitHub Issues. These are easier to track than comments on social media posts, M5Burner entries, etc. If something isn't working, please include:
* Firmware version
* How you installed it (M5Burner, compiled yourself, esptool.py)
* Hardware 
* Specifically what feature and options are not working
* How you determined that it is not working and what testing you did. Include device models and operating systems of what you tested with, and any errors or relevant output from Serial Monitor, if applicable.
* If you figure out how to fix a bug you identified, PRs are welcome!

## Contributing
Contributions are welcome. Please look at the GitHub Issues for ideas to help enhance the project. When submitting a Pull Request, please target the develop branch. The easiest way to do this is to fork ALL branches, or to simply create a "develop" branch in your own fork, then use GitHub to Sync your develop branch.  Take note of how certain hardware (like the LED and RTC) are defined and gated in the code and try to stick to those patterns.

Things I'd like help on:
* A new "Region" of TV-B-Gone that's full of additional IR codes that can turn RGB LED strips, air conditioners, fans, sound bars and the like on and off
* Help refactoring how menus work. The way it was built was fine when it was one main menu with a few options. Now, adding a new sub-menu involves a lot of duplicated code.
* Responder for ESP32 (Need to port lib from [ESP8266LLMNR](https://github.com/esp8266/Arduino/tree/master/libraries/ESP8266LLMNR) to ESP32 [as discussed here](https://www.esp8266.com/viewtopic.php?p=91251))
* SMB client
* Simple network scan (Already with ARP scan but results of it should be a menu, being able to select hosts and check ports)
* Bluetooth Keyboard
* Adding support for RFID (IncursioHack) and other m5-stack modules such as ethernet cable

Things I probably won't merge in:
* Bulk wifi deauthentication spamming
* Bluetooth spam that potentially disrupts health and fitness trackers, smart watches, etc.

## References
https://github.com/spacehuhn/ArduinoPcap

https://github.com/n0xa/m5stick-nemo

https://github.com/m5stack/M5Cardputer

https://github.com/caioluders/DPWO

https://github.com/seemoo-lab/openhaystack/tree/main/Firmware/ESP32
