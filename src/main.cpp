#include "main.hpp"
#include "actuators/pos_controller.hpp"
#include "actuators/vel_controller.hpp"
#include "actuators/pump_interface.hpp"
#include <Arduino.h>
#include <ESP32Servo.h>
#include <WiFi.h>
#include <WebServer.h>
#include <ESP32Servo.h>

// Configuration - replace these with your network credentials if you want STA mode
char WIFI_SSID[] = "YOUR_SSID";      // change to your WiFi SSID
char WIFI_PASS[] = "YOUR_PASS";      // change to your WiFi password

// AP fallback configuration (used if STA connect fails)
const char AP_SSID[] = "ESP32S3-AP";
const char AP_PASS[] = "esp32pass"; // minimum 8 chars for WPA2

WebServer server(80);

const int LED_PIN = LED_BUILTIN; // builtin LED pin
bool led_state = false;

// HTML page served to clients
const char index_html[] PROGMEM = R"rawliteral(
<!doctype html>
<html>
  <head>
    <meta name="viewport" content="width=device-width, initial-scale=1">
    <title>ESP32S3 Pump Control</title>
    <style> body { font-family: Arial, Helvetica, sans-serif; text-align:center; margin-top:30px;} input[type=number]{width:80px;font-size:1.1rem;padding:6px;} button{font-size:1.1rem;padding:8px 14px;margin-left:8px;} .state{margin-top:15px;font-weight:bold;} </style>
  </head>
  <body>
    <h1>ESP32S3 Pump</h1>
    <div>
      <label for="speed">Speed (%)</label>
      <input id="speed" type="number" min="0" max="100" value="0">
      <button onclick="send()">Set</button>
    </div>
    <div class="state" id="state">Speed: unknown</div>
    <script>
      async function send(){
        const v = document.getElementById('speed').value;
        await fetch('/set?speed=' + encodeURIComponent(v));
        updateState();
      }
      async function updateState(){
        try{
          const r = await fetch('/state');
          const j = await r.json();
          document.getElementById('state').innerText = 'Speed: ' + j.speed + '%';
          document.getElementById('speed').value = j.speed;
        }catch(e){
          document.getElementById('state').innerText = 'Speed: (no response)';
        }
      }
      updateState();
      setInterval(updateState, 2000);
    </script>
  </body>
</html>
)rawliteral";
Servo Servoforangle;
const int SERVO_PIN = 21; // use a suitable pin

void handleSet(){
  IPAddress remote = server.client().remoteIP();
  Serial.print("HTTP GET /set from "); Serial.println(remote);
  if (server.hasArg("speed")){
    String s = server.arg("speed");
    int v = s.toInt();
    actuators::pump::writeToPump(v);
    String reply = "{";
    reply += "\"speed\":";
    reply += String(v);
    reply += "}";
    server.send(200, "application/json", reply);
    // Turn pump off after 2 seconds
    delay(2000); 
    actuators::pump::writeToPump(0);
  } else {
    server.send(400, "text/plain", "missing 'speed' parameter");
  }
}

void handleRoot() {
  IPAddress remote = server.client().remoteIP();
  Serial.print("HTTP GET / from "); Serial.println(remote);
  server.send_P(200, "text/html", index_html);
}

void handleState() {
  IPAddress remote = server.client().remoteIP();
  Serial.print("HTTP GET /state from "); Serial.println(remote);
  String payload = "{";
  payload += "\"state\":";
  payload += (led_state ? "true" : "false");
  payload += "}";
  server.send(200, "application/json", payload);
}

void handleNotFound(){
  Serial.print("NotFound: ");
  Serial.print(server.uri());
  Serial.print(" from ");
  Serial.println(server.client().remoteIP());
  server.send(404, "text/plain", "Not found");
}

void startAP(){
  WiFi.mode(WIFI_AP);
  bool ok = WiFi.softAP(AP_SSID, AP_PASS);
  if(ok){
    IPAddress ip = WiFi.softAPIP();
    Serial.print("Started AP. Connect to "); Serial.print(AP_SSID); Serial.print(" ");
    Serial.print("and open http://"); Serial.println(ip);
  } else {
    Serial.println("Failed to start AP");
  }
}

void setup(){
  Serial.begin(115200);
  delay(100);

  pinMode(LED_PIN, OUTPUT);
  digitalWrite(LED_PIN, LOW);
  actuators::pump::initPump();

  Serial.println();
  Serial.println("Attempting to connect to WiFi");
  WiFi.mode(WIFI_STA);
  WiFi.begin(WIFI_SSID, WIFI_PASS);

  unsigned long start = millis();
  const unsigned long CONNECT_TIMEOUT = 10000UL; // 10s
  while (WiFi.status() != WL_CONNECTED && millis() - start < CONNECT_TIMEOUT) {
    delay(250);
    Serial.print('.');
  }
  Serial.println();

  if (WiFi.status() == WL_CONNECTED) {
    IPAddress ip = WiFi.localIP();
    Serial.print("Connected to WiFi. IP: "); Serial.println(ip);
  } else {
    Serial.println("WiFi connect failed, starting Access Point");
    startAP();
  }

  // Setup web server routes
  server.on("/", HTTP_GET, handleRoot);
  server.on("/set", HTTP_GET, handleSet);
  server.on("/state", HTTP_GET, handleState);
  server.begin();

  Serial.println("HTTP server started");
  Servoforangle.attach(SERVO_PIN);
  Servoforangle.write(90); // default to center
}

void loop(){
  server.handleClient();
}


