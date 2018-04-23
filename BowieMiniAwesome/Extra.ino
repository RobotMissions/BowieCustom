void moveTheArm(int armPos, int step, int del) {

  if(bowie.bowiearm.getArmParked()) bowie.bowiearm.unparkArm();
  if(bowie.bowiescoop.getEndParked()) bowie.bowiescoop.unparkEnd();

  int prev_pos = bowie.bowiearm.getArmPos();
  int endPos = bowie.bowiescoop.getEndPos();
  if(prev_pos > armPos) { // headed towards ARM_MIN
    for(int i=prev_pos; i>armPos; i-=step) {
      bowie.bowiearm.arm.writeMicroseconds(i);
      bowie.bowiearm.arm2.writeMicroseconds(SERVO_MAX_US - i + SERVO_MIN_US);
      bowie.bowiearm.setArmPos(i);
      if(i >= ARM_HOME) {
        endPos = bowie.clawParallelValBounds(i, ARM_HOME, ARM_MAX, END_HOME-100, END_PARALLEL_TOP+300);
      } else {
        endPos = bowie.clawParallelValBounds(i, ARM_MIN, ARM_HOME, END_PARALLEL_BOTTOM-100, END_HOME-100);
      }
      bowie.bowiescoop.scoop.writeMicroseconds(endPos);
      bowie.bowiescoop.setEndPos(endPos);
      delay(del);
    }
  } else if(prev_pos <= armPos) { // headed towards ARM_MAX
    for(int i=prev_pos; i<armPos; i+=step) {
      bowie.bowiearm.arm.writeMicroseconds(i);
      bowie.bowiearm.arm2.writeMicroseconds(SERVO_MAX_US - i + SERVO_MIN_US);
      bowie.bowiearm.setArmPos(i);
      if(i >= ARM_HOME) {
        endPos = bowie.clawParallelValBounds(i, ARM_HOME, ARM_MAX, END_HOME-100, END_PARALLEL_TOP+300);
      } else {
        endPos = bowie.clawParallelValBounds(i, ARM_MIN, ARM_HOME, END_PARALLEL_BOTTOM-100, END_HOME-100);
      }
      bowie.bowiescoop.scoop.writeMicroseconds(endPos);
      bowie.bowiescoop.setEndPos(endPos);
      delay(del); 
    }
  }
  bowie.bowiearm.arm.writeMicroseconds(armPos);
  bowie.bowiearm.arm2.writeMicroseconds(SERVO_MAX_US - armPos + SERVO_MIN_US);
  bowie.bowiearm.setArmPos(armPos);
  delay(del);
  
  
}

void emptyScoop() {

  uint16_t prev_arm_pos = bowie.bowiearm.getArmPos();
  uint16_t prev_end_pos = bowie.bowiescoop.getEndPos();

  if(bowie.bowiearm.getArmParked()) bowie.bowiearm.unparkArm();
  if(bowie.bowiescoop.getEndParked()) bowie.bowiescoop.unparkEnd();

  uint16_t endPos;
  uint16_t armPos;
  uint16_t tiltEndPos;
  
  // tilt the scoop up
  if(prev_arm_pos < ARM_HOME) {
    
    tiltEndPos = END_PARALLEL_BOTTOM-600;
    if(prev_end_pos < tiltEndPos) { // increment
      for(int i=prev_end_pos; i<tiltEndPos; i++) {
        bowie.bowiescoop.scoop.writeMicroseconds(i);
        bowie.bowiescoop.setEndPos(i);
        delay(2);  
      }  
    } else { // decrement
      for(int i=prev_end_pos; i>tiltEndPos; i--) {
        bowie.bowiescoop.scoop.writeMicroseconds(i);
        bowie.bowiescoop.setEndPos(i);
        delay(2);  
      }  
    }
    bowie.bowiescoop.scoop.writeMicroseconds(tiltEndPos);
    bowie.bowiescoop.setEndPos(tiltEndPos);

  }

  // raise the arm while keeping scoop parallel to ground
  for(int i=prev_arm_pos; i<ARM_MAX; i+=2) {
    bowie.bowiearm.arm.writeMicroseconds(i);
    bowie.bowiearm.arm2.writeMicroseconds(SERVO_MAX_US - i + SERVO_MIN_US);
    bowie.bowiearm.setArmPos(i);
    endPos = bowie.clawParallelValBounds(i, prev_arm_pos, ARM_MAX, tiltEndPos, END_PARALLEL_TOP); // END_HOME
    bowie.bowiescoop.scoop.writeMicroseconds(endPos);
    bowie.bowiescoop.setEndPos(endPos);
    delay(3); 
  }
  bowie.bowiescoop.scoop.writeMicroseconds(END_PARALLEL_TOP);
  bowie.bowiescoop.setEndPos(endPos);
  bowie.bowiearm.arm.writeMicroseconds(ARM_MAX);
  bowie.bowiearm.arm2.writeMicroseconds(SERVO_MAX_US - ARM_MAX + SERVO_MIN_US);
  bowie.bowiearm.setArmPos(ARM_MAX);
  delay(3);
  
  // ---

  
  bowie.bowiehopper.moveLid(LID_MIN, 4, 1); // open lid

  delay(100);

  //bowie.bowiescoop.moveEnd(END_MIN, 3, 2); // dump scoop
  bowie.bowiescoop.scoop.writeMicroseconds(END_MIN);
  bowie.bowiescoop.setEndPos(END_MIN);
  delay(100);
  for(int i=0; i<4; i++) {
    //bowie.bowiescoop.moveEnd(END_MIN-100, 3, 1);
    bowie.bowiescoop.scoop.writeMicroseconds(END_MIN-100);
    bowie.bowiescoop.setEndPos(END_MIN-100);
    delay(100);
    //bowie.bowiescoop.moveEnd(END_MIN+100, 3, 1);
    bowie.bowiescoop.scoop.writeMicroseconds(END_MIN+100);
    bowie.bowiescoop.setEndPos(END_MIN+100);
    delay(100);
  }
  bowie.bowiescoop.scoop.writeMicroseconds(END_MIN);
  bowie.bowiescoop.setEndPos(END_MIN);
  delay(100);
  
  // lower the arm back to the previous position

  moveTheArm(ARM_HOME, 3, 3);

  /*
  // TODO: this is buggy
  for(int i=ARM_MAX; i>prev_arm_pos; i-=2) {
    bowie.bowiearm.arm.writeMicroseconds(i);
    bowie.bowiearm.arm2.writeMicroseconds(SERVO_MAX_US - i + SERVO_MIN_US);
    bowie.bowiearm.setArmPos(i);
    endPos = bowie.clawParallelValBounds(i, prev_arm_pos, ARM_MAX, END_HOME, END_PARALLEL_TOP);
    bowie.bowiescoop.scoop.writeMicroseconds(endPos);
    bowie.bowiescoop.setEndPos(endPos);
    delay(3); 
  }
  bowie.bowiescoop.scoop.writeMicroseconds(prev_end_pos);
  bowie.bowiescoop.setEndPos(prev_end_pos);
  bowie.bowiearm.arm.writeMicroseconds(prev_arm_pos);
  bowie.bowiearm.arm2.writeMicroseconds(SERVO_MAX_US - prev_arm_pos + SERVO_MIN_US);
  bowie.bowiearm.setArmPos(prev_arm_pos);
  delay(3);
  */

  //bowie.bowiehopper.lid.writeMicroseconds(LID_MAX);
  delay(100);
  bowie.bowiehopper.moveLid(LID_MAX, 4, 1); // close lid

}


