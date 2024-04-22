/*
  ESP32 WEB SERVER

  This sketch provides an example of creating a Web Server with ESP32 devices.
  It allow to control a pair of led through two respective buttons on a web page.
  An MPU 6050 makes it possible to monitor the oreintation of the breadboard 
  on which it is mounted on, which is showed too on the web page.

  As a style source for HTML, Bootstrap is used.

  The circuit:
    - LED1: GPIO26
    - LED2: GPIO27

  Created 27/08/2021
  By Dott. Daniel Rossi
  Copyright: ESP32 WEB SERVER © 2021 - GPLv3
  Edited by Sreeram Ramesh
  Github: https://github.com/Sreeram-Ramesh

  YOUTUBE: Https://youtube.com/c/ProjectoOfficial
  COLLABORATION E-MAIL: miniprojectsofficial@gmail.com
*/

// First of all, we need to use a library to make it easier to build our webserver!

#include <WiFi.h>

// Libraries to use the IMU MPU 6050

// Uncommment MPU 6050 code marked with mpu6050 integration comment once done with LEDs.

// mpu6050 integration
// #include <Adafruit_MPU6050.h>
// #include <Adafruit_Sensor.h>
// #include <Wire.h>
// Adafruit_MPU6050 mpu;

/*
Then let's create some macros for defining the pins, and CONNECT_TIME referes to the time 
to wait for connecting to wifi router, and TIMETOUTTIME referes to the max wait time for 
answering to a client.
*/

#define LED1 26
#define LED2 27
#define CONNECT_TIME 10000
#define TIMEOUTTIME 5000

// Now we need to insert wifi's SSID and Password
// Try it out with ssn credentials, 
// if weak use your hotspot name and password.

const char* ssid = "iPhone 12";
const char* password = "12345678";
WiFiServer server(80);

// We need a header for managing the client request and 
// response and two string which will show the led state on 
//the web page

String header;
String LED1State = "off";
String LED2State = "off";

// these two variables help us to keep track of 
// the web page response time

size_t currentTime = millis();
size_t previousTime = 0;

// Let us move to void setup

void setup() {
  
  // Let's set the baud rate to begin serial communication 
  // with our esp32 
  
  Serial.begin(115200);

  // Let us next intiate the various gpio pins that we are to use.

  pinMode(LED1, OUTPUT);
  pinMode(LED2, OUTPUT);

  // Good practice to turn off the LEDs by default.

  digitalWrite(LED1, LOW);
  digitalWrite(LED2, LOW);

  // mpu6050 integration
  // if(!mpu.begin()) {

  //   while(1){

  //     delay(20);
  //   }
  // }


  // Here's the connection, for starting it we use.begin() 
  // of WiFi object and pass to it the ssid and password.
  // Then we monitor wifi status until it is connected, 
  // but if 10 seconds are past and wifi is still not connected,
  // it will abort.

  Serial.print("Attempting to connect to ");
  Serial.println(ssid);
  WiFi.begin(ssid, password);

  size_t start_time = millis();
  size_t dot_time = millis();

  // P.S. every 250ms we print a dot because we want to make it look more professional.

  while ((WiFi.status() != WL_CONNECTED) && (start_time + CONNECT_TIME) > millis()) {
    if (dot_time + 250 < millis()) {
      Serial.print(".");
      dot_time = millis();
    }
  }
  if (WiFi.status() != WL_CONNECTED){
    Serial.println("Connection failed");
    while (1);
  }

  // Well, when wifi is connected we inform the user through serial communication
  // (initialized at 115200 baud), and we show the server IP Address with WiFi.localIP()

  Serial.println("");
  Serial.println("WiFi connected.");
  Serial.println("ESP32 IP address: ");
  Serial.println(WiFi.localIP());
  server.begin();
}

// Let's move on to void loop

