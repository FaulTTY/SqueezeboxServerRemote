#include <ESP8266WiFi.h>
#include <ESP8266HTTPClient.h>

const char* ssid     = "";
const char* password = "";

const int mutePin = 16; // D0
const int volUpPin = 14; // D5
const int volDownPin = 12; // D6
int muteState = 0;
int volUpState = 0;
int volDownState = 0;

void setup() {
  Serial.begin(115200);

  // We start by connecting to a WiFi network
  WiFi.mode(WIFI_STA);
  WiFi.begin(ssid, password);

  Serial.println();
  Serial.println();
  Serial.print("Wait for WiFi... ");

  while (WiFi.status() != WL_CONNECTED) {
    Serial.print(".");
    delay(500);
  }

  Serial.println("");
  Serial.println("WiFi connected");
  Serial.println("IP address: ");
  Serial.println(WiFi.localIP());

  pinMode(mutePin, INPUT);
  pinMode(volUpPin, INPUT);
  pinMode(volDownPin, INPUT);

  delay(500);
}

void loop() {
  muteState = digitalRead(mutePin);
  volUpState = digitalRead(volUpPin);
  volDownState = digitalRead(volDownPin);
  
  HTTPClient http;
  http.begin("http://10.1.10.13:9000/jsonrpc.js");
  int httpCode;

  if (muteState == HIGH) {
    Serial.println("Mute pressed");
    httpCode = http.POST("{\"id\":0,\"params\":[\"b8:27:eb:20:ca:ad\",[\"button\",\"muting\"]],\"method\":\"slim.request\"}");
    delay(200); // extra delay to prevent double mute
  } else if (volUpState == HIGH) {
    Serial.println("VolUp pressed");
    httpCode = http.POST("{\"id\":0,\"params\":[\"b8:27:eb:20:ca:ad\",[\"button\",\"volup\"]],\"method\":\"slim.request\"}");
  } else if (volDownState == HIGH) {
    Serial.println("VolDown pressed");
    httpCode = http.POST("{\"id\":0,\"params\":[\"b8:27:eb:20:ca:ad\",[\"button\",\"voldown\"]],\"method\":\"slim.request\"}");
  }

  http.end();

  delay(100);
}
