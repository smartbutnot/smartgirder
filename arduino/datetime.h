int thisHour = 0;

// Every hour (or 20s after startup) sync the local clock with the NTP client
void sync_clock(void)
{
  if ((now() > syncTimeout))
  {
    Serial.println("Update local clock");
    setTime(timeClient.getUTCEpochTime());
    syncTimeout = now() + 3600;

    time_t utc = now();
    time_t local = myTZ->toLocal(utc, &tcr);
    Serial.print(hour(local));
    Serial.print(":");
    Serial.print(minute(local));
    Serial.print(" ");
    Serial.print(dayShortStr(weekday(local)));
    Serial.print(" ");
    Serial.print(day(local));
    Serial.print("/");
    Serial.println(month(local));
  }
}

//Show the day of week, date and time

boolean displayClock() {

  time_t utc = now();
  time_t local = myTZ->toLocal(utc, &tcr);

  /*
    Serial.print(hour(local));
    Serial.print(":");
    Serial.print(minute(local));
    Serial.print(" ");
    Serial.print(dayShortStr(weekday(local)));
    Serial.print(" ");
    Serial.print(day(local));
    Serial.print("/");
    Serial.println(month(local));
  */

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
  display.setCursor(xOffset + 32 + 18, 23);
  if (minute(local) < 10) {
    display.print("0");
    display.setCursor(xOffset + 32 + 24, 23);
  }
  display.print(minute(local));

  display.show();

  return true;
}

//If we detect an Alexa timer, display it as a countdown, then flash 00:00 and return to the previous panel (currently overwrites the TFL panel)

void displayTimer() {

  int xOffset = timerOffset;
  long secsRemain = timerEpoch - now();

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
