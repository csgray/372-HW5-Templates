
#define CATCH_CONFIG_FAST_COMPILE
#include <memory>
using std::unique_ptr;
using std::shared_ptr;
#include <algorithm>
using std::max;

#include "catch.hpp"
#include "Shape.h"
#include "SimpleShapes.h"
#include "ComplexShapes.h"

//Not sure how we want to do coords like this or should we just undle in the constructor.
TEST_CASE("Simple Shape Construction: Min Circle", "[shapes]"){
    unique_ptr<Shape> shape(new Circle(1));
    shape->setCursor(1,1);
    REQUIRE(shape->getHeight()==2);
    REQUIRE(shape->getPostscript()=="0 0 1 0 360 arc stroke");
    REQUIRE(shape->finalize()=="gsave 1 1 translate 0 0 1 0 360 arc stroke grestore");
}

TEST_CASE("Simple Shape Construction: Circle", "[shapes]"){
    unique_ptr<Shape> shape(new Circle(10));
    shape->setCursor(10,10);
    REQUIRE(shape->getHeight()==20);
    REQUIRE(shape->getPostscript()=="0 0 10 0 360 arc stroke");
    REQUIRE(shape->finalize()=="gsave 10 10 translate 0 0 10 0 360 arc stroke grestore");
}
TEST_CASE("Simple Shape Construction: Polygon", "[shapes]"){
    unique_ptr<Shape> shape(new Polygon(5,10));
    shape->setCursor(100,100);
    REQUIRE(shape->getHeight()==10);
    REQUIRE(shape->getPostscript()=="/S 5 def /H 10 def /A 360 S div def A cos H mul H sub A sin H mul 0 sub atan rotate -90 rotate H 0 moveto S{ A cos H mul A sin H mul lineto /A A 360 S div add def } repeat closepath stroke");
    REQUIRE(shape->finalize()=="gsave 100 100 translate /S 5 def /H 10 def /A 360 S div def A cos H mul H sub A sin H mul 0 sub atan rotate -90 rotate H 0 moveto S{ A cos H mul A sin H mul lineto /A A 360 S div add def } repeat closepath stroke grestore");

}

TEST_CASE("Simple Shape Construction: Rectangle", "[shapes]"){
    unique_ptr<Shape> shape(new Rectangle(5,10));
    shape->setCursor(5,5);
    REQUIRE(shape->getHeight()==10);
    REQUIRE(shape->getPostscript()=="/W 2.5 def /H 5 def newpath -W -H moveto W -H lineto W H lineto -W H lineto closepath stroke");
    REQUIRE(shape->finalize()=="gsave 5 5 translate /W 2.5 def /H 5 def newpath -W -H moveto W -H lineto W H lineto -W H lineto closepath stroke grestore");

}
TEST_CASE("Simple Shape Construction: Spacer", "[shapes]"){
    unique_ptr<Shape> shape(new Spacer(5,10));
    shape->setCursor(5,5);
    REQUIRE(shape->getHeight()==10);
}
TEST_CASE("Simple Shape Construction: Square", "[shapes]"){
    unique_ptr<Shape> shape(new Square(10));
    shape->setCursor(5,5);
    REQUIRE(shape->getHeight()==10);
    REQUIRE(shape->getPostscript()=="/W 5 def /H 5 def newpath -W -H moveto W -H lineto W H lineto -W H lineto closepath stroke");
    REQUIRE(shape->finalize()=="gsave 5 5 translate /W 5 def /H 5 def newpath -W -H moveto W -H lineto W H lineto -W H lineto closepath stroke grestore");

}
TEST_CASE("Simple Shape Construction: Triangle", "[shapes]"){
    unique_ptr<Shape> shape(new Triangle(10));
    shape->setCursor(5,5);
    REQUIRE(shape->getHeight()==10);
    REQUIRE(shape->getPostscript()=="/W 5 def /H 5 def newpath -W -H moveto W -H lineto 0 H lineto closepath stroke");
    REQUIRE(shape->finalize()=="gsave 5 5 translate /W 5 def /H 5 def newpath -W -H moveto W -H lineto 0 H lineto closepath stroke grestore");

}

TEST_CASE( "Rotation: Triangle Info Check", "[compoundShapes]") {
    shared_ptr<Shape> shape(new Triangle(10));
    shape->setCursor(5,5);
    Rotation angle(270);
    shared_ptr<Shape> rotated(new Rotated(shape, angle));
    REQUIRE(rotated->getLocX()==5);
    REQUIRE(rotated->getLocY()==5);
    REQUIRE(rotated->getHeight()==10);
    REQUIRE(rotated->getWidth()==10);
    REQUIRE(rotated->getPostscript()=="270 rotate /W 5 def /H 5 def newpath -W -H moveto W -H lineto 0 H lineto closepath stroke");
    REQUIRE(rotated->finalize()=="gsave 5 5 translate 270 rotate /W 5 def /H 5 def newpath -W -H moveto W -H lineto 0 H lineto closepath stroke grestore");
}

TEST_CASE("Compound Shape Construction: Rotation 90", "[compoundShapes]") {
	shared_ptr<Shape> shape(new Triangle(10));
	shape->setCursor(5, 5);
	Rotation angle(90);
	shared_ptr<Shape> rotated(new Rotated(shape, angle));
	REQUIRE(rotated->getPostscript() == "90 rotate /W 5 def /H 5 def newpath -W -H moveto W -H lineto 0 H lineto closepath stroke");
	REQUIRE(rotated->finalize() == "gsave 5 5 translate 90 rotate /W 5 def /H 5 def newpath -W -H moveto W -H lineto 0 H lineto closepath stroke grestore");

}
TEST_CASE( "Rotation: Triangle 180","[compoundShapes]") {
    shared_ptr<Shape> shape(new Triangle(10));
    shape->setCursor(5,5);
    Rotation angle(180);
    shared_ptr<Shape> rotated(new Rotated(shape, angle));
    REQUIRE(rotated->getPostscript()=="180 rotate /W 5 def /H 5 def newpath -W -H moveto W -H lineto 0 H lineto closepath stroke");
    REQUIRE(rotated->finalize()=="gsave 5 5 translate 180 rotate /W 5 def /H 5 def newpath -W -H moveto W -H lineto 0 H lineto closepath stroke grestore");

}
TEST_CASE( "Rotation: Triangle 270","[compoundShapes]") {
    shared_ptr<Shape> shape(new Triangle(10));
    shape->setCursor(5,5);
    Rotation angle(270);
    shared_ptr<Shape> rotated(new Rotated(shape, angle));
    REQUIRE(rotated->getPostscript()=="270 rotate /W 5 def /H 5 def newpath -W -H moveto W -H lineto 0 H lineto closepath stroke");
    REQUIRE(rotated->finalize()=="gsave 5 5 translate 270 rotate /W 5 def /H 5 def newpath -W -H moveto W -H lineto 0 H lineto closepath stroke grestore");

}

TEST_CASE( "Compound Shape - Vertical Shapes: Triangle Square Circle") {
    shared_ptr<Shape> circle(new Circle(15));   // Circles are radius*2 so this has a height and width of 30
    shared_ptr<Shape> square(new Square(20));
    shared_ptr<Shape> triangle(new Triangle(10));
    shared_ptr<Shape> vertical(new VerticalShape( {circle, square, triangle} ));

    SECTION("Constructor") {    
        // The height of the resulting shape's bounding box is the sum of the heights of the component shapes.
        REQUIRE( vertical->getHeight() == triangle->getHeight() + square->getHeight() + circle->getHeight() ); // This should be 60.
        // The width of the resulting shape's bounding box is the maximum width of the widths of the component shapes.
        REQUIRE( vertical->getWidth() == max( max(triangle->getWidth(), square->getWidth()), circle->getWidth() ) );
        // Shape shapes[i+1]'s bounding box is located directly above the bounding box of shapes[i],

        // and both bounding boxes are vertically aligned around their center.
        REQUIRE( triangle->getLocX() == square->getLocX() );
        REQUIRE( square->getLocX() == circle->getLocX() );
    }

    SECTION("PostScript") {
        REQUIRE(vertical->getPostscript()=="TRIANGLE SQUARE CIRCLE");
    }
}