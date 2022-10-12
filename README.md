# Powerwheels_Controller

Software for a custom Arduino Mega based controller for a kids ride on vehicle. 
The controller ties directly to the vehicle's wiring harness and uses existing switches (power, forward, reverse, pedal, and hi/low speed). 

All of the vehicle's typical actions happen within the Car class. Remote overrides are passed into the class which allows the vehicle's typical operation to be altered.

The remote control overrides happen through a 6 channel transmitter with the following features:
* Speed/Steering Override
* Top Speed and Acceleration Remote Adjustments
* Remote Vehicle Disable/EStop

## Software Features to Add
* Position based steering in place of speed based.
* Further development of the UIM (User interface Module) class 
   * Display of menus noting the vehicles current stats
   * Allow car properties to be changed in vehicle
   * Actively display current car speed in real time
   * Show Car's top speed from within the vehicle
   * Note when the car is in override or has any errors
* EEPROM saved settings to be reloaded on boot
* Automatic emergency braking with hardware additions.

## Recent Additions
* Independent control of the rear wheels based on how the steering wheel is turned. [Pull Request](https://github.com/etogburn/Powerwheels_Controller/pull/2)
* Thermistors implemented on all motors. Vehicle disabled when motors are over temperature. [Pull Request](https://github.com/etogburn/Powerwheels_Controller/pull/1)
* Implementation of a PPM signal stream utilizing [PPM-Reader](https://www.arduino.cc/reference/en/libraries/ppm-reader/) library to clean up the channel reading and make it more efficient.
   * This required a hardware change to a RC reciever that has PPM signal output. See the Hardware section for more details.
* Improved status screens and screen updating while the vehicle is being operated.

## Software to Clean up
* A clean solution for handling and not duplicating the variables required for display on, and modification from the UIM.
   * A likely implementation here will include a class specific for "Screens" or "Menu Items".

## Hardware
The hardware is comprised of the following:
* [Arduino Mega 2560R3](https://store-usa.arduino.cc/products/arduino-mega-2560-rev3?selectedStore=us)
* [Arduino Mega Prototype Shield](https://www.adafruit.com/product/192)
* 3 x [IBT_2 (BTS7960B Motor Drivers)](https://www.amazon.com/HiLetgo-BTS7960-Driver-Arduino-Current/dp/B00WSN98DC/ref=sr_1_3?crid=1VQW8QXXNJXIU&keywords=IBT_2&qid=1652839442&sprefix=ibt_%2Caps%2C373&sr=8-3)
* [FlySky FS-G7P remote and FS-R7P Reciever](https://www.flysky-cn.com/g7pdescription)
* [Adafruit I2C RGB LCD Shield with Buttons](https://www.adafruit.com/product/714)
* [HT-NTC100K 3950 Thermistor](https://www.amazon.com/gp/product/B07F3SMRLJ/ref=ppx_yo_dt_b_asin_title_o04_s00?ie=UTF8&psc=1)

### Hardware Todo
* Add potentiometer to the main steering shaft.
* Add ultrasonic range finding sensors to stop the vehicle automatically and safely.
* Add motor current feedback from the motor drivers.
* Add Hall effect pedal for granular speed control from within the vehicle
* Create a PCB to neatly pack the components and potentially add further features.
