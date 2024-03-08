#include <WiFi.h>
#include <PubSubClient.h>

#define LED 2

const char* ssid = "LUCI  WALKER";
const char* password = "Luci@119";
const char* mqtt_server = "172.16.0.121";

WiFiClient espClient;
PubSubClient client(espClient);

//Kết nối wifi
void setup_Wifi(){
  WiFi.begin(ssid,password);
  while(WiFi.status()!= WL_CONNECTED){
    delay(500);
    Serial.print(".");
  }
  Serial.println("");
  Serial.println("đã kết nối wifi");
  Serial.println("ip address: ");
  Serial.println("WiFi.localIP()");
}

//Kết nối mqtt
void reconnect(){
  while (!client.connected()) {
    Serial.print("Attempting MQTT connection...");
    String clientId = "ESP32Client-01";
    if (client.connect(clientId.c_str())) {
      Serial.println("connected");
      client.subscribe("led");
     } else {
      Serial.print("failed, rc=");
      Serial.print(client.state());
      Serial.println(" try again in 5 seconds");
      delay(5000);
    }
  }
}

//Nhận tin nhắn từ broker
void callBack(char* topic, byte* payload, unsigned int lenght){
  Serial.print("tin nhan [");
  Serial.print(topic);
  Serial.print("]");
  for(int i = 0 ; i <lenght; i++){
    Serial.print((char)payload[i]);
  }
  Serial.println();
    if(strcmp(topic, "led")==0)
    { 
      if((char)payload[0] == '1'){ //on
        digitalWrite(LED,HIGH);
        Serial.print("on");
      }
      else if((char)payload[0] == '0'){ //off
        digitalWrite(LED, LOW);
        Serial.print("off");
      }
    }
}

void setup() {
  Serial.begin(115200);
  setup_Wifi();
  client.setServer(mqtt_server, 1883);
  client.setCallback(callBack);
  pinMode(LED,OUTPUT);
}

void loop()
{
  if (!client.connected()) {
      reconnect();
  }
  client.loop();
  delay(2000);
}
