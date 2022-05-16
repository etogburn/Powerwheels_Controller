#include "UIM_Controller.h"

UIM_Controller::UIM_Controller(String startMsg) {
  _startMessage = startMsg;
};

void UIM_Controller::Begin() {
  clear();
  begin(16, 2);
}

void UIM_Controller::HandleEvents() {
  //bool screen1[3];
  long now = millis();

  // if(now > _lastLCDUpdate + TIME_LCD_UPDATE) {
  //   switch(_currentScreen) {
  //   case 0:
  //     SetScreenWelcome();
  //     break;
  //   case 1:
  //     SetScreenPedals(screen1);
  //     break;
  //   case 2:
  //     break;
  //   }
  //   SetScreen(_currentScreen);
  //   _lastLCDUpdate = now;
  // }
  
  if(now > _lastButtonRead + TIME_BUTTON_READ) {
    ReadButtons();
    _lastButtonRead = now;
  }
  
}



void UIM_Controller::SetScreen(uint8_t screen) {
  if(screen >= 0 && screen < NUM_OF_SCREENS) {
    _currentScreen = screen;
  } else if (screen >= NUM_OF_SCREENS) {
    _currentScreen = 0;
  } else {
    _currentScreen = NUM_OF_SCREENS - 1;
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
      SetScreen(_currentScreen--);
      break;
    case BTN_RIGHT:
      //setBacklight(TEAL);
      SetScreen(_currentScreen++);
      break;
    case BTN_SELECT:
      setBacklight(VIOLET);
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

void UIM_Controller::SetScreenPedals(bool inputs[3]) {
  print("FWD:");
  print(inputs[0]);
  print(" ");
  print("REV:");
  print(inputs[1]);
  print(" ");
  setCursor(0,1);
  print("HILO:");
  print(inputs[2]);
  print("  ");
}

void UIM_Controller::SetScreenRemote() {
  print("TH:");
  // print(remote.GetThrottle());
  print(" ST:");
  // print(remote.GetSteering());
  print(" E:");
  // print(remote.GetEStop());
  print("           ");
  setCursor(0,1);
  // print("M:");
  // print(remote.GetMode());
  print(" L:" );
  // print(remote.GetLKnob());
  print(" R:");
  // print(remote.GetRKnob());
  print("            ");
}