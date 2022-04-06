
char weekdown[4]="0";
char weekup[4]="0";
char livedown[8]="0";
char clients[4]="0";
int hourBytesArray[]={0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0};
int dayBytesArray[]={0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0};
int monthBytesArray[]={0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0};
int graph=1;
long nextGraphRotate = -10000;

//MQTT server publishes bandwidth stats per minute, hour and day, each series gets displayed as a graph

void displayHourGraph(int xOffset) {
  for (int x=xOffset; x<(xOffset+60); x++) {
    int megs=hourBytesArray[x-xOffset];
    //Serial.print(x-xOffset);
    //Serial.print(":");
    //Serial.print(megs);
    //Serial.print(",");
    for (int y = 0; y < megs && y < 15; y++) {
      display.drawPixel(x+1,  31 - y, rgbColorBrightness(211, 230, 255, brightness));
    }
  }
  //Serial.println("");
}

void displayDayGraph(int xOffset) {
  int x=xOffset+7;
  for (int i=0; i<24; i++) {
    int gigs=dayBytesArray[i];
    for (int y = 0; y < gigs && y < 15; y++) {
        display.drawPixel(x,  31 - y, rgbColorBrightness(125, 200, 255, brightness));
        display.drawPixel(x+1,  31 - y, rgbColorBrightness(125, 200, 255, brightness));
    }
    x=x+2;
  }
}

void displayMonthGraph(int xOffset) {
  int x=xOffset;
  for (int i=0; i<31; i++) {
    int gigs=monthBytesArray[i];
    for (int y = 0; y < gigs && y < 15; y++) {
        display.drawPixel(x,  31 - y, rgbColorBrightness(250, 150, 150, brightness));
        display.drawPixel(x+1,  31 - y, rgbColorBrightness(250, 150, 150, brightness));
    }
    x=x+2;
  }
}

void displayNetworkGraph(int nowSecs){

  //Rotate continuously through the 3 types of network graph
  if (nowSecs  > nextGraphRotate) {
      nextGraphRotate = nowSecs+graphRotateDelay;
      graph++;
      if (graph==4){
        graph=1;
      }
  }
  
  int xOffset=networkOffset;
  display.fillRect(xOffset,16,64,16,display.color565(0, 0, 0));  
  
  if (graph==1){
    displayHourGraph(xOffset);
  } else if (graph==2){
    displayDayGraph(xOffset);
  } else if (graph==3){
    displayMonthGraph(xOffset);
  }
  display.show();
    
}

//Show most recent upload, download and client numbers, also the most recent live bandwidth usage

boolean displayNetworkStats(){

  int xOffset=networkOffset;

  display.fillRect(xOffset,0,64,16,display.color565(0, 0, 0));
  display.setTextSize(1);
  
  // Draw the download icon and stats
  drawIcon(xOffset+5, 1, 5, 7, down_icon);
  display.setCursor(xOffset+11,1);
  display.setTextColor(rgbColorBrightness(250, 80, 0, brightness));
  display.print(weekdown);

  // Draw the upload icon and stats
  drawIcon(xOffset+54, 1, 5, 7, up_icon);
  display.setTextColor(rgbColorBrightness(125, 200, 255, brightness));
  drawRightAlignedText(xOffset+54, 1, weekup);

  // Draw the client icon and stats
  drawIcon(xOffset+5, 9, 7, 7, client_icon);
  display.setCursor(xOffset+14,9);
  display.setTextColor(rgbColorBrightness(34,77,23, brightness));
  display.print(clients);

  //Draw the live bandwidth
  display.setTextColor(rgbColorBrightness(110, 10, 204, brightness));
  drawRightAlignedText(xOffset+59, 9, livedown);
  display.show();

  return true;
}
