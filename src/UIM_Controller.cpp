#include "UIM_Controller.h"


UIM_Controller::UIM_Controller() {
};

void UIM_Controller::Begin() {
  begin(16, 2);
  clear();
}

void UIM_Controller::HandleEvents(CarStats car) {
  _car = car;
  long now = millis();

  if(now > _lastScreenChange + TIME_SCREEN_CHANGE) {
    if(_bootScreen && _screenCount == 0) {
      _bootScreen = false;
    } else {
      _scrollScreens ? _screenCount++ : 0;
    }
    _lastScreenChange = now;
    // clear();
  }

  if(now > _lastLCDUpdate + TIME_LCD_UPDATE) {
    SetScreen();
    BacklightController();
    _lastLCDUpdate = now;
  }
  
  if(now > _lastButtonRead + TIME_BUTTON_READ) {
    ReadButtons();
    _lastButtonRead = now;
  }
}

void UIM_Controller::SetScreen() {
  static int16_t lastScreenCount;
  bool writeStaticText = (_screenCount != lastScreenCount);
  if(_screenCount < 1 && _bootScreen) {
    SetScreenWelcome();
    lastScreenCount = -1;
  } else {
    switch(_screenCount % NUM_OF_SCREENS) {
    case 0:
      SetScreenTemps(writeStaticText);
      break;
    case 1:
      SetScreenRemoteMain(writeStaticText);
      break;
    case 2:
      SetScreenRemoteAux(writeStaticText);
      break;
    }
    lastScreenCount = _screenCount;
  }
}

void UIM_Controller::BacklightController() {
  if(_car.remote.estop || _car.overTemp) {
    setBacklight(RED);
  } else {
    setBacklight(TEAL);
  }
}

void UIM_Controller::ReadButtons() {
  uint8_t buttons = readButtons();

  for(uint8_t i = 0; i < BTN_NUMBER; i++) {
    if (Btn[i].IsPressed(buttons)) {
      ButtonPressed(i);
      delay(250);
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
  print(START_MESSAGE);
  setCursor(0,1);
  print(VERSION);
}

void UIM_Controller::SetScreenTemps(bool writeStaticText) {
  if(writeStaticText) {
    createChar_progmem(DEGREE_CHAR, degreeChar);
    setCursor(0,1);
    print("L:  ");
    write(DEGREE_CHAR);
    print(" R:  ");
    write(DEGREE_CHAR);
    print(" S:  ");
    write(DEGREE_CHAR);
    print("  ");
  }

  SetScreenMainBanner(0);

  setCursor(2,1);
  print(_car.motorDriveL.temp);
  setCursor(8,1);
  print(_car.motorDriveR.temp);
  setCursor(14,1);
  print(_car.motorSteer.temp);
}

void UIM_Controller::SetScreenRemoteAux(bool writeStaticText) {
  if(writeStaticText) {
    home();
    print("4:     5:        ");
    setCursor(0,1);
    print("6:     7:         ");
  }
  setCursor(2,0);
  PrintVal(_car.remote.channel4, 4);
  setCursor(9,0);
  PrintVal(_car.remote.channel5, 4);
  setCursor(2,1);
  PrintVal(_car.remote.channel6, 4);
  setCursor(9,1);
  PrintVal(_car.remote.channel7, 4);
}

void UIM_Controller::SetScreenRemoteMain(bool writeStaticText) {
  if(writeStaticText) {
    home();
    print("TH:     ST:        ");
    setCursor(0,1);
    print("EStop:           ");
  }
  setCursor(3,0);
  PrintVal(_car.remote.throttle, 4, true);
  setCursor(10,0);
  PrintVal(_car.remote.steer, 4, true);
  
  setCursor(6,1);
  if(_car.remote.estop) {
    print("Active    ");
  } else {
    print("Inactive  ");
  }
}

void UIM_Controller::PrintVal(int16_t val, uint8_t maxDigits, bool hasSign) {
  int16_t tempVal = abs(val);
  uint8_t valDigits = 1;

  val < 0 || hasSign ? valDigits++ : 0;

  while(tempVal >= 10) {
    tempVal/=10;
    valDigits++;
  }

  if(valDigits > maxDigits) {
    return;
  }

  valDigits -= hasSign ? 1 : 0;
  
  for(uint8_t i = valDigits; i < maxDigits; i++) {
    if(hasSign) {
      if(val > 0) {
        print("+");
      } else if(val <= 0) {
        print("-");
      }
    } else {
      // print(" ");
      print("-");
      // print("0");
    }
  }

  print(hasSign && val < 0 ? val*-1: val);
}

void UIM_Controller::SetScreenMainBanner(uint8_t row) {
  setCursor(0,row);
  if(_car.remote.estop) {
    print("<EMERGENCY STOP>");
  } else if(_car.overTemp) {
    print("Too Hot-Disabled");
  } else {
    if(_car.remote.steer != 0 || _car.remote.throttle != 0) {
      print("Remote Mot:");
    } else if(_car.speed == 0) {
      print("Park   Mot:");
    } else if(_car.pedal == -1) {
      createChar_progmem(BACK_ARROW_CHAR, backArrowChar);
      print("Rev ");
      write(BACK_ARROW_CHAR);
      write(BACK_ARROW_CHAR);
      print(" Mot:");
    } else if(_car.pedal == 1) {
      createChar_progmem(FWD_ARROW_CHAR, fwdArrowChar);
      print("For ");
      write(FWD_ARROW_CHAR);
      write(FWD_ARROW_CHAR);
      print(" Mot:");
    }
    setCursor(11,row);
    PrintVal(abs(_car.speed), 3);
    print("  ");
  }
  
}