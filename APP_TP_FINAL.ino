 //Declaracion de Librerías
#include "RedWifi.h"
#include "Sensores.h"
#include <HTTPClient.h>


//Declaracion de pines de sensores
#define PIN_DHT    4  // Pin digital al que se conecta el sensor DHT11
#define PIN_LDR    5   // Pin al que se conecta el sensor LDR
#define PIN_SUELO  6   // Pin al que se conecta el sensor de suelo
#define PIN_LLUVIA 7   // Pin digital al que se conecta el sensor de lluvia

//Declaracion de pines de salida
#define PIN_BOMBA    10 // Pin que habilita bomba de riego
#define PIN_LUZ      11 // Pin que habilita LUZ
#define PIN_VENT     12 // Pin que habilita Ventilación

//Credenciales red WIFI
const char* ssid = "MOVISTAR WIFI7361";  // SSID
const char* password = "bocajuniors12";  //Clave

//Direccion del servidor
String serverName = "http://isaaccarranza.ar/test/GET_DatosESP32.php";
String serverNameStatus = "http://isaaccarranza.ar/test/estado_riego.php";

//Declaracion de Variables
unsigned long lastTime = 0; //Se utiliza una variable para controlar el tiempo transcurrido
unsigned long timerDelay = 3000; //3 seg
IPAddress IP_ESP32;
String apiKeyValue; //"tPmAT5Ab3j7F9";//variable para validar petciones a php

//Declaracion de Variables medidas de sensores
int sensor_humSuelo; 
float sensor_humAmbiente;
float sensor_tempAmbiente;
int sensor_nivelLluvia;
int sensor_luzAmbiente;
String cadenaDatos;

//Declaracion de Variables para control de salidas
boolean estadoRiegoManual = false; //control mediante el boton en la web
boolean estadoVentManual = false; 
boolean estadoLuzManual = false; 


void setup() {
  // put your setup code here, to run once:
Serial.begin(115200);
IP_ESP32 = conectarWifi(ssid, password);

Serial.println(IP_ESP32);
pinMode(PIN_BOMBA, OUTPUT);
pinMode(PIN_VENT, OUTPUT);
pinMode(PIN_LUZ, OUTPUT);

}

void loop() {

  if ((millis() - lastTime) > timerDelay) {
 
   if (WiFi.status() == WL_CONNECTED) {

      WiFiClient client;
      HTTPClient http;

      //Asigancion de valores tomados de sensores
      sensor_humSuelo = humedadTierra(PIN_SUELO); 
      sensor_humAmbiente = humedadAmbiente(PIN_DHT);
      sensor_tempAmbiente = temperaturaAmbiente(PIN_DHT);
      sensor_nivelLluvia = nivel_de_lluvia(PIN_LLUVIA);
      sensor_luzAmbiente = luzAmbiente(PIN_LDR);
        
      //Creacion de codigo para validar en php
      apiKeyValue = CrearCodigoValidacion(sensor_humSuelo, sensor_humAmbiente, sensor_tempAmbiente, sensor_nivelLluvia, sensor_luzAmbiente);
      Serial.println(apiKeyValue);
      //Creacion de cadena con variables y valores
      cadenaDatos = "?n=" + apiKeyValue + "&ht=" + sensor_humSuelo +  "&ha=" + sensor_humAmbiente +  "&temp=" + sensor_tempAmbiente + "&l=" + sensor_nivelLluvia + "&kjl="+ sensor_luzAmbiente;
      
      //Creacion de cadena a enviar por get
      String serverPath = serverName + cadenaDatos ;

      // Tu ruta URL o dirección IP al servicio web en una url "limpia"
      http.begin(client, serverPath.c_str());

      // Envía HTTP GET request
      int httpCodigoRespuesta = http.GET(); //

      if (httpCodigoRespuesta > 0) {        //En caso que la respuesta de la conexion GET sea por eje. 200 o 404 se imprimirá, en caso que no responda el servidor el codigo es -1
        Serial.print("HTTP codigo respuesta: ");
        Serial.println(httpCodigoRespuesta);
        String payload = http.getString();
        Serial.println(payload);
      }
      else {
        Serial.print("Error code: ");
        Serial.println(httpCodigoRespuesta);
      }
      // Libera recursos
      http.end();
      Serial.println("---------------------------------------------------------------------------------------------------");
      delay(1000);
      // Peticion para estados de salidas
      // Tu ruta URL o dirección IP al servicio web en una url "limpia"
      http.begin(client, serverNameStatus);

      // Envía HTTP GET request
      int httpCodigoRespta = http.GET(); //

      if (httpCodigoRespuesta > 0) {        //En caso que la respuesta de la conexion GET sea por eje. 200 o 404 se imprimirá, en caso que no responda el servidor el codigo es -1
        Serial.print("HTTP codigo respuesta2: ");
        Serial.println(httpCodigoRespuesta);
        String payload = http.getString();
        Serial.println(payload);

         int ledStatus = payload.toInt();

            // Hacer algo con el valor recibido (por ejemplo, encender el LED)
            if (ledStatus == 1) {
                Serial.println("led encendido");
                digitalWrite(LED_BUILTIN, (0,0,255));

            } else {
                Serial.println("led apagao");
                digitalWrite(LED_BUILTIN, LOW);
            }
      }
      else {
        Serial.print("Error code: ");
        Serial.println(httpCodigoRespuesta);
      }
      // Libera recursos
      http.end();
      // Espera antes de realizar la siguiente solicitud
      delay(1000);
    }
    else{
      Serial.println("Error al conectarse a la red WIFI");
    }
    //------------------------------------------------------------------------------------------------
    //Logica de control de riego iluminación y ventilacion
    //Control automatico y manual de bomba

    if ((sensor_humSuelo <= 50 && sensor_luzAmbiente <=30 && sensor_nivelLluvia <= 15) || (estadoRiegoManual == true && sensor_humSuelo <=90 && sensor_nivelLluvia <= 15)) {

     digitalWrite(PIN_BOMBA, HIGH);
     delay(10000);
     digitalWrite(PIN_BOMBA, LOW);
     estadoRiegoManual = false;
    }
    
    //Control automatico y manual de ventilación

    if ((sensor_tempAmbiente >= 35) || (sensor_humAmbiente >= 70) || (estadoVentManual == true)){
     
    digitalWrite(PIN_VENT, HIGH);
    delay(10000);
    digitalWrite(PIN_VENT, LOW);
    estadoVentManual = false;
    }
    
    //Control automatico y manual de luz

    if((sensor_luzAmbiente <=30)||(estadoLuzManual == true)){
      digitalWrite(PIN_LUZ, HIGH);
    }

    else {

      digitalWrite(PIN_LUZ, LOW);

    }

 
  lastTime = millis();
  }

}


