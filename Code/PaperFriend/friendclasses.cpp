#include "friendclasses.h"

#include <iostream>
#include <string>

Friend::Friend(std::string name, int type, int duration) : name(name), type(type), duration(duration) {}
Friend::~Friend() {}

std::string Friend::get_name() const {
    return name;
}

void Friend::set_name(std::string name) {
    this->name=name;
}

int Friend::get_duration() const {
    return duration;
}

void Friend::set_duration(int duration) {
    this->duration=duration;
}

int Friend::get_type() const{
    return true;
}

void Friend::set_type(int type){
    this->type = type;
}

bool Friend::equal(Friend other){
    return this->name == other.name;
}
