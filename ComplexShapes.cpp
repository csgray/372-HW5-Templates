#include <string>
using std::string;
#include <sstream>
using std::stringstream;
#include "headers/ComplexShapes.h"

string ComplexShape::getPostscript() {
    stringstream ss;
    for (auto shape : shapes )
        ss << shape->finalize() << " ";
    string ps = ss.str();
    ps.pop_back(); // Delete the last space!
    return ps;
}

string Rotated::getPostscript(){
    stringstream ss;
    ss << angle << " rotate " << shape->getPostscript();
    return ss.str();
}

string Scaled::getPostscript(){
    stringstream ss;
    ss << getFX() << " " << getFY() << " scale " << shape->getPostscript();
    return ss.str();
}
