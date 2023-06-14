/*
DMP GANTENG
21102121_MANSUR JULIANTO
21102122_M.AMMAR IZZUDIN
21102116_ASYAFA DITRA AL-HAUNA
21102145_DIMAS TEGUH RAMADHANI
*/

#include <Wire.h> // 
#include <LiquidCrystal_I2C.h> // 
#include <NewPing.h> // Library HCSR04

LiquidCrystal_I2C lcd(0x27, 20, 4); // Setting address lcd di 0x27 atau 32
NewPing sonar(19,18); //Trig pin 19 dan Echo pin 18
int relay = 23; // deklarasi relay pada pin 23

void setup()
{
  lcd.init(); // Inisialisasi lcd
  lcd.backlight(); // Menyalakan backlight lcd
  lcd.setCursor(0, 0);
  lcd.print ("DMP GANTENG");
  lcd.setCursor(0, 1);
  lcd.print("Kelompok 000");
  delay(3000);
  pinMode(relay, OUTPUT);
  digitalWrite(relay, LOW); // setting relay default off
  Serial.begin(9600); // Komunikasi dengan serial monitor
}

void loop()
{
  unsigned int jarak = sonar.ping_cm();

  // Menampilkan output ke lcd
  lcd.clear();
  lcd.setCursor(0, 0);
  lcd.print("Tinggi: ");

  // Cek jarak ketika lebih dari jangkauan
  if (jarak > 50) {
    lcd.clear();
    lcd.setCursor(0, 0);
    lcd.print("Diluar Jangkauan...");
    lcd.setCursor(0, 1);
    lcd.print("Max 50cm bree...");
    digitalWrite(relay, LOW);
  } else {
    lcd.print(jarak);
    lcd.setCursor(12, 0);
    lcd.print("cm");

    // Mengontrol relay on/off dengan jarak
    if (jarak >= 15) {
      digitalWrite(relay, HIGH);
      lcd.setCursor(0, 1);
      lcd.print("Relay: ON");

    } else if (jarak <= 5) {
      digitalWrite(relay, LOW);
      lcd.setCursor(0, 1);
      lcd.print("Relay: OFF");

    }
  }

  // Menmapilkan output ke serial monitor
  Serial.print("Jarak: ");
  Serial.print(jarak);
  Serial.println(" cm");

  delay(500);
}