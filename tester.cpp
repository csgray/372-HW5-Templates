#define CATCH_CONFIG_MAIN  // This tells Catch to provide a main() - only do this in one cpp file
#include <memory>
using std::unique_ptr;
#include "catch.hpp"
#include "CPS.h"

SECTION( "Base Class: Cursor", "[base]" ) {
    unique_ptr<Shape> shape(new Shape());
        
    TEST_CASE("Default Cursor: "){
        REQUIRE(shape->getLocX()==0);
        REQUIRE(shape->getLocY()==0);
    }
    TEST_CASE("Move Cursor: 200,200"){
        shape->setCursor(200,200);
        REQUIRE(shape->getLocX()==200);
        REQUIRE(shape->getLocY()==200);
    }
    TEST_CASE("Max Cursor: 840,700"){
        shape->setCursor(840,700);
        REQUIRE(shape->getLocX()==840);
        REQUIRE(shape->getLocY()==600);
    }  
}
//Not sure how we want to do coords like this or should we just undle in the constructor.
SECTION("Simple Shape Construction: Circle", "[shapes]"){
    TEST_CASE("Circle 10"){
        unique_ptr<Shape> shape(new Circle(10));
        shape->setCusor(10,10);
        REQUIRE(shape->getSize()==10);
        REQUIRE(shape->getPostscript()=="gsave 10 10 translate 0 0 10 0 360 arc stroke grestore");
}
//Polygon will need more work on postscript requires rotation for other N-sides
SECTION("Simple Shape Construction: Polygon", "[shapes]"){
    TEST_CASE( "Polygon 6,10") {
        unique_ptr<Shape> shape(new Polygon(5,10));
        shape->setCusor(5,5);
        REQUIRE(shape->getSize()==10);
        REQUIRE(shape->getPostscript()=="gsave /S 5 def /H 100 def /A 360 S div def 100 100 translate H 0 moveto S{ A cos H mul A sin H mul lineto /A A 360 S div add def } repeat closepath stroke grestore");
    }
}

SECTION("Simple Shape Construction: Rectangle", "[shapes]"){
    TEST_CASE( "Rectangle: 5,10") {
        unique_ptr<Shape> shape(new Rectangle(5,10));
        shape->setCusor(5,5);
        REQUIRE(shape->getSize()==10);
        REQUIRE(shape->getPostscript()=="gsave 5 5 translate newpath -2.5 -5 moveto 2.5 -5 lineto 2.5 5 lineto -2.5 5 lineto closepath stroke grestore");
    }
}
SECTION("Simple Shape Construction: Spacer", "[shapes]"){
    TEST_CASE( "Spacer: 5,10") {
        unique_ptr<Shape> shape(new Spacer(5,10));
        shape->setCusor(5,5);
        REQUIRE(shape->getSize()==10);
    }
}
SECTION("Simple Shape Construction: Square", "[shapes]"){
    TEST_CASE( "Square: 10") {
        unique_ptr<Shape> shape(new Square(10));
        shape->setCusor(5,5);
        REQUIRE(shape->getSize()==10);
        REQUIRE(shape->getPostscript()=="gsave 5 5 translate newpath -5 -5 moveto 5 -5 lineto 5 5 lineto -5 5 lineto closepath stroke grestore");
    }
}
SECTION("Simple Shape Construction: Triangle", "[shapes]"){
    TEST_CASE( "Triangle: 10") {
        unique_ptr<Shape> shape(new Triangle(10));
        shape->setCusor(5,5);
        REQUIRE(shape->getSize()==10);
        REQUIRE(shape->getPostscript()=="gsave 5 5 translate newpath -5 -5 moveto 5 -5 lineto 0 5 lineto closepath stroke grestore");
    }
}
SECTION("Compound Shape Construction: Rotation Square", "[compoundShapes]"){
    TEST_CASE( "Rotation: ") {
        unique_ptr<Shape> shape(new Square(10));
        shape->setCusor(5,5);
        Rotation angle(180);
        Rotated(shape, angle);
        //check
        
    }
}
