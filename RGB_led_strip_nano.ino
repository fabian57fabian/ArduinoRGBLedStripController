//Created by Fabian on 03/03/2019
//Feel free to reuse the code as you wish. 

//variables for pins
int  REDPIN = 5;
int GREENPIN = 6;
int  BLUEPIN = 3;

#define FADESPEED 100 // make this higher to slow down the stansition between colors
#define KEEPCOLORTIME 5 //time to keep a color

//this variables will be keeping the state of r, g, b colors over all the duration
int r, g, b;

void setup() {
  pinMode(REDPIN, OUTPUT);
  pinMode(GREENPIN, OUTPUT);
  pinMode(BLUEPIN, OUTPUT);
  r = 0;
  g = 0;
  b = 0;
  Serial.begin(9600);
}

// Sets a color at given pin to value (in [0, 255] and updates state variable given as a pointer
// It is a setter method
void set_color(int pin, int *c, int value) {
  if (value >= 0 && value <= 255) {
    *c = value;
    analogWrite(pin, *c);
  } else {
    Serial.print("ERROR: value ");
    Serial.print(value);
    Serial.print(" not inside [0, 255].\n");
  }
}

// Receives:
//a pin and pointer to its state for the color to add
//two no pins and pointer to their state varibale to gradually remove their color
void _go_to_color(int pin, int* pin_state, int no_pin_1, int no_pin_2, int* no_pin_1_state, int* no_pin_2_state, int fade_speed) {
  //insert new color removing the others
  Serial.println("  Setting a color and backing others");
  while (*pin_state < 255) {
    set_color(pin, pin_state, *pin_state + 1);
    if (*no_pin_1_state > 0) {
      set_color(no_pin_1, no_pin_1_state, *no_pin_1_state - 1);
    }
    if (*no_pin_2_state > 0) {
      set_color(no_pin_2, no_pin_2_state, *no_pin_2_state - 1);
    }
    delay(fade_speed);
  }
  Serial.println("  Removing remaining colors");
  //finish removing the others
  while (*no_pin_1_state > 0) {
    set_color(no_pin_1, no_pin_1_state, *no_pin_1_state - 1);
    delay(fade_speed / 2);
  }
  while (*no_pin_2_state > 0) {
    set_color(no_pin_2, no_pin_2_state, *no_pin_2_state - 1);
    delay(fade_speed / 2);
  }
}

//Given a pin and a fade speed, calls _go_to_color function with needed vars
void go_to_color(int pin, int fade_speed) {
  if (pin == GREENPIN) {
    _go_to_color(pin, &g, BLUEPIN, REDPIN, &b, &r, fade_speed);
  } else if (pin == BLUEPIN) {
    _go_to_color(pin, &b, REDPIN, GREENPIN, &r, &g, fade_speed);
  } else if (pin == REDPIN) {
    _go_to_color(pin, &r, BLUEPIN, GREENPIN, &b, &g, fade_speed);
  } else {
    Serial.write("ERROR: pin ");
    Serial.write(pin);
    Serial.write(" not equal to BLUE, RED or GREEN pin");
  }
}

//Function that creates a transition between red, green and blue color
void fade_colors_slow(int keep_color_time, int fade_time) {
  go_to_color(REDPIN, fade_time);
  delay(keep_color_time);
  go_to_color(GREENPIN, fade_time);
  delay(keep_color_time);
  go_to_color(BLUEPIN, fade_time);
  delay(keep_color_time);
}

// Function that creates a rough change between 3 colors (red, green, blue)
void change_colors_rough (int keep_color_time) {
  set_color(REDPIN, &r, 255);
  set_color(GREENPIN, &g, 0);
  set_color(BLUEPIN, &b, 0);
  delay(keep_color_time);

  set_color(REDPIN, &r, 0);
  set_color(GREENPIN, &g, 255);
  set_color(BLUEPIN, &b, 0);
  delay(keep_color_time);

  set_color(REDPIN, &r, 0);
  set_color(GREENPIN, &g, 0);
  set_color(BLUEPIN, &b, 255);
  delay(keep_color_time);
}

// Function that sets all colors as 255 (FULL ON)
void all_on() {
  set_color(REDPIN, &r, 255);
  set_color(GREENPIN, &g, 255);
  set_color(BLUEPIN, &b, 255);
}

void loop() {
  Serial.println("Setting all on");
  all_on();
  delay(1000 * 5);
  
  Serial.println("fading slow");
  fade_colors_slow(KEEPCOLORTIME * 1000, 50);
  
  Serial.println("Setting all on");
  all_on();
  delay(1000 * KEEPCOLORTIME);
  
  Serial.println("Changing rough");
  change_colors_rough(KEEPCOLORTIME * 1000);
}
