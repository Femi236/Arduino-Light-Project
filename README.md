# Arduino Light Porject

This is my experience in attempting to use an Arduino UNO to control LED lights to react to the music being played. This idea was heavily inspired by [Devon Crawford](https://youtu.be/lU1GVVU9gLU) and this [MakeUseOf article](https://www.muo.com/tag/connect-led-light-strips-arduino/).

The project was completed before the GitHub Repository was created so this is just a dump of all of the files.

## How To Use

The final working copy is under [/Versions/light-project-8.0](.\Versions\light-project-8.0). Upload it to the Arduino UNO after building the circuit and installing the libraries.

### Required Materials

- Arduino UNO (preferrably the [starter kit](https://www.dennisdeal.com/products/super-starter-kit-voor-arduino-uno-mega-2560-met-tutorial?utm_source=google&utm_medium=cpc&utm_campaign=gss&gclid=CjwKCAiA65iBBhB-EiwAW253W9yrQtyoewKMIdYrPubImicIgMZe9Zb9K7zWoDYO1ggqun1bB0xE1BoChAcQAvD_BwE))
- 1 x Bread Board
- 1 x WS2811/12/12B 5v LED strip
- 1 x 220-440 Ohm Resistor
- 1 x 100-1000 microFarad Capacitor
- 1 x 5V Power Supply
- 1 x IR Receiver
- 1 x IR Remote controls
- Jumper Wires

### Required Libraries

- FastLED.h
- IRremote.h
