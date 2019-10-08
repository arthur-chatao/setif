

#include <WiFi.h>

#include <PubSubClient.h>

#include <Wire.h>

#include <Adafruit_Sensor.h>

#include "DHT.h"

// Replace the next variables with your SSID/Password combination

const char* ssid = "Robotica";

const char* password = "robotica";

// Add your MQTT Broker IP address, example:

//const char* mqtt_server = "192.168.1.144";

const char *mqtt_server = "soldier.cloudmqtt.com";
const char *mqtt_user = "okfogzrl";
const char *mqtt_pass = "VMlhvoQZXVVn";
const int mqtt_port = 12528;

WiFiClient espClient;

PubSubClient client(espClient);

long lastMsg = 0;

char msg[50];

int value = 0;

 

#define DHTPIN 27     // pino que estamos conectado

#define DHTTYPE DHT11 // DHT 11


// Conecte pino 1 do sensor (esquerda) ao +5V

// Conecte pino 2 do sensor ao pino de dados definido em seu Arduino

// Conecte pino 4 do sensor ao GND

// Conecte o resistor de 10K entre pin 2 (dados)

// e ao pino 1 (VCC) do sensor

DHT dht(DHTPIN, DHTTYPE);

 

void setup()

{

//  pinMode(porta_potenciometro, INPUT);

  //pinMode(porta_ventilador, OUTPUT);

  Serial.begin(115200);

  Serial.println("DHTxx test!");

  dht.begin();

 

  setup_wifi();

  client.setServer(mqtt_server, mqtt_port);

  client.setCallback(callback);

}

void setup_wifi() {

  delay(10);

  // We start by connecting to a WiFi network

  Serial.println();

  Serial.print("Connecting to ");

  Serial.println(ssid);

  WiFi.begin(ssid, password);

  while (WiFi.status() != WL_CONNECTED) {

    delay(500);

    Serial.print(".");

  }

  Serial.println("");

  Serial.println("WiFi connected");

  Serial.println("IP address: ");

  Serial.println(WiFi.localIP());

}

void callback(char* topic, byte* message, unsigned int length) {

  Serial.print("Message arrived on topic: ");

  Serial.print(topic);

  Serial.print(". Message: ");

  String messageTemp;

  for (int i = 0; i < length; i++) {

    Serial.print((char)message[i]);

    messageTemp += (char)message[i];

  }

  Serial.println();

 

  // Feel free to add more if statements to control more GPIOs with MQTT

  // If a message is received on the topic esp32/output, you check if the message is either "on" or "off".

  // Changes the output state according to the message

 

if (String(topic) == "esp32/output") {

    Serial.print("Changing output to ");

    if(messageTemp == "on"){

      Serial.println("on");

      //digitalWrite(porta_ventilador, 1); 
      }

    else if(messageTemp == "off"){

      Serial.println("off");

    //digitalWrite(porta_ventilador, 0);
    }

    }

/*if (String(topic) == "esp32/slider") {

    Serial.print("Changing output to ");

    if(slider > "29"){

      Serial.println("on");

      digitalWrite(porta_ventilador, 1);    }

    else if(slider < "29" ){

      Serial.println("off");

    digitalWrite(porta_ventilador, 0);    }

    }*/

}

void reconnect() {

  // Loop until we're reconnected

  while (!client.connected()) {

    Serial.print("Attempting MQTT connection...");

    // Attempt to connect

    if (client.connect("ESP8266Client", mqtt_user, mqtt_pass)) {

      Serial.println("connected");

      // Subscribe

      client.subscribe("esp32/output");

      //client.subscribe("esp32/slider");

    } else {

      Serial.print("failed, rc=");

      Serial.print(client.state());

      Serial.println(" try again in 5 seconds");

      // Wait 5 seconds before retrying

      delay(5000);

    }

  }

}

void loop()
{

  if (!client.connected()) {

    reconnect();

  }

  client.loop();

 

  // A leitura da temperatura e umidade pode levar 250ms!

  // O atraso do sensor pode chegar a 2 segundos.

  float h = dht.readHumidity();

  float t = dht.readTemperature();

  // testa se retorno é valido, caso contrário algo está errado.

  if (isnan(t) || isnan(h)) // not a number ( numero nao válido)

  {

    //Serial.println("Failed to read from DHT");

  }

  else

  {

     long now = millis();

  if (now - lastMsg > 5000) {

     char tempString[8];

        lastMsg = now;

        dtostrf(t, 1, 2, tempString);


      client.publish("esp32/temperature", tempString);


    Serial.print("Umidade: ");

    Serial.print(h);

    Serial.print(" %t");

    Serial.print("Temperatura: ");

    Serial.print(t);

    Serial.println(" *C  ");

  }

  }

    /*if (t > pot)

      digitalWrite(porta_ventilador, 1);

    else

      digitalWrite(porta_ventilador, 0);

      */

    

}
  
  
  

