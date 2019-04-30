#include "controller.h"

Controller::Controller() {}

Controller::Controller(const QString& opcode, const QString& fun) :
    opcode(opcode),
    fun(fun) {

}

bool Controller::isALU() {
    return false;
}
bool Controller::isJump() {
    return true;
}
bool Controller::isJal() {
    return false;
}
bool Controller::isJalr() {
    return false;
}
bool Controller::isJr() {
    return false;
}

int Controller::getALUCtrl() {

}
