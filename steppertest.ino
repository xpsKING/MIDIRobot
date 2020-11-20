#include <A4988.h>
#include <BasicStepperDriver.h>
#include <DRV8825.h>
#include <DRV8834.h>
#include <DRV8880.h>
#include <MultiDriver.h>
#include <SyncDriver.h>

// using a 200-step motor (most common)
#define MOTOR_STEPS 200
// configure the pins connected
#define DIR 6
#define STEP 9
#define MS1 10
#define MS2 11
#define MS3 5
bool noteOn=false;
bool receivedNoteNum=false;
bool receivedNoteVel=false;
bool noteOff=false;
int lastNote=60;
DRV8825 stepper(MOTOR_STEPS, DIR, STEP, MS1, MS2, MS3);

void setup() {
    // Set target motor RPM to 1RPM and microstepping to 1 (full step mode)
    stepper.begin(240, 1);
    Serial.begin(9600);
}
int noteToDeg(int msg){
    lastNote=msg;

    if(msg == 60){
        return 250;
    }
    if(msg == 61){
        return 100;
    }
}
void loop() {

 int msg = Serial.read();
 
 if(msg == 144&&!noteOn&&!receivedNoteNum &&!receivedNoteVel){ 
  noteOn=true;
  noteOff=false;
 }
 if(noteOn&& !receivedNoteNum &&!receivedNoteVel &&!noteOff &&msg <128){
  
  stepper.rotate(noteToDeg(msg));
  receivedNoteNum=true;
 }
 if(noteOn&& receivedNoteNum && !receivedNoteVel &&!noteOff&&msg <128){
  //gets velocity
  receivedNoteVel=true;
 }
 if(noteOn&& receivedNoteNum && receivedNoteVel && msg==128 &&!noteOff){
  //noteOff signal
  noteOn=false;
  receivedNoteNum=false;
  receivedNoteVel=false;
  stepper.rotate(-1* noteToDeg(lastNote));  
  noteOff=true;
  
  
 }
 
 
   

}
