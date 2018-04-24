#include <string>
using std::string;
#include <sstream>
using std::stringstream;
#include "headers/ComplexShapes.h"

string ComplexShape::outputPostScript() {
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

void Layered::setHeightWidth(vector<shared_ptr<Shape>> shapes) {
    for (auto shape : shapes) {
        if (getHeight() < shape->getWidth())
            setHeight(shape->getHeight());
        if (getWidth() < shape->getWidth())
            setWidth(shape->getWidth());
    }
};

void Layered::setCoordinates(vector<shared_ptr<Shape>> shapes) {
    auto startShape = shapes[0];
    for (auto shape : shapes )
        shape->setCursor(startShape->getLocX(), startShape->getLocY());
};
