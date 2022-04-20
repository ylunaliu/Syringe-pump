//Define useful parameter here
#define dirPin 2
#define stepPin 3
#define switchUp 4
#define switchDown 5
String inputString = ""; //string holding the incoming data
String command; //String that stores the command (setVol, setflow, getStatus, Stop, Push, Draw, whoami)
float commandval; //stores the speed or flowrate
boolean stringComplete = false; //whether the string is complete
float flowrate = 800; 
float volume = 300; 
boolean posDirection = true; //direction for the pump
int delimiterIndex[2];
int state = 0; //state of the pump
int steps = 0; // should get updated when stop/push/draw command comes in and while running in loop
float delaytime = 2;

void setup() {
  // put your setup code here, to run once:
  Serial.begin(9600);
  inputString.reserve(100);
  pinMode(stepPin, OUTPUT);
  pinMode(dirPin, OUTPUT);
  pinMode(switchUp, INPUT);
  pinMode(switchDown, INPUT);

  //Set the spinning direction cw/ccw
  digitalWrite(dirPin, HIGH);
  
}


void serialEvent(){
  while(Serial.available()){
    // get the new byte
    char inChar = (char)Serial.read();
    // add it to the inputString
    inputString += inChar;
    // if the incoming character is a newline, set a flag
    // so the main loop can do something about it
    if(inChar == '\n'){
      stringComplete = true;
      }
    }
  }



void loop() {
  // print the string when a newline arrives:
  if(stringComplete){
    //find the delimiters and store them in a char array
    delimiterIndex[0] = inputString.indexOf(',');
    delimiterIndex[1] = inputString.indexOf(',', delimiterIndex[0]+1);
    
    // use the numbers stores in the char array to split the inputString into pieces.
    String command = inputString.substring(0, delimiterIndex[0]);
    String commandvalue = inputString.substring(delimiterIndex[0]+1, delimiterIndex[1]);

    commandval=commandvalue.toFloat();
    //clear the inputString
    inputString = "";
    //change the boolean state
    stringComplete = false;

    if(command == "setvolume"){
      Serial.print("set the volume to ");
      Serial.print(commandvalue);
      volume = commandval; 
      }

    if(command == "setflowrate"){
      Serial.print("set the flow rate to ");
      Serial.print(commandvalue);
      flowrate = commandval;
      delaytime = 25 / (4*flowrate)*1000;
      }
     
    if(command =="push"){
      Serial.println("Push pump");
      steps = 4.8*volume;
      state = 2;
      digitalWrite(dirPin, HIGH);
      }

    if(command == "draw"){
      Serial.print("Draw pump");
      steps = 4.8*volume;
      state = 1;
      digitalWrite(dirPin, LOW);
      }
    
    if(command == "stop"){
      Serial.print("Stop");
      steps = 0;
    //  Serial.print(steps);
      }
    
    if(command == "whoami"){
      Serial.print("I am the 10ml pump");
      }

    if(command == "getstatus"){
     // Serial.print("Status");
      Serial.println(state);
      }
  }
    
    //draw
    //precautionary measures for debouncing
    if((steps > 0) & (state==1)){
      if(digitalRead(switchUp)==HIGH){
        steps= 0;
        state = 0;
      }
      if(digitalRead(switchUp)==HIGH){
        steps= 0;
        state = 0;
      }
      digitalWrite(stepPin, HIGH);
      delay((int)delaytime);
      digitalWrite(stepPin, LOW);
      delay((int)delaytime);
      steps -= 1;
      }

    //push
    if((steps >0) & (state==2)){
      if(digitalRead(switchDown)==HIGH){
        steps= 0;
        state = 0;
      }
      if(digitalRead(switchDown)==HIGH){
        steps= 0;
        state = 0;
      }
      digitalWrite(stepPin, HIGH);
      delay((int)delaytime);
      digitalWrite(stepPin, LOW);
      delay((int)delaytime);
      steps -= 1;
      }    

}
