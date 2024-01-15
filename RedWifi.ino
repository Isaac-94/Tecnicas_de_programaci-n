 #include <WiFi.h> 



/* Añade tu SSID & Clave para acceder a tu Wifi */
//const char* ssid = "silencio";  // Tu SSID
//const char* password = "perroperro";  //Tu Clave
 
 IPAddress conectarWifi( const char* ssid, const char* password){
 Serial.println(ssid);
 Serial.println(password);
 WiFi.mode(WIFI_STA);
  WiFi.begin(ssid, password);
  Serial.println("");
 
  // Wait for connection
  while (WiFi.status() != WL_CONNECTED) {
    delay(500);
    Serial.print(".");
  
    }
  

  Serial.println("");
  Serial.print("Conectado a Wifi con IP: ");
  return WiFi.localIP();
 }

//---------------------------------------------------------------

 float CrearCodigoValidacion(int a, float b, float c, int d, int e){
  float valor = (a + b + c + d + e)/2; 
   
  return valor;
 }