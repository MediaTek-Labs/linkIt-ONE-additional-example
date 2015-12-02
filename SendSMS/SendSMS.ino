#include <LGSM.h>
char phonenum[20] = "4088581234";

void setup() {
  // put your setup code here, to run once:
  Serial.begin(9600);
  
  while(!LSMS.ready())
    delay(1000);
  
  Serial.println("SIM ready for work!");
  
  LSMS.beginSMS(phonenum);
  LSMS.print("hi");
  if(LSMS.endSMS()){
    Serial.println("SMS is sent");
  }else{
    Serial.println("SMS is not sent");
  }
}

void loop() {
  // put your main code here, to run repeatedly:

}
