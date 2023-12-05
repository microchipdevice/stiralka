/*******************************************************************
    A telegram bot that sends you a message when ESP
    starts up

    Parts:
    D1 Mini ESP8266 * - http://s.click.aliexpress.com/e/uzFUnIe
    (or any ESP8266 board)

      = Affilate

    If you find what I do useful and would like to support me,
    please consider becoming a sponsor on Github
    https://github.com/sponsors/witnessmenow/


    Written by Brian Lough
    YouTube: https://www.youtube.com/brianlough
    Tindie: https://www.tindie.com/stores/brianlough/
    Twitter: https://twitter.com/witnessmenow
 *******************************************************************/

#include <ESP8266WiFi.h>
#include <WiFiClientSecure.h>
#include <UniversalTelegramBot.h>
#include <ArduinoJson.h>

// Wifi network station credentials
#define WIFI_SSID "name network"
#define WIFI_PASSWORD "Password"
// Telegram BOT Token (Get from Botfather)
#define BOT_TOKEN "token telegram bota"

// Use @myidbot (IDBot) to find out the chat ID of an individual or a group
// Also note that you need to click "start" on a bot before it can
// message you
#define CHAT_ID_vov "chat ID"
#define CHAT_ID2_vlad "454936220"


const unsigned long BOT_MTBS = 1000; // mean time between scan messages

X509List cert(TELEGRAM_CERTIFICATE_ROOT);
WiFiClientSecure secured_client;
UniversalTelegramBot bot(BOT_TOKEN, secured_client);
unsigned long bot_lasttime; // last time messages' scan has been done

const int ledPin = LED_BUILTIN;
int status=0;
int status1=0;
int status2=0;
int status3=0;
String idchat;
void pic(int zd) { 
  digitalWrite(D8, HIGH);
  delay(zd);
  digitalWrite(D8, LOW); 
  delay(zd);
 }

void handleNewMessages(int numNewMessages)
{
  Serial.print("handleNewMessages ");
  Serial.println(numNewMessages);

  for (int i = 0; i < numNewMessages; i++)
  {
    String chat_id = bot.messages[i].chat_id;
    String text = bot.messages[i].text;
idchat=chat_id;
if (text == "/start" || "start" || "старт"|| "пуск"|| "go"|| "го"|| "Start"|| "Go"|| "Го"|| "Старт"|| "Пуск")  
    {
      pic(1000);
      digitalWrite(ledPin, HIGH); // turn the LED on (HIGH is the voltage level)
      delay(1000);
      digitalWrite(ledPin, LOW); 
      bot.sendMessage(chat_id, "Запуск...", "");
      

    //ожидание запуска стирки
    while (1) 
    { 
    delay(2000);
    if (digitalRead(D5)==LOW && digitalRead(D1)==LOW && digitalRead(D2)==HIGH && digitalRead(D6)==HIGH )  //если индикатор состояния и интикатор "стирка" включены
    { 
      
    bot.sendMessage(chat_id, "Стирка запущена", ""); break;  
    }
    if (digitalRead(D5)==LOW  && digitalRead(D6)==LOW && digitalRead(D2)==HIGH && digitalRead(D1)==HIGH ) // если индикатор состояния и интикатор "отжим" включены
    {
      
bot.sendMessage(chat_id, "Отжим или Слив запущен", ""); break; 
    }

    if (digitalRead(D5)==LOW  && digitalRead(D2)==LOW && digitalRead(D6)==HIGH && digitalRead(D1)==HIGH ) // если индикатор состояния и интикатор "полоскание" включены
    {
      
bot.sendMessage(chat_id, "Полоскание или Смягчение запущено", ""); break; 
    }


   if (digitalRead(D6)==HIGH  && digitalRead(D2)==HIGH &&  digitalRead(D1)==HIGH) //
    { 
pic(2000);
bot.sendMessage(chat_id, "ERROR", ""); 
    }
    }

while (1) 
{
  delay(3);
if (digitalRead(D1)==LOW && digitalRead(D2)==HIGH && digitalRead(D6)==HIGH && status==0) 
{
status=1;
 bot.sendMessage(chat_id, "Статус: СТИРКА.", ""); 
}
delay(3);
if (digitalRead(D2)==LOW && digitalRead(D1)==HIGH && digitalRead(D6)==HIGH && status1==0 )
{
status1=1;
 bot.sendMessage(chat_id, "Статус: ПОЛОСКАНИЕ.", "");
}
delay(3);
if (digitalRead(D6)==LOW  && digitalRead(D2)==HIGH && digitalRead(D1)==HIGH && status2==0) 
{
  status2=1;
 bot.sendMessage(chat_id, "Статус: ОТЖИМ.", "");
}
delay(3);
if (digitalRead(D2)==LOW  && digitalRead(D1)==LOW  && digitalRead(D6)==LOW ) 
{
status3=1; 
break;
 
}

}


    }
  }
}

void setup()
{
  Serial.begin(96000);
  Serial.println();

  pinMode(ledPin, OUTPUT); // initialize digital ledPin as an output.
  delay(10);
  digitalWrite(ledPin, LOW); // initialize pin as off (active LOW)
  pinMode(ledPin, OUTPUT); // (IO 2) пуск
  pinMode( D8, OUTPUT);    // (IO 15) pic
  pinMode(D6, INPUT_PULLUP);      // (IO 12) отжим
   pinMode(D2, INPUT_PULLUP);     // (IO 4) полоскание
    pinMode(D1, INPUT_PULLUP);    // (IO 5) стирка
     pinMode(D5, INPUT_PULLUP);   // (IO 14) статус пуска 


  // attempt to connect to Wifi network:
  configTime(0, 0, "pool.ntp.org");      // get UTC time via NTP
  secured_client.setTrustAnchors(&cert); // Add root certificate for api.telegram.org
  Serial.print("Connecting to Wifi SSID ");
  Serial.print(WIFI_SSID);
  WiFi.begin(WIFI_SSID, WIFI_PASSWORD);
  while (WiFi.status() != WL_CONNECTED)
  {
    Serial.print(".");
    delay(500);
  }
  Serial.print("\nWiFi connected. IP address: ");
  Serial.println(WiFi.localIP());

  // Check NTP/Time, usually it is instantaneous and you can delete the code below.
  Serial.print("Retrieving time: ");
  time_t now = time(nullptr);
  while (now < 24 * 3600)
  {
    Serial.print(".");
    delay(100);
    now = time(nullptr);
  }
  Serial.println(now);
  pic(100); 
  pic(100);
  pic(100);
  bot.getUpdates(bot.last_message_received + 1);
  
}

void loop()
{

if(status3==1)
{
  while (1)
  {
    pic(500);
    pic(500);
    pic(500);
    bot.sendMessage(idchat, "СТИРКА ЗАВЕРШЕНА", ""); 
    delay(20000);
  }
  
}



  if (millis() - bot_lasttime > BOT_MTBS)
  {
    int numNewMessages = bot.getUpdates(bot.last_message_received + 1);

    while (numNewMessages)
    {
      Serial.println("got response");
      handleNewMessages(numNewMessages);
      numNewMessages = bot.getUpdates(bot.last_message_received + 1);
    }

    bot_lasttime = millis();
  }
}
