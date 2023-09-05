#include <Servo.h>
#define SERVO_1_PIN 11
#define SERVO_1_MIN 75
#define SERVO_1_MAX 180
#define SERVO_1_START_PERCENTAGE 0
#define SERVO_1_SPEED 1
#define SERVO_1_LABEL "vertical"

#define SERVO_2_PIN 10
#define SERVO_2_MIN 0
#define SERVO_2_MAX 180
#define SERVO_2_START_PERCENTAGE 50
#define SERVO_2_SPEED 1
#define SERVO_2_LABEL "horizontal"

#define MODE_UNKNOWN 0

#define MODE_SWEEP_STR "sweep"
#define MODE_SWEEP 1

struct s_t {
  char label[32];
  Servo *servo;
  int min, max;
  int pin;
  int start_percentage, pos, pos_percentage;
  int speed;
};

Servo s1, s2;
s_t servo1, servo2;

void setup() {
  Serial.begin(115200); 
  while (!Serial) {
    ;
  }  
  Serial.println("Serial 0 OK");

  Serial.println("Waiting on Serial 1");
  Serial1.begin(115200); 
  while (!Serial1) {
    ;
  }
  Serial.println("Serial 1 OK");
  Serial1.println("Serial 1 OK");

  
  servo1.servo = &s1;
  servo1.pin = SERVO_1_PIN;
  servo1.min = SERVO_1_MIN;
  servo1.max = SERVO_1_MAX;
  servo1.speed = SERVO_1_SPEED;
  servo1.start_percentage = SERVO_1_START_PERCENTAGE;
  sprintf(servo1.label, SERVO_1_LABEL);

  servo2.servo = &s2;
  servo2.pin = SERVO_2_PIN;
  servo2.min = SERVO_2_MIN;
  servo2.max = SERVO_2_MAX;
  servo2.speed = SERVO_2_SPEED;
  servo2.start_percentage = SERVO_2_START_PERCENTAGE;
  sprintf(servo2.label, SERVO_2_LABEL);


  servo1.servo->attach(servo1.pin); 
  Serial.println("Servo 1 Attached");
          

  servo2.servo->attach(servo2.pin); 
  Serial.println("Servo 2 Attached");

  servo_percentage(&servo1, servo1.start_percentage);
  servo_percentage(&servo2, servo2.start_percentage);


}
String get_serial_string(){
  if (Serial.available() > 0) {    
    String i = Serial.readString();
    return(i);
  }else if (Serial1.available() > 0) {    
    String i = Serial1.readString();
    return(i);    
  }
  return(String(""));
}

int handle_serial(){
  String instr = get_serial_string();
  instr.trim();                        // remove any \r \n whitespace at the end of the String
  if (instr == MODE_SWEEP_STR) {
    return(MODE_SWEEP);
  } else {
    return(MODE_UNKNOWN);
  }  
}

void loop(){
  int MODE = handle_serial();
  switch(MODE){
    case MODE_SWEEP:
      servo_sweep(&servo1);
      servo_sweep(&servo2);
    break;
    default:
      Serial.println("Unhandled mode");
    break;
  }
}

void servo_sweep(s_t *s){
  int x = 0;
  for (x = s->pos_percentage; x <= 100; x += 1) { 
    servo_percentage(s, x);     
    delay(s->speed);                      
  }
  for (x = s->pos_percentage; x >= 0; x -= 1) { 
    servo_percentage(s, x);     
    delay(s->speed);                      
  }
  
}

void servo_percentage(s_t *s, int x){
  int v = map(x, 0, 100, s->min, s->max);

  Serial.print("Servo ");
  Serial.print(s->pin);
  Serial.print(" ");
  Serial.print(x);
  Serial.print("% => ");
  Serial.print(v);
  Serial.print("\n");

  s->servo->write(v);  
  s->pos = v;
  s->pos_percentage = x;

}

