/*
 * Bowie Yellow Discovery
 * -------------------------------
 * 
 * Behaviours for Bowie (Mini, using Teensy 3.2) 
 * for the Robo-Obstacle Course workshop at the 
 * Canadian Science and Technology Museum, in 
 * collaboration with MadeMill
 * 
 * Erin RobotGrrl for RobotMissions
 * Feb. 23rd, 2018
 * --> http://RobotMissions.org
 * 
 * MIT license, check LICENSE for more information
 * All text above must be included in any redistribution
 * 
 * 
 * hopper does not tilt super well - servo is too weak
 * 
 * 
 */

#include "BowieClaw.h"
#include "MegaBowieShoreline.h"

#define ROBOT_ID 3

MegaBowieShoreline bowie;

bool completed_action[] = { false, false, false, false, false, false };
long last_called[] = { 0, 0, 0, 0, 0, 0 };
long current_time = 0;

void control(Msg m);

#define SERVO_CLAW 24
#define SERVO_CLAW_KEY 7
BowieClaw bowieclaw = BowieClaw();
void clawInterrupt(int key, int val);

int frame_delay = 3000;

void setup() {
  Serial.begin(9600);
  bowie = MegaBowieShoreline();
  bowie.begin();
  bowie.setRobotID(ROBOT_ID);
  bowie.disableDefaultActions();
  bowie.set_control_callback(control);

  bowieclaw.begin();
  bowieclaw.setServoClawPin(SERVO_CLAW);
  bowieclaw.setServoInterruptCallback(clawInterrupt);
  bowieclaw.initServo();

}

int frame_duration = 0;
int step_delay = 0;
int step_bound = 0;
int step_size = 0;
int total_time = 0;
int end_pos = 0;
int end_start = 0;
int end_final = 0;
int end_slice = 0;
int claw_pos = 0;
int claw_start = 0;
int claw_final = 0;
int claw_slice = 0;
int arm_pos = 0;
int arm_start = 0;
int arm_final = 0;
int arm_slice = 0;
int lid_pos = 0;
int lid_start = 0;
int lid_final = 0;
int lid_slice = 0;

