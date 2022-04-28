#include "UIM_Controller.h"

UIM_Controller::UIM_Controller(String _startMsg) {
  startMessage_ = _startMsg;

};

void UIM_Controller::Begin() {
  lcd_->clear();
  lcd_->begin(16, 2);
  lcd_->print(startMessage_);
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

  lcd_->setCursor(0, 1);
  // print the number of seconds since reset:
  lcd_->print(millis() / 1000);

}

void UIM_Controller::ReadButtons() {

  if (upBtn.IsPressed()) {
    ButtonUpPressed();
  }
  if (downBtn.IsPressed()) {
    ButtonDownPressed();
  }
  if (leftBtn.IsPressed()) {
    ButtonLeftPressed();
  }
  if (rightBtn.IsPressed()) {
    ButtonRightPressed();
  }
  if (selectBtn.IsPressed()) {
    ButtonSelectPressed();
  }
  
}

void UIM_Controller::ButtonUpPressed() {
  lcd_->setBacklight(YELLOW);
}

void UIM_Controller::ButtonDownPressed() {
  lcd_->setBacklight(RED);
}

void UIM_Controller::ButtonLeftPressed() {
  lcd_->setBacklight(GREEN);
}

void UIM_Controller::ButtonRightPressed() {
  lcd_->setBacklight(TEAL);
}

void UIM_Controller::ButtonSelectPressed() {
  lcd_->setBacklight(VIOLET);
}
