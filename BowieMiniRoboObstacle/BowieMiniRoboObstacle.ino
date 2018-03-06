/*
 * Bowie Mini Robo-Obstacle Course
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
 */

#include "MiniBowieShoreline.h"

#define ROBOT_ID 3

MiniBowieShoreline bowie;

bool completed_action[] = { false, false, false, false, false, false };
long last_called[] = { 0, 0, 0, 0, 0, 0 };
long current_time = 0;

void control(Msg m);

void setup() {
  Serial.begin(9600);
  bowie = MiniBowieShoreline();
  bowie.begin();
  bowie.setRobotID(ROBOT_ID);
  bowie.disableDefaultActions();
  bowie.set_control_callback(control);
}

void loop() {
  current_time = millis();
  bowie.update(false);
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
        }
      }

      // empty
      if(packets[0].cmd == 'G') { // 3 green button
        if(packets[0].val == 1 && completed_action[4] == false) {
          resetCompletes();
          completed_action[4] = true;
          emptyScoop();
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

          bowie.bowielights.setLight(99, MAX_BRIGHTNESS);
          
          // drive forward a bit
          if(BOT_DEBUG_MINI) Serial << "Going to MOTORS FWD 255...";
          bowie.bowiedrive.rampSpeed(true, 100, 255, 20, 10);
          bowie.bowiedrive.goSpeed(true, 255, 250);
          // stop motors!
          if(BOT_DEBUG_MINI) Serial << "Going to MOTORS FWD 0...";
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
          if(BOT_DEBUG_MINI) Serial << "Going to MOTORS BWD 255...";
          bowie.bowiedrive.rampSpeed(false, 100, 255, 20, 10);
          bowie.bowiedrive.goSpeed(false, 255, 250);
          // stop motors!
          if(BOT_DEBUG_MINI) Serial << "Going to MOTORS BWD 0...";
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

          moveTheArm(ARM_HOME, 3, 3);

          delay(1000);
        


          // stop motors!
          if(BOT_DEBUG_MINI) Serial << "Going to MOTORS BWD 0...";
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



          // lower the arm back to the previous position

          
          moveTheArm(ARM_HOME, 3, 3);

          delay(1000);

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

          for(int i=0; i<2; i++) {
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
          // TODO
          resetCompletes();
          completed_action[3] = true;
        } else if(packets[0].val == 0) {
          completed_action[3] = false;
        }
      }
      
      // n/a
      if(packets[0].cmd == 'B') { // 5 blue button
        if(packets[0].val == 1) {
          
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


