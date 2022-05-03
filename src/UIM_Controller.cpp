#include "UIM_Controller.h"

UIM_Controller::UIM_Controller(String _startMsg) {

  startMessage_ = _startMsg;

};

void UIM_Controller::Begin() {
  clear();
  begin(16, 2);
  print(startMessage_);
  setCursor(0,1);
  print(VERSION);
  
}

void UIM_Controller::HandleEvents() {

  long now = millis();

  if(now > lastLCDUpdate_ + TIME_LCD_UPDATE) {
    UpdateLCD();
    lastLCDUpdate_ = now;
  }
  
  if(now > lastButtonRead_ + TIME_BUTTON_READ) {
    ReadButtons();
    lastButtonRead_ = now;
  }
  
}

void UIM_Controller::UpdateLCD() {

  //setCursor(0, 1);
  // print the number of seconds since reset:
  //print(millis()/1000);
  //print("     ");

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
      setBacklight(GREEN);
      break;
    case BTN_RIGHT:
      setBacklight(TEAL);
      break;
    case BTN_SELECT:
      setBacklight(VIOLET);
      break;
    default:
      break;
  }
}
