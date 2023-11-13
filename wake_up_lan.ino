#include <ESP8266WiFi.h>
#include <ESP8266HTTPClient.h>
#include <WiFiClientSecure.h>
#include <UniversalTelegramBot.h>

#include <Ticker.h>

#include <WiFiUdp.h>
WiFiUDP UDP;
#include <WakeOnLan.h>
WakeOnLan WOL(UDP);
HTTPClient https;

Ticker watchdogTimer;
int watchdogTimeSec = 300;

Ticker blinker;
int blinkerTimeSec = 1;
int blinkerCountCurrent = 0;
int blinkerCountLimit = 5;

const int ledPin = 03;
int ledStatus = 0;

void watchdogCallback(){
  Serial.println("Watchdog timer expired. Restarting...");
  ESP.restart();  // Restart the device
}

void blinkerCallback(){
  blinkerCountCurrent++;
  if(blinkerCountCurrent > 4){
    digitalWrite(ledPin, HIGH);
  }
  
  if(blinkerCountCurrent > blinkerCountLimit){
    blinkerCountCurrent = 0;
    digitalWrite(ledPin, LOW);
  }
}

// Wifi network station credentials
#define WIFI_SSID "Wifi Name"
#define WIFI_PASSWORD "Wifi Password"

// Telegram BOT Token (Get from Botfather)
#define BOT_TOKEN "3534534535345:asdasgdw23y9ry2huwersdfsfd"

const unsigned long BOT_MTBS = 500;  // mean time between scan messages

X509List cert(TELEGRAM_CERTIFICATE_ROOT);
WiFiClientSecure secured_client;
UniversalTelegramBot bot(BOT_TOKEN, secured_client);
unsigned long bot_lasttime;  // last time messages' scan has been done

int DELAY_CONST = 100;
String admin_chat_id = "chat_id_434"; // Use this bot to get chat id https://t.me/chat_id_echo_bot
String chat_id = admin_chat_id;
int first_run = 1;

void wakeMyPC() {
    const char *MACAddress = "xx:xx:xx:xx:xx:xx";
  
    WOL.sendMagicPacket(MACAddress); // Send Wake On Lan packet with the above MAC address. Default to port 9.
    // WOL.sendMagicPacket(MACAddress, 7); // Change the port number
}

void wakeOfficePC() {
    const char *MACAddress = "xx:xx:xx:xx:xx:xx";
    const char *secureOn = "xx:xx:xx:xx:xx:xx";
  
    WOL.sendSecureMagicPacket(MACAddress, secureOn); // Send Wake On Lan packet with the above MAC address and SecureOn feature. Default to port 9.
    // WOL.sendSecureMagicPacket(MACAddress, secureOn, 7); // Change the port number
}

void handleNewMessages(int numNewMessages) {
  Serial.print("handleNewMessages ");
  Serial.println(numNewMessages);

  for (int i = 0; i < numNewMessages; i++) {
    String chat_id = bot.messages[i].chat_id;
    String text = bot.messages[i].text;

    String from_name = bot.messages[i].from_name;
    if (from_name == "")
      from_name = "Guest";

    if(admin_chat_id != chat_id){
      bot.sendMessage(chat_id, "You're not authorized!", "");
      bot.sendMessage(admin_chat_id, "Unauthorized!\nID: "  + chat_id, "");
      return;
    }

    if (text == "/pcon") {
      digitalWrite(ledPin, HIGH);  // turn the LED on (HIGH is the voltage level)
      ledStatus = 1;
      // bot.sendMessage(chat_id, "Led is ON", "");
      delay(500);
      digitalWrite(ledPin, LOW);
      ledStatus = 0;
      wakeMyPC();
      bot.sendMessage(chat_id, "PC_ON Done", "");
    } 
    else if (text == "/pcoff") {
      ledStatus = 0;
      digitalWrite(ledPin, HIGH);  // turn the LED on (HIGH is the voltage level)
      ledStatus = 1;
      delay(2500);
      digitalWrite(ledPin, LOW);
      ledStatus = 0;
      bot.sendMessage(chat_id, "PC_OFF Done", "");
    } else if (text == "/status") {
      if (ledStatus) {
        bot.sendMessage(chat_id, "Led is ON", "");
      } else {
        bot.sendMessage(chat_id, "Led is OFF", "");
      }
    } else if (text.indexOf("/change_delay") != -1) {
      // Find the position of the space character after the command
      int spacePos = text.indexOf(" ", 0);

      // If a space character is found
      if (spacePos != -1) {
        // Extract the delay value from the text
        String delayString = text.substring(spacePos + 1);
        // Convert the delay value to an integer
        int newDelay = delayString.toInt();
        // Update the DELAY_CONST variable
        DELAY_CONST = newDelay;

        bot.sendMessage(chat_id, "Delay updated to " + String(DELAY_CONST) + " ms");
      } else {
        bot.sendMessage(chat_id, "Please specify a delay value");
      }
    } else if (text == "/status_all") {
      String pinStatus = "Pin: STATUS";
      int status = 0;
      for (int pin = 0; pin <= 16; pin++) {
        status = digitalRead(pin);
        pinStatus += String(pin) + ": " + String(status) + "\n";
      }
      bot.sendMessage(chat_id, pinStatus);
    } else  //if (text == "/start")
    {
      Serial.println(from_name);
      String welcome = "Welcome to Universal Arduino Telegram Bot library, " + from_name + ".\n";
      welcome += "This is Flash Led Bot example.\n\n";
      welcome += "/pcon : to switch the Led ON\n";
      welcome += "/pcoff : to switch the Led OFF\n";
      welcome += "/status : Returns current status of LED\n";
      welcome += "/status_all: Returns all pins current status\n";
      welcome += "/change_delay: Changes delay between checking messages";
      bot.sendMessage(chat_id, welcome);
    }
  }
}


void setup() {
  Serial.begin(9600);
  Serial.println();

  pinMode(ledPin, OUTPUT);  // initialize digital ledPin as an output.

  // attempt to connect to Wifi network:
  configTime(0, 0, "pool.ntp.org");       // get UTC time via NTP
  
  blinker.attach(blinkerTimeSec, blinkerCallback);
  watchdogTimer.attach(watchdogTimeSec, watchdogCallback);
  Serial.print("Connecting to Wifi SSID ");
  Serial.print(WIFI_SSID);
  WiFi.begin(WIFI_SSID, WIFI_PASSWORD);
  while (WiFi.status() != WL_CONNECTED) {
    Serial.print(".");
    delay(500);
  }
  Serial.print("\nWiFi connected. IP address: ");
  Serial.println(WiFi.localIP());
  watchdogTimer.detach();
  watchdogTimer.attach(watchdogTimeSec, watchdogCallback);
  secured_client.setTrustAnchors(&cert);  // Add root certificate for api.telegram.org
  
}

void loop() {
  if (millis() - bot_lasttime > BOT_MTBS) {
    int numNewMessages = bot.getUpdates(bot.last_message_received + 1);

    while (numNewMessages) {
      Serial.println("got response");
      handleNewMessages(numNewMessages);
      numNewMessages = bot.getUpdates(bot.last_message_received + 1);
      watchdogTimer.detach();
      watchdogTimer.attach(watchdogTimeSec, watchdogCallback);
      Serial.println("Watchdog timer reset..." + String(watchdogTimeSec) + " seconds!");
    }

    bot_lasttime = millis();
  }
  delay(DELAY_CONST);
}
