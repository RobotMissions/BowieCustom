void moveTheArm(int armPos, int step, int del) {

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
  delay(del);
  
  
}

void resetCompletes() {
  for(int i=0; i<6; i++) {
    completed_action[i] = false;
  }
}

void emptyScoop() {

  uint16_t prev_arm_pos = bowie.bowiearm.getArmPos();

  // raise the arm while keeping scoop parallel to ground

  uint16_t endPos;
  uint16_t armPos;

  for(int i=prev_arm_pos; i<ARM_MAX; i+=2) {
    bowie.bowiearm.arm.writeMicroseconds(i);
    bowie.bowiearm.arm2.writeMicroseconds(SERVO_MAX_US - i + SERVO_MIN_US);
    bowie.bowiearm.setArmPos(i);
    endPos = bowie.clawParallelValBounds(i, prev_arm_pos, ARM_MAX, END_HOME, END_PARALLEL_TOP);
    bowie.bowiescoop.scoop.writeMicroseconds(endPos);
    bowie.bowiescoop.setEndPos(endPos);
    delay(3); 
  }
  bowie.bowiearm.arm.writeMicroseconds(ARM_MAX);
  bowie.bowiearm.arm2.writeMicroseconds(SERVO_MAX_US - ARM_MAX + SERVO_MIN_US);
  bowie.bowiearm.setArmPos(ARM_MAX);
  bowie.bowiescoop.scoop.writeMicroseconds(END_PARALLEL_TOP);
  bowie.bowiescoop.setEndPos(endPos);
  delay(3);

  // ---

  bowie.bowiehopper.moveLid(LID_MIN, 4, 1); // open lid

  bowie.bowiescoop.moveEnd(END_MIN, 3, 2); // dump scoop
  for(int i=0; i<4; i++) {
    bowie.bowiescoop.moveEnd(END_MIN-100, 3, 1);
    delay(10);
    bowie.bowiescoop.moveEnd(END_MIN+100, 3, 1);
    delay(10);
  }

  // lower the arm back to the previous position

  for(int i=ARM_MAX; i>prev_arm_pos; i-=2) {
    bowie.bowiearm.arm.writeMicroseconds(i);
    bowie.bowiearm.arm2.writeMicroseconds(SERVO_MAX_US - i + SERVO_MIN_US);
    bowie.bowiearm.setArmPos(i);
    endPos = bowie.clawParallelValBounds(i, prev_arm_pos, ARM_MAX, END_HOME, END_PARALLEL_TOP);
    bowie.bowiescoop.scoop.writeMicroseconds(endPos);
    bowie.bowiescoop.setEndPos(endPos);
    delay(3); 
  }
  bowie.bowiearm.arm.writeMicroseconds(prev_arm_pos);
  bowie.bowiearm.arm2.writeMicroseconds(SERVO_MAX_US - prev_arm_pos + SERVO_MIN_US);
  bowie.bowiearm.setArmPos(prev_arm_pos);
  bowie.bowiescoop.scoop.writeMicroseconds(END_PARALLEL_TOP);
  bowie.bowiescoop.setEndPos(endPos);
  delay(3);
  
  bowie.bowiehopper.moveLid(LID_MAX, 4, 1); // close lid

}


void wave() {

  uint16_t prev_arm_pos = bowie.bowiearm.getArmPos();

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
  bowie.bowiearm.arm.writeMicroseconds(ARM_MAX-300);
  bowie.bowiearm.arm2.writeMicroseconds(SERVO_MAX_US - ARM_MAX-300 + SERVO_MIN_US);
  bowie.bowiearm.setArmPos(ARM_MAX);
  bowie.bowiescoop.scoop.writeMicroseconds(END_PARALLEL_TOP);
  bowie.bowiescoop.setEndPos(endPos);
  delay(3);

  // ---

  
  for(int i=0; i<5; i++) {
    bowie.bowiescoop.moveEnd(END_MAX+500, 3, 1);
    delay(100);
    bowie.bowiescoop.moveEnd(END_MAX+200, 3, 1);
    delay(100);
  }

  
  // lower the arm back to the previous position

  for(int i=ARM_MAX-300; i>prev_arm_pos; i-=2) {
    bowie.bowiearm.arm.writeMicroseconds(i);
    bowie.bowiearm.arm2.writeMicroseconds(SERVO_MAX_US - i + SERVO_MIN_US);
    bowie.bowiearm.setArmPos(i);
    endPos = bowie.clawParallelValBounds(i, prev_arm_pos, ARM_MAX-300, END_HOME, END_PARALLEL_TOP);
    bowie.bowiescoop.scoop.writeMicroseconds(endPos);
    bowie.bowiescoop.setEndPos(endPos);
    delay(3); 
  }
  bowie.bowiearm.arm.writeMicroseconds(prev_arm_pos);
  bowie.bowiearm.arm2.writeMicroseconds(SERVO_MAX_US - prev_arm_pos + SERVO_MIN_US);
  bowie.bowiearm.setArmPos(prev_arm_pos);
  bowie.bowiescoop.scoop.writeMicroseconds(END_HOME);
  bowie.bowiescoop.setEndPos(endPos);
  delay(3);

  
}




