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
      setBacklight(YELLOW);
      break;
    case BTN_DOWN:
      setBacklight(RED);
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
  print("");
  if(_car.pedal == 0) {
    print("Park");
  } else if(_car.pedal == -1) {
    print("Rev ");
  } else {
    print("For ");
  }
  print(" L:");
  print(_car.motorDriveL.speed);
  print(" R:");
  print(_car.motorDriveR.speed);
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
  print("TH:");
  print(_car.remoteThrottle);
  print(" ST:");
  print(_car.remoteSteer);
  print(" E:");
  print(_car.estop);
  print("           ");
  setCursor(0,1);
  print("M:");
  print(_car.mode);
  print(" L:" );
  print(_car.remoteLKnob);
  print(" R:");
  print(_car.remoteRKnob);
  print("            ");
}