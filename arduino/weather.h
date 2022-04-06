//Displays the weather icon and temperature. Many more variables are available. MQTT server pulls from openweathermap.

char weatherIcon[4]="01d";
char temp[4]="";
char tempWithC[5]="";
char temp_max[4]="";
char temp_min[4]="";
char humidity[4]="";
char windspeed[4]="";
char sunset[6]="";
char sunrise[6]="";
char description[100]="";
char padString[12]="           ";

boolean displayWeather() {
  int xOffset=weatherOffset;

  //Serial.println("Display weather");

  display.fillRect(xOffset,0,32,32,display.color565(0, 0, 0));

  if (strcmp(weatherIcon, "01d") == 0) drawIcon(xOffset,0,32,25,sun);
  else if (strcmp(weatherIcon, "01n") == 0) drawIcon(xOffset,0,32,25,moon2);
  else if (strcmp(weatherIcon, "02d") == 0) drawIcon(xOffset,0,32,25,cloud_sun);
  else if (strcmp(weatherIcon, "02n") == 0) drawIcon(xOffset,0,32,25,cloud_moon);
  else if (strcmp(weatherIcon, "03d") == 0 || strcmp(weatherIcon, "03n") == 0 || strcmp(weatherIcon, "04d") == 0 || strcmp(weatherIcon, "04n") == 0) drawIcon(xOffset,0,32,25,cloud);
  else if (strcmp(weatherIcon, "09d") == 0 || strcmp(weatherIcon, "09n") == 0) drawIcon(xOffset,0,32,25,cloud_rain);
  else if (strcmp(weatherIcon, "10d") == 0) drawIcon(xOffset,0,32,25,cloud_sun_rain);
  else if (strcmp(weatherIcon, "10n") == 0) drawIcon(xOffset,0,32,25,cloud_rain);
  else if (strcmp(weatherIcon, "11d") == 0) drawIcon(xOffset,0,32,25,cloud_sun_lightning);
  else if (strcmp(weatherIcon, "11n") == 0) drawIcon(xOffset,0,32,25,cloud_lightning);
  else if (strcmp(weatherIcon, "13d") == 0 || strcmp(weatherIcon, "13n") == 0) drawIcon(xOffset,0,32,25,snow);
  else if (strcmp(weatherIcon, "50d") == 0 || strcmp(weatherIcon, "50n") == 0) drawIcon(xOffset,0,32,25,mist);
  else drawIcon(xOffset,0,32,25,cloud);

  sprintf(tempWithC,"%sC",temp);
  int tempPos=(32-(strlen(tempWithC)*6))/2;
  display.setCursor(xOffset+tempPos,24);
  display.setTextSize(1);
  display.setTextColor(rgbColorBrightness(255, 255, 255, brightness));
  display.print(tempWithC);

  display.show();

  return true;
}