void loop() {
  current_time = millis();
  bowie.update(false);

  bowie.bowiehopper.lid.writeMicroseconds(LID_MAX);
  delay(3000);

  

  // ---

  // 0. end to down
  // 1. set claw to closed
  // 2. move end to position
  end_start = END_MAX;
  end_final = END_REFLEX;
  end_pos = end_start;
  
  claw_start = CLAW_HOME;
  claw_final = CLAW_MIN;
  claw_pos = claw_start;
  
  step_delay = 2;
  step_size = 4;
  step_bound = (int)floor( abs( end_start - end_final ) / step_size );
  total_time = step_delay * step_bound;
  
  for(int i=0; i<step_bound; i++) {
    
    if(end_start < end_final) {
      
      end_pos = end_start + (i*step_size);
      if(end_pos > end_final) end_pos = end_final;
      bowie.bowiescoop.scoop.writeMicroseconds(end_pos);

      if(claw_start < claw_final) {
        claw_pos = map(end_pos, end_start, end_final, claw_start, claw_final);
      } else if(claw_start > claw_final) {
        //claw_pos = map(end_pos, end_start, end_final, claw_final, claw_start);
        claw_pos = map(end_pos, end_start, end_final, claw_start, claw_final);
      }
      Serial << "A " << claw_pos << "   " << claw_start << " -> " << claw_final << endl;
      bowieclaw.claw.writeMicroseconds(claw_pos);
      
    } else if(end_start > end_final) {

      end_pos = end_start - (i*step_size);
      if(end_pos < end_final) end_pos = end_final;
      bowie.bowiescoop.scoop.writeMicroseconds(end_pos);

      if(claw_start < claw_final) {
        claw_pos = map(end_pos, end_final, end_start, claw_start, claw_final);
      } else if(claw_start > claw_final) {
        //claw_pos = map(end_pos, end_final, end_start, claw_final, claw_start);
        claw_pos = map(end_pos, end_final, end_start, claw_start, claw_final);
      }
      Serial << "B " << claw_pos << "   " << claw_start << " -> " << claw_final << endl;
      bowieclaw.claw.writeMicroseconds(claw_pos);
      
    }

    delay(step_delay);
    
  }
  bowie.bowiescoop.scoop.writeMicroseconds(end_final);
  bowieclaw.claw.writeMicroseconds(claw_final);
  
  delay(frame_delay);

  // ----

  // 3. open claw
  // 4. move arm to position
  claw_start = claw_final;   // 1800
  claw_final = CLAW_MAX;     // 1000
  claw_pos = claw_start;
  
  arm_start = ARM_MAX;       // 2200
  arm_final = ARM_HOME-300;  // 1100
  arm_pos = arm_start;
  
  step_delay = 4;
  step_size = 4;
  step_bound = (int)floor( abs(arm_start - arm_final) / step_size);
  total_time = step_delay * step_bound;
  
  for(int i=0; i<step_bound; i++) {
    
    if(arm_start < arm_final) {
    
      arm_pos = arm_start + (i*step_size);
      if(arm_pos > arm_final) arm_pos = arm_final;
      bowie.bowiearm.arm.writeMicroseconds(arm_pos);
      bowie.bowiearm.arm2.writeMicroseconds(SERVO_MAX_US - arm_pos + SERVO_MIN_US);

      if(claw_start < claw_final) {
        claw_pos = map(arm_pos, arm_start, arm_final, claw_start, claw_final);
      } else if(claw_start > claw_final) {
        //claw_pos = map(arm_pos, arm_start, arm_final, claw_final, claw_start);
        claw_pos = map(arm_pos, arm_start, arm_final, claw_start, claw_final);
      }
      Serial << "C " << claw_pos << "   " << claw_start << " -> " << claw_final << endl;
      bowieclaw.claw.writeMicroseconds(claw_pos);

    } else if(arm_start > arm_final) {
      
      arm_pos = arm_start - (i*step_size);
      if(arm_pos < arm_final) arm_pos = arm_final;
      bowie.bowiearm.arm.writeMicroseconds(arm_pos);
      bowie.bowiearm.arm2.writeMicroseconds(SERVO_MAX_US - arm_pos + SERVO_MIN_US);

      if(claw_start < claw_final) {
        claw_pos = map(arm_pos, arm_final, arm_start, claw_start, claw_final);
      } else if(claw_start > claw_final) {
        claw_pos = map(arm_pos, arm_final, arm_start, claw_final, claw_start);
        //claw_pos = map(arm_pos, arm_final, arm_start, claw_start, claw_final);
      }
      Serial << "D " << claw_pos << "   " << claw_start << " -> " << claw_final << endl;
      bowieclaw.claw.writeMicroseconds(claw_pos);
      
    }
    
    delay(step_delay);
    
  }
  bowie.bowiearm.arm.writeMicroseconds(arm_final);
  bowie.bowiearm.arm2.writeMicroseconds(SERVO_MAX_US - arm_final + SERVO_MIN_US);
  bowieclaw.claw.writeMicroseconds(claw_final);

  delay(frame_delay);

  // ---

  // 5. close claw
  // 6. move end middle
  end_start = end_final;
  end_final = END_PARALLEL_BOTTOM;
  end_pos = end_start;
  
  claw_start = claw_final;
  claw_final = CLAW_MIN;
  claw_pos = claw_start;

  // grab it
  for(int i=claw_start; i<claw_final; i+=3) {
    bowieclaw.claw.writeMicroseconds(i);
    delay(2);
  }
  delay(1000);
  
  step_delay = 2;
  step_size = 4;
  step_bound = (int)floor( abs( end_start - end_final ) / step_size );
  total_time = step_delay * step_bound;
  
  for(int i=0; i<step_bound; i++) {
    
    if(end_start < end_final) {
      
      end_pos = end_start + (i*step_size);
      if(end_pos > end_final) end_pos = end_final;
      bowie.bowiescoop.scoop.writeMicroseconds(end_pos);

      if(claw_start < claw_final) {
        claw_pos = map(end_pos, end_start, end_final, claw_start, claw_final);
      } else if(claw_start > claw_final) {
        //claw_pos = map(end_pos, end_start, end_final, claw_final, claw_start);
        claw_pos = map(end_pos, end_start, end_final, claw_start, claw_final);
      }
      //Serial << "E " << claw_pos << "   " << claw_start << " -> " << claw_final << endl;
      //bowieclaw.claw.writeMicroseconds(claw_pos);
      
    } else if(end_start > end_final) {

      end_pos = end_start - (i*step_size);
      if(end_pos < end_final) end_pos = end_final;
      bowie.bowiescoop.scoop.writeMicroseconds(end_pos);
      
      if(claw_start < claw_final) {
        claw_pos = map(end_pos, end_final, end_start, claw_start, claw_final);
      } else if(claw_start > claw_final) {
        claw_pos = map(end_pos, end_final, end_start, claw_final, claw_start);
        //claw_pos = map(end_pos, end_final, end_start, claw_start, claw_final);
      }
      //Serial << "F " << claw_pos << "   " << claw_start << " -> " << claw_final << endl;
      //bowieclaw.claw.writeMicroseconds(claw_pos);
      
    }

    delay(step_delay);
    
  }
  bowie.bowiescoop.scoop.writeMicroseconds(end_final);
  //bowieclaw.claw.writeMicroseconds(claw_final);
  
  delay(frame_delay);


  // ---
  // 7. move end up
  // 8. move arm up
  end_start = end_final;
  end_final = END_HOME-200;//END_MIN;
  end_pos = end_start;

  arm_start = arm_final;
  arm_final = ARM_MAX-400;//ARM_MAX-100;//ARM_MAX;
  arm_pos = arm_start;

  lid_start = LID_MAX;
  lid_final = LID_MIN;
  lid_pos = lid_start;
  
  step_delay = 5;
  step_size = 3;
  step_bound = (int)floor( abs(arm_start - arm_final) / step_size);
  total_time = step_delay * step_bound;
  
  for(int i=0; i<step_bound; i++) {
    
    if(arm_start < arm_final) {
    
      arm_pos = arm_start + (i*step_size);
      if(arm_pos > arm_final) arm_pos = arm_final;
      bowie.bowiearm.arm.writeMicroseconds(arm_pos);
      bowie.bowiearm.arm2.writeMicroseconds(SERVO_MAX_US - arm_pos + SERVO_MIN_US);

      if(end_start < end_final) {
        end_pos = map(arm_pos, arm_start, arm_final, end_start, end_final);
      } else if(end_start > end_final) {
        //end_pos = map(arm_pos, arm_start, arm_final, end_final, end_start);
        end_pos = map(arm_pos, arm_start, arm_final, end_start, end_final);
      }
      Serial << "G " << end_pos << "   " << end_start << " -> " << end_final << endl;
      bowie.bowiescoop.scoop.writeMicroseconds(end_pos);

      if(lid_start < lid_final) {
        lid_pos = map(end_pos, end_start, end_final, lid_start, lid_final);
      } else if(lid_start > lid_final) {
        //lid_pos = map(end_pos, end_start, end_final, lid_final, lid_start);
        lid_pos = map(end_pos, end_start, end_final, lid_start, lid_final);
      }
      bowie.bowiehopper.lid.writeMicroseconds(lid_pos);
      
    } else if(arm_start > arm_final) {
      
      arm_pos = arm_start - (i*step_size);
      if(arm_pos < arm_final) arm_pos = arm_final;
      bowie.bowiearm.arm.writeMicroseconds(arm_pos);
      bowie.bowiearm.arm2.writeMicroseconds(SERVO_MAX_US - arm_pos + SERVO_MIN_US);

      if(end_start < end_final) {
        end_pos = map(arm_pos, arm_final, arm_start, end_start, end_final);
      } else if(claw_start > claw_final) {
        //end_pos = map(arm_pos, arm_final, arm_start, end_final, end_start);
        end_pos = map(arm_pos, arm_final, arm_start, end_start, end_final);
      }
      Serial << "H " << end_pos << "   " << end_start << " -> " << end_final << endl;
      bowie.bowiescoop.scoop.writeMicroseconds(end_pos);

      if(lid_start < lid_final) {
        lid_pos = map(end_pos, end_final, end_start, lid_start, lid_final);
      } else if(lid_start > lid_final) {
        //lid_pos = map(end_pos, end_final, end_start, lid_final, lid_start);
        lid_pos = map(end_pos, end_final, end_start, lid_start, lid_final);
      }
      bowie.bowiehopper.lid.writeMicroseconds(lid_pos);
      
    }

    delay(step_delay);
    
  }
  bowie.bowiearm.arm.writeMicroseconds(arm_final);
  bowie.bowiearm.arm2.writeMicroseconds(SERVO_MAX_US - arm_final + SERVO_MIN_US);
  bowie.bowiescoop.scoop.writeMicroseconds(end_final);
  bowie.bowiehopper.lid.writeMicroseconds(lid_final);

  delay(frame_delay);

  // ---

  /*
  // -- SKIP --
  // 9. move end down a bit
  // 10. move arm back a bit
  // 11. open lid
  
  end_start = end_final;
  end_final = END_HOME-200;
  end_pos = end_start;
  
  arm_start = arm_final;
  arm_final = ARM_MAX-100;
  arm_pos = arm_start;

  lid_start = LID_MAX;
  lid_final = LID_MIN;
  lid_pos = lid_start;
  
  step_delay = 6;
  step_size = 2;
  step_bound = (int)floor( abs( end_start - end_final ) / step_size );
  total_time = step_delay * step_bound;
  
  for(int i=0; i<step_bound; i++) {
    
    if(end_start < end_final) {
      
      end_pos = end_start + (i*step_size);
      if(end_pos > end_final) end_pos = end_final;
      bowie.bowiescoop.scoop.writeMicroseconds(end_pos);

      if(arm_start < arm_final) {
        arm_pos = map(end_pos, end_start, end_final, arm_start, arm_final);
      } else if(arm_start > arm_final) {
        //arm_pos = map(end_pos, end_start, end_final, arm_final, arm_start);
        arm_pos = map(end_pos, end_start, end_final, arm_start, arm_final);
      }
      bowie.bowiearm.arm.writeMicroseconds(arm_pos);
      bowie.bowiearm.arm2.writeMicroseconds(SERVO_MAX_US - arm_pos + SERVO_MIN_US);

      if(lid_start < lid_final) {
        lid_pos = map(end_pos, end_start, end_final, lid_start, lid_final);
      } else if(lid_start > lid_final) {
        //lid_pos = map(end_pos, end_start, end_final, lid_final, lid_start);
        lid_pos = map(end_pos, end_start, end_final, lid_start, lid_final);
      }
      bowie.bowiehopper.lid.writeMicroseconds(lid_pos);
      
    } else if(end_start > end_final) {

      end_pos = end_start - (i*step_size);
      if(end_pos < end_final) end_pos = end_final;
      bowie.bowiescoop.scoop.writeMicroseconds(end_pos);

      if(arm_start < arm_final) {
        arm_pos = map(end_pos, end_final, end_start, arm_start, arm_final);
      } else if(arm_start > arm_final) {
        //arm_pos = map(end_pos, end_final, end_start, arm_final, arm_start);
        arm_pos = map(end_pos, end_final, end_start, arm_start, arm_final);
      }
      bowie.bowiearm.arm.writeMicroseconds(arm_pos);
      bowie.bowiearm.arm2.writeMicroseconds(SERVO_MAX_US - arm_pos + SERVO_MIN_US);

      if(lid_start < lid_final) {
        lid_pos = map(end_pos, end_final, end_start, lid_start, lid_final);
      } else if(lid_start > lid_final) {
        //lid_pos = map(end_pos, end_final, end_start, lid_final, lid_start);
        lid_pos = map(end_pos, end_final, end_start, lid_start, lid_final);
      }
      bowie.bowiehopper.lid.writeMicroseconds(lid_pos);
      
    }

    delay(step_delay);
    
  }
  bowie.bowiescoop.scoop.writeMicroseconds(end_final);
  bowie.bowiearm.arm.writeMicroseconds(arm_final);
  bowie.bowiearm.arm2.writeMicroseconds(SERVO_MAX_US - arm_final + SERVO_MIN_US);
  bowie.bowiehopper.lid.writeMicroseconds(lid_final);
  
  delay(frame_delay); 
  */


  // ---

  // 12. move arm fwd a bit
  // 13. tilt end up a bit
  // 14. open claw
  arm_start = arm_final;
  arm_final = ARM_MAX-100;//ARM_MAX;
  arm_pos = arm_start;

  end_start = end_final;
  end_final = END_MIN+500;
  end_pos = end_start;

  claw_start = claw_final;
  claw_final = CLAW_MAX;
  claw_pos = claw_start;
  
  step_delay = 4;
  step_size = 3;
  step_bound = (int)floor( abs(arm_start - arm_final) / step_size);
  total_time = step_delay * step_bound;
  
  for(int i=0; i<step_bound; i++) {
    
    if(arm_start < arm_final) {
    
      arm_pos = arm_start + (i*step_size);
      if(arm_pos > arm_final) arm_pos = arm_final;
      bowie.bowiearm.arm.writeMicroseconds(arm_pos);
      bowie.bowiearm.arm2.writeMicroseconds(SERVO_MAX_US - arm_pos + SERVO_MIN_US);

      if(end_start < end_final) {
        end_pos = map(arm_pos, arm_start, arm_final, end_start, end_final);
      } else if(end_start > end_final) {
        //end_pos = map(arm_pos, arm_start, arm_final, end_final, end_start);
        end_pos = map(arm_pos, arm_start, arm_final, end_start, end_final);
      }
      Serial << "I " << end_pos << "   " << end_start << " -> " << end_final << endl;
      bowie.bowiescoop.scoop.writeMicroseconds(end_pos);

      if(claw_start < claw_final) {
        claw_pos = map(arm_pos, arm_start, arm_final, claw_start, claw_final);
      } else if(claw_start > claw_final) {
        claw_pos = map(arm_pos, arm_start, arm_final, claw_start, claw_final);
      }
      bowieclaw.claw.writeMicroseconds(claw_pos);
      
    } else if(arm_start > arm_final) {
      
      arm_pos = arm_start - (i*step_size);
      if(arm_pos < arm_final) arm_pos = arm_final;
      bowie.bowiearm.arm.writeMicroseconds(arm_pos);
      bowie.bowiearm.arm2.writeMicroseconds(SERVO_MAX_US - arm_pos + SERVO_MIN_US);

      if(end_start < end_final) {
        end_pos = map(arm_pos, arm_final, arm_start, end_start, end_final);
      } else if(claw_start > claw_final) {
        //end_pos = map(arm_pos, arm_final, arm_start, end_final, end_start);
        end_pos = map(arm_pos, arm_final, arm_start, end_start, end_final);
      }
      Serial << "J " << end_pos << "   " << end_start << " -> " << end_final << endl;
      bowie.bowiescoop.scoop.writeMicroseconds(end_pos);

      if(claw_start < claw_final) {
        claw_pos = map(arm_pos, arm_final, arm_start, claw_start, claw_final);
      } else if(lid_start > lid_final) {
        //lid_pos = map(end_pos, end_final, end_start, lid_final, lid_start);
        claw_pos = map(arm_pos, arm_final, arm_start, claw_start, claw_final);
      }
      bowie.bowiescoop.scoop.writeMicroseconds(end_pos);
      
    }

    delay(step_delay);
    
  }
  bowie.bowiearm.arm.writeMicroseconds(arm_final);
  bowie.bowiearm.arm2.writeMicroseconds(SERVO_MAX_US - arm_final + SERVO_MIN_US);
  bowie.bowiescoop.scoop.writeMicroseconds(end_final);
  bowieclaw.claw.writeMicroseconds(claw_final);

  delay(frame_delay);


  // ---

  // 15. shake end and claw
  Serial << "15. SHAKE END AND CLAW" << endl;
  for(int i=0; i<3; i++) {
    bowie.bowiescoop.scoop.writeMicroseconds(END_MIN+300);
    bowieclaw.claw.writeMicroseconds(CLAW_MAX);
    delay(100);
    bowie.bowiescoop.scoop.writeMicroseconds(END_MIN+500);
    bowieclaw.claw.writeMicroseconds(CLAW_MAX+300);
    delay(100);
  }
  
  delay(frame_delay);

  delay(5000);

  // ---

  // 16. close claw
  // 17. move end middle
  

  

  /*

  // 16. close claw
  Serial << "16. CLAW_MIN" << endl;
  bowieclaw.moveClaw(CLAW_MIN);
  delay(frame_delay);

  // 17. move end middle
  Serial << "17. END_PARALLEL_BOTTOM" << endl;
  bowie.bowiescoop.moveEnd(END_PARALLEL_BOTTOM);
  delay(frame_delay);

  // 18. move arm back a bit
  Serial << "18. ARM_MAX-100" << endl;
  bowie.bowiearm.moveArm(ARM_MAX-100);
  delay(frame_delay);

  // ---

  // 19. close lid
  Serial << "19. LID_MAX" << endl;
  bowie.bowiehopper.moveLid(LID_MAX);
  delay(frame_delay);

  // 20. move arm up
  Serial << "20. ARM_MAX" << endl;
  bowie.bowiearm.moveArm(ARM_MAX);
  delay(frame_delay);

  // 21. move end up
  Serial << "21. END_MIN" << endl;
  bowie.bowiescoop.moveEnd(END_MIN);
  delay(frame_delay);

  // ---

  */

}

