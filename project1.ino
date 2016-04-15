#include <Bridge.h>
#include <Temboo.h>


const String CLIENT_ID = "872347227061-umi8m2llce1vgvlmvvj3l5rmn48ofohb.apps.googleusercontent.com";
const String CLIENT_SECRET = "YlPk2PB56Aqsu-nrf45VNF4a";
const String REFRESH_TOKEN = "1/aibV3jAcrZk241CWp9vq_G6wDBtgbmXuaBgmIqCbze4";
const String SPREADSHEET_TITLE = "alarmProject";

#define TEMBOO_ACCOUNT "pgorman"  // your Temboo account name 
#define TEMBOO_APP_KEY_NAME "alarmProject"  // your Temboo app key name
#define TEMBOO_APP_KEY  "LdcKxgbkaAPXJ81Lqdx5GkvPTBVVK8z0"  // your Temboo app key





const int touchPin = 4;     // the number of the touch pin
const int buzzerPin = 6;      // the number of the buzzer pin

// variables will change:
int touchState = 0;         // variable for reading the tilt status

void setup() {
  // initialize the buzzer pin as an output:
  pinMode(buzzerPin, OUTPUT);      
  // initialize the touch pin as an input:
  pinMode(touchPin, INPUT);     

  // for debugging, wait until a serial console is connected
  Serial.begin(9600);
  delay(4000);
  while(!Serial);

  Serial.print("Initializing the bridge...");
  Bridge.begin();
  Serial.println("Done");





  
}

void loop()
{
  unsigned long now = millis();
  
  // read the state of the touch value:
  touchState = digitalRead(touchPin);

  

  // check if the touch is close.
  // if it is, the tiltState is HIGH:
  if (touchState == LOW) {     
    // turn buzzer on:    
    digitalWrite(buzzerPin, HIGH);  

    

    Serial.println("Getting sensor value...");

    // get the value we want to append to our spreadsheet
    unsigned long sensorValue = getSensorValue();

    Serial.println("Appending value to spreadsheet...");

    // we need a Process object to send a Choreo request to Temboo
    TembooChoreo AppendRowChoreo;

    // invoke the Temboo client
    // NOTE that the client must be reinvoked and repopulated with
    // appropriate arguments each time its run() method is called.
    AppendRowChoreo.begin();
    
    // set Temboo account credentials
    AppendRowChoreo.setAccountName(TEMBOO_ACCOUNT);
    AppendRowChoreo.setAppKeyName(TEMBOO_APP_KEY_NAME);
    AppendRowChoreo.setAppKey(TEMBOO_APP_KEY);
    
    // identify the Temboo Library choreo to run (Google > Spreadsheets > AppendRow)
    AppendRowChoreo.setChoreo("/Library/Google/Spreadsheets/AppendRow");
    
    // set the required Choreo inputs
    // see https://www.temboo.com/library/Library/Google/Spreadsheets/AppendRow/ 
    // for complete details about the inputs for this Choreo
    
    // your Google application client ID
    AppendRowChoreo.addInput("ClientID", CLIENT_ID);
    // your Google application client secert
    AppendRowChoreo.addInput("ClientSecret", CLIENT_SECRET);
    // your Google OAuth refresh token
    AppendRowChoreo.addInput("RefreshToken", REFRESH_TOKEN);

    // the title of the spreadsheet you want to append to
    // NOTE: substitute your own value, retaining the "SpreadsheetTitle:" prefix.
    AppendRowChoreo.addInput("SpreadsheetTitle", SPREADSHEET_TITLE);

    // convert the time and sensor values to a comma separated string
    String rowData(now);
    rowData += ",";
    rowData += sensorValue;

    // add the RowData input item
    AppendRowChoreo.addInput("RowData", rowData);

    // run the Choreo and wait for the results
    // The return code (returnCode) will indicate success or failure 
    unsigned int returnCode = AppendRowChoreo.run();

    // return code of zero (0) means success
    if (returnCode == 0) 
    {
      Serial.println("Success! Appended " + rowData);
      Serial.println("");
    }
    else 
    {
      // return code of anything other than zero means failure  
      // read and display any error messages
      while (AppendRowChoreo.available()) 
      {
        char c = AppendRowChoreo.read();
        Serial.print(c);
      }
    }

    AppendRowChoreo.close();
  }
//}




    
    
  
 else 
  {
    // turn buzzer off:
    digitalWrite(buzzerPin, LOW); 
  }

}

// this function simulates reading the value of a sensor 
unsigned long getSensorValue() 
{
  return analogRead(4);
}


