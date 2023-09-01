#include <Servo.h>
#define SERVO_1_PIN 11
#define SERVO_1_MIN 75
#define SERVO_1_MAX 180
#define SERVO_1_START_PERCENTAGE 0

#define SERVO_2_PIN 10
#define SERVO_2_MIN 0
#define SERVO_2_MAX 180
#define SERVO_2_START_PERCENTAGE 50

struct s_t {
  Servo *servo;
  int min, max, pin, start_percentage, pos, pos_percentage;
};

Servo s1, s2;
s_t servo1, servo2;
int d = 10;


void setup() {
  Serial.begin(9600); 
  while (!Serial) {
    ;
  }
  servo1.servo = &s1;
  servo1.pin = SERVO_1_PIN;
  servo1.min = SERVO_1_MIN;
  servo1.max = SERVO_1_MAX;
  servo1.start_percentage = SERVO_1_START_PERCENTAGE;

  servo2.servo = &s2;
  servo2.pin = SERVO_2_PIN;
  servo2.min = SERVO_2_MIN;
  servo2.max = SERVO_2_MAX;
  servo2.start_percentage = SERVO_2_START_PERCENTAGE;


  servo1.servo->attach(servo1.pin); 
  Serial.println("Servo 1 Attached");
          

  servo2.servo->attach(servo2.pin); 
  Serial.println("Servo 2 Attached");

  servo_percentage(&servo1, servo1.start_percentage);
  servo_percentage(&servo2, servo2.start_percentage);


}


void loop(){
  servo_sweep(&servo1);
  servo_sweep(&servo2);
}

void servo_sweep(s_t *s){
  int x = 0;
  for (x = s->pos_percentage; x <= 100; x += 1) { 
    servo_percentage(s, x);     
    delay(d);                      
  }
  for (x = s->pos_percentage; x >= 0; x -= 1) { 
    servo_percentage(s, x);     
    delay(d);                      
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
