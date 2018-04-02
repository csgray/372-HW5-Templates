#ifndef COMPLEXSHAPES_H
#define COMPLEXSHAPES_H
#include <memory>
using std::shared_ptr;
#include <string>
using std::string;
#include <vector>
using std::vector;
#include <initializer_list>
using std::initializer_list;

#include "Shape.h"


class Rotation{
private:
    int angle;
public:
    Rotation(int a){
        if(a == 90 || a == 180 || a == 270){
            angle = a;
        }
        else
        {
            angle = 0;
        }
    }
    int getAngle(){
        return angle;
    };
};

class Rotated : public Shape{
private:
    int angle;
    shared_ptr<Shape> shape;

public:
    Rotated(shared_ptr<Shape> s,Rotation a) : shape(s){
        angle = a.getAngle();
        setWidth(s->getWidth());
        setHeight(s->getHeight());
        setCursor(s->getLocX(),s->getLocY());
    }
    string getPostscript() override;
};

class VerticalShape : public Shape{
private:
    vector<shared_ptr<Shape>> shapes;

public:
    VerticalShape(initializer_list<shared_ptr<Shape>> list) {
        setHeight(0);   // HACK: Why is an empty VerticalShape starting with a height of 10?!

        for (auto shape : list) {
            shapes.push_back(shape);
            setHeight(getHeight() + shape->getHeight());
            if (getWidth() <  shape->getWidth())
                setWidth(shape->getWidth());
        };
    }

    string getPostscript() override;
};

#endif