//################################################
// Dual Speed Fans - Dash Example
// 4/25/2023
// Started by Dustin Arrell
//################################################

//**************** Fans *****************
#define Fan1_Pin 9 //FAN output pin1
#define Fan2_Pin 10 //FAN output pin2

//************** Variables **************
int SpeedOld_Fan1 = 0, SpeedOld_Fan2 = 0, bufferArray[3];

//**************** Setup ****************
void setup() {
  pinMode(Fan1_Pin, OUTPUT); 
  pinMode(Fan2_Pin, OUTPUT);
   
  //Start Serial
  Serial.begin(115200); // opens serial port, sets data rate to 115200 bps
}

//**************** Loop *****************
void loop(){
    if (Serial.available() >= 3) { //if 3 bits available in the Serial buffer...     
      bufferArray[0] = Serial.read();    
      if (bufferArray[0]  == 'S'){ 
         bufferArray[1] = Serial.read();    
         bufferArray[2] = Serial.read();    
         if (SpeedOld_Fan1 != bufferArray[1]){
            SpeedOld_Fan1 = bufferArray[1];
            analogWrite(Fan1_Pin, bufferArray[1]);
         }   
         if (SpeedOld_Fan2 != bufferArray[2]){
            SpeedOld_Fan2 = bufferArray[2];
            analogWrite(Fan2_Pin, bufferArray[2]);
         }       
      }
   }
}
