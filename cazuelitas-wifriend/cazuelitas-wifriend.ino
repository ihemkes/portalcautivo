// ESP8266 WiFi Captive Portal 
// By ihemkes (github.com/ihemkes)

// Libraries
#include <ESP8266WiFi.h>
#include <DNSServer.h> 
#include <ESP8266WebServer.h>
#include <EEPROM.h>

// Default SSID name
const char* SSID_NAME = "🍲 Las Cazuelitas 🌼";

// Default main strings
#define SUBTITLE "🍲🥗 Sabor Casero 🍽🍳"
#define TITLE "Encuesta de satisfacción"
#define BODY "Ayúdenos a mejorar nuestro servicio. Por favor conteste las siguientes preguntas para acceder al internet gratuito: "
#define POST_TITLE "Gracias por su preferencia 🙏"
String POST_BODY = "<div><img src=\"data:image/png; base64,iVBORw0KGgoAAAANSUhEUgAAASwAAAEsCAYAAAB5fY51AAAABGdBTUEAALGPC/xhBQAAACBjSFJNAAB6JgAAgIQAAPoAAACA6AAAdTAAAOpgAAA6mAAAF3CculE8AAAABmJLR0QAAAAAAAD5Q7t/AAAACXBIWXMAAABgAAAAYADwa0LPAAAGlUlEQVR42u3dQU4rRxhGUYjeDpDZ//o8YAvOyMOXttIUf93mnGn0TNmQqxp8ar8/Ho/HG0DAP9MHAHiVYAEZggVkCBaQIVhAhmABGYIFZAgWkCFYQIZgARmCBWQIFpAhWECGYAEZggVkCBaQIVhAhmABGYIFZAgWkCFYQIZgARmCBWQIFpAhWECGYAEZggVkCBaQIVhAhmABGYIFZAgWkCFYQMaf6QM8fXx8vH19fU0fY5nH4/Gf//39/X3p6692dP7dz3fk6Py//e/3p7hhARmCBWQIFpAhWECGYAEZggVkCBaQsc0O68j9fn+73W7Tx/irszucXXYuq85f35mddfW/35/ihgVkCBaQIVhAhmABGYIFZAgWkCFYQEZmh3Xk7M7nyOod0OrnSdV3UKvPv/rvZ/X7O/v+K9ywgAzBAjIEC8gQLCBDsIAMwQIyBAvIuMwO6+pW77SustPh2tywgAzBAjIEC8gQLCBDsIAMwQIyBAvIsMO6iN2f5zT9PKrpz4fv4YYFZAgWkCFYQIZgARmCBWQIFpAhWEDGZXZY9ec5nd0prf7ewqPXX/35r95xTf/9TP/8CjcsIEOwgAzBAjIEC8gQLCBDsIAMwQIyMjusz8/P6SMstfp5Uqt3PtM//8j053f1v9+f4oYFZAgWkCFYQIZgARmCBWQIFpAhWEDG+2N6IMNLpndau3/v4Fn+N2hwwwIyBAvIECwgQ7CADMECMgQLyBAsIGObHdb0Tmf6e/12f57UatPfu3hW/XyVvz83LCBDsIAMwQIyBAvIECwgQ7CADMECMrbZYR0edPPnMU3vcHb/Na5+f9M7oukd2e6//+/ihgVkCBaQIVhAhmABGYIFZAgWkCFYQMZldlh1u/8a6jux+g5v+nsjp39/T25YQIZgARmCBWQIFpAhWECGYAEZggVkZHZYp9/oxXc4q9//6ueN1V//7M+fZocF8M0EC8gQLCBDsIAMwQIyBAvIECwg48/0AV61+/OYpndUZ19/+nlL03Z//6vPt8vO6ogbFpAhWECGYAEZggVkCBaQIVhAhmABGZkd1tV3TNOvf/bnT5v+fHb//e6+Y3yVGxaQIVhAhmABGYIFZAgWkCFYQIZgARnb7LB234nsfr76Tu2s6R3a9Pf6Te/QfoobFpAhWECGYAEZggVkCBaQIVhAhmABGdvssI5MPy9o9x1R/XzT3+u4Wv3z24UbFpAhWECGYAEZggVkCBaQIVhAhmABGdvssFbvTKZ3Pr73cNbuO6Sr7wy/ixsWkCFYQIZgARmCBWQIFpAhWECGYAEZ2+ywzlq9E9p9B1Z/XtTu5z863/SO6eo7uic3LCBDsIAMwQIyBAvIECwgQ7CADMECMrbZYU0/r6p+/unnZU2ff3oHdfb9r3793T+fV7lhARmCBWQIFpAhWECGYAEZggVkCBaQsc0Oa3onsvtO6ey/n/7eu9X//uzrT+/MplXO74YFZAgWkCFYQIZgARmCBWQIFpAhWEDGNjusI6t3UtM7qLPnP3u+6fc3vQOa3oGttsuO6iw3LCBDsIAMwQIyBAvIECwgQ7CADMECMjI7rNU7kvpOZfp5WFe3+w7s6t/r+OSGBWQIFpAhWECGYAEZggVkCBaQIVhAxjY7rI+Pj7evr6/pYyxztHNZvaO5+s5q+vOZ3jld/ff75IYFZAgWkCFYQIZgARmCBWQIFpAhWEDGNjusI/f7/e12u00f46+md2TTO6DV59v9/Xle289wwwIyBAvIECwgQ7CADMECMgQLyBAsICOzwzqy+nlAu+9gzp7v6s+TWn2+3a3+3sOf4oYFZAgWkCFYQIZgARmCBWQIFpAhWEDGZXZYdat3PtM7oumd2C47ov/7/lb/+90/nyc3LCBDsIAMwQIyBAvIECwgQ7CADMECMuywIlY/j2r6ewHrz9Oa/vyO7H6+V7lhARmCBWQIFpAhWECGYAEZggVkCBaQcZkd1i47kVWmvzdwesczvfNa/fdV2UFNc8MCMgQLyBAsIEOwgAzBAjIEC8gQLCAjs8P6/PycPsKo1Tum374Dmn5e1urzT5/vu7hhARmCBWQIFpAhWECGYAEZggVkCBaQ8f6oDDCAX88NC8gQLCBDsIAMwQIyBAvIECwgQ7CADMECMgQLyBAsIEOwgAzBAjIEC8gQLCBDsIAMwQIyBAvIECwgQ7CADMECMgQLyBAsIEOwgAzBAjIEC8gQLCBDsIAMwQIyBAvIECwgQ7CADMECMgQLyPgXCUtHKTdlhXcAAAAldEVYdGRhdGU6Y3JlYXRlADIwMjQtMDQtMDlUMTk6NTE6MjcrMDA6MDBno+PfAAAAJXRFWHRkYXRlOm1vZGlmeQAyMDI0LTA0LTA5VDE5OjUxOjI3KzAwOjAwFv5bYwAAAABJRU5ErkJggg==\"/>"
                  "</div>"
                  "&nbsp"
                  "Escanea el còdigo QR para acceder al internet o ingresa a la red con los siguientes datos:<br>"
                  "&nbsp<br>"
                  "<table><tr><th>WiFi</th><th>Contraseña</th></tr><tr><td>comerico</td><td><i>salsaverde</i></td></tr></table>";
