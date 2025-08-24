#include <LiquidCrystal.h>
#include <Keypad.h>
#include "pitches.h"
#include "WiFi.h"
#include <HTTPClient.h>

#define DEVICE_NAME "game_1"

// Wi-Fi Credentials
const char* ssid = "UCSD-GUEST"; // Wi-Fi SSID
const char* password = ""; // Wi-Fi Password (empty if none)
unsigned long lastWiFiCheckTime = 0;
const unsigned long wifiCheckInterval = 10000; // Check every 10 seconds

// Apps Script URL for Sheet
String Web_App_URL = "https://script.google.com/macros/s/AKfycby6tQuxwJNh96bDOGEywhGtjb037TwChgm3f7RJI5lEb7PyBRkS998mO30AnnCaW5f9rg/exec";

// LCD and Keypad Setup
const int rs = 22, en = 23, d4 = 21, d5 = 19, d6 = 18, d7 = 5;
LiquidCrystal lcd(rs, en, d4, d5, d6, d7);

const byte ROWS = 4;
const byte COLS = 3;
char keys[ROWS][COLS] = {
  {'1', '2', '3'},
  {'4', '5', '6'},
  {'7', '8', '9'},
  {'*', '0', '#'}
};
byte colPins[COLS] = {27, 14, 12};
byte rowPins[ROWS] = {32, 33, 25, 26};
Keypad keypad = Keypad(makeKeymap(keys), rowPins, colPins, ROWS, COLS);

// Buzzer and Melody Setup
const int BUZZER_PIN = 13;
int melody_Incorrect[] = {NOTE_A2, NOTE_G2};
int noteDurations_Incorrect[] = {6, 2};
int melody_Correct[] = {NOTE_AS5, NOTE_B5, NOTE_C6, NOTE_CS6, NOTE_E6, NOTE_CS6, NOTE_E6, NOTE_CS6};
int noteDurations_Correct[] = {6, 6, 6, 4, 3, 4, 1};

// Variables
int idx = 0;
char key;
int lengthOfPassword = 6; // Length of the code
char correctCode[6] = {'7', '5', '6', '2', '4', '7'};
char input[6];
bool codeCracked = false;

void connectToWiFi(bool verbose = true);

void setup() {
  Serial.begin(115200);
  lcd.begin(16, 2);

  // Wi-Fi Connection
  connectToWiFi();

  // Initial LCD message
  lcd.clear();
  lcd.print("ENTER CODE:");
  lcd.cursor();
  lcd.blink();
  lcd.setCursor(0, 1);
}

void loop() {
  // Background Wi-Fi check
  if (millis() - lastWiFiCheckTime >= wifiCheckInterval) {
    lastWiFiCheckTime = millis();

    if (WiFi.status() != WL_CONNECTED) {
      Serial.println("WiFi disconnected. Reconnecting...");
      //lcd.clear();
      //lcd.print("Reconnecting..."); 
      connectToWiFi(false);
    }
  }
/*
  key = keypad.waitForKey();
  input[idx] = key;
  idx++;
  lcd.write(key);
*/

  char key = keypad.getKey();
  if (key != NO_KEY) {
    input[idx] = key;
    idx++;
    lcd.write(key);

    if (idx >= lengthOfPassword) {
      delay(1000);
      codeCracked = true;
      for (int i = 0; i < lengthOfPassword; i++) {
        if (input[i] != correctCode[i]) {
          codeCracked = false;
          break;
        }
      }

      if (codeCracked) {
        lcd.clear();
        lcd.print("YOU WIN");
        lcd.noCursor();
        lcd.noBlink();
        playMelody(true);
        sendDataToGoogleSheet();
        displayNextGame();
      } else {
        lcd.clear();
        lcd.print("INCORRECT");
        lcd.setCursor(0, 1);
        lcd.print("TRY AGAIN");
        playMelody(false);
      }

      delay(2000);
      idx = 0;
      lcd.clear();
      lcd.print("ENTER CODE:");
      lcd.setCursor(0, 1);
      lcd.cursor();
      lcd.blink();
    }
  }
}

