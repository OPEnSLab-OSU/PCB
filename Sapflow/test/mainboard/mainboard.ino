
#include <OPEnS_RTC.h>

const int ALARM_DURATION = 10; 		// Number of seconds before alarm goes off

// Instance of DS3231 RTC
RTC_DS3231 RTC_DS; 


#define ALARM_PIN 12


volatile bool alarmFlag;
void alarmISR() { 
	detachInterrupt(digitalPinToInterrupt(ALARM_PIN)); 
	alarmFlag = true;
	Serial.println("Alarm triggered");
}


void setup() 
{ 
	pinMode(ALARM_PIN, INPUT_PULLUP);  // Pull up resistors required for Active-Low interrupts
	pinMode(LED_BUILTIN, OUTPUT);
  pinMode(5, OUTPUT);
  pinMode(6, OUTPUT);
  pinMode(A3, OUTPUT);
  digitalWrite(A3, LOW);
	digitalWrite(LED_BUILTIN, HIGH);

	RTC_DS.begin();
 #if 1
	DateTime t(__DATE__, __TIME__); 
  t = t + TimeSpan(5);   //account for compilation time
  RTC_DS.adjust(t);
  #endif

	Serial.begin(115200);
	while (!Serial); // Won't start anything until serial is open, comment this line out if powering from battery
	delay(1000);
	Serial.println("Initialized Serial");

	alarmFlag = true;

	Serial.println("\n ** Setup Complete ** ");
}

void loop() 
{
	if (alarmFlag) {
    digitalWrite(6, HIGH);
    delay(1000);
    digitalWrite(5, LOW);
    delay(1000);
    RTC_DS.clearAlarm();
    digitalWrite(A3, HIGH);
    delay(200);
    digitalWrite(A3, LOW);
		
		// Setup interrupt and alarm
		// Mysterious issue: Must call this function twice
		attachInterrupt( digitalPinToInterrupt(ALARM_PIN), alarmISR, LOW );
		attachInterrupt( digitalPinToInterrupt(ALARM_PIN), alarmISR, LOW );

		DateTime alarmTime = RTC_DS.now()+TimeSpan(ALARM_DURATION);
		Serial.print("Alarm set to ");
		Serial.println(alarmTime.text());
		RTC_DS.setAlarm(alarmTime);
    digitalWrite(6, LOW);
    digitalWrite(5, HIGH);
		alarmFlag = false;
	}
}
