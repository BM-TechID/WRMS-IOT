#include <Wire.h>
#include <LiquidCrystal_I2C.h>
#include <NewPing.h>
#include <WiFi.h>
#include <WiFiClientSecure.h>
#include <UniversalTelegramBot.h>
#include <ArduinoJson.h>

LiquidCrystal_I2C lcd(0x27, 20, 4);
NewPing sonar(19, 18);
int relay = 23;

#define WIFI_SSID "BM NETWORK"
#define WIFI_PASSWORD "0895811110703"
#define BOT_TOKEN "6228844206:AAE2q-OpC0BbJRU7d562cwBhcUeXd8Oy3A4"
#define CHAT_ID "682723201"

WiFiClientSecure secured_client;
UniversalTelegramBot bot(BOT_TOKEN, secured_client);

bool isRelayOn = false;
bool isWaterFull = false;

void setup()
{
  lcd.init();
  lcd.backlight();
  lcd.setCursor(0, 0);
  lcd.print("DMP GANTENG");
  lcd.setCursor(0, 1);
  lcd.print("Kelompok 000");
  delay(3000);
  pinMode(relay, OUTPUT);
  digitalWrite(relay, LOW);
  Serial.begin(9600);

  Serial.begin(115200);
  Serial.println();

  Serial.print("Connecting to Wifi SSID ");
  Serial.print(WIFI_SSID);
  WiFi.begin(WIFI_SSID, WIFI_PASSWORD);
  secured_client.setCACert(TELEGRAM_CERTIFICATE_ROOT);
  while (WiFi.status() != WL_CONNECTED)
  {
    Serial.print(".");
    delay(500);
  }
  Serial.print("\nWiFi connected. IP address: ");
  Serial.println(WiFi.localIP());
  lcd.clear();
  lcd.setCursor(0, 0);
  lcd.print("WiFi connected. IP address: ");
  lcd.setCursor(0, 1);
  lcd.println(WiFi.localIP());

  Serial.print("Retrieving time: ");
  configTime(0, 0, "pool.ntp.org");
  time_t now = time(nullptr);
  while (now < 24 * 3600)
  {
    Serial.print(".");
    delay(100);
    now = time(nullptr);
  }
  Serial.println(now);

  bot.sendMessage(CHAT_ID, "Bot started up", "");
}

void loop()
{
  unsigned int jarak = sonar.ping_cm();

  lcd.clear();
  lcd.setCursor(0, 0);
  lcd.print("Tinggi: ");

  if (jarak > 50)
  {
    lcd.clear();
    lcd.setCursor(0, 0);
    lcd.print("Diluar Jangkauan...");
    lcd.setCursor(0, 1);
    lcd.print("Max 50cm bree...");
    digitalWrite(relay, LOW);
    bot.sendMessage(CHAT_ID, "Diluar Jangkauan... \nJarak Max 50 cm", "");
    isWaterFull = false;
  }
  else
  {
    lcd.print(jarak);
    lcd.setCursor(12, 0);
    lcd.print("cm");

    if (jarak >= 40)
    {
      if (!isRelayOn)
      {
        digitalWrite(relay, HIGH);
        lcd.setCursor(0, 1);
        lcd.print("Relay: ON");
        bot.sendMessage(CHAT_ID, "---\nRelay: ON\nJetpam Hidup\n---\nToren sedang diisi air...", "");
        isRelayOn = true;
      }
    }
    else if (jarak <= 10)
    {
      if (isRelayOn)
      {
        digitalWrite(relay, LOW);
        lcd.setCursor(0, 1);
        lcd.print("Relay: OFF");
        bot.sendMessage(CHAT_ID, "---\nRelay: OFF\nJetpam: Mati\n---\nAir sudah penuh bree...\n---", "");
        isRelayOn = false;
      }
    }

    if (jarak <= 13 && !isWaterFull)
    {
      bot.sendMessage(CHAT_ID, "Air sudah mendekati penuh", "");
      isWaterFull = true;
    }
    else if (jarak > 12 && isWaterFull)
    {
      isWaterFull = false;
    }
  }

  Serial.print("Jarak: ");
  Serial.print(jarak);
  Serial.println(" cm");

  delay(500);
}
