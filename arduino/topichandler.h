//One big if/else if switching on the MQTT topic name. Usually just takes that value and puts it in the right variable.

void callback(char* topic, byte* payload, unsigned int length) {
  Serial.print("Message arrived on topic: ");
  Serial.print(topic);
  Serial.print(". Message: ");
  
  char payloadAsChars[length];
  for (int i = 0; i < length; i++) {
    payloadAsChars[i]=(char)payload[i];
  }
  payloadAsChars[length]='\0';
  Serial.println(payloadAsChars);
  
  if (strcmp(topic, "network/historic_bandwidth/weekdown/") == 0) {
    strcpy(weekdown,payloadAsChars);
    displayNetworkStats();
  }
  else if (strcmp(topic, "network/historic_bandwidth/weekup/") == 0) {
    strcpy(weekup,payloadAsChars);
    displayNetworkStats();
  }
  else if (strcmp(topic, "network/devices/") == 0) {
    strcpy(clients,payloadAsChars);
    displayNetworkStats();
  }
  else if (strcmp(topic, "network/live_bandwidth") == 0) {
    int k=atoi(payloadAsChars);
    if(k>1024){
      sprintf(livedown, "%d", k/1024);
      strcat(livedown,"M");
    }
    else{
      strcpy(livedown,payloadAsChars);
      strcat(livedown,"K");
    }
    displayNetworkStats();
  }
  else if (strcmp(topic, "sign/delay") == 0) {
    graphRotateDelay=atoi((char*)payload);
  }
  else if (strcmp(topic, "sign/brightness") == 0) {
    brightness=atoi((char*)payload);
    forceRefresh=true;
  }
  else if ((strcmp(topic, "TFL/DLR") == 0) and not timerMode) {
    StaticJsonDocument<256> json;
    deserializeJson(json, payload, length);
    for (int x = 0; x < sizeof(trainArray) / sizeof(trainArray[0]); x++)
    {
      trainArray[x][0] = '\0';
    }
    int i=0;
    for (JsonVariantConst v : json.as<JsonArray>()) {
      char traintime[10]="";
      strcpy(traintime,v["time"]);
      char traindest[30]="";
      strcpy(traindest,v["destination"]);
      strcpy(trainArray[i],v["time"]);
      strcpy(trainArray[i+1],v["destination"]);
      i=i+2;
    }
    displayTFL();
  }
  else if (strncmp(topic, "network/historic_bandwidth/days",31) == 0) {
    char *last = strrchr(topic, '/');
    if (last != NULL) {
        //Get the last token for the index, and divide the gig value by 2 for a better layout
        monthBytesArray[atoi(last+1)]=(atoi(payloadAsChars)/2);
    }
  }
  else if (strncmp(topic, "network/historic_bandwidth/minutes",34) == 0) {
    char *last = strrchr(topic, '/');
    if (last != NULL) {
        //We get megs per minute and the bar height is 15 pixels. So MB/10 is a good bet.
        int minvalue=(atoi(payloadAsChars))/10;
        //Serial.println(minvalue);
        //Serial.println(last+1);
        hourBytesArray[atoi(last+1)]=minvalue;
    }
  }
  else if (strncmp(topic, "network/historic_bandwidth/hours",32) == 0) {
    char *last = strrchr(topic, '/');
    if (last != NULL) {
        dayBytesArray[atoi(last+1)]=atoi(payloadAsChars);
    }
  }
  else if (strcmp(topic, "weather/weather") == 0){
    StaticJsonDocument<266> json;
    deserializeJson(json, payload, length);
    strcpy(weatherIcon, json["icon"]);
    strcpy(description, json["description"]);
    strcpy(temp, json["temp"]);
    strcpy(temp_max, json["temp_max"]);
    strcpy(temp_min, json["icon"]);
    strcpy(humidity, json["humidity"]);
    strcpy(windspeed, json["windspeed"]);
    strcpy(sunrise, json["sunrise"]);
    strcpy(sunset, json["sunset"]);
    displayWeather();
  }
  else if ((strcmp(topic, "alexa/timer") == 0) and not timerMode){
    timerEpoch=atol(payloadAsChars);
    long checkSecs=timerEpoch-now();
    if(checkSecs>5 and checkSecs<(24*60*60)){
      Serial.print("Good timer: ");
      Serial.print(checkSecs);
      Serial.print(" remaining. ");
      timerMode=true;
    } 
  }
  else if (strcmp(topic, "planes/callsign") == 0){
    strcpy(flightCallsign,payloadAsChars);
  }
  else if (strcmp(topic, "planes/aircraft") == 0){
    strcpy(flightAircraft,payloadAsChars);
  }
  else if (strcmp(topic, "planes/route") == 0){
    strcpy(flightRoute,payloadAsChars);
    flightTime=now()+30;
    showFlight=true;
  }
  else if (strcmp(topic, "car/location") == 0){
    if(strcmp(payloadAsChars,"home") == 0){
      if(showCar){
        Serial.println("Clear car mode");
        clearFlight();
        showCar=false;
      }
    }
    else{
      Serial.print("Car mode: ");
      Serial.println(carPostcode);
      showCar=true;
      displayCarLocation();
    }
  }
    else if (strcmp(topic, "car/battery") == 0){
    strcpy(carCharge,payloadAsChars);
    displayDashboard();
  }
  else if (strcmp(topic, "car/postcode") == 0){
    Serial.println("Car postcode");
    strcpy(carPostcode,payloadAsChars);
  }
  else if (strcmp(topic, "thermostat/temperature") == 0){
    strcpy(homeTemp,payloadAsChars);

    displayDashboard();
  }
  else if (strcmp(topic, "thermostat/humidity") == 0){
    strcpy(homeHumidity,payloadAsChars);
    displayDashboard();
  }
  else if (strcmp(topic, "thermostat/state") == 0){
    if (strcmp(payloadAsChars, "heating") == 0){
      heating=true;
    } else {
      heating=false;
    }
    displayDashboard();
  }
  else if (strcmp(topic, "plants/moisture") == 0){
    strcpy(plantMoisture,payloadAsChars);
    displayDashboard();
  }
  else if (strcmp(topic, "weather/rain_chance") == 0){
    strcpy(rainChance,payloadAsChars);
    displayDashboard();
  }
  else if (strcmp(topic, "weather/air quality") == 0){
    strcpy(airQuality,payloadAsChars);
    displayDashboard();
  }


}
