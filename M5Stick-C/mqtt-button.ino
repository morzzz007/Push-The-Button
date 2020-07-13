#include <M5StickC.h>
#include <WiFi.h>
#include <WiFiClient.h>
#include <PubSubClient.h>

char *mqttServerAddress = "192.168.0.195";
char *ssid = "Captain Choli Churro";
char *password = "hagymaleves";

int pressCounter = 0;
char payloadBuffer[50];

WiFiClient netClient;
PubSubClient MqttClient;

void connect_to_wifi() {
  M5.Lcd.printf("Connecting to Wifi...");

  WiFi.begin(ssid, password);
  while (WiFi.status() != WL_CONNECTED) {
    delay(500);
    M5.Lcd.printf(".");
  }
  M5.Lcd.printf("\nWiFi Connected.");
}

void connect_to_mqtt() {
  MqttClient.setServer(mqttServerAddress, 1883);
  MqttClient.setClient(netClient);
  MqttClient.connect("M5StickC");
}

void setup() {
  M5.begin();
  M5.Lcd.setRotation(3);

  connect_to_wifi();
  connect_to_mqtt();

  M5.Lcd.fillScreen(BLACK);
}

void loop() {
  M5.update();
  MqttClient.loop();

  M5.Lcd.setCursor(10, 20);
  if (M5.BtnA.wasPressed()) {
    sprintf(payloadBuffer, "pressed %d times", ++pressCounter);
    MqttClient.publish("m5stack/button", payloadBuffer);
    M5.Lcd.setTextColor(GREEN, BLACK);
    M5.Lcd.printf("The button was pressed.");
    delay(1000);
  } else {
    M5.Lcd.setTextColor(WHITE, BLACK);
    M5.Lcd.printf("The button is released.");
    M5.Lcd.setCursor(0, 30);
    M5.Lcd.printf("\nWiFi %s.",
                  WiFi.status() == 3 ? "connected" : "disconnected");
    M5.Lcd.printf("\nMqtt %s.",
                  MqttClient.state() == 0 ? "connected" : "disconnected");
  }
}