void wave() {

  uint16_t prev_arm_pos = bowie.bowiearm.getArmPos();
  uint16_t prev_end_pos = bowie.bowiescoop.getEndPos();

  if(bowie.bowiearm.getArmParked()) bowie.bowiearm.unparkArm();
  if(bowie.bowiescoop.getEndParked()) bowie.bowiescoop.unparkEnd();

  // raise the arm while keeping scoop parallel to ground

  uint16_t endPos;
  uint16_t armPos;

  for(int i=prev_arm_pos; i<ARM_MAX-300; i+=2) {
    bowie.bowiearm.arm.writeMicroseconds(i);
    bowie.bowiearm.arm2.writeMicroseconds(SERVO_MAX_US - i + SERVO_MIN_US);
    bowie.bowiearm.setArmPos(i);
    endPos = bowie.clawParallelValBounds(i, prev_arm_pos, ARM_MAX, END_HOME, END_PARALLEL_TOP);
    bowie.bowiescoop.scoop.writeMicroseconds(endPos);
    bowie.bowiescoop.setEndPos(endPos);
    delay(3); 
  }
  bowie.bowiescoop.scoop.writeMicroseconds(END_PARALLEL_TOP);
  bowie.bowiescoop.setEndPos(END_PARALLEL_TOP);
  bowie.bowiearm.arm.writeMicroseconds(ARM_MAX-300);
  bowie.bowiearm.arm2.writeMicroseconds(SERVO_MAX_US - ARM_MAX-300 + SERVO_MIN_US);
  bowie.bowiearm.setArmPos(ARM_MAX-300);
  delay(300);

  // ---

  
  for(int i=0; i<4; i++) {
    bowie.bowiescoop.moveEnd(END_MAX+500, 3, 1);
    delay(100);
    bowie.bowiescoop.moveEnd(END_MAX+200, 3, 1);
    delay(100);
  }
  bowie.bowiescoop.setEndPos(END_MAX+200);
  
  // lower the arm back to the previous position

  for(int i=ARM_MAX-300; i>prev_arm_pos; i-=2) {
    bowie.bowiearm.arm.writeMicroseconds(i);
    bowie.bowiearm.arm2.writeMicroseconds(SERVO_MAX_US - i + SERVO_MIN_US);
    bowie.bowiearm.setArmPos(i);
    endPos = bowie.clawParallelValBounds(i, prev_arm_pos, ARM_MAX-300, END_HOME, prev_end_pos);
    bowie.bowiescoop.scoop.writeMicroseconds(endPos);
    bowie.bowiescoop.setEndPos(endPos);
    delay(3); 
  }
  bowie.bowiescoop.scoop.writeMicroseconds(prev_end_pos);
  bowie.bowiescoop.setEndPos(prev_end_pos);
  bowie.bowiearm.arm.writeMicroseconds(prev_arm_pos);
  bowie.bowiearm.arm2.writeMicroseconds(SERVO_MAX_US - prev_arm_pos + SERVO_MIN_US);
  bowie.bowiearm.setArmPos(prev_arm_pos);
  delay(3);

}

void resetCompletes() {
  for(int i=0; i<6; i++) {
    completed_action[i] = false;
  }
}


