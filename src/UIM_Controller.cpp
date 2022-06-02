#include "UIM_Controller.h"

UIM_Controller::UIM_Controller(String startMsg) {
  _startMessage = startMsg;
};

void UIM_Controller::Begin() {
  clear();
  begin(16, 2);
}

void UIM_Controller::HandleEvents(CarStats car) {
  _car = car;
  long now = millis();

  if(_scrollScreens && now > _lastScreenChange + TIME_SCREEN_CHANGE) {
    _screenCount++;
    _lastScreenChange = now;
    clear();
  }

  if(now > _lastLCDUpdate + TIME_LCD_UPDATE) {
    SetScreen();
    setBacklight(TEAL);
    _lastLCDUpdate = now;
  }
  
  if(now > _lastButtonRead + TIME_BUTTON_READ) {
    ReadButtons();
    _lastButtonRead = now;
  }
  
}

void UIM_Controller::SetScreen() {
  if(_screenCount < 1 && _bootScreen) {
    SetScreenWelcome();
  } else {
    _bootScreen = false;
    switch(_screenCount % NUM_OF_SCREENS) {
    case 0:
      SetScreenInCar();
      break;
    case 1:
      SetScreenRemote();
      break;
    }
  }
}

void UIM_Controller::ReadButtons() {
  uint8_t buttons = readButtons();

  for(uint8_t i = 0; i < BTN_NUMBER; i++) {
    if (Btn[i].IsPressed(buttons)) {
      ButtonPressed(i);
    }
  }
  
}

void UIM_Controller::ButtonPressed(uint8_t button) {
  switch(button) {
    case BTN_UP:
      //setBacklight(YELLOW);
      break;
    case BTN_DOWN:
      //setBacklight(RED);
      break;
    case BTN_LEFT:
      //setBacklight(GREEN);
      _screenCount++;
      _scrollScreens = false;
      break;
    case BTN_RIGHT:
      //setBacklight(TEAL);
      _screenCount--;
      _scrollScreens = false;
      break;
    case BTN_SELECT:
      //setBacklight(VIOLET);
      _scrollScreens = true;
      break;
    default:
      break;
  }
}

void UIM_Controller::SetScreenWelcome() {
  setCursor(0,0);
  print(_startMessage);
  setCursor(0,1);
  print(VERSION);
}

void UIM_Controller::SetScreenInCar() {
  home();
  if(_car.remote.estop) {
    print("STOP");
  } else if(_car.remote.steer != 0 || _car.remote.throttle != 0) {
    print("Remo");
  } else if(_car.speed == 0) {
    print("Park");
  } else if(_car.pedal == -1) {
    print("Rev ");
  } else if(_car.pedal == 1) {
    print("For ");
  }
  print(" MOT:");
  if(abs(_car.speed) < 10) {
    print("  ");
  } else if(abs(_car.speed) < 100) {
    print(" ");
  }
  print(abs(_car.speed));
  print("         ");
  setCursor(0,1);
  print("L:");
  print(_car.motorDriveL.temp);
  print(" R:");
  print(_car.motorDriveR.temp);
  print(" S:");
  print(_car.motorSteer.temp);
  print("  ");
}

void UIM_Controller::SetScreenRemote() {
  home();
  // print("TH:");
  // print(_car.remote.throttle);
  // print(" ST:");
  // print(_car.remote.steer);
  print(" E:");
  print(_car.remote.estop);
  print("4:");
  print(_car.remote.channel4);
  print(" 5:" );
  print(_car.remote.channel5);
  print("           ");
  setCursor(0,1);
  print(" 6:");
  print(_car.remote.channel6);
  print(" 7:");
  print(_car.remote.channel7);
  print("            ");
}