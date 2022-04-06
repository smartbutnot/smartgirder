# smartgirder
My RGB matrix smart girder project using the Adafruit Matrixportal and Arduino

![alt text](https://github.com/smartbutnot/smartgirder/blob/main/girder.jpg?raw=true)

This is based on the [Adafruit Matrix Portal](https://www.adafruit.com/product/4745), you will also need:

1. Some RGB matrixes from your favourite online supplier (they're crazy cheap from Ali Express, less so from elsewhere). This is 4 64x32 panels in a chain.
2. A 5v power supply - conventional wisdom would be at least 10a, but I'm running all this on a self-contained little 3a plug because I'm not lighting up that many pixels
3. Some wiring to connect 1 and 2 together
4. Some [arcylic diffusers](https://learn.adafruit.com/weather-display-matrix/led-matrix-diffuser) to make the display look nicer
5. An MQTT server to serve up the values to display. I haven't put up the code for that because it's a mess of cron scripts and all sorts, but a lot of it can be published straight out of [home assistant](https://www.home-assistant.io/).
