#include <ezButton.h>
#include <Wire.h>
#include <U8g2lib.h>
#include <Arduino.h>

#include <WiFi.h>

const char* ssid = "<Your SSID";
const char* password = "Your Passwd";

// Set web server port number to 80
WiFiServer server(80);

// Variable to store the HTTP request
String header;
\;
// Auxiliar variables to store the current output state
String output26State = "off";
String output27State = "off";
String c1 = "QR Code";
String c2 = "Driver License";

// Assign output variables to GPIO pins
const int output26 = 26;
const int output27 = 27;

// Current time
unsigned long currentTime = millis();
// Previous time
unsigned long previousTime = 0; 
// Define timeout time in milliseconds (example: 2000ms = 2s)
const long timeoutTime = 2000;


#define BUTTON_PIN  18 // ESP32 pin GPIO18, which connected to button
#define SCL_OLED 22
#define SDA_OLED 21
ezButton button(BUTTON_PIN);  // create ezButton object that attach to pin 7;
unsigned long lastCount = 0;

//[page buffer, size = 128 bytes]
U8G2_SH1106_128X64_NONAME_1_SW_I2C OLED(U8G2_R0, SCL_OLED, SDA_OLED, /*reset=*/U8X8_PIN_NONE );

// vit id
const unsigned char card1 [] PROGMEM = {
	0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 
	0xdd, 0xb6, 0xdb, 0xdd, 0xb6, 0xdd, 0xfd, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 
	<your img's converted byte array here>
};

// Aadhar card
const unsigned char card2 [] PROGMEM = {
  <card 2 converted byte array>
};


// Driving License
const unsigned char card3 [] PROGMEM = {
  <card 3 converted byte array>
};

// QR code
const unsigned char card4 [] PROGMEM = {
  <card 4 converted byte array>
};

// Drv logo
const unsigned char card0 [] PROGMEM = {
    <card 0 converted byte array>
};

void setup() {

  Serial.begin(115200);
  // Initialize the output variables as outputs
  pinMode(output26, OUTPUT);
  pinMode(output27, OUTPUT);
  // Set outputs to LOW
  digitalWrite(output26, LOW);
  digitalWrite(output27, LOW);

  // Connect to Wi-Fi network with SSID and password
  Serial.print("Connecting to ");
  Serial.println(ssid);
  WiFi.begin(ssid, password);
  while (WiFi.status() != WL_CONNECTED) {
    delay(500);
    Serial.print(".");
  }
  // Print local IP address and start web server
  Serial.println("");
  Serial.println("WiFi connected.");
  Serial.println("IP address: ");
  Serial.println(WiFi.localIP());
  server.begin();


  OLED.begin(); 
  OLED.setFont(u8g2_font_t0_12b_mf);
  OLED.setColorIndex(1);
  button.setDebounceTime(50); // set debounce time to 50 milliseconds
  button.setCountMode(COUNT_FALLING);

}

