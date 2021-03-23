#include <SimpleDHT.h>
#include <ESP8266WiFi.h>
#include <PubSubClient.h>
#include <ArduinoJson.h>

int pinDHT11 = 2; // GPIO2 of ESP8266
const char* ssid = "MERCURY_AF2E";//手机Wi-Fi热点
const char* password = "xiaoli1808085";//手机Wi-Fi密码
const char* mqtt_server = "49.233.91.252";//服务器网址或者IP地址

//const String macAddress = WiFi.macAddress();
//const char* clientID = macAddress.c_str();    //MAC地址
const char* clientID = "solomon";          //连接mqtt用户名

WiFiClient espClient;
PubSubClient client(espClient);
SimpleDHT11 dht11(pinDHT11);

long lastMsg = 0;
char msg[50];
int value = 0;

void setup() {
  //pinMode(BUILTIN_LED, OUTPUT);     // 将BUILTIN_LED引脚初始化为输出
  Serial.begin(115200);
  setup_wifi();
  client.setServer(mqtt_server, 1883);
  client.setCallback(callback);
}

void setup_wifi() {

  delay(10);
  WiFi.begin(ssid, password);     // 我们从连接到WiFi网络开始
  while (WiFi.status() != WL_CONNECTED) {
    delay(500);
  }
  Serial.println("WiFi connected");
}

///接收反馈
void callback(char* topic, byte* payload, unsigned int length) {
 Serial.print("Message arrived ["); Serial.print(topic);   Serial.print("] ");
 
  for (int i = 0; i < length; i++) {
    Serial.print((char)payload[i]);
  }  

}

void reconnect() {
  // 循环，直到我们重新连接
  while (!client.connected()) {
    
    if (client.connect(clientID)) {               //connected
      Serial.print("MQTT连接成功");                 // 连接mqtt成功
  //    client.publish("ESP_01主题", "hello world");  // 一旦连接，发布公告…
      client.subscribe("DHT11");               // 一旦连接，发布主题
    } else {
      delay(5000);                                 // 等待5秒再重试
    }
  }
}

void loop() {

  if (!client.connected()) {
    reconnect();
  }
  client.loop();


////温度湿度
  byte temperature = 0;
  byte humidity = 0;
  int err = SimpleDHTErrSuccess;
  if ((err = dht11.read(&temperature, &humidity, NULL)) != SimpleDHTErrSuccess) {
    Serial.print("Read DHT11 failed, err="); Serial.println(err);delay(1000);
    return;
  }

        DynamicJsonDocument data(256);
        data["temp"] = temperature;
        data["hum"] = humidity;
        // publish temperature and humidity
        char json_string[256];
        serializeJson(data, json_string);
        // {"temp":23.5,"hum":55}
        Serial.println(json_string);
        client.publish("DHT11", json_string, false);
        delay(4000); 
  
//   String tmp = "温度:";            //字符串拼接
//   tmp += String(temperature);
//   tmp += String(" 湿度:");
//   tmp += String(humidity);
//  
//  Serial.print((int)temperature); Serial.print(" *C, "); 
//  Serial.print((int)humidity); Serial.println(" H");
//  client.publish("DHT11", tmp.c_str());//发送主题
//  delay(2000);  
////温度湿度 
}
