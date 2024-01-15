

#include "DHT.h"
#define SENSOR_MIN 0    // Valor de funcion de Sensor de lluvia
#define SENSOR_MAX 521  // Valor de funcion de Sensor de lluvia

//-------------------------------------HUMEDAD DHT11-------------------------------------------------


float humedadAmbiente(int port)
{
  DHT dht1(port, DHT11);
  dht1.begin();
  float humedad = dht1.readHumidity();

  if (isnan(humedad))
  {
    Serial.println("Failed to  read humidity from DHT sensor!");
  }
  delay(100);

  return humedad;
}
//--------------------------------------TEMPERATURA DHT11------------------------------------------------

float temperaturaAmbiente(int port)
{
  DHT dht(port, DHT11);
  dht.begin();
  float temperatura = dht.readTemperature();

  if (isnan(temperatura))
  {
    Serial.println("Failed to read temperature from DHT sensor!");
  }

  return temperatura;
}

//-------------------------------------------HUMEDAD SUELO-------------------------------------------

int humedadTierra(int aux)
{
  int valor;
  int humedad;

  valor = analogRead(aux);
  humedad = (100 - ((valor / 4095.00) * 100));
 
/*
  if (humedad <= 33)
  {

    neopixelWrite(RGB_BUILTIN, RGB_BRIGHTNESS, 0, 0); // Red
    delay(1000);
  }
  else if (humedad >= 34 && humedad <= 66)
  {
    neopixelWrite(RGB_BUILTIN, 0, RGB_BRIGHTNESS, 0); // Green
    delay(1000);
  }
  else
  {
    neopixelWrite(RGB_BUILTIN, 0, 0, RGB_BRIGHTNESS); // Blue
    delay(1000);
  }
  // put your main code here, to run repeatedly:
  neopixelWrite(RGB_BUILTIN, 0, 0, 0); // Off / black
  delay(1000);                         /* Wait for 1000mS */
  // put your main code here, to run repeatedly:
  return humedad;
}

//--------------------------------------------SENSOR LLUVIA------------------------------------------

int nivel_de_lluvia(int val)
{
  int value = 0; // variable to store the sensor value
  int nivel = 0;

  value = analogRead(val);                            // read the analog value from sensor
  nivel = map(value, SENSOR_MIN, SENSOR_MAX, 0, 5); // 4 levels
  return nivel;
}

//---------------------------------------------SENSOR LUZ-----------------------------------------

int luzAmbiente(int pines)
{
  const long A = 6000; // Resistencia en oscuridad en KΩ
  const int B = 15;    // Resistencia a la luz (10 Lux) en KΩ
  const int Rc = 11;   // Resistencia calibracion en KΩ
  int Valor;
  int nivel_de_luz;

  Valor = analogRead(pines);

  Valor = constrain(Valor, 700, 4095);
  nivel_de_luz = map(Valor, 700, 4095, 0, 100);
   //nivel_de_luz = ((long)Valor*A*10)/((long)B*Rc*(1024-Valor));    //usar si LDR entre A0 y Vcc (como en el esquema anterior)
  return nivel_de_luz;
  
}

//-----------------------------------------------------------------------------------------
