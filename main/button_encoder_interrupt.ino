extern volatile int button_flag;
extern volatile int rotary_encoder_flag;


ICACHE_RAM_ATTR void soundOnOrOff(){
  button_flag = 1;
}

ICACHE_RAM_ATTR void rotary_encoder ()  {
  //you have to get the value of the rotary encoder as soon as possible
  rottary_encoder_B = digitalRead(RotaryPinB);
  rotary_encoder_flag = 1;
}


void change_pattern(unsigned long *last_button_time) {
  if (millis() - *last_button_time > 200)
  {
    *last_button_time = millis();
    if (pattern_chosen >= 4) {
      pattern_chosen = 0;
    }
    else {
      pattern_chosen++;
    }
  }
}


void change_power_encoder_interrupt() {
  static unsigned long lastInterruptTime = 0;
  unsigned long interruptTime = millis();

  if (interruptTime - lastInterruptTime > 20) {
    if ((rottary_encoder_B == LOW) && (duty_cycle > 5)) {
      duty_cycle -= 5; // Could be -5 or -10
    }
    else if ((rottary_encoder_B == HIGH) && (duty_cycle < 100)) {
      duty_cycle += 5; // Could be +5 or +10
    }
  }
  // Keep track of when we were here last (no more than every 5ms)
  lastInterruptTime = interruptTime;
}
