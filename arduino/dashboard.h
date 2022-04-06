
char homeTemp[5] = "";
char homeHumidity[5] = "";
char plantMoisture[5] = "";
char rainChance[5] = "";
char airQuality[5] = "";
char carCharge[5] = "";
char flightDetails[30] = "";
char flightCallsign[12];
char flightAircraft[12];
char flightRoute[12];
char carPostcode[7] = "";

unsigned long flightTime = 0;
boolean showFlight = false;
boolean showCar = false;
boolean heating = false;

int colOneStart=dashOffset+1;
int colOneEnd=dashOffset+29;
int colTwoStart=dashOffset+34;
int colTwoEnd=dashOffset+63;

void displayDashboard() {

  //Serial.print("Data update: temp=");
  //Serial.print(homeTemp);
  //Serial.print(", humidity=");
  //Serial.print(homeHumidity);
  //Serial.print(", plantM=");
  //Serial.print(plantMoisture);
  //Serial.print(", rain chance=");
  //Serial.print(rainChance);
  //Serial.print(", air qual=");
  //Serial.print(airQuality);
  //Serial.print(", car charge=");
  //Serial.print(carCharge);
  //Serial.println(".");

  display.fillRect(dashOffset, 8, 64, 24, display.color565(0, 0, 0));
  display.setTextSize(1);

  // Draw the house icon and stats
  if (heating) {
    drawIcon(colOneStart, 8, 7, 7, house_heat);
  } else {
    drawIcon(colOneStart, 8, 7, 7, house);
  }
  display.setTextColor(rgbColorBrightness(255, 255, 255, brightness));
  drawRightAlignedText(colOneEnd, 9, homeTemp);

  // Draw the plant icon and stats
  drawIcon(colOneStart, 17, 7, 7, plant);
  display.setTextColor(rgbColorBrightness(255, 255, 255, brightness));
  drawRightAlignedText(colOneEnd, 17, plantMoisture);

  // Draw the air icon and stats
  drawIcon(colOneStart, 25, 7, 7, air);
  display.setTextColor(rgbColorBrightness(255, 255, 255, brightness));
  drawRightAlignedText(colOneEnd, 25, airQuality);

  // Draw the droplet icon and stats
  drawIcon(colTwoStart+1, 8, 8, 8, droplet);
  display.setTextColor(rgbColorBrightness(255, 255, 255, brightness));
  drawRightAlignedText(colTwoEnd, 9, homeHumidity);

  // Draw the cloud icon and stats
  drawIcon(colTwoStart, 17, 8, 8, small_cloud);
  display.setTextColor(rgbColorBrightness(255, 255, 255, brightness));
  drawRightAlignedText(colTwoEnd, 17, rainChance);

  // Draw the car icon and stats
  drawIcon(colTwoStart, 24, 8, 8, car);
  display.setTextColor(rgbColorBrightness(255, 255, 255, brightness));
  drawRightAlignedText(colTwoEnd, 25, carCharge);

  display.show();
}


void clearFlight() {

  display.fillRect(dashOffset, 0, 64, 8, display.color565(0, 0, 0));
  display.show();

}

void displayFlightString(char* flightString) {

  //Serial.print("Display flight: ");
  //Serial.println(flightString);
  display.fillRect(dashOffset, 0, 64, 8, display.color565(0, 0, 0));
  display.setTextSize(1);

  // Draw the plane icon and stats
  drawIcon(colOneStart-1, 1, 8, 8, plane2);
  display.setCursor(colOneStart + 16, 1);
  display.setTextColor(rgbColorBrightness(255, 255, 255, brightness));
  display.print(flightString);
  display.show();

}

void displayCarLocation() {

  display.fillRect(dashOffset, 0, 64, 8, display.color565(0, 0, 0));
  display.setTextSize(1);

  // Draw the car icon and stats
  drawIcon(colOneStart-1, 0, 8, 8, car);
  display.setCursor(colOneStart + 16, 0);
  display.setTextColor(rgbColorBrightness(255, 255, 255, brightness));
  display.print(carPostcode);

  display.show();

}

void clearCar() {


  display.fillRect(dashOffset, 24, 0, 8, display.color565(0, 0, 0));
  display.show();

}

void displayFlight(int nowSecs){
  if(showFlight and nowSecs>flightTime){
    Serial.println("Reset flight status");
    clearFlight();
    showFlight=false;
  } 
  else if(showFlight and nowSecs>(flightTime-10)) {
    displayFlightString(flightRoute);
  }
  else if(showFlight and nowSecs>(flightTime-20)) {
    displayFlightString(flightAircraft);
  }
  else if(showFlight and nowSecs>(flightTime-30)) {
    displayFlightString(flightCallsign);
  }
}