// Function to connect to Wi-Fi
void connectToWiFi(bool verbose) {
  if (verbose) {
    Serial.println("WIFI mode : STA");
  }
  WiFi.mode(WIFI_STA);

  while (WiFi.status() != WL_CONNECTED) {
    if (verbose) {
      Serial.print("Connecting to ");
      Serial.println(ssid);
    }
    WiFi.begin(ssid, password);
    unsigned long startAttemptTime = millis();

    while (WiFi.status() != WL_CONNECTED && millis() - startAttemptTime < 10000) {
      if (verbose) {
        lcd.clear();
        lcd.print("Connecting...");
      }
      delay(500);
      if (verbose) Serial.print(".");
    }

    if (WiFi.status() != WL_CONNECTED) {
      if (verbose) {
        Serial.println("\nFailed to connect. Retrying...");
        lcd.clear();
        lcd.print("Retrying WiFi...");
        delay(2000);
      }
    }
  }

  if (verbose) {
    lcd.clear();
    lcd.print("WiFi connected");
    Serial.println("WiFi connected");
    delay(2000);
    lcd.clear();
    lcd.print("ENTER CODE:");
    lcd.setCursor(0, 1);
    lcd.cursor();
    lcd.blink();
  } else {
    Serial.println("WiFi reconnected (silent).");
  }
}



// Function to play melodies
void playMelody(bool correct) {
  if (correct) {
    for (int thisNote = 0; thisNote < sizeof(noteDurations_Correct) / sizeof(int); thisNote++) {
      int noteDuration = 1000 / noteDurations_Correct[thisNote];
      tone(BUZZER_PIN, melody_Correct[thisNote], noteDuration);
      int pauseBetweenNotes = noteDuration * 1.30;
      delay(pauseBetweenNotes);
      noTone(BUZZER_PIN);
    }
  } else {
    for (int thisNote = 0; thisNote < sizeof(noteDurations_Incorrect) / sizeof(int); thisNote++) {
      int noteDuration = 1000 / noteDurations_Incorrect[thisNote];
      tone(BUZZER_PIN, melody_Incorrect[thisNote], noteDuration);
      int pauseBetweenNotes = noteDuration * 1.30;
      delay(pauseBetweenNotes);
      noTone(BUZZER_PIN);
    }
  }
}

// Function to send data to Google Sheets
void sendDataToGoogleSheet() {
  if (WiFi.status() == WL_CONNECTED) {
    String Send_Data_URL = Web_App_URL + "?action=espwrite&game=" + String(DEVICE_NAME);

    Serial.println("Send data to Google Spreadsheet...");
    Serial.print("URL : ");
    Serial.println(Send_Data_URL);

    lcd.clear();
    lcd.print("Sending data...");
    
    HTTPClient http;
    http.begin(Send_Data_URL.c_str());
    http.setFollowRedirects(HTTPC_STRICT_FOLLOW_REDIRECTS);
    int httpCode = http.GET();
    Serial.print("HTTP Status Code : ");
    Serial.println(httpCode);

    if (httpCode > 0) {
      String payload = http.getString();
      Serial.println("Payload : " + payload);
    } else {
      Serial.println("Failed to send data");
    }
    
    http.end();
    lcd.clear();
    lcd.print("Data sent");
    delay(2000);  // Wait for 2 seconds
    lcd.clear();
    lcd.print("ENTER CODE:");
    lcd.setCursor(0, 1);
    lcd.cursor();
    lcd.blink();
  } else {
    Serial.println("WiFi not connected");
    lcd.clear();
    lcd.print("WiFi not connected");
  }
}

void displayNextGame() {
  if (WiFi.status() == WL_CONNECTED) {
    // Construct the request URL
    String nextGameUrl = Web_App_URL + "?action=espread&game=" + String(DEVICE_NAME);

    Serial.println("Receiving data from Google Spreadsheet...");
    Serial.print("URL : ");
    Serial.println(nextGameUrl);

    lcd.clear();
    lcd.print("Loading data...");
    
    HTTPClient http;
    http.begin(nextGameUrl.c_str());
    http.setFollowRedirects(HTTPC_STRICT_FOLLOW_REDIRECTS);
    int httpCode = http.GET();  // Make the HTTP GET request
    Serial.print("HTTP Status Code : ");
    Serial.println(httpCode);

    if (httpCode > 0) {
      String payload = http.getString();  // Get the response from Apps Script
      Serial.println("Payload : " + payload);  // Print the payload to the Serial Monitor

      // Display the received data (payload) on the LCD screen
      lcd.clear();
      lcd.print("Next Game:");
      lcd.setCursor(0, 1);
      lcd.print(payload);  // Display the payload (next game value)
    } else {
      Serial.println("Failed to receive data");
      lcd.clear();
      lcd.print("Data fetch failed");
    }
    
    http.end();  // Close the HTTP connection

    delay(5000);  // Wait for 5 seconds before clearing the display
    lcd.clear();
    lcd.print("ENTER CODE:");
    lcd.setCursor(0, 1);
    lcd.cursor();
    lcd.blink();
  } else {
    Serial.println("WiFi not connected");
    lcd.clear();
    lcd.print("WiFi not connected");
  }
}
