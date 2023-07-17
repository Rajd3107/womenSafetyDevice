#define BUTTON_PIN 21 // GIOP21 pin connected to button
#define BUZZER_PIN 27

// Variables will change:
int lastState = LOW; // the previous state from the input pin
int currentState;     // the current reading from the input pin
String response;
void setup() {
  Serial.begin(9600);
  Serial2.begin(115200);
  delay(3000);
  // test_sim800_module();
  pinMode(BUZZER_PIN, OUTPUT);
  ledcSetup(0, 4000, 8); // channel 0, 2 kHz, 8-bit resolution
  // Attach the buzzer to the LEDC channel
  ledcAttachPin(BUZZER_PIN, 0);
  // initialize the pushbutton pin as an pull-up input
  pinMode(BUTTON_PIN, INPUT_PULLUP);
  getLocation();
}

void getLocation()
{ 
  Serial.println("Initializing...");
  delay(1000);
  
  // Serial2.println("AT+CGATT=1"); // Attach to Serial2
  // delay(2000);
  Serial2.println("AT+SAPBR=3,1,\"Contype\",\"GPRS\""); // Set the Serial2 context to HTTP
  delay(2000);
  Serial2.println("AT+SAPBR=3,1,\"APN\",\"www\""); // Set the APN
  delay(2000);
  // Serial2.println("AT+COPS=?");
  // delay(500);
  // while (Serial2.available()) {
  //   Serial.write(Serial2.read());
  // }
Serial2.println("AT+CSQ");
  delay(500);
  while (Serial2.available()) {
    Serial.write(Serial2.read());
  }


  Serial.println();
  Serial2.println("AT+SAPBR=1,1"); // Open the Serial2 context
  delay(2000);
  Serial2.println("AT+SAPBR=4,1"); // Open the Serial2 context
  delay(2000);
  // Serial2.println("AT+CIICR"); // Bring up wireless connection
  // delay(2000);
  // Serial2.println("AT+CIFSR"); // Get the local IP address
  // delay(2000);
  Serial2.println("AT+CLBS=1,1"); // Get the location using GSM network
  delay(25000);
  while (Serial2.available()) {
    char c = Serial2.read();
    Serial.write(c);
    response += c;
  }
  Serial2.println("AT+SAPBR=0,1"); // Open the Serial2 context
  delay(2000);
}



void ringer()
{
  int frequency = 2000;
  ledcWriteTone(0, frequency);
  delay(2000);
  
  // Turn off the buzzer for 500 ms
  ledcWriteTone(0, 0);
  delay(500);
}

void make_call()
{
  
  Serial2.println("ATD+919767527025;"); // replace +9170 with the phone number you want to call

  delay(5000); // wait for 5 seconds for the call to connect
  Serial2.write(0x1A); // send the "End of Text" character to end the call
  delay(5000); // wait for 5 seconds for the call to end
}

void send_SMS() {
  
  Serial2.println("AT+CMGF=1"); // set the SMS mode to text mode
  Serial.println("AT+CMGF=1"); // set the SMS mode to text mode

  while (Serial2.available())
  {
    Serial.write(Serial2.read());//Forward what Software Serial received to Serial Port
  }
  delay(2000);
  //
 String googleMapsLink;
  String locationData;
    // Extract latitude and longitude from the response
  int clbsIndex = response.indexOf("+CLBS:");
  if (clbsIndex != -1) {
    int commaIndex = response.indexOf(',', clbsIndex);
    if (commaIndex != -1) {
      locationData = response.substring(commaIndex + 1);

      int secondCommaIndex = locationData.indexOf(',');
      
      if (secondCommaIndex != -1) {
        String latitude = locationData.substring(0, secondCommaIndex);
     
        String longitude = locationData.substring(secondCommaIndex + 1,secondCommaIndex+9);

        googleMapsLink = "https://www.google.com/maps/place/" + longitude + "," + latitude;
         
      }
    }
  }
  Serial2.println("AT+CMGS=\"+919767527025\""); // replace with the phone number you want to send SMS to
  delay(2000);
  Serial2.print("Help! I am In Danger   \n"); // the SMS message
  Serial2.print(googleMapsLink); 
  // Serial2.print("https://www.google.com/maps/place/Research+Lab/16.8456964,74.601159");
  // delay(1000);

  // Serial2.write(0x1A);
  Serial2.write(26); // send the "End of Text" character to send the SMS
  
  delay(6000); // wait for 5 seconds for the SMS to be sent
  Serial2.println("AT+CMGS=\"+917038368795\""); // replace with the phone number you want to send SMS to
  delay(2000);
  Serial2.print("Help! I am In Danger   \n"); // the SMS message
  Serial2.print(googleMapsLink); 
  // Serial2.print("https://www.google.com/maps/place/Research+Lab/16.8456964,74.601159");
  // delay(1000);

  // Serial2.write(0x1A);
  Serial2.write(26);
  delay(5000);

  //

  Serial2.println("AT+CMGS=\"+919823186064\""); // replace with the phone number you want to send SMS to
  delay(2000);
  Serial2.print("Help! I am In Danger   \n"); // the SMS message
  Serial2.print(googleMapsLink); 
  // Serial2.print("https://www.google.com/maps/place/Research+Lab/16.8456964,74.601159");
  // delay(1000);

  // Serial2.write(0x1A);
  Serial2.write(26);
  delay(5000);
}


void loop() {
  // read the state of the switch/button:
  currentState = digitalRead(BUTTON_PIN);

  if (lastState == LOW && currentState == HIGH) {
    // Wait for a short delay to debounce the button
    delay(50);

    // Read the button state again
    currentState = digitalRead(BUTTON_PIN);

    // Check if the state is still HIGH after debouncing
    if (currentState == HIGH) {
      Serial.println("The state changed from LOW to HIGH");
      delay(500);
      ringer();
      send_SMS();
      delay(2000);
      make_call();
     
     
    }
  }

  // save the last state
  lastState = currentState;
}



