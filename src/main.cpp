#include <GxEPD2_7C.h>
#include <Fonts/FreeMonoBold9pt7b.h>
#include <WiFi.h>
#include <DateTime.h>
#include <credentials.h>

// Select the display type
GxEPD2_7C<GxEPD2_730c_ACeP_730, GxEPD2_730c_ACeP_730::HEIGHT/4> display(GxEPD2_730c_ACeP_730(/*CS=*/ 5, /*DC=*/ 17, /*RST=*/ 16, /*BUSY=*/ 4));



void setupDateTime() {
  DateTime.setServer("0.pt.pool.ntp.org");
  DateTime.setTimeZone("WEST");
  DateTime.begin();
  if (!DateTime.isTimeValid()) {
    Serial.println("Failed to get time from server.");
  } else {
    Serial.printf("Date Now is %s\n", DateTime.toISOString().c_str());
    Serial.printf("Timestamp is %ld\n", DateTime.now());
 }
}


void setup() {
  // Initialize Serial
  Serial.begin(9600);

  // Initialize WiFi
  WiFi.begin(ssid, password);
  while (WiFi.status() != WL_CONNECTED) {
    delay(1000);
    Serial.println("Connecting to WiFi...");
  }
  Serial.println("Connected to WiFi");

  setupDateTime();

  // Initialize the display
  display.init();
  display.setTextColor(GxEPD_BLACK);
  display.setFont(&FreeMonoBold9pt7b);
  display.setFullWindow();
  display.firstPage();


  time_t oneday = 60 * 60 * 24;
  time_t curDate = DateTime.now(); 
 

  do {
    display.fillScreen(GxEPD_WHITE);
    // Make Grid
    size_t step = 48;
    for (size_t h = 8; h <= 21; h++)
    {
      display.setCursor(7, step);
      display.fillRect(48, step-20, 800, 1, GxEPD_RED);
      //Show hours
      if (h < 10) {
      display.printf(" %d",h);
      } else {
        display.print(h);
      }
      step += 31;
    }
 
    step = 60;
    String days[7] = {"Dom", "Seg", "Ter", "Qua", "Qui", "Sex", "Sab"};
    //Show day
    for (size_t nd = 0; nd < 7; nd++) 
    {
      tm *ltm = localtime(&curDate);
      
      display.setCursor(step, 20);
      display.printf("%s %d", days[ltm->tm_wday], ltm->tm_mday);
      
      step += 110;
      curDate += oneday;
      DateTime.setTime(curDate);
    }

    //Create columns 
    //display.fillRect(20, 40, 800, 1, GxEPD_BLACK);
    display.fillRect(40, 25, 2, 470, GxEPD_BLACK);
    step = 150;
    for (size_t i = 0; i < 7; i++)
    {
        display.fillRect(step, 20, 2, 470, GxEPD_RED);
        step += 110;
    }
  }
  while (display.nextPage());
}

void loop() {
  // Nothing to do here
}