void slowClawFrames() {

  // 0. end to down
  Serial << "0. END_MAX" << endl;
  bowie.bowiescoop.moveEnd(END_MAX);
  delay(frame_delay);

  // 1. set claw to closed
  Serial << "1. CLAW_MIN" << endl;
  bowieclaw.moveClaw(CLAW_MIN);
  delay(frame_delay);

  // 2. move end to position
  Serial << "2. END_REFLEX" << endl;
  bowie.bowiescoop.moveEnd(END_REFLEX);
  delay(frame_delay);

  // 3. open claw
  Serial << "3. CLAW_MAX" << endl;
  bowieclaw.moveClaw(CLAW_MAX);
  delay(frame_delay);

  // 4. move arm to position
  Serial << "4. ARM_HOME-300" << endl;
  bowie.bowiearm.moveArm(ARM_HOME-300);
  delay(frame_delay);

  // 5. close claw
  Serial << "5. CLAW_MIN" << endl;
  bowieclaw.moveClaw(CLAW_MIN);
  delay(frame_delay);

  // 6. move end middle
  Serial << "6. END_PARALLEL_BOTTOM" << endl;
  bowie.bowiescoop.moveEnd(END_PARALLEL_BOTTOM);
  delay(frame_delay);

  // 7. move end up
  Serial << "7. END_MIN" << endl;
  bowie.bowiescoop.moveEnd(END_MIN);
  delay(frame_delay);

  // 8. move arm up
  Serial << "8. ARM_MAX" << endl;
  bowie.bowiearm.moveArm(ARM_MAX);
  delay(frame_delay);

  // 9. move end down a bit
  Serial << "9. END_HOME-400" << endl;
  bowie.bowiescoop.moveEnd(END_HOME-400);
  delay(frame_delay);

  // 10. move arm back a bit
  Serial << "10. ARM_MAX-100" << endl;
  bowie.bowiearm.moveArm(ARM_MAX-100);
  delay(frame_delay);

  // 11. open lid
  Serial << "11. LID_MIN" << endl;
  bowie.bowiehopper.moveLid(LID_MIN);
  delay(frame_delay);

  // 12. move arm fwd a bit
  Serial << "12. ARM_MAX" << endl;
  bowie.bowiearm.moveArm(ARM_MAX);
  delay(frame_delay);

  // 13. tilt end up a bit
  Serial << "13. END_MIN+500" << endl;
  bowie.bowiescoop.moveEnd(END_MIN+500);
  delay(frame_delay);
  
  // 14. open claw
  Serial << "14. CLAW_MAX" << endl;
  bowieclaw.moveClaw(CLAW_MAX);
  delay(frame_delay);

  // 15. shake end and claw
  Serial << "15. SHAKE END AND CLAW" << endl;
  for(int i=0; i<3; i++) {
    bowieclaw.moveClaw(CLAW_MAX);
    delay(40);
    bowieclaw.moveClaw(CLAW_MAX+300);
    delay(40);
  }
  for(int i=0; i<3; i++) {
    bowie.bowiescoop.moveEnd(END_MIN+300);
    delay(40);
    bowie.bowiescoop.moveEnd(END_MIN+500);
    delay(40);
  }
  delay(frame_delay);

  // 16. close claw
  Serial << "16. CLAW_MIN" << endl;
  bowieclaw.moveClaw(CLAW_MIN);
  delay(frame_delay);

  // 17. move end middle
  Serial << "17. END_PARALLEL_BOTTOM" << endl;
  bowie.bowiescoop.moveEnd(END_PARALLEL_BOTTOM);
  delay(frame_delay);

  // 18. move arm back a bit
  Serial << "18. ARM_MAX-100" << endl;
  bowie.bowiearm.moveArm(ARM_MAX-100);
  delay(frame_delay);

  // 19. close lid
  Serial << "19. LID_MAX" << endl;
  bowie.bowiehopper.moveLid(LID_MAX);
  delay(frame_delay);

  // 20. move arm up
  Serial << "20. ARM_MAX" << endl;
  bowie.bowiearm.moveArm(ARM_MAX);
  delay(frame_delay);

  // 21. move end up
  Serial << "21. END_MIN" << endl;
  bowie.bowiescoop.moveEnd(END_MIN);
  delay(frame_delay);
  
}

void clawInterrupt(int key, int val) {
  // This will be called any time there is a long
  // duration movement - in between the delays.
  // This is a good place to check for serial
  // messages. Val = the position.
  
  if(key == SERVO_CLAW_KEY) {
    // It's the scoop!
  }
}

void control(Msg m) {
  // Received an action from the controller. The data is
  // packed into the Msg struct. Core actions with this data
  // will be done inside of the main robot mission program.
  // All Msgs will be passed through to this function, even
  // if there is / is not a core action associated with it.
  // You can do custom actions with this data here.

  Serial << "---RECEIVED ACTION---" << endl;
  Serial << "action: " << m.action << endl;
  Serial << "command: " << m.pck1.cmd << endl;
  Serial << "key: " << m.pck1.key << endl;
  Serial << "val: " << m.pck1.val << endl;
  Serial << "command: " << m.pck2.cmd << endl;
  Serial << "key: " << m.pck2.key << endl;
  Serial << "val: " << m.pck2.val << endl;
  Serial << "delim: " << m.delim << endl;

  Packet packets[2] = { m.pck1, m.pck2 };

  // eg #P1,0,N1,2!
  //    #P3,1,N3,0!

  if(m.action == '#') {

    if(packets[0].key == 1) {

      // drive
      if(packets[0].cmd == 'P') { // 1 red button
        if(packets[0].val == 1) { // sends drive joystick cmds on operator side
        }
      }

      // arm
      if(packets[0].cmd == 'Y') { // 2 yellow button
        if(packets[0].val == 1) { // sends arm joystick cmds on operator side
          if(bowie.bowiearm.getArmParked()) bowie.bowiearm.unparkArm();
        }
      }

      // empty
      if(packets[0].cmd == 'G') { // 3 green button
        if(packets[0].val == 1 && completed_action[4] == false) { // && current_time-last_called[4] >= 5000) {
          emptyScoop();
          resetCompletes();
          completed_action[4] = true;
          last_called[4] = current_time;
        } else if(packets[0].val == 0) {
          completed_action[4] = false;
        }
      }

      // up
      if(packets[0].cmd == 'W') { // 4 white button
        if(packets[0].val == 1 && completed_action[3] == false) {
          moveTheArm(ARM_MAX, 3, 3);
          resetCompletes();
          completed_action[3] = true;
        } else if(packets[0].val == 0) {
          completed_action[3] = false;
        }
      }
      
      // mid
      if(packets[0].cmd == 'B') { // 5 blue button
        if(packets[0].val == 1 && completed_action[4] == false) {
          moveTheArm(ARM_HOME, 3, 3);
          resetCompletes();
          completed_action[4] = true;
        } else if(packets[0].val == 0) {
          completed_action[4] = false;
        }
      }

      // down
      if(packets[0].cmd == 'N') { // 6 black button
        if(packets[0].val == 1 && completed_action[5] == false) {
          moveTheArm(ARM_MIN, 3, 3);
          resetCompletes();
          completed_action[5] = true;
        } else if(packets[0].val == 0) {
          completed_action[5] = false;
        }
      }

    } // -- end of mode 1




    

    if(packets[0].key == 3) {

      // dance
      if(packets[0].cmd == 'P') { // 1 red button
        if(packets[0].val == 1 && completed_action[0] == false) { 

          if(bowie.bowiearm.getArmParked()) bowie.bowiearm.unparkArm();

          bowie.bowielights.setLight(99, MAX_BRIGHTNESS);
          
          // drive forward a bit
          if(BOT_DEBUG_MEGA) Serial << "Going to MOTORS FWD 255...";
          bowie.bowiedrive.rampSpeed(true, 100, 255, 20, 10);
          bowie.bowiedrive.goSpeed(true, 255, 250);
          // stop motors!
          if(BOT_DEBUG_MEGA) Serial << "Going to MOTORS FWD 0...";
          bowie.bowiedrive.rampSpeed(true, 255, 100, 10, 5);
          bowie.bowiedrive.goSpeed(true, 0, 0);

          // blink
          for(int i=0; i<2; i++) {
            bowie.bowielights.setLight(99, MAX_BRIGHTNESS);
            delay(200);
            bowie.bowielights.setLight(99, MIN_BRIGHTNESS);
            delay(200);
          }

          bowie.bowielights.setLight(99, MAX_BRIGHTNESS);
          
          // drive backward
          if(BOT_DEBUG_MEGA) Serial << "Going to MOTORS BWD 255...";
          bowie.bowiedrive.rampSpeed(false, 100, 255, 20, 10);
          bowie.bowiedrive.goSpeed(false, 255, 250);
          // stop motors!
          if(BOT_DEBUG_MEGA) Serial << "Going to MOTORS BWD 0...";
          bowie.bowiedrive.rampSpeed(true, 255, 100, 10, 5);
          bowie.bowiedrive.goSpeed(true, 0, 0);

          // turn
          bowie.bowiedrive.motor_setDir(0, true);
          bowie.bowiedrive.motor_setSpeed(0, 255);
          bowie.bowiedrive.motor_setDir(1, false);
          bowie.bowiedrive.motor_setSpeed(1, 255);

          // raise the arm while keeping scoop parallel to ground

          uint16_t prev_arm_pos = bowie.bowiearm.getArmPos();

          moveTheArm(ARM_MAX, 3, 3);
          delay(1000);

          // stop motors!
          if(BOT_DEBUG_MEGA) Serial << "Going to MOTORS BWD 0...";
          bowie.bowiedrive.rampSpeed(true, 255, 100, 10, 5);
          bowie.bowiedrive.goSpeed(true, 0, 0);

          // blink
          bowie.bowielights.setLight(99, 60);
          delay(200);
          bowie.bowielights.setLight(99, 0);
          delay(200);
          bowie.bowielights.setLight(99, 150);
          delay(200);
          bowie.bowielights.setLight(99, 0);
          delay(200);
          bowie.bowielights.setLight(99, MAX_BRIGHTNESS);
          delay(200);
          bowie.bowielights.setLight(99, 0);
          delay(200);
          bowie.bowielights.setLight(99, MAX_BRIGHTNESS);
          delay(200);

          // turn
          bowie.bowiedrive.motor_setDir(0, false);
          bowie.bowiedrive.motor_setSpeed(0, 255);
          bowie.bowiedrive.motor_setDir(1, true);
          bowie.bowiedrive.motor_setSpeed(1, 255);

          // arm to middle
          moveTheArm(ARM_HOME, 3, 3);
          delay(1000);

          // lower the arm back to the previous position
          moveTheArm(prev_arm_pos, 3, 3);
          delay(1000);

          // blink
          bowie.bowielights.setLight(99, 60);
          delay(200);
          bowie.bowielights.setLight(99, 0);
          delay(200);
          bowie.bowielights.setLight(99, 150);
          delay(200);
          bowie.bowielights.setLight(99, 0);
          delay(200);
          bowie.bowielights.setLight(99, MAX_BRIGHTNESS);
          delay(200);
          bowie.bowielights.setLight(99, 0);
          delay(200);
          bowie.bowielights.setLight(99, MAX_BRIGHTNESS);
          delay(200);

          resetCompletes();
          completed_action[0] = true;
        } else if(packets[0].val == 0) {
          completed_action[0] = false;
        }
      }

      // blink
      if(packets[0].cmd == 'Y') { // 2 yellow button
        if(packets[0].val == 1) {// && completed_action[1] == false) { 

          for(int i=0; i<4; i++) {
            bowie.bowielights.setLight(99, MAX_BRIGHTNESS);
            delay(200);
            bowie.bowielights.setLight(99, MIN_BRIGHTNESS);
            delay(200);
          }
          
          resetCompletes();
          completed_action[1] = true;
        } else if(packets[0].val == 0) {
          completed_action[1] = false;
        }
      }

      // wave
      if(packets[0].cmd == 'G') { // 3 green button
        if(packets[0].val == 1 && completed_action[2] == false) { 
          wave();
          resetCompletes();
          completed_action[2] = true;
        } else if(packets[0].val == 0) {
          completed_action[2] = false;
        }
      }

      // sleep
      if(packets[0].cmd == 'W') { // 4 white button
        if(packets[0].val == 1 && completed_action[3] == false) { 
          bowie.bowiearm.parkArm();
          bowie.bowiescoop.parkEnd();
          resetCompletes();
          completed_action[3] = true;
        } else if(packets[0].val == 0) {
          completed_action[3] = false;
        }
      }
      
      // AWSUM
      if(packets[0].cmd == 'B') { // 5 blue button
        if(packets[0].val == 1) {

          // blinky lights
          for(int j=0; j<2; j++) {
            for(int i=0; i<4; i++) {
              bowie.bowielights.setLight(99, MIN_BRIGHTNESS);
              bowie.bowielights.setLight(i, MAX_BRIGHTNESS);
              delay(80);
            }
            for(int i=3; i>=0; i--) {
              bowie.bowielights.setLight(99, MIN_BRIGHTNESS);
              bowie.bowielights.setLight(i, MAX_BRIGHTNESS);
              delay(80);
            }
          }

          uint16_t prev_end_pos = bowie.bowiescoop.getEndPos();

          for(int j=0; j<2; j++) {

            bowie.bowiescoop.moveEnd(END_MAX-500, 3, 1);
            for(int i=0; i<2; i++) {
              bowie.bowiescoop.moveEnd(END_MAX-400, 2, 1);
              delay(80);
              bowie.bowiescoop.moveEnd(END_MAX-600, 2, 1);
              delay(80);
            }
            delay(100);
  
            bowie.bowiescoop.moveEnd(END_MAX+200, 3, 1);
            for(int i=0; i<2; i++) {
              bowie.bowiescoop.moveEnd(END_MAX+100, 2, 1);
              delay(80);
              bowie.bowiescoop.moveEnd(END_MAX+300, 2, 1);
              delay(80);
            }
            delay(100);

          }
          
          bowie.bowiescoop.setEndPos(prev_end_pos);

        }
      }

      // n/a
      if(packets[0].cmd == 'N') { // 6 black button
        if(packets[0].val == 1) {
          
        }
      }

    } // -- end of mode 3

  } // -- end of '#' action specifier


  
}


