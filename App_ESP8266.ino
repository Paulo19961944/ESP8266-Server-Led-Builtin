#include <ESP8266WiFi.h>
#include <ESP8266WebServer.h>

// Configurações da rede Wi-Fi
const char* ssid = "YOUR_WIFI";
const char* password = "YOUR_PASSWORD";

// Cria uma instância do servidor na porta 80
ESP8266WebServer server(80);

// Estado do LED
bool ledState = HIGH;

// Função para lidar com a requisição para acender o LED
void handleTurnOnLed() {
  ledState = LOW;
  digitalWrite(LED_BUILTIN, ledState);
  server.send(200, "text/plain", "LED Acendido");
}

// Função para lidar com a requisição para apagar o LED
void handleTurnOffLed() {
  ledState = HIGH;
  digitalWrite(LED_BUILTIN, ledState);
  server.send(200, "text/plain", "LED Apagado");
}

// Função para lidar com a requisição para piscar o LED rapidamente
void handleBlinkFast() {
  for (int i = 0; i < 10; i++) {
    digitalWrite(LED_BUILTIN, LOW);
    delay(100);
    digitalWrite(LED_BUILTIN, HIGH);
    delay(100);
  }
  server.send(200, "text/plain", "LED Piscando Rápido");
}

// Função para lidar com a requisição para piscar o LED lentamente
void handleBlinkSlow() {
  for (int i = 0; i < 10; i++) {
    digitalWrite(LED_BUILTIN, LOW);
    delay(500);
    digitalWrite(LED_BUILTIN, HIGH);
    delay(500);
  }
  server.send(200, "text/plain", "LED Piscando Lento");
}

// Função para servir o HTML
void handleRoot() {
  String html = R"rawliteral(
<!DOCTYPE html>
<html lang="pt-br">
<head>
    <meta charset="UTF-8">
    <meta name="viewport" content="width=device-width, initial-scale=1.0">
    <title>IoT ESP8266</title>
    <link rel="stylesheet" href="https://cdnjs.cloudflare.com/ajax/libs/font-awesome/6.0.0/css/all.min.css">
    <style>
        *{
            margin: 0;
            padding: 0;
            box-sizing: border-box;
        }

        body{
            background-color: #f7f7f7;
            font-family: Arial, Helvetica, sans-serif;
            display: flex;
            flex-direction: column;
            align-items: center;
            width: 100vw;
            min-height: 100vh;
        }

        h1{
            margin: 32px 0 48px;
        }

        .container{
            display: flex;
            flex-direction: column;
            align-items: center;
        }

        #acendeApaga, #pisca{
            display: flex;
            justify-content: center;
            align-items: center;
            gap: 32px;
        }

        #pisca{
            margin-top: 32px;
        }

        #acenderLed, #apagarLed, #piscaLedRapido, #piscaLedLento{
            display: flex;
            justify-content: center;
            align-items: center;
            border-radius: 16px;
            width: 280px;
            height: 216px;
        }

        #acenderLed{
            background-color: #aee3bc;
        }

        #apagarLed{
            background-color: #ffafaf;
        }

        #piscaLedRapido{
            background-color: #f2e171;
        }

        #piscaLedLento{
            background-color: #71f0f2;
        }

        .fa-lightbulb{
            font-size: 26px;
            margin-right: 16px;
        }

        @media screen and (max-width: 651px){
            #acenderLed, #apagarLed, #piscaLedRapido, #piscaLedLento{
                width: 148px;
                height: 160px;
            }

            h1{
                font-size: 1.5rem;
                margin: 48px 0 96px;
            }

            p{
                font-size: 0.75rem;
            }

            .fa-lightbulb{
                font-size: 14px;
                margin-right: 8px;
            }
        }
    </style>
</head>
<body>
    <main class="container">
        <h1>IoT ESP8266</h1>
        <section id="acendeApaga">
            <article id="acenderLed" onclick="fetch('/on')">
                <i class="fa-solid fa-lightbulb acenderLamp"></i>
                <p>Acender Led</p>
            </article>
            <article id="apagarLed" onclick="fetch('/off')">
                <i class="fa-solid fa-lightbulb apagarLamp"></i>
                <p>Apagar Led</p>
            </article>
        </section>
        <section id="pisca">
            <article id="piscaLedRapido" onclick="fetch('/blinkFast')">
                <i class="fa-solid fa-lightbulb piscaLampRapido"></i>
                <p>Pisca Led (Rápido)</p>
            </article>
            <article id="piscaLedLento" onclick="fetch('/blinkSlow')">
                <i class="fa-solid fa-lightbulb piscaLampLento"></i>
                <p>Pisca Led (Lento)</p>
            </article>
        </section>
    </main>
</body>
</html>
)rawliteral";

  server.send(200, "text/html", html);
}

void setup() {
  // Inicializa o LED
  pinMode(LED_BUILTIN, OUTPUT);
  digitalWrite(LED_BUILTIN, HIGH);

  // Inicializa a conexão Wi-Fi
  WiFi.begin(ssid, password);
  Serial.begin(115200);
  Serial.println();

  while (WiFi.status() != WL_CONNECTED) {
    delay(500);
    Serial.print(".");
  }
  
  Serial.println("");
  Serial.println("Conectado ao Wi-Fi");
  Serial.print("IP Address: ");
  Serial.println(WiFi.localIP());

  // Define as rotas do servidor
  server.on("/", HTTP_GET, handleRoot);
  server.on("/on", HTTP_GET, handleTurnOnLed);
  server.on("/off", HTTP_GET, handleTurnOffLed);
  server.on("/blinkFast", HTTP_GET, handleBlinkFast);
  server.on("/blinkSlow", HTTP_GET, handleBlinkSlow);

  // Inicia o servidor
  server.begin();
}

void loop() {
  server.handleClient();
}
