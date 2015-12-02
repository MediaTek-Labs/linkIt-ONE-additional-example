#include <LGSM.h>

void setup() {
  // put your setup code here, to run once:
  Serial.begin(9600);
  
  while(!LSMS.ready())
    delay(1000);
  Serial.println("SIM ready for work!");
}

void loop() {
  // put your main code here, to run repeatedly:
  char buf[20];
  int v;
  if(LSMS.available())
  {
    Serial.println("There is a new message.");
    
    LSMS.remoteNumber(buf, 20);
    Serial.print("Number:");
    Serial.println(buf);
    
    Serial.print("Content:");
    while(true)
    {
      v = LSMS.read();
      if(v < 0)
        break;
      Serial.print((char)v);
    }
    Serial.println();
    
    LSMS.flush();
  }
  delay(1000);
}
