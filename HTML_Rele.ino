#include <WiFi.h>
#include <WebServer.h>

// Configuración WiFi
const char* ssid = "IoTB";
const char* password = "inventaronelVAR";

// Crear un objeto de servidor web en el puerto 80
WebServer server(80);

// Pines para controlar
const int pin4 = 4;
const int pin5 = 5;

void setup() {
  Serial.begin(115200);
  
  // Inicializar pines como salidas
  pinMode(pin4, OUTPUT);
  pinMode(pin5, OUTPUT);

  // Conectar a la red WiFi
  WiFi.begin(ssid, password);
  Serial.print("Conectando a WiFi...");
  while (WiFi.status() != WL_CONNECTED) {
    delay(1000);
    Serial.print(".");
  }
  Serial.println();
  Serial.print("Conectado a WiFi. Dirección IP: ");
  Serial.println(WiFi.localIP());

  // Configurar las rutas del servidor web
  server.on("/", handleRoot);
  server.on("/toggle", HTTP_POST, handleToggle); // Set to handle POST requests
  server.begin();
  Serial.println("Servidor HTTP iniciado");
}

void loop() {
  // Manejar solicitudes HTTP
  server.handleClient();
}

// Función para manejar la solicitud a la raíz ("/")
void handleRoot() {
  String html = "<!DOCTYPE html><html>";
  html += "<head><meta http-equiv='refresh' content='10'/>";
  html += "<title>Control de Pines</title>";
  html += "<style>";
  html += "body { font-family: Arial, sans-serif; background-color: #f0f8ff; text-align: center; padding: 50px;}";
  html += ".container { background-color: #ffffff; padding: 20px; border-radius: 10px; box-shadow: 0px 0px 10px rgba(0, 0, 0, 0.1); max-width: 400px; margin: auto;}";
  html += "h1 { color: #333333; }";
  html += "button { padding: 10px 20px; font-size: 16px; margin: 5px; }";
  html += "</style></head>";
  html += "<body>";
  html += "<div class='container'>";
  html += "<h1>Control de Pines 4 y 5</h1>";
  html += "<form action='/toggle' method='POST' style='display:inline;'>";
  html += "<button name='pin' value='4'>Toggle Pin 4</button>";
  html += "</form>";
  html += "<form action='/toggle' method='POST' style='display:inline;'>";
  html += "<button name='pin' value='5'>Toggle Pin 5</button>";
  html += "</form>";
  html += "</div>";
  html += "</body></html>";

  server.send(200, "text/html", html);
}

// Función para manejar el cambio de estado de los pines
void handleToggle() {
  if (server.hasArg("pin")) {
    int pin = server.arg("pin").toInt();
    if (pin == 4 || pin == 5) {
      digitalWrite(pin, !digitalRead(pin));  // Cambiar el estado del pin
    }
  }
  // Redirigir de vuelta a la página principal
  server.sendHeader("Location", "/");
  server.send(302, "text/plain", "Redirecting...");
}
