const byte PC_RESET = 2;
const byte PC_ON_OFF = 3;

const byte R1 = A0;
const byte R2 = A1;
const byte R3 = A2;
const byte R4 = A3; 

byte stage = 0;
byte help_send_stage = 0;
String number;

void setup() {
  pinMode(PC_ON_OFF, OUTPUT);
  digitalWrite(PC_ON_OFF, LOW);

  pinMode(PC_RESET, OUTPUT);
  digitalWrite(PC_RESET, LOW);  

  pinMode(R1, OUTPUT);
  digitalWrite(R1, HIGH); // Высокий уровень - реле выключено

  pinMode(R2, OUTPUT);
  digitalWrite(R2, HIGH);

  pinMode(R3, OUTPUT);
  digitalWrite(R3, HIGH);

  pinMode(R4, OUTPUT);
  digitalWrite(R4, HIGH);       
  
  // initialize both serial ports:
  //Serial.begin(115200);
  GSM.begin(115200);
  //delay(1000);

  //Serial.println("GSM Relay ready!");
 
}

void SendSms(const String& number, const String& messages){
  String cmd = "AT+CMGS=\"+"+number+"\"";
  GSM.println(cmd); 
  if (GSM.find(">")) {
    GSM.print(messages);
    GSM.print((char)26);
  }
}

