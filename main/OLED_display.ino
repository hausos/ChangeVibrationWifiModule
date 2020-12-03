void OLED_display_setup() {
  display.begin();

/*-------------display short message-----------------*/
    display.clear();
    display.draw_string(2, 20, "Well hello there!");
    display.display();
    delay(1000);      // so user sees the message
}


/* -------------updates the display ------------------ */
void OLED_update_display() {
  // Write out to serial monitor the value and direction
  //Serial.println(duty_cycle, pattern_chosen);
  display.clear();
  if (pattern_chosen == 0) {
    OLED_power_off();
  } else {
    OLED_display_pattern();
    OLED_display_duty_cycle();
  }
  OLED_show_if_wifi_connected();
  display.display();
}

void OLED_show_if_wifi_connected() {
  display.draw_string(4, 55, "Wifi: ");
  if (Wifi_present) {
    display.draw_string(40, 55, "ON");
  }
  else {
    display.draw_string(40, 55, "OFF");
  }
}


 void OLED_display_duty_cycle() {
  char duty_buffer[4];
  // power display
  display.draw_string(4, 2, "Power: ");
  itoa(duty_cycle, duty_buffer, 10);
  display.draw_string(60, 2, duty_buffer);
  display.draw_rectangle(0,20,duty_cycle,31,OLED::SOLID);
 }

 void OLED_display_pattern() {
  // signal display
  char *pattern_buffer;
  display.draw_string(4, 40, "Signal: ");
  display.draw_string(60, 40, which_signal_is_it(pattern_buffer));
 }


void OLED_power_off() {
  Serial.println("Power off");
  display.clear();
  display.draw_string(30, 30, "POWER OFF");
  display.display();
}


void OLED_wifi_off() {
  Serial.println("Unable to connect to Wifi");
  display.clear();
  display.draw_string(10, 20, "Unable to connect to Wifi");
  display.draw_string(10, 40, "Restart to try again ....");
  display.display();
}

void OLED_connecting_to_wifi() {
  display.clear();
  display.draw_string(5, 30, "Connecting to ...");
  display.draw_string(20, 40, ssid);
  display.display();
}