void loop() {
  // mpu6050 integration
  // sensors_event_t acc, gcc, temp;

  // First of all we create a WiFiClient object which is returned by server.available().
  // We need to use it later.

  WiFiClient client = server.available();

  // In fact we first check if client is not null,
  // it's a waste of resources to run code in this case.
  // If it is not null we set the two Time variables at the same
  // start time and we create an empty string current line which will be use for process the request

  if (client) {
    currentTime = millis();
    previousTime = currentTime;
    Serial.println("New Client.");
    String currentLine = "";

  // All the code before the row: header ="";
  // will be inside the while loop. So while the client is 
  // connected and the request/response time is not elapsed:

    while (client.connected() && currentTime - previousTime <= TIMEOUTTIME) {
      
    // We save the current time and after that, we read a character 
    // using the client object and then we send it to the serial monitor.
    // We also concat this character to the header string.
      
      currentTime = millis();
      if (client.available()) {
        char c = client.read();
        Serial.write(c);
        header += c;

      // If the new character (c) is a "\n" and the current line is empty
      // it means that the request is terminated (this is how HTTP headers terminate).
      // So we start to print the response http header

        if (c == '\n') {
          if (currentLine.length() == 0) {
            client.println("HTTP/1.1 200 OK");
            client.println("Content-type:text/html");
            client.println("Connection: close");
            client.println();

          // Here we define commands, for example: if the request header contains a 
          // " GET /LED1/on" it means that the user has pushed the LED 1 button on the web page for turning it on.
          // So we set the LED1State to "on" and we send a digitalWrite command to the real LED. Same thing for the other statements!

            if (header.indexOf("GET /LED1/on") >= 0) {
              Serial.println("LED 1 on");
              LED1State = "on";
              digitalWrite(LED1, HIGH);
            } else if (header.indexOf("GET /LED1/off") >= 0) {
              Serial.println("LED1 off");
              LED1State = "off";
              digitalWrite(LED1, LOW);
            } else if (header.indexOf("GET /LED2/on") >= 0) {
              Serial.println("LED2 on");
              LED2State = "on";
              digitalWrite(LED2, HIGH);
            } else if (header.indexOf("GET /LED2/off") >= 0) {
              Serial.println("LED2 off");
              LED2State = "off";
              digitalWrite(LED2, LOW);
            }
  

          // After controlling the hardware, we need to show something to the user, inform him on what just happened.
          // Here starts the HTML for the HTTP Response

            client.println("<!DOCTYPE html><html>");
            client.println("<head><meta name=\"viewport\" content=\"width=device-width, initial-scale=1\">");
            
          // This is usefull for not having issues with character 
          // on web page (use UTF-8)

            client.println("<meta charset=\"UTF-8\">");

          // Here we refresh the page every 5 seconds (this for updating the mpu 6050 sensor value)

            client.println("<meta http-equiv=\"refresh\" content=\"5\" >");
            client.println("<link rel=\"icon\" href=\"data:,\">");

          // Then we import Bootstrap framework and icons (fontawesome)

            client.println("<link rel=\"stylesheet\" href=\"https://stackpath.bootstrapcdn.com/bootstrap/4.3.1/css/bootstrap.min.css\" integrity=\"sha384-ggOyR0iXCbMQv3Xipma34MD+dH/1fQ784/j6cY/iJTQUOhcWr7x9JvoRxT2MZw1T\" crossorigin=\"anonymous\">");
            client.println("<link rel=\"stylesheet\" href=\"https://use.fontawesome.com/releases/v5.6.3/css/all.css\" integrity=\"sha384-UHRtZLI+pbxtHCWp1t77Bi1L4ZtiqrqD80Kn4Z8NTSRyMA2Fd33n5dQ8lWUE00s/\" crossorigin=\"anonymous\">");

            client.println("<script src=\"https://code.jquery.com/jquery-3.3.1.slim.min.js\" integrity=\"sha384-q8i/X+965DzO0rT7abK41JStQIAqVgRVzpbzo5smXKp4YfRvH+8abtTE1Pi6jizo\" crossorigin=\"anonymous\"></script>");
            client.println("<script src=\"https://cdnjs.cloudflare.com/ajax/libs/popper.js/1.14.7/umd/popper.min.js\" integrity=\"sha384-UO2eT0CpHqdSJQ6hJty5KVphtPhzWj9WO1clHTMGa3JDZwrnQq4sF86dIHNDz0W1\" crossorigin=\"anonymous\"></script>");
            client.println("<script src=\"https://stackpath.bootstrapcdn.com/bootstrap/4.3.1/js/bootstrap.min.js\" integrity=\"sha384-JjSmVgyd0p3pXB1rRibZUAYoIIy6OrQ6VrjIEaFf/nJGzIxFDsf4x0xIM+B07jRM\" crossorigin=\"anonymous\"></script>");

          // Here we define the web page object's style

            client.println("<style>");
            client.println("html {font-family: Helvetica; display: inline-block; margin: 0px auto; text-align: center;}");
            client.println(".mybtn {padding: 16px 40px; font-size: 30px;} ");
            client.println(".par { font-size: 16px;}");
            client.println("p {text-align: center;}");
            client.println(".mpu6050 {text-align: center; border: none; margin: 2px; padding: 16px 40px; font-size: 30px;}");
            client.println("</style></head>");

          // Then body starts, this is what will be showed to the user

            client.println("<body class=\"bg-light\">");
            client.println("<h1 style=\"text-align:center; \" class=\"display-4\">ESP32 Web Server </h1> ");
            client.println("<br />");

          // This is the structure of one of the two buttons.
          // If led is off we show the "ON Button", otherwise the "OFF Button".
          // As you can seem, the button referes to a link that we checked before for changing led state! (e.g. href="/LED1/off") 

            client.println("<p class=\"par\">GPIO 26 - State " + LED1State + " </p> ");
            if (LED1State == "off") {
              client.println("<p><a href =\"/LED1/on\" role=\"button\" class=\"btn btn-success mybtn\" >ON</a></p>");
            } else {
              client.println("<p><a href=\"/LED1/off\" role=\"button\" class=\"btn btn-danger mybtn\">OFF</a></p>");
            }
            client.println("<br />");

            client.println("<p class=\"par\">LED2 - State " + LED2State + "</p>");
            if (LED2State == "off") {
              client.println("<p><a href=\"/LED2/on\" role=\"button\" class=\" mybtn btn btn-success mybtn\" >ON</a></p>");
            } else {
              client.println("<p><a href=\"/LED2/off\" role=\"button\" class=\" mybtn btn btn-danger mybtn\">OFF</a></p>");
            }
            client.println("<br />");

            // mpu6050 integration
            // This'll be our piece of code to obtain the live angle reading 
            // of the mpu6050.
            // mpu.getEvent(&acc, &gcc, &temp)
            // client.println("<p class=\"par\">MPU 6050 Raw Data </p>");
            // client.println("<p> <span class=\"MPU6050\" style=\"background-color: #ffc107!important; color:#212529!important;\"> " + gcc.gyro.x + " : X rotation </span></p>");
            // client.println("<p> <span class=\"MPU6050\" style=\"background-color: #ffc107!important; color:#212529!important;\"> " + gcc.gyro.y + " : Y rotation </span></p>");
            // client.println("<p> <span class=\"MPU6050\" style=\"background-color: #ffc107!important; color:#212529!important;\"> " + gcc.gyro.z + " : Z rotation </span></p>");

          // A footer for social links, aesthetics and what not.

            client.println("<br />");
            client.println("<footer class=\"bg-dark text-center text-white\">");
            client.println("<div class=\"container pt-4 \">");
            client.println("<section class=\"mb-4\">");
            client.println("<a class=\"btn btn-outline-light btn-floating m-1\" href=\"https://www.instagram.com/buildclubssn\" target=\"_blank\" rel=\"noopener noreferrer\" role=\"button\">");
            client.println("<i class=\"fab fa-instagram\"></i>");
            client.println("</a>");
            client.println("<a class=\"btn btn-outline-light btn-floating m-1\" href=\"https://www.linkedin.com/company/build-club-ssn\" target=\"_blank\" rel=\"noopener noreferrer\" role=\"button\">");
            client.println("<i class=\"fab fa-linkedin\"></i>");
            client.println("</a>");
            client.println("<a class=\"btn btn-outline-light btn-floating m-1\" href=\"https://github.com/BuildClubSSN\" target=\"_blank\" rel=\"noopener noreferrer\" role=\"button\">");
            client.println("<i class=\"fab fa-github\"></i>");
            client.println("</a>");
            client.println("</section>");
            client.println("</div>");

            client.println("</footer>");


            client.println("</body></html>");
            client.println();
            break;
          } else {

          // If the current length is not zero, we reset currentline string (inside of "if(c == '\n')")

            currentLine = "";
          }
        } else if (c != '\r') {

        // If c is not '\n', if it is not '\r' too, we concat c to the current line

          currentLine += c;
        }
      }
    }

  // End of while loop, we reset the header and disconnect the client

    header = "";
    client.stop();
    Serial.println("Client disconnected.");
    Serial.println("");
  }

  // outside of "if(client)", at the end of the file, we add a 1ms delay

  delay(1);
}