//Shows the next 4 trains scheduled at the local station, MQTT values get read in from api.tfl.gov.uk

char trainArray[][30] = {"0", "", "", "", "", "", "", ""};

void displayTFL() {

  int xOffset=tflOffset;
  display.fillRect(xOffset,0,64,32,display.color565(0, 0, 0));

  //Serial.println("Display trains.");
  display.setTextSize(1);
  for (int i=0;i<8;i=i+2){
    display.setCursor(xOffset+1,i*4);
    display.setTextColor(rgbColorBrightness(255, 80, 0, brightness));
    int timelen=strlen(trainArray[i]);
    display.print(trainArray[i]);
    display.setCursor((strlen(trainArray[i])*7)+1+xOffset,i*4);
    display.setTextColor(rgbColorBrightness(250, 250, 250, brightness));
    int stationlen=strlen(trainArray[i+1]);
    int totallen=stationlen+timelen;
    if (totallen>9) {
      trainArray[i+1][10-timelen]=0;
    }
    display.print(trainArray[i+1]);
  }
  display.show();


 }
