#include "UIM_Controller.h"

UIM_Controller::UIM_Controller(String _startMsg) {
  startMessage_ = _startMsg;

};

void UIM_Controller::Begin() {
  lcd_.clear();
  lcd_.begin(16, 2);
  lcd_.print(startMessage_);
}

void UIM_Controller::Display() {
  // (note: line 1 is the second row, since counting begins with 0):
  lcd_.setCursor(0, 1);
  // print the number of seconds since reset:
  lcd_.print(millis() / 1000);
}

