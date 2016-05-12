#include <SoftwareSerial.h>
SoftwareSerial mySerial(10, 11); // RX, TX

String ordenes[]=
    {  //"AT+RST",    //restart
       "AT+CWMODE=3", //wifimode sta+ap
       "AT+CIFSR" ,   //get ip address
       "AT+CIPMUX=1", // tcp/updp conections
       "AT+CIPSERVER=1,80", // Set as server = <mode>[,<port>] mode 0 to close server mode; mode 1 to open; port = port
       "END"        // Para reconocer el fin de los comandos AT
    };

void setup()
{   
    mySerial.begin(115200);
    //Serial3.begin(115200);
    Serial.begin(115200); 
    delay (1000);
    int index = 0;
    while(ordenes[index] != "END")
        {  mySerial.println(ordenes[index++]);
            Serial.println(ordenes[index++]);
           while ( true)
             {   String s = GetLineWIFI();
                 if ( s!= "") Serial.println(s);
                 if ( s.startsWith("no change"))   
                         break;
                 if ( s.startsWith("OK"))   
                         break;
                 if ( s.startsWith("ready"))   
                         break;
                // if (millis()-T >10000) break;  
             }
          Serial.println("....................");
      } 
}
void loop() 
{  while (mySerial.available() >0 )
    {  char c = mySerial.read();
    Serial.println (c);
       if (c == 71) 
          {   Serial.println("Enviada Web Request");
              webserver();
              delay(500);
          }
   }
}

void http(String output)
{
    mySerial.print("AT+CIPSEND=0,");              // Send TCP/IP data
    Serial.print("AT+CIPSEND=0,");
    mySerial.println(output.length());
     Serial.println(output.length());
    if (mySerial.find(">"))                       // Si recibimos la peticion del mensaje
       {    Serial.println(output);
            mySerial.println(output);            //Aqui va el string hacia el WIFI 
            delay(10);
            while ( mySerial.available() > 0 ) 
              { if (  mySerial.find("SEND OK") )    // Busca el OK en la respuesta del wifi
                      break; 
              }
       }
}
void webserver(void) 
    {    http("<!DOCTYPE HTML>");
         http("<html>");
         http("<head><title>LECTURAS ANALOGICAS.</title>");
         http("<meta http-equiv=\"refresh\" content=\"15\"></head>");
         http("<body><h1> Situacion Ambiente</h1>");
  
         for (int analogChannel = 0; analogChannel < 6; analogChannel++) 
         {
            int sensorReading = analogRead(analogChannel);
            http("analog input ");
            http( String(analogChannel));
            http(" is ");
            http(String(sensorReading));
            http("<br />");
          }
        http("<p><em> La página se actualiza cada 15 segundos.</em></p></body></html>");

       delay(1);
       mySerial.println("AT+CIPCLOSE=0");
       Serial.println("AT+CIPCLOSE=0");
       //delay(500);
    }
String GetLineWIFI()
   {   String S = "" ;
       if (mySerial.available())
          {    char c = mySerial.read(); ;
                while ( c != '\n' )            //Hasta que el caracter sea intro
                  {     S = S + c ;
                        delay(25) ;
                        c = mySerial.read();
                   }
                 return( S ) ;
          }
   }


