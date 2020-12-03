#define BUZZER D7
#define BUTTON D6
#define RotaryPinA D3  // Used for generating interrupts using CLK signal
#define RotaryPinB D5

#include <oled.h>         // external library for OLED display
#include <ESP8266WiFi.h>

OLED display=OLED(4,5,16,0x3C,128,64,true);

/* ------------ variables used everywhere --------*/
volatile float pulse_duration = 1000;      // duration of the sinusoidal signal in ms
volatile float duty_cycle = 50;            // default value of duty, pattern
volatile int pattern_chosen = 1;
volatile int button_flag = 0;     // flag changes, when the button is pressed
volatile int rotary_encoder_flag = 0;      // flag changes, when the encoder is rotated
volatile bool rottary_encoder_B;         // used to immediately store the value of RotaryPinB
volatile bool Wifi_present = 0;          //changes if it can't connect to wifi

/* -----------for WIFI ------------------- */
    
// Replace with your network credentials
const char* ssid     = "341FF2";
const char* password = "slovenija123";

// Set web server port number to 80
WiFiServer server(80);

// Variable to store the HTTP request
String header;

/* -------------------------------------------- */
/******* SETUP    SETUP    SETUP   SETUP*********/
/* ------------------------------------------- */

void setup() {  
  pinMode (BUZZER, OUTPUT); // set the digital IO pin mode,
  pinMode (BUTTON,INPUT);  // enable the pullup resistor  
  pinMode(RotaryPinA, INPUT); //encoder pin
  pinMode(RotaryPinB, INPUT); //encoder pin
  
  Serial.begin(9600);

  OLED_display_setup();         // setup display
  
  OLED_connecting_to_wifi();      // display connecting to Wifi message
  Wifi_present = setup_wifi();    // setup wifi connection

  if (!Wifi_present) {
    OLED_wifi_off();
    delay(2000);      // delay so the user can see the message
  }
  
  // interrupt is defined at the end, when everything sets up
  //interrupt for button and encoder
  attachInterrupt(digitalPinToInterrupt(BUTTON), soundOnOrOff, FALLING);
  attachInterrupt(digitalPinToInterrupt(RotaryPinA), rotary_encoder, RISING);

  OLED_update_display();     // display current values on OLED
}


/* -------------------------------------------- */
/******* LOOP    LOOP    LOOP   LOOP *********/
/* ------------------------------------------- */

void loop() {
  // ------------- variables used only inside loop ------------------------------------
  int time_period;      //changes between 0 and 
  int last_pattern_chosen = pattern_chosen;       //for looking if pattern_chosen has changed
  int last_duty_cycle = duty_cycle;
  int time_to_wait_between_periods = 5;     //how long to wait when changing signal (AnalogWrite)
  unsigned long last_button_time = 0;         // last time the button has been pressed
  unsigned long time_period_now = 0;          // used for changing signal
  int max_duty_cycle_binary;

  // --------------- main loop -----------------------------------------------------------
  while(true) {
    // ---------------------- looks if button has been pressed --------------
    if (button_flag == 1) {
        button_flag = 0;
        change_pattern(&last_button_time);
    }
    // ----------------------------------------------------------------------

    // -------------------- look if encoder has been rotated ----------------
    if (rotary_encoder_flag == 1) {
      rotary_encoder_flag = 0;
      change_power_encoder_interrupt();
    }
    // ---------------------------------------------------------------------- 

    //  -------- zanka za menjat signal vsakih 5 milisekund -----------------
    if ( (millis() - time_period_now) > time_to_wait_between_periods) {
      // reset the time
      time_period_now = millis();

      // nardimo iteracijo, tako da ne rabimo for zanke
      if (time_period > pulse_duration) {
        time_period = 0;
      }
      else {
        time_period += 5;
      }
      max_duty_cycle_binary = (int) (1023 * duty_cycle * 0.01);
      signal(time_period, max_duty_cycle_binary);
    }
    // -----------------------------------------------------------------------

    // ------------------ update OLED if any value changes -------------------
    if ((pattern_chosen != last_pattern_chosen) || (duty_cycle != last_duty_cycle)) {
        OLED_update_display();
        last_pattern_chosen = pattern_chosen ;
        last_duty_cycle = duty_cycle;
    }
    // -----------------------------------------------------------------------

    // -------------- looks if any changes have been made in Wifi ------------
    if (Wifi_present) {     // checks only if the connecion has been made
      poslji_na_wifi();
    }
    //------------------------------------------------------------------------
    yield();        // important to reset the watchdog - because we are inside a while(true) loop
  }
  // -------------x----------- main loop ------------- x ------------------------
}
