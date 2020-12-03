int setup_wifi() {
  int time_out = 0;   //when time_out = 40 * delay(500), then return 

  // Connect to Wi-Fi network with SSID and password
  Serial.print("Connecting to ");
  Serial.println(ssid);
  WiFi.begin(ssid, password);
  while (WiFi.status() != WL_CONNECTED) {
    delay(500);
    Serial.print(".");
    if (time_out > 40) {
        return 0;
        break;
    }
    time_out++;
  }
  // Print local IP address and start web server
  Serial.println("");
  Serial.println("WiFi connected.");
  Serial.println("IP address: ");
  Serial.println(WiFi.localIP());
  server.begin();
  return 1;
}


void poslji_na_wifi() {
    WiFiClient client = server.available();   // Listen for incoming clients
  if (client) {                             // If a new client connects,
    Serial.println("New Client.");          // print a message out in the serial port
    String currentLine = "";                // make a String to hold incoming data from the client
    const long timeoutTime = 2000;  
    unsigned long currentTime = millis();
    unsigned long previousTime = currentTime;
    String output5State = "off";
    
    while (client.connected() && currentTime - previousTime <= timeoutTime) { // loop while the client's connected
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
            if (header.indexOf("GET /on") >= 0) {
              pattern_chosen = 0;
            } else if (header.indexOf("GET /off") >= 0) {
              pattern_chosen = 1;
            }


            // power up  - increase the duty cycle
            else if (header.indexOf("GET /power_up") >= 0) {
              if (duty_cycle < 100) {
                duty_cycle += 10;
              }  
            }

            // power down  - decrease the duty cycle
            else if (header.indexOf("GET /power_down") >= 0) {
              if (duty_cycle > 10) {
                duty_cycle -= 10;
              }
            }

            // change pattern
            else if (header.indexOf("GET /mode_forward") >= 0) {
              if (pattern_chosen < 4) {
                pattern_chosen++;
              }
              else {
                pattern_chosen = 1;
              }
            }
            
            // change_pattern
            else if (header.indexOf("GET /mode_back") >= 0) {
               if (pattern_chosen > 1) {
                  pattern_chosen--;
               }
               else {
                  pattern_chosen = 4;
               }
            }

 /* -------------HEAD--------------------HEAD----------------------- */

            client.println("<!DOCTYPE html><html lang=\"en\" xmlns=\"http://www.w3.org/1999/xhtml\"><head><meta name=\"viewport\" content=\"width=device-width, initial-scale=1\"><link rel=\"icon\" href=\"data:,\"><style>");
            client.println("html {font-family: Georgia;display: inline-block;margin: 0px auto;text-align: center;}");
            client.println("h1 {font-size: 35px;color: #4d3534;}");
            client.println("p {font-size: 20px;margin: 10px;}");
            client.println(".btn-group button {margin: auto;margin-top: 0px;margin-bottom: 10px;color: white;cursor: pointer;position: relative;}");
            client.println(".btn-group:after {content: "";clear: both;display: table;}");
            client.println(".btn-group button:not(:last-child) {border-right: none; /* Prevent double borders */}");
            client.println(".button1 {background-color: #7d5d3b;border: 0;font-family: Georgia;display: inline-block;cursor: pointer;color: #ffffff;font-size: 19px;font-weight: bold;padding: 10px 10px;text-decoration: none;text-shadow: 0px 0px 0px #4d3534;height: 70px;width: 70px;}");
            client.println(".button1:hover {background-color: #634b30;}");
            client.println(".button1:active {position: relative;top: 1px;}");
            client.println("</style></head>");

 /* --------x-----HEAD----------x----------HEAD-------------x---------- */
 /* -------------BODY--------------------BODY------------------------ */
            
            // Web Page Heading
            client.println("<body><div><h1>Change vibration patterns</h1></div>");
            
            // Display current state, and ON/OFF buttons for GPIO 5  

            // If the output5State is off, it displays the ON button       
            if (pattern_chosen) {
              client.println("<div class=\"btn-group\"><a href=\"/on\"><button class=\"button1\">ON</button></a></div>");
            } else {
              client.println("<div class=\"btn-group\"><a href=\"/off\"><button class=\"button1\">OFF</button></a></div>");
            } 
               
            //display buttons for increasing and decreasing the duty_cycle
            client.println("<p>Power: " + String(duty_cycle) + "</p>");
            client.println("<div class=\"btn-group\"><a href=\"/power_down\"><button class=\"button1\"> - </button></a>");
            client.println("<a href=\"/power_up\"><button class=\"button1\"> + </button></a></div>");
            
            //display buttons for changing the pattern of signal
            client.println("<p>Signal: " + which_signal_is_it_wifi() + "</p>");
            client.println("<div class=\"btn-group\"><a href=\"/mode_back\"><button class=\"button1\"> < </button></a>");
            client.println("<a href=\"/mode_forward\"><button class=\"button1\"> > </button></a></div>");

            client.println("</body></html>");
            
 /* -----x--------BODY---------x-----------BODY----------------x-------- */

            
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
}