#define PASS_TITLE "Registros"
#define CLEAR_TITLE "Registros eliminados"

// Init system settings
const byte HTTP_CODE = 200;
const byte DNS_PORT = 53;
const byte TICK_TIMER = 1000;
IPAddress APIP(172, 0, 0, 1); // Gateway

String allPass = "";
String newSSID = "";
String currentSSID = "";

// For storing passwords in EEPROM.
int initialCheckLocation = 20; // Location to check whether the ESP is running for the first time.
int passStart = 30;            // Starting location in EEPROM to save password.
int passEnd = passStart;       // Ending location in EEPROM to save password.


unsigned long bootTime=0, lastActivity=0, lastTick=0, tickCtr=0;
DNSServer dnsServer; ESP8266WebServer webServer(80);

String input(String argName) {
  String a = webServer.arg(argName);
  a.replace("<","&lt;");a.replace(">","&gt;");
  a.substring(0,200); return a; }

String footer() { 
  return "</div><div class=q><a>San Clemente de Lima #250. Bahía de Banderas, Nay.</a></div>";
}

String header(String t) {
  String a = String(currentSSID);
  String CSS = "article { background: #ffd859; padding: 1.3em; }" 
    "body { color: #333; font-family: Century Gothic, sans-serif; font-size: 18px; line-height: 24px; margin: 0; padding: 0; }"
    "div { padding: 0.5em; margin-left:auto; margin-right:auto;}"
    "h1 { padding: 0.5em; margin-left:auto; margin-right:auto;}"
    "input[type=text]{ width: 100%; padding: 9px 10px; margin: 8px 0; box-sizing: border-box; border-radius: 0; border: 1px solid #555555; border-radius: 10px; font-size:20px; }"
    "input[type=number]{ padding: 9px 10px; margin: 8px 0; box-sizing: border-box; border-radius: 0; border: 1px solid #555555; border-radius: 10px; font-size:20px; }"
    "input[type=tel]{ width: 100%; padding: 9px 10px; margin: 8px 0; box-sizing: border-box; border-radius: 0; border: 1px solid #555555; border-radius: 10px; font-size:20px; }"
    "input[type=submit]{ width: 100%; padding: 9px 10px; margin: 8px 0; box-sizing: border-box; border-radius: 0; border: 1px solid #555555; border-radius: 10px; font-size:20px; }"
    "input[type=checkbox]{ vertical-align:left; border: 1px solid #555555 }"
    "img {margin-left:auto; margin-right:auto}"
    "table, th, td { margin-left: 60px; margin-right:auto; border: 1px solid black;.}"
    "label { color: #333; display: block; font-style: italic; font-weight: bold; display: inline-block; }"
    "nav { background: #0066ff; color: #fff; display: block; font-size: 1.3em; padding: 1em; }"
    "nav b { display: block; font-size: 1.5em; margin-bottom: 0.5em; } "
    "textarea { width: 100%; }";
  String h = "<!DOCTYPE html><html>"
    "<head><title>" + a + " :: " + t + "</title>"
    "<meta name=viewport content=\"width=device-width,initial-scale=1\">"
    "<style>" + CSS + "</style>"
    "<meta charset=\"UTF-8\"></head>"
    "<body><nav><b>" + a + "</b> " + SUBTITLE + "</nav><div><h1>" + t + "</h1></div><div>";
  return h; }