void loop() {
  switch (stage) {
    case 0:
      if (GSM.find("+PBREADY")) {
        stage = 1;
        //Serial.println("Modem ready!");
        GSM.println("AT+CMGF=1");        
      }
      break;
    case 1:
      if (GSM.find("OK\r\n")) {
        stage = 2;
        //Serial.println("set TextMode!");
        GSM.println("AT+CSCS=\"GSM\"");      
      }
      break;
    case 2:
      if (GSM.find("OK\r\n")) {
        stage = 3;
        //Serial.println("set Character!");
        GSM.println("AT+CNMI=2,2,0,0,0");     
      }
      break;        
    case 3:
      if (GSM.find("OK\r\n")) {
        stage = 4;
        //Serial.println("set AutoReceiving!");    
      }
      break;   
    case 4:
      if (help_send_stage > 0) {
        switch(help_send_stage) {
          case 1:
            help_send_stage = 2;
            SendSms(number, "SIM-HOSTING resetor 1.0. Autor: roman@gavrilow.ru");
            break; 
          case 2:
            if (GSM.find("OK\r\n")) {
              help_send_stage = 3;
              SendSms(number, "pc: on/reset - startup or reset SIM-HOSTING server");
            }
            break; 
          case 3:
            if (GSM.find("OK\r\n")) {
              help_send_stage = 4;
              SendSms(number, "1/2/3/4: on/off/reset - power 1 or 2 or 3 or 4: on / off / off -> 5cek -> on");
            }
            break; 
          case 4:
            if (GSM.find("OK\r\n")) {
              help_send_stage = 0;
              SendSms(number, "all: on/off/reset - 1,2,3,4 power: on / off / off -> 5cek -> on");
            }
            break; 
        }
        return;
      }
    
      if (GSM.find("+CMT:")) {
        if (GSM.find("\"+")) {
          number = GSM.readStringUntil('\"');
        }
        
        if (GSM.find("\r\n")) {
          String incommingData = GSM.readStringUntil('\n');
          incommingData.trim();

          
          //Serial.print(incommingData);

          incommingData.toLowerCase();

          if (incommingData.startsWith("pc:")) {
            String cmd = incommingData.substring(incommingData.indexOf(":")+1);
            cmd.trim();
            //Serial.println(cmd);
            if (cmd.equals("on")) {
              digitalWrite(PC_ON_OFF, HIGH);
              delay(500);
              digitalWrite(PC_ON_OFF, LOW);
              SendSms(number, "Server is ON");
            } else if (cmd.equals("reset")) {
              digitalWrite(PC_RESET, HIGH);
              delay(500);
              digitalWrite(PC_RESET, LOW); 
              SendSms(number, "Server RESETED!");             
            }
           
          } else if (incommingData.startsWith("1:")) {
            String cmd = incommingData.substring(incommingData.indexOf(":")+1);
            cmd.trim();
            //Serial.println(cmd);
            if (cmd.equals("on")) {
              digitalWrite(R1, HIGH);
              SendSms(number, "1 is ON");
            } else if (cmd.equals("off")) {
              digitalWrite(R1, LOW);
              SendSms(number, "1 is OFF");             
            } else if (cmd.equals("reset")) {
              digitalWrite(R1, LOW);
              delay(5000);
              digitalWrite(R1, HIGH); 
              SendSms(number, "1 is RESETED!");             
            } 
          } else if (incommingData.startsWith("2:")) {
            String cmd = incommingData.substring(incommingData.indexOf(":")+1);
            cmd.trim();
            //Serial.println(cmd);
            if (cmd.equals("on")) {
              digitalWrite(R2, HIGH);
              SendSms(number, "2 is ON");
            } else if (cmd.equals("off")) {
              digitalWrite(R2, LOW);
              SendSms(number, "2 is OFF");             
            } else if (cmd.equals("reset")) {
              digitalWrite(R2, LOW);
              delay(5000);
              digitalWrite(R2, HIGH); 
              SendSms(number, "2 is RESETED!");             
            } 
          } else if (incommingData.startsWith("3:")) {
            String cmd = incommingData.substring(incommingData.indexOf(":")+1);
            cmd.trim();
            //Serial.println(cmd);
            if (cmd.equals("on")) {
              digitalWrite(R3, HIGH);
              SendSms(number, "3 is ON");
            } else if (cmd.equals("off")) {
              digitalWrite(R3, LOW);
              SendSms(number, "3 is OFF");             
            } else if (cmd.equals("reset")) {
              digitalWrite(R3, LOW);
              delay(5000);
              digitalWrite(R3, HIGH); 
              SendSms(number, "3 is RESETED!");             
            } 
          } else if (incommingData.startsWith("4:")) {
            String cmd = incommingData.substring(incommingData.indexOf(":")+1);
            cmd.trim();
            //Serial.println(cmd);
            if (cmd.equals("on")) {
              digitalWrite(R4, HIGH);
              SendSms(number, "4 is ON");
            } else if (cmd.equals("off")) {
              digitalWrite(R4, LOW);
              SendSms(number, "4 is OFF");             
            } else if (cmd.equals("reset")) {
              digitalWrite(R4, LOW);
              delay(5000);
              digitalWrite(R4, HIGH); 
              SendSms(number, "4 is RESETED!");             
            } 
          } else if (incommingData.startsWith("all:")) {
            String cmd = incommingData.substring(incommingData.indexOf(":")+1);
            cmd.trim();
            //Serial.println(cmd);
            if (cmd.equals("on")) {
              digitalWrite(R1, HIGH);
              digitalWrite(R2, HIGH);
              digitalWrite(R3, HIGH);
              digitalWrite(R4, HIGH);
              SendSms(number, "ALL is ON");
            } else if (cmd.equals("off")) {
              digitalWrite(R1, LOW);
              digitalWrite(R2, LOW);
              digitalWrite(R3, LOW);
              digitalWrite(R4, LOW);              
              SendSms(number, "ALL is OFF");             
            } else if (cmd.equals("reset")) {
              digitalWrite(R1, LOW);
              digitalWrite(R2, LOW);
              digitalWrite(R3, LOW);
              digitalWrite(R4, LOW); 
              delay(5000);
              digitalWrite(R1, HIGH);
              digitalWrite(R2, HIGH);
              digitalWrite(R3, HIGH);
              digitalWrite(R4, HIGH);
              SendSms(number, "ALL is RESETED!");             
            } 
          } else if (incommingData.startsWith("help")) {
            help_send_stage = 1;    
          } else {
            SendSms(number, "komanda ne naidena :(");                    
          }
        }
      }
      break;
           
  }
}
