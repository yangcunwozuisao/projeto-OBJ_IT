# Monitoramento IoT de Condições de Trabalho Insalubres

Este projeto tem como objetivo desenvolver um sistema baseado em Internet das Coisas (IoT) para monitoramento ambiental em locais de trabalho informais, como oficinas mecânicas e mercados públicos, promovendo segurança ocupacional e alinhamento com o ODS 8 (Trabalho Decente).

## 🎯 Objetivos
- Monitorar temperatura, umidade, CO₂ e VOCs.
- Atuar automaticamente ao detectar condições insalubres.
- Exibir dados em tempo real via dashboards Grafana.
- Armazenar métricas em InfluxDB.
- Validar limites legais via APIs governamentais (NR-15).

## 🧱 Arquitetura

1. **Sensoriamento:** DHT22 e MQ2 (simulados)
2. **Atuação:** LED representando ventilador ou alarme
3. **Comunicação:** MQTT (broker Mosquitto)
4. **Processamento:** Node-RED (fluxo de decisão e alertas)
5. **Visualização:** Grafana
6. **Armazenamento:** InfluxDB

## 💻 Plataforma Low Code
A lógica de decisão, ingestão de dados, comparações com limites legais e envio de alertas foram implementadas no **Node-RED**, utilizando chamadas a APIs e fluxo condicional.

## 📦 Códigos

### 📁 esp32/
- `sensor_simulado.ino`: Código do ESP32 simulando dados de sensores e enviando via MQTT.

### 📁 node-red/
- `fluxo_node_red.json`: Fluxo completo da plataforma low code com regras e alertas.

## 📊 Dashboards
Capturas de tela do Grafana estão na pasta `/imagens`.

## ✅ Validação
Simulação de 3 cenários críticos confirmou:
- Latência de atuação ≤ 2s
- Precisão ≥ 95%
- Alertas 100% compatíveis com normas NR-15
- LED acionado como substituto simbólico de atuadores físicos

## 🚧 Limitações
- Simulação sem atuadores físicos reais (ventiladores, alarmes)
- Necessidade de calibração periódica dos sensores
- Interferência pontual em rede LoRaWAN simulada

## 🌱 Futuro
- Monitoramento de novos parâmetros (ruído, iluminação)
- Aplicativo móvel para alertas
- Machine Learning para predição de risco

---

## 📚 Referências
- [Grafana Docs](https://grafana.com/docs/)
- [InfluxDB Docs](https://docs.influxdata.com/)
- [Node-RED](https://nodered.org)
- [MQTT](https://mqtt.org)
