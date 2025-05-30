#include <WiFi.h>
#include <PubSubClient.h>

// --- Configurações Wi-Fi ---G
const char* SSIDName = "Wokwi-GUEST";
const char* SSIDPass = "";

// --- Configurações MQTT ---
const char* BrokerURL = "broker.hivemq.com";
const int BrokerPort = 1883;
const char* MQTTClientName = "disp_2";

// --- Tópicos MQTT ---
const char* Topico_Temp = "pub_sub/Temperatura";
const char* Topico_Umi = "pub_sub/Umidade";
const char* Topico_Ar = "pub_sub/QualidadeAr";

// --- Limites para atuação ---
const float LIMITE_TEMP = 35.0;
const int LIMITE_AR = 1000;

// --- Objetos ---
WiFiClient espClient;
PubSubClient clienteMQTT(espClient);

// Estado atual do LED para evitar escrever repetidamente
bool ledEstado = false;

void ativarLed(bool ligar) {
  if (ledEstado != ligar) {
    digitalWrite(LED_BUILTIN, ligar ? HIGH : LOW);
    Serial.printf("LED %s\n", ligar ? "ATIVADO" : "DESATIVADO");
    ledEstado = ligar;
  }
}

void callback(char* topic, byte* payload, unsigned int length) {
  // Garantir que o payload é uma string terminada em '\0'
  char mensagem[length + 1];
  memcpy(mensagem, payload, length);
  mensagem[length] = '\0';

  String valorStr = String(mensagem);
  float valor = valorStr.toFloat();

  Serial.printf("Mensagem recebida no tópico '%s': %s (valor float: %.2f)\n", topic, mensagem, valor);

  bool ligarLed = false;
  
  if (strcmp(topic, Topico_Temp) == 0 && valor > LIMITE_TEMP) {
    ligarLed = true;
  }
  else if (strcmp(topic, Topico_Ar) == 0 && valor > LIMITE_AR) {
    ligarLed = true;
  }

  ativarLed(ligarLed);
}

void conectarWiFi() {
  Serial.printf("Conectando ao Wi-Fi: %s\n", SSIDName);
  WiFi.begin(SSIDName, SSIDPass);

  unsigned long startAttemptTime = millis();

  // Timeout de 15 segundos para conexão Wi-Fi
  while (WiFi.status() != WL_CONNECTED && millis() - startAttemptTime < 15000) {
    Serial.print(".");
    delay(500);
  }

  if (WiFi.status() == WL_CONNECTED) {
    Serial.println("\nWi-Fi conectado!");
    Serial.print("IP: ");
    Serial.println(WiFi.localIP());
  } else {
    Serial.println("\nFalha ao conectar no Wi-Fi");
  }
}

void conectarMQTT() {
  while (!clienteMQTT.connected()) {
    Serial.print("Tentando conectar no MQTT...");
    if (clienteMQTT.connect(MQTTClientName)) {
      Serial.println("Conectado ao broker!");
      clienteMQTT.subscribe(Topico_Temp);
      clienteMQTT.subscribe(Topico_Ar);
    } else {
      Serial.print("Falha, rc=");
      Serial.print(clienteMQTT.state());
      Serial.println(" tentando novamente em 2 segundos");
      delay(2000);
    }
  }
}

void setup() {
  Serial.begin(9600);
  pinMode(LED_BUILTIN, OUTPUT);
  ativarLed(false);

  conectarWiFi();

  clienteMQTT.setServer(BrokerURL, BrokerPort);
  clienteMQTT.setCallback(callback);

  conectarMQTT();
}

void loop() {
  if (WiFi.status() != WL_CONNECTED) {
    conectarWiFi();
  }

  if (!clienteMQTT.connected()) {
    conectarMQTT();
  }

  clienteMQTT.loop();
}
