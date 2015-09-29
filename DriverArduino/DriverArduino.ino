#include <SerialCommand.h>

#include <SparkFunAutoDriver.h>
#include <SPI.h>


AutoDriver board(10, 6, 7 );
SerialCommand sCmd;

unsigned long temp;

void setup() {
  // put your setup code here, to run once:

  Serial.begin(9600);

  temp = board.getParam(STATUS);
  Serial.print("board status: ");
  Serial.println(temp, HEX);

  Serial.println("resetting device");
  board.resetDev();

  dSPIN_config();
  cmdSetup();
}


void cmdSetup(){
  sCmd.addCommand("HELLO", sayHello);        // Echos the string argument back
  sCmd.addCommand("RUN",     processCommand);  // Converts two arguments to integers and echos them back
  sCmd.addCommand("STOP", stop);
  sCmd.addCommand("HALT", emergency);
  sCmd.addCommand("SET", processConfig);
  sCmd.setDefaultHandler(unrecognized);
  Serial.println("Ready");
  
}

void sayHello(){
  char* arg;
  arg = sCmd.next();    // Get the next argument from the SerialCommand object buffer
  if (arg != NULL) {    // As long as it existed, take it
    Serial.print("HELLO ");
    Serial.println(arg);
  }
  else {
    Serial.println("HELLO *");
  }
}


void processConfig(){

  
}

void processCommand(){

  int aNumber=-1;
  float steps=-1;
  char *arg;

//  Serial.println("We're in processCommand");
  arg = sCmd.next();
  if (arg != NULL) {
    aNumber = atoi(arg);    // Converts a char string to an integer
//    Serial.print("First argument was: ");
//    Serial.println(aNumber);
  }
  else {
//    Serial.println("No arguments");
  }

  arg = sCmd.next();
  if (arg != NULL) {
    steps = atof(arg);
//    Serial.print("Second argument was: ");
//    Serial.println(steps);
  }
  else {
//    Serial.println("No second argument");
  }

 byte dir = (aNumber == 0) ? REV : FWD;
 if (steps!=-1 && dir != -1) {
 run(dir, steps); 
 Serial.print("RUN ");
 Serial.print(  (aNumber == 0) ? "REV" : "FWD");
 Serial.print(" ");
 Serial.println(steps);
 } else {
  unrecognized("");
 }
 
}

// This gets set as the default handler, and gets called when no other command matches.
void unrecognized(const char *command) {
  Serial.println("ERROR Command not supported");
}



void dSPIN_config(){
  board.configSyncPin(BUSY_PIN, 0);
  board.setVoltageComp(VS_COMP_DISABLE); // don't compensate for motor V
  board.configStepMode(STEP_SEL_1_128);
  board.setMaxSpeed(10000);        // 10000 steps/s max
  board.setFullSpeed(10000);       // microstep below 10000 steps/s
  board.setAcc(10000);             // accelerate at 10000 steps/s/s
  board.setDec(10000);
  board.setPWMFreq(PWM_DIV_2, PWM_MUL_2);
  board.setOscMode(INT_16MHZ_OSCOUT_16MHZ);
}

void run(byte dir, float stepsPerSec){

  
  board.run(dir,stepsPerSec);

}

void stop(){

  Serial.println("STOP");
  board.softStop();
  board.hardHiZ();
}

void emergency(){

  Serial.println("HALT");
  board.hardStop();  
  board.hardHiZ();
}

void loop() {
  // put your main code here, to run repeatedly:
   sCmd.readSerial(); 
}
