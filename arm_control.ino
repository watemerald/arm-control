//Include the servo control library
#include <Servo.h>

//Include the predefined constans
#include "Constants.h"

//Servo 1 - base servo
Servo servo1; 
//Servo 2 - shoulder servo
Servo servo2;
//Servo 3 - elbow servo
Servo servo3;

static int pos1;
static int pos2;
static int pos3;

//Miliseconds per rotation step
static const float rotation_speed = 1000.0 / degrees_per_second;

void setup() {
  Serial.begin(9600);
  
  servo1.attach(8);   
  //ARM servo (green) 20-110 degree
  servo2.attach(9);
  servo3.attach(10);
  
  Serial.println("Ready");
  
  park();  
}

//Checks if movement to destination derees is allowed
boolean allowed_movement(int dest1, int dest2, int dest3){
  boolean check_pos1 = dest1 >= min_servo1 && dest1 <= max_servo1;
  boolean check_pos2 = dest2 >= min_servo2 && dest2 <= max_servo2;
  boolean check_pos3 = dest3 >= min_servo3 && dest3 <= max_servo3;
  
  return check_pos1 && check_pos2 && check_pos3;
} 

//Actual movement functionsteps_per_second
void move_func(int dest1, int dest2, int dest3){
  
  if (allowed_movement(dest1, dest2, dest3)){
    int distance1 = dest1-pos1;
    int distance2 = dest2-pos2;
    int distance3 = dest3-pos3;
  
  
    int maxdistance = max(max(abs(distance1), abs(distance2)), abs(distance3));
    //How long should the longest servo rotation take in seconds
    float duration = abs(maxdistance) / float(degrees_per_second); 
    
    //Number of steps needed to do that (rounded up)
    int length = (int) (duration * 1000.0 / rotation_speed +0.5); 
    
    for (int i=0; i<length; i+=1) {
       int p1 = ((int) pos1 + ((float)i/length) * distance1);
       servo1.write(p1);
       int p2 = ((int) pos2 + ((float)i/length) * distance2);
       servo2.write(p2);
       int p3 = ((int) pos3 + ((float)i/length) * distance3);
       servo3.write(p3);
       delay(rotation_speed);
    }
    
//    servo1.write(pos1);
    pos1 = dest1;
    pos2 = dest2;
    pos3 = dest3;
  }
  
  //Log current position of servos
  Serial.print("(");
  Serial.print(pos1);
  Serial.print(", ");
  Serial.print(pos2);
  Serial.print(", ");
  Serial.print(pos3);
  Serial.print(")\n");
}

void park()
{
  pos1 = start_servo1;
  servo1.write(pos1);
  pos2 = start_servo2;
  servo2.write(pos2);
  pos3 = start_servo3;  
  servo3.write(pos3);
}

void loop() {

  static int v = 0;
  static int dest1 = start_servo1;
  static int dest2 = start_servo2;
  static int dest3 = start_servo3;
  
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
      case 's':
        dest2 = v;
        v = 0;
        break;
      case 'l':
        dest3 = v;
        v = 0;
        break;
      case 'e':
        move_func(dest1, dest2, dest3);
        break;
      case 'p':
        park();
        break;
    }
  }
 // Servo::refresh();

} 
