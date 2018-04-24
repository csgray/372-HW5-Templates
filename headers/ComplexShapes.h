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

class ComplexShape : public Shape {
private:
    void virtual setHeightWidth(vector<shared_ptr<Shape>>) = 0;
    void virtual setCoordinates(vector<shared_ptr<Shape>>) = 0;
    string outputPostScript();

protected:
    vector<shared_ptr<Shape>> shapes;

public:
    ComplexShape(initializer_list<shared_ptr<Shape>> list) {
    for (auto shape : list)
        shapes.push_back(shape);
    }

    string getPostscript() override final {
        setHeightWidth(shapes);
        setCoordinates(shapes);
        string postScript = outputPostScript();
        return postScript;
    }

};

class Rotation {
private:
    int angle;
public:
    Rotation(int a) {
        if (a == 90 || a == 180 || a == 270)
            angle = a;
        else
            angle = 0;
    }
    int getAngle() {
        return angle;
    };
};

class Rotated : public Shape{
private:
    int angle;
    shared_ptr<Shape> shape;

public:
    Rotated(shared_ptr<Shape> s, Rotation a) : shape(s) {
        angle = a.getAngle();
        if(angle == 90 || angle == 270){
            setWidth(s->getHeight());
            setHeight(s->getWidth());
        }
        else{
            setWidth(s->getWidth());
            setHeight(s->getHeight());
        }
        setCursor(s->getLocX(),s->getLocY());
    }
    string getPostscript() override;
};

class Scaled : public Shape{
private:
    double fx;
    double fy;
    shared_ptr<Shape> shape;

public:
    Scaled(shared_ptr<Shape> s, double _fx, double _fy) : fx(_fx), fy(_fy), shape(s){
        setWidth(s->getWidth());
        setHeight(s->getHeight());
        setCursor(s->getLocX(),s->getLocY());
    }
    string getPostscript() override;

    double getFX(){
        return fx;
    }

    double getFY(){
        return fy;
    }
};

class Layered : public ComplexShape {
private:
    void setHeightWidth(vector<shared_ptr<Shape>> shapes) override;
    void setCoordinates(vector<shared_ptr<Shape>> shapes) override;
public:
    using ComplexShape::ComplexShape;
};

class VerticalShape : public ComplexShape {
private:
    void setHeightWidth(vector<shared_ptr<Shape>> shapes) override;
    void setCoordinates(vector<shared_ptr<Shape>> shapes) override;
public:
    using ComplexShape::ComplexShape;
};

class HorizontalShape : public ComplexShape {
private:
    void setHeightWidth(vector<shared_ptr<Shape>> shapes) override;
    void setCoordinates(vector<shared_ptr<Shape>> shapes) override;
public:
    using ComplexShape::ComplexShape;
};

#endif
