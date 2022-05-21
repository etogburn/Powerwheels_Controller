# Powerwheels_Controller

Software for a custom Arduino Mega based controller for a kids ride on vehicle. 
The controller ties directly to the vehicle's wiring harness and uses existing switches (power, forward, reverse, pedal, and hi/low speed). 

All of the vehicle's typical actions happen within the Car class. Remote overrides are passed into the class which allows the vehicle's typical operation to be altered.

The remote control overrides happen through a 6 channel transmitter with the following features:
* Speed/Steering Override
* Top Speed and Acceleration Remote Adjustments
* Remote Vehicle Disable/EStop

## Software Features to Add
* Thermistor implementation within the motor class to prevent overheating motors.
   * See [`motor-temperature-sensors`](https://github.com/etogburn/Powerwheels_Controller/tree/motor-temperature-sensors) branch for the beginnings of this implementation.
* Position based steering in place of speed based.
* Independent control of the rear wheels based on how the steering wheel is turned.
  * See [`drive-motor-steering-assist`](https://github.com/etogburn/Powerwheels_Controller/tree/drive-motor-steering-assist) branch for the current working implementation.
* Further development of the UIM (User interface Module) class 
   * Display of menus noting the vehicles current stats
   * Allow car properties to be changed in vehicle
   * Actively display current car speed in real time
   * Show Car's top speed from within the vehicle
   * Note when the car is in override or has any errors
* EEPROM saved settings to be reloaded on boot

## Recent Additions
* Thermistors implemented on all motors. Vehicle disabled when motors are over temperature.

## Software to Clean up
* Remote Channels class 
   * Reduce the need to setup each interrupt individually in the `setupChannels` method and pass a pointer to the class into Remote Control
   * A solution involving the Remote Channels consumed purely inside the Remote Control class is preferred.
* A clean solution for handling and not duplicating the variables required for display on, and modification from the UIM.
   * A likely implementation here will include a class specific for "Screens" or "Menu Items".

## Hardware
The hardware is comprised of the following:
* [Arduino Mega 2560R3](https://store-usa.arduino.cc/products/arduino-mega-2560-rev3?selectedStore=us)
* [Arduino Mega Prototype Shield](https://www.adafruit.com/product/192)
* 3 x [IBT_2 (BTS7960B Motor Drivers)](https://www.amazon.com/HiLetgo-BTS7960-Driver-Arduino-Current/dp/B00WSN98DC/ref=sr_1_3?crid=1VQW8QXXNJXIU&keywords=IBT_2&qid=1652839442&sprefix=ibt_%2Caps%2C373&sr=8-3)
* [DUMBORC X6F 6 Channel RC Reciever](http://www.dumborc.com/en/2019/10/18/dumborc-x6-2-4g-6ch-transmitter/)
* [Adafruit I2C RGB LCD Shield with Buttons](https://www.adafruit.com/product/714)

### Hardware Todo
* Add potentiometer to the main steering shaft.
* Replace current RC reciever with a PPM/iBus version like [this](https://www.flysky-cn.com/ia6b-canshu)
   * This will greatly reduce the overhead required to process the input channels. They operate on 4 external interrupts and 2 pulse measurement functions currently.
* Add motor current feedback from the motor drivers.
* Add Hall effect pedal for granular speed control from within the vehicle
* Create a PCB to neatly pack the components and potentially add further features.