void loop() {

  WiFiClient client = server.available();   // Listen for incoming clients

  if (client) {                             // If a new client connects,
    currentTime = millis();
    previousTime = currentTime;
    Serial.println("New Client.");          // print a message out in the serial port
    String currentLine = "";                // make a String to hold incoming data from the client
    while (client.connected() && currentTime - previousTime <= timeoutTime) {  // loop while the client's connected
      currentTime = millis();
      if (client.available()) {             // if there's bytes to read from the client,
        char c = client.read();             // read a byte, then
        Serial.write(c);                    // print it out the serial monitor
        header += c;
        if (c == '\n') {                    // if the byte is a newline character
          // if the current line is blank, you got two newline characters in a row.
          // that's the end of the client HTTP request, so send a response:
          if (currentLine.length() == 0) {
            // HTTP headers always start with a response code (e.g. HTTP/1.1 200 OK)
            // and a content-type so the client knows what's coming, then a blank line:
            client.println("HTTP/1.1 200 OK");
            client.println("Content-type:text/html");
            client.println("Connection: close");
            client.println();
            
            // turns the GPIOs on and off
            if (header.indexOf("GET /26/on") >= 0) {
              Serial.println("Card 1");
              output26State = "on";
              c1 = "ID";
              OLED.firstPage();
              do {
                OLED.drawXBMP(0, 0, 128, 64, card1);
              } while ( OLED.nextPage() );
            } else if (header.indexOf("GET /26/off") >= 0) {
              Serial.println("Card 2");
              output26State = "off";
              c1 = "QR Code";
              OLED.firstPage();
              do {
                OLED.drawXBMP(0, 0, 128, 64, card4);
              } while ( OLED.nextPage() );
            } else if (header.indexOf("GET /27/on") >= 0) {
              Serial.println("Card 3");
              output27State = "on";
              c2 = "Card";
              OLED.firstPage();
              do {
                OLED.drawXBMP(0, 0, 128, 64, card2);
              } while ( OLED.nextPage() );
            } else if (header.indexOf("GET /27/off") >= 0) {
              Serial.println("Card 4");
              output27State = "off";
              c2 = "Some License";
              OLED.firstPage();
              do {
                OLED.drawXBMP(0, 0, 128, 64, card3);
              } while ( OLED.nextPage() );
            }
            
            // Display the HTML web page
            client.println("<!DOCTYPE html><html>");
            client.println("<head><meta name=\"viewport\" content=\"width=device-width, initial-scale=1\">");
            client.println("<link rel=\"icon\" href=\"data:,\">");
            // CSS to style the on/off buttons 
            // Feel free to change the background-color and font-size attributes to fit your preferences
            client.println("<style>html { font-family: Helvetica; display: inline-block; margin: 0px auto; text-align: center;}");
            client.println(".button { background-color: #4CAF50; border: none; color: white; padding: 16px 40px;");
            client.println("text-decoration: none; font-size: 30px; margin: 2px; cursor: pointer;}");
            client.println(".button2 {background-color: #555555;}</style></head>");
            
            
            // Web Page Heading
            client.println("<body><h1>Welcome To All In One Card Server</h1>");
            
            // Display current state, and ON/OFF buttons for GPIO 26  
            client.println("<p>Display " + c1 + "</p>");
            // If the output26State is off, it displays the ON button       
            if (output26State=="off") {
              client.println("<p><a href=\"/26/on\"><button class=\"button\">Next</button></a></p>");
            } else {
              client.println("<p><a href=\"/26/off\"><button class=\"button button2\">Previous</button></a></p>");
            } 
               
            // Display current state, and ON/OFF buttons for GPIO 27  
            client.println("<p>Display " + c2 + "</p>");
            // If the output27State is off, it displays the ON button       
            if (output27State=="off") {
              client.println("<p><a href=\"/27/on\"><button class=\"button\">Next</button></a></p>");
            } else {
              client.println("<p><a href=\"/27/off\"><button class=\"button button2\">Previous</button></a></p>");
              client.println("<p><a href=\"/27/off\"><button class=\"button button3\">RESET</button></a></p>");
            }
            client.println("</body></html>");
            // client.println("<button type="button">RESET</button>");
            // The HTTP response ends with another blank line
            client.println();
            // Break out of the while loop
            break;
          } else { // if you got a newline, then clear currentLine
            currentLine = "";
          }
        } else if (c != '\r') {  // if you got anything else but a carriage return character,
          currentLine += c;      // add it to the end of the currentLine
        }
      }
    }
    // Clear the header variable
    header = "";
    // Close the connection
    client.stop();
    Serial.println("Client disconnected.");
    Serial.println("");
  }
  


  button.loop(); // MUST call the loop() function first
  unsigned long count = button.getCount();
  
  if (count != lastCount) {
      int imgID = count % 5;
      if ( imgID == 0) {
        OLED.firstPage();
        do {
        OLED.drawXBMP(0, 0, 128, 64, card0);
        } while ( OLED.nextPage() );
        
      } else if ( imgID == 1) {
        OLED.firstPage();
        do {
        OLED.drawXBMP(0, 0, 128, 64, card1);
        } while ( OLED.nextPage() );

      } else if ( imgID == 2) {
        OLED.firstPage();
        do {
        OLED.drawXBMP(0, 0, 128, 64, card2);
        } while ( OLED.nextPage() );

      } else if ( imgID == 3) {
        OLED.firstPage();
        do {
        OLED.drawXBMP(0, 0, 128, 64, card3);
        } while ( OLED.nextPage() );

      } else if ( imgID == 4) {
        OLED.firstPage();
        do {
        OLED.drawXBMP(0, 0, 128, 64, card4);
        } while ( OLED.nextPage() );
      }

      lastCount = count;
  }
}
