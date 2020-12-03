void signal(int time_period, int max_duty_cycle_binary)
{

  int change_duty;

  switch(pattern_chosen) {
    case 0:
      analogWrite(BUZZER, 0);
      digitalWrite(BUZZER, 0);
      break;
    case 1:
      // const. signal
      analogWrite(BUZZER, max_duty_cycle_binary); //calculates so that the max duty = 1023
      break;
    case 2:
      // sinusoidal signal
      change_duty = (int)((max_duty_cycle_binary/2) * sin(2 * 3.14 * time_period / pulse_duration) + max_duty_cycle_binary/2);
      analogWrite(BUZZER, change_duty);
      break;
    case 3:
      // triangle signal
      change_duty = (int) (max_duty_cycle_binary/pulse_duration * time_period);
      analogWrite(BUZZER, change_duty);
      break;
    case 4:
      // square signal
      if (time_period == 0) {
        analogWrite(BUZZER, max_duty_cycle_binary);
      }
      else if ((time_period == pulse_duration/2)){
        analogWrite(BUZZER, 0);
        digitalWrite(BUZZER, 0);
      }
      break;
  }
}


char * which_signal_is_it (char* pattern_buffer) {
  if (pattern_chosen == 0) {
    pattern_buffer = "OFF";
  }
  else if (pattern_chosen == 1) {
    pattern_buffer = "flat";
  }
  else if (pattern_chosen == 2) {
    pattern_buffer = "sine";
  }
  else if (pattern_chosen == 3) {
    pattern_buffer = "saw";
  }
  else if (pattern_chosen == 4) {
    pattern_buffer = "square";
  }
  return pattern_buffer;
}

String which_signal_is_it_wifi() {
  String pattern_buffer;
  if (pattern_chosen == 0) {
    pattern_buffer = "OFF";
  }
  else if (pattern_chosen == 1) {
    pattern_buffer = "flat";
  }
  else if (pattern_chosen == 2) {
    pattern_buffer = "sine";
  }
  else if (pattern_chosen == 3) {
    pattern_buffer = "saw";
  }
  else if (pattern_chosen == 4) {
    pattern_buffer = "square";
  }
  return pattern_buffer;
}