String index() {
  return header(TITLE) + "<div>" + BODY + "</ol></div><div><form action=/post method=post>"+

    "&nbsp"+
    
    "<label>*Nombre:</label>"+
    "<input type=text name=m required></input><br>"+
   
    "&nbsp<br>"+
    
    "<label>*Edad:</label><br>"+
    "<input type=number id=age name=e min=5 max=100 required></input><br>"+
    
    "&nbsp<br>"+

    "<label>Teléfono:</label>"+
    "<label>(10 dígitos)</label>"+
    "<input type=tel id=phone name=phone pattern=[0-9]{10 ></input><br>"+
   
    
    "<input id=siofertas type=checkbox name=siofertas value=✅>Deseo recibir el menú diario, promociones y ofertas por Whatsapp</input><br>"+

    "&nbsp<br>"+
 
    
    "<label>*Comida favorita:</label>"+
    "<input type=text name=c required> </input><br>"+
    
    
    "&nbsp<br>"+
   
    "<label>Comentarios o sugerencias:</label>"+
    "<input type=text name=cs> </input><br>"+

   
    "&nbsp<br>"+
    
    "<input type=submit value=Enviar>"+
    
    "</form>" + footer();
}

String posted() {
  String data = "";
  String nombre = input("m");
  String edad = input("e");
  String telefono = input("phone");
  String ofertas = input("siofertas");
  if (ofertas != "✅"){
    ofertas="❌";}
    
  String comida = input("c");
  String comentarios = input("cs");
  
  
  data = "<tr><td>" + nombre + "</td><td>" + edad + "</td><td>" + telefono + "</td><td>" + ofertas + "</td><td>" + comida + "</td><td>" + comentarios + "</td></tr>" ; // Adding info in a table
  allPass += data;                       // Updating the full list

  // Storing passwords to EEPROM.
  for (int i = 0; i <= data.length(); ++i)
  {
    EEPROM.write(passEnd + i, data[i]); // Adding password to existing password in EEPROM.
  }

  passEnd += data.length(); // Updating end position of passwords in EEPROM.
  EEPROM.write(passEnd, '\0');
  EEPROM.commit();
  return header(POST_TITLE) + POST_BODY + footer();
}

String pass() {
  return header(PASS_TITLE) + "<table><tr><th>Nombre</th><th>Edad</th><th>Teléfono</th><th>Ofertas?</th><th>Comida Fav</th><th>Comentarios</th></tr>" + allPass + "</table><br><center><p><a style=\"color:blue\" href=/>Volver a cuestionario</a></p><p><a style=\"color:blue\" href=/clear>Limpiar registros</a></p></center>" + footer();
}

