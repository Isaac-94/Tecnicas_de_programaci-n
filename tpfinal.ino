int humedadSuelo;
int humedad;

void setup() {
  // put your setup code here, to run once:
    Serial.begin(115200);
  Serial.println("Hello, ESP32!");
 pinMode(10,INPUT);
 
}

void loop() {
  humedadSuelo=analogRead(1000);
  humedad = ( 100 - ( (humedadSuelo/4095.00) * 100 ) );
  Serial.print(humedad);
  Serial.println("%");
  digitalWrite(LED_BUILTIN, (0,0,255));
  delay(1000);
  digitalWrite(LED_BUILTIN, LOW);
  delay(1000);


 if (humedad<=33){

  neopixelWrite(RGB_BUILTIN,RGB_BRIGHTNESS,0,0); // Red
  delay(1000);

 }
 else if(humedad>=34 && humedad<=66){
  neopixelWrite(RGB_BUILTIN,0,RGB_BRIGHTNESS,0); // Green
  delay(1000);


 }
 else {  
  neopixelWrite(RGB_BUILTIN,0,0,RGB_BRIGHTNESS); // Blue
  delay(1000);


 }
    // put your main code here, to run repeatedly:
  neopixelWrite(RGB_BUILTIN,0,0,0); // Off / black
  delay(1000);              /* Wait for 1000mS */
  // put your main code here, to run repeatedly:

}
