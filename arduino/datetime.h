int thisHour = 0;


void printDateTime(time_t t, const char *tz)
{
  char buf[32];
  char m[4];    // temporary storage for month string (DateStrings.cpp uses shared buffer)
  strcpy(m, monthShortStr(month(t)));
  sprintf(buf, "%.2d:%.2d:%.2d %s %.2d %s %d %s",
          hour(t), minute(t), second(t), dayShortStr(weekday(t)), day(t), m, year(t), tz);
  Serial.println(buf);
}

//We register this function with NTPClient_Generic so it tells us when we successfully update
void NTPcallback(NTPClient *c){
  time_t ep=c->getUTCEpochTime();
  time_t local = myTZ->toLocal(ep, &tcr);
  Serial.print("NTP update received: ");
  printDateTime(local,tcr -> abbrev);
  ntpForce=now()+3600;
}


//Show the day of week, date and time

boolean displayClock() {


  time_t utc=timeClient.getUTCEpochTime();
  time_t local = myTZ->toLocal(utc, &tcr);
  //printDateTime(local, tcr -> abbrev);

  thisHour = hour(local);

  if ((thisHour == dimHour) and not (brightness == dimBrightness)) {
    brightness = dimBrightness;
    forceRefresh = true;
  }
  else if ((thisHour == brightHour) and not (brightness == brightBrightness)) {
    brightness = brightBrightness;
    forceRefresh = true;;
  }

  int xOffset = clockOffset;
  display.fillRect(xOffset + 32, 0, 32, 32, display.color565(0, 0, 0));
  display.setTextSize(1);

  int dayPos = (32 - (strlen(dayShortStr(weekday(local))) * 6)) / 2;
  display.setCursor(xOffset + 32 + dayPos, 3);
  display.setTextColor(rgbColorBrightness(125, 200, 255, brightness));
  display.print(dayShortStr(weekday(local)));

  display.setCursor(xOffset + 32, 13);
  if (day(local) < 10) {
    display.print("0");
    display.setCursor(xOffset + 32 + 6, 13);
  }
  display.print(day(local));

  display.setCursor(xOffset + 32 + 12, 13);
  display.print("/");
  display.setCursor(xOffset + 32 + 18, 13);
  if (month(local) < 10) {
    display.print("0");
    display.setCursor(xOffset + 32 + 24, 13);
  }
  display.print(month(local));


  display.setCursor(xOffset + 32, 23);
  display.setTextColor(rgbColorBrightness(250, 80, 0, brightness));
  if (hour(local) < 10) {
    display.print("0");
    display.setCursor(xOffset + 32 + 6, 23);
  }
  display.print(thisHour);
  display.setCursor(xOffset + 32 + 12, 23);
  display.print(":");
  display.setTextColor(rgbColorBrightness(250, 80, 0, brightness));
  display.setCursor(xOffset + 32 + 18, 23);
  if (minute(local) < 10) {
    display.print("0");
    display.setCursor(xOffset + 32 + 24, 23);
  }
  display.print(minute(local));
  if (now()>ntpForce){
    //Somehow we've failed to keep the NTP signal up to date. Display a warning pixel.
    display.fillRect(257, 0, 1, 1, display.color565(255, 0, 0));
    display.show();
    if (now()>(ntpForce+600)){
      display.fillRect(257, 0, 2, 2, display.color565(255, 0, 0));
      display.show();
      //Still out of time sync, so try resetting stuff.
      //WiFi.disconnect();
      //WiFi.end();
      //WiFiStatus = WL_IDLE_STATUS;
      bool forceOK=timeClient.forceUpdate();
      if (forceOK){
        display.fillRect(257, 0, 2, 2, display.color565(0, 255, 0));
        display.show();
      }
      ntpForce=now()+600;
    }
  }
  display.show();

  return true;
}

//If we detect an Alexa timer, display it as a countdown, then flash 00:00 and return to the previous panel (currently overwrites the TFL panel)

void displayTimer() {

  int xOffset = timerOffset;
  long secsRemain = timerEpoch - timeClient.getUTCEpochTime();

  //Display a timer if it's above 0 and below 100 minutes (too many digits at this size otherwise and don't want to show a timer all day long)
  if (secsRemain > 0 and secsRemain < 6000) {

    display.fillRect(xOffset, 0, 64, 32, display.color565(0, 0, 0));
    int minRemain = secsRemain / 60;
    int secRemain = secsRemain - (minRemain * 60);
    Serial.print("Timer: ");
    Serial.print(minRemain);
    Serial.print(":");
    Serial.println(secRemain);

    display.setCursor(xOffset + 2, 8);
    display.setTextSize(2);
    display.setTextColor(rgbColorBrightness(250, 250, 250, brightness));
    if (minRemain < 10) {
      display.print("0");
      display.setCursor(xOffset + 14, 8);
      display.print(minRemain);
    }
    else {
      display.print(minRemain);
    }
    display.setCursor(xOffset + 26, 8);
    display.print(":");
    display.setCursor(xOffset + 38, 8);
    if (secRemain < 10) {
      display.print("0");
      display.setCursor(xOffset + 50, 8);
      display.print(secRemain);
    }
    else {
      display.print(secRemain);
    }
    
    display.show();

  }
  else {
    Serial.println("Timer end.");
    timerMode = false;
    display.fillRect(xOffset, 0, 64, 32, display.color565(0, 0, 0));
    for (int i = 0; i < 4; i++) {
      display.setTextSize(2);
      display.setCursor(xOffset + 2, 8);
      display.setTextColor(rgbColorBrightness(250, 250, 250, brightness));
      display.print("00:00");
      display.show();
      delay(500);
      display.fillRect(xOffset, 0, 64, 32, display.color565(0, 0, 0));
      display.show();
      delay(500);
    }
    forceRefresh = true;
  }


}
