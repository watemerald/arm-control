//Include the servo control library
#include <Servo.h>

//Include the predefined constans
#include "Constants.h"

Servo servo1; 

static int pos1;

//Miliseconds per rotation step
static const float rotation_speed = 1000.0 / degrees_per_second;

void setup() {
  Serial.begin(9600);
  
  //Servo1 - base servo
  servo1.attach(8);   
  //ARM servo (green) 20-110 degree

  Serial.println("Ready");
  
  park();  
}

//Checks if movement to destination derees is allowed
boolean allowed_movement(int dest1){
  boolean check_pos1 = dest1 >= min_servo1 && dest1 <= max_servo1;
  
  return check_pos1;
} 

//Actual movement functionsteps_per_second
void move_func(int dest1){
  
  if (allowed_movement(dest1)){
    int distance1 = dest1-pos1;
  
    //How long should the longest servo rotation take in seconds
    float duration = abs(distance1) / float(degrees_per_second); 
    
    //Number of steps needed to do that (rounded up)
    int length = (int) (duration * 1000.0 / rotation_speed +0.5); 
    
    for (int i=0; i<length; i+=1) {
       int p1 = ((int) pos1 + ((float)i/length) * distance1);
       servo1.write(p1);
       delay(rotation_speed);
    }
    
//    servo1.write(pos1);
    pos1 = dest1;
  }
  
  //Log current position of servos
  Serial.print("(");
  Serial.print(pos1);
  Serial.print(")\n");
}

void park()
{
  pos1 = 20;
  servo1.write(pos1);
}

void loop() {

  static int v = 0;
  static int dest1;
  
  if ( Serial.available()) {
    char ch = Serial.read();

    switch(ch) {  
      case '0'...'9':
        v = v * 10 + ch - '0';
        break;
      case 'r':
        dest1 = v;
        v = 0;
        break;
      case 'e':
        move_func(dest1);
        break;
      case 'p':
        park();
        break;
    }
  }
 // Servo::refresh();

} 