String ssid() {
  return header("Change SSID") + "<p>Desde aquí puedes cambiar el SSID. Después de presionar el botón \"Cambiar SSID\" perderás la conexión, conéctate desde la nueva SSID.</p>" + "<form action=/postSSID method=post><label>Nuevo SSID:</label>"+
    "<input type=text name=s></input><input type=submit value=\"Cambiar SSID\"></form>" + footer();
}

String postedSSID() {
  String postedSSID = input("s"); newSSID="<li><b>" + postedSSID + "</b></li>";
  for (int i = 0; i < postedSSID.length(); ++i) {
    EEPROM.write(i, postedSSID[i]);
  }
  EEPROM.write(postedSSID.length(), '\0');
  EEPROM.commit();
  WiFi.softAP(postedSSID);
  return postedSSID;
}

String clear() {
  allPass = "";
  passEnd = passStart; // Setting the password end location -> starting position.
  EEPROM.write(passEnd, '\0');
  EEPROM.commit();
  return header(CLEAR_TITLE) + "<div><p>Todos los registros han sido eliminados.</div></p><center><a style=\"color:blue\" href=/>Volver a página principal</a></center>" + footer();
}

void BLINK() { // The built-in LED will blink 5 times after a password is posted.
  for (int counter = 0; counter < 10; counter++)
  {
    // For blinking the LED.
    digitalWrite(BUILTIN_LED, counter % 2);
    delay(500);
  }
}

void setup() {
  // Serial begin
  Serial.begin(115200);
  
  bootTime = lastActivity = millis();
  EEPROM.begin(512);
  delay(10);

  // Check whether the ESP is running for the first time.
  String checkValue = "first"; // This will will be set in EEPROM after the first run.

  for (int i = 0; i < checkValue.length(); ++i)
  {
    if (char(EEPROM.read(i + initialCheckLocation)) != checkValue[i])
    {
      // Add "first" in initialCheckLocation.
      for (int i = 0; i < checkValue.length(); ++i)
      {
        EEPROM.write(i + initialCheckLocation, checkValue[i]);
      }
      EEPROM.write(0, '\0');         // Clear SSID location in EEPROM.
      EEPROM.write(passStart, '\0'); // Clear password location in EEPROM
      EEPROM.commit();
      break;
    }
  }
  
  // Read EEPROM SSID
  String ESSID;
  int i = 0;
  while (EEPROM.read(i) != '\0') {
    ESSID += char(EEPROM.read(i));
    i++;
  }

  // Reading stored password and end location of passwords in the EEPROM.
  while (EEPROM.read(passEnd) != '\0')
  {
    allPass += char(EEPROM.read(passEnd)); // Reading the store password in EEPROM.
    passEnd++;                             // Updating the end location of password in EEPROM.
  }
  
  WiFi.mode(WIFI_AP);
  WiFi.softAPConfig(APIP, APIP, IPAddress(255, 255, 255, 0));

  // Setting currentSSID -> SSID in EEPROM or default one.
  currentSSID = ESSID.length() > 1 ? ESSID.c_str() : SSID_NAME;

  Serial.print("Current SSID: ");
  Serial.print(currentSSID);
  WiFi.softAP(currentSSID);  

  // Start webserver
  dnsServer.start(DNS_PORT, "*", APIP); // DNS spoofing (Only for HTTP)
  webServer.on("/post",[]() { webServer.send(HTTP_CODE, "text/html", posted()); BLINK(); });
  webServer.on("/ssid",[]() { webServer.send(HTTP_CODE, "text/html", ssid()); });
  webServer.on("/postSSID",[]() { webServer.send(HTTP_CODE, "text/html", postedSSID()); });
  webServer.on("/datos",[]() { webServer.send(HTTP_CODE, "text/html", pass()); });
  webServer.on("/clear",[]() { webServer.send(HTTP_CODE, "text/html", clear()); });
  webServer.onNotFound([]() { lastActivity=millis(); webServer.send(HTTP_CODE, "text/html", index()); });
  webServer.begin();

  // Enable the built-in LED
  pinMode(BUILTIN_LED, OUTPUT);
  digitalWrite(BUILTIN_LED, HIGH);
}


void loop() { 
  if ((millis() - lastTick) > TICK_TIMER) {lastTick = millis();} 
dnsServer.processNextRequest(); webServer.handleClient(); }
