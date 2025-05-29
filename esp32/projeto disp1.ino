#include <WiFi.h>
#include <PubSubClient.h>
#include <DHTesp.h>

// Wi-Fi
char SSIDName[] = "Wokwi-GUEST";
char SSIDPass[] = "";

// MQTT
char BrokerURL[] = "broker.hivemq.com";
char BrokerUserName[] = "";
char BrokerPassword[] = "";
char MQTTClientName[] = "ATV_4";
int BrokerPort = 1883;

char Topico_01[] = "pub_sub/Temperatura";
char Topico_02[] = "pub_sub/Umidade";
char Topico_03[] = "pub_sub/Chave";
char Topico_04[] = "pub_sub/QualidadeAr";

// Pinos
const int DHT_PIN = 15;
const int MQ2_PIN = 34;

WiFiClient espClient;
PubSubClient clienteMQTT(espClient);
DHTesp dhtSensor;

void callback(char* topic, byte* payload, unsigned int length) {
  String comando = "";
  for (int i = 0; i < length; i++) {
    comando += (char)payload[i];
  }

  Serial.print("Comando recebido em ");
  Serial.print(topic);
  Serial.print(": ");
  Serial.println(comando);

  if (comando == "1") {
    digitalWrite(LED_BUILTIN, HIGH);
    Serial.println("LED ligado");
  } else if (comando == "0") {
    digitalWrite(LED_BUILTIN, LOW);
    Serial.println("LED desligado");
  }
}

void mqttReconnect() {
  while (!clienteMQTT.connected()) {
    Serial.println("Conectando-se ao broker MQTT...");
    if (clienteMQTT.connect(MQTTClientName, BrokerUserName, BrokerPassword)) {
      Serial.println("Conectado ao broker!");
      clienteMQTT.subscribe(Topico_03);
    } else {
      Serial.print("Falha, rc=");
      Serial.print(clienteMQTT.state());
      Serial.println(" tentando novamente em 5s...");
      delay(5000);
    }
  }
}

void setup() {
  Serial.begin(9600);
  pinMode(LED_BUILTIN, OUTPUT);
  dhtSensor.setup(DHT_PIN, DHTesp::DHT22);
  clienteMQTT.setServer(BrokerURL, BrokerPort);
  clienteMQTT.setCallback(callback);

  Serial.print("Conectando-se ao Wi-Fi");
  WiFi.begin(SSIDName, SSIDPass);
  while (WiFi.status() != WL_CONNECTED) {
    delay(100);
    Serial.print(".");
  }
  Serial.println();
  Serial.println("Wi-Fi conectado!");
  Serial.print("IP: ");
  Serial.println(WiFi.localIP());
}

void loop() {
  if (WiFi.status() != WL_CONNECTED) {
    Serial.println("Wi-Fi desconectado. Tentando reconectar...");
    WiFi.begin(SSIDName, SSIDPass);
    delay(5000);
  }

  if (!clienteMQTT.connected()) {
    mqttReconnect();
  }
  clienteMQTT.loop();

  TempAndHumidity dados = dhtSensor.getTempAndHumidity();

  char tempBuffer[8];
  dtostrf(dados.temperature, 1, 2, tempBuffer);
  clienteMQTT.publish(Topico_01, tempBuffer);
  Serial.print("Temperatura publicada: ");
  Serial.println(tempBuffer);

  char umidadeBuffer[8];
  dtostrf(dados.humidity, 1, 2, umidadeBuffer);
  clienteMQTT.publish(Topico_02, umidadeBuffer);
  Serial.print("Umidade publicada: ");
  Serial.println(umidadeBuffer);

  int qualidadeAr = analogRead(MQ2_PIN);
  char qualidadeArBuffer[8];
  itoa(qualidadeAr, qualidadeArBuffer, 10);
  clienteMQTT.publish(Topico_04, qualidadeArBuffer);
  Serial.print("Qualidade do ar publicada: ");
  Serial.println(qualidadeArBuffer);

  delay(2000);
}
