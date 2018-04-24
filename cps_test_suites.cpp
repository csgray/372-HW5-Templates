#include <memory>
using std::shared_ptr;
#include <algorithm>
using std::max;
#include <fstream>
using std::ifstream;
#include <iostream>
using std::endl;
#include <string>
using std::getline;
#include <sstream>
using std::stringstream;

#include "headers/catch.hpp"
#include "headers/Shape.h"
#include "headers/SimpleShapes.h"
#include "headers/ComplexShapes.h"
#include "headers/UniqueShapes.h"
#include "headers/cps.h"

TEST_CASE("Checking cursor pos","[cursor]"){
    shared_ptr<Shape> shape(new Circle(5));
    shape->setCursor(1,1);
    REQUIRE(shape->getLocX()==1);
    REQUIRE(shape->getLocY()==1);
    shape->setCursor(100,100);
    REQUIRE(shape->getLocX()==100);
    REQUIRE(shape->getLocY()==100);
    shape->setCursor(80,1000);
    REQUIRE(shape->getLocX()==80);
    REQUIRE(shape->getLocY()==1000);
    shape->setCursor(-8,-9);
    REQUIRE(shape->getLocX()==-8);
    REQUIRE(shape->getLocY()==-9);
}

TEST_CASE("Simple Shape Construction: Min Circle", "[shapes]"){
    shared_ptr<Shape> shape(new Circle(1));
    shape->setCursor(1,1);
    REQUIRE(shape->getHeight()==2);
    REQUIRE(shape->getPostscript()=="0 0 1 0 360 arc");
    REQUIRE(shape->finalize()=="gsave 1 1 translate 0 0 1 0 360 arc stroke grestore");
}

TEST_CASE("Simple Shape: Extreme values", "[shapes]"){
    shared_ptr<Shape> shape(new Circle(500));
    shape->setCursor(1,1);
    REQUIRE(shape->getHeight()==1000);
    REQUIRE(shape->getPostscript()=="0 0 500 0 360 arc");
    REQUIRE(shape->finalize()=="gsave 1 1 translate 0 0 500 0 360 arc stroke grestore");
}

TEST_CASE("Simple Shape Construction: Circle", "[shapes]"){
    shared_ptr<Shape> shape(new Circle(10));
    shape->setCursor(10,10);
    REQUIRE(shape->getHeight()==20);
    REQUIRE(shape->getPostscript()=="0 0 10 0 360 arc");
    REQUIRE(shape->finalize()=="gsave 10 10 translate 0 0 10 0 360 arc stroke grestore");
}
TEST_CASE("Simple Shape Construction: Polygon", "[shapes]"){
    shared_ptr<Shape> shape(new Polygon(5,10));
    shape->setCursor(100,100);
    REQUIRE(shape->getHeight()==10);
    REQUIRE(shape->getPostscript()=="/S 5 def /H 5 def /A 360 S div def A cos H mul H sub A sin H mul 0 sub atan rotate -90 rotate H 0 moveto S{ A cos H mul A sin H mul lineto /A A 360 S div add def } repeat closepath");
    REQUIRE(shape->finalize()=="gsave 100 100 translate /S 5 def /H 5 def /A 360 S div def A cos H mul H sub A sin H mul 0 sub atan rotate -90 rotate H 0 moveto S{ A cos H mul A sin H mul lineto /A A 360 S div add def } repeat closepath stroke grestore");
}

TEST_CASE("Simple Shape Construction: Rectangle", "[shapes]"){
    shared_ptr<Shape> shape(new Rectangle(5,10));
    shape->setCursor(5,5);
    REQUIRE(shape->getHeight()==10);
    REQUIRE(shape->getPostscript()=="/W 2.5 def /H 5 def newpath W neg H neg moveto W H neg lineto W H lineto W neg H lineto closepath");
    REQUIRE(shape->finalize()=="gsave 5 5 translate /W 2.5 def /H 5 def newpath W neg H neg moveto W H neg lineto W H lineto W neg H lineto closepath stroke grestore");
}
TEST_CASE("Simple Shape Construction: Spacer", "[shapes]"){
    shared_ptr<Shape> shape(new Spacer(5,10));
    shape->setCursor(5,5);
    REQUIRE(shape->getHeight()==10);
}
TEST_CASE("Simple Shape Construction: Square", "[shapes]"){
    shared_ptr<Shape> shape(new Square(10));
    shape->setCursor(5,5);
    REQUIRE(shape->getHeight()==10);
    REQUIRE(shape->getPostscript()=="/W 5 def /H 5 def newpath W neg H neg moveto W H neg lineto W H lineto W neg H lineto closepath");
    REQUIRE(shape->finalize()=="gsave 5 5 translate /W 5 def /H 5 def newpath W neg H neg moveto W H neg lineto W H lineto W neg H lineto closepath stroke grestore");

}
TEST_CASE("Simple Shape Construction: Triangle", "[shapes]"){
    shared_ptr<Shape> shape(new Triangle(10));
    shape->setCursor(5,5);
    REQUIRE(shape->getHeight()==10);
    REQUIRE(shape->getPostscript()=="/W 5 def /H 5 def newpath W neg H neg moveto W H neg lineto 0 H lineto closepath");
    REQUIRE(shape->finalize()=="gsave 5 5 translate /W 5 def /H 5 def newpath W neg H neg moveto W H neg lineto 0 H lineto closepath stroke grestore");

}

TEST_CASE("Rotation: Triangle Info Check", "[compoundShapes]") {
    shared_ptr<Shape> shape(new Triangle(10));
    shape->setCursor(5,5);
    Rotation angle(270);
    shared_ptr<Shape> rotated(new Rotated(shape, angle));
    REQUIRE(rotated->getLocX()==5);
    REQUIRE(rotated->getLocY()==5);
    REQUIRE(rotated->getHeight()==10);
    REQUIRE(rotated->getWidth()==10);
    REQUIRE(rotated->getPostscript()=="270 rotate /W 5 def /H 5 def newpath W neg H neg moveto W H neg lineto 0 H lineto closepath");
    REQUIRE(rotated->finalize()=="gsave 5 5 translate 270 rotate /W 5 def /H 5 def newpath W neg H neg moveto W H neg lineto 0 H lineto closepath stroke grestore");
}

TEST_CASE("Compound Shape Construction: Rotation 90", "[compoundShapes]") {
	shared_ptr<Shape> shape(new Triangle(10));
	shape->setCursor(5, 5);
	Rotation angle(90);
	shared_ptr<Shape> rotated(new Rotated(shape, angle));
	REQUIRE(rotated->getPostscript() == "90 rotate /W 5 def /H 5 def newpath W neg H neg moveto W H neg lineto 0 H lineto closepath");
	REQUIRE(rotated->finalize() == "gsave 5 5 translate 90 rotate /W 5 def /H 5 def newpath W neg H neg moveto W H neg lineto 0 H lineto closepath stroke grestore");

}
TEST_CASE("Rotation: Triangle 180","[compoundShapes]") {
    shared_ptr<Shape> shape(new Triangle(10));
    shape->setCursor(5,5);
    Rotation angle(180);
    shared_ptr<Shape> rotated(new Rotated(shape, angle));
    REQUIRE(rotated->getPostscript()=="180 rotate /W 5 def /H 5 def newpath W neg H neg moveto W H neg lineto 0 H lineto closepath");
    REQUIRE(rotated->finalize()=="gsave 5 5 translate 180 rotate /W 5 def /H 5 def newpath W neg H neg moveto W H neg lineto 0 H lineto closepath stroke grestore");

}
TEST_CASE("Rotation: Triangle 270","[compoundShapes]") {
    shared_ptr<Shape> shape(new Triangle(10));
    shape->setCursor(5,5);
    Rotation angle(270);
    shared_ptr<Shape> rotated(new Rotated(shape, angle));
    REQUIRE(rotated->getPostscript()=="270 rotate /W 5 def /H 5 def newpath W neg H neg moveto W H neg lineto 0 H lineto closepath");
    REQUIRE(rotated->finalize()=="gsave 5 5 translate 270 rotate /W 5 def /H 5 def newpath W neg H neg moveto W H neg lineto 0 H lineto closepath stroke grestore");

}

TEST_CASE("Rotation: Multiply times","[compoundShapes]"){
    shared_ptr<Shape> shape(new Triangle(10));
    shape->setCursor(5,5);
    Rotation angle(270);
    shared_ptr<Shape> rotated1(new Rotated(shape, angle));
    shared_ptr<Shape> rotated2(new Rotated(rotated1, angle));
    shared_ptr<Shape> rotated3(new Rotated(rotated2, angle));
    shared_ptr<Shape> rotated4(new Rotated(rotated3, angle));
    shared_ptr<Shape> rotated5(new Rotated(rotated4, angle));
    REQUIRE(rotated5->getPostscript()=="270 rotate 270 rotate 270 rotate 270 rotate 270 rotate /W 5 def /H 5 def newpath W neg H neg moveto W H neg lineto 0 H lineto closepath");
}

TEST_CASE("Unique Shape: Circle with Arcs", "[uniqueShapes]") {
    shared_ptr<Shape> shape(new CircleWithArcs(10));
    shape->setCursor(10,10);
    Rotation angle(90);
    shared_ptr<Shape> rotated(new Rotated(shape, angle));
    REQUIRE(shape->getHeight() == 20);
    REQUIRE(rotated->getPostscript() == "90 rotate 0 0 newpath 0 30 360 { 3 copy pop 10 0 4 index arc closepath stroke pop } for");
    REQUIRE(rotated->finalize() == "gsave 10 10 translate 90 rotate 0 0 newpath 0 30 360 { 3 copy pop 10 0 4 index arc closepath stroke pop } for stroke grestore");
}


TEST_CASE ("Scaled Shape to certain dimensions" ,"[compoundShapes]") {
    shared_ptr<Shape> shape(new Triangle(10));
    shape->setCursor(5,5);
    shared_ptr<Shape> scaled1(new Scaled(shape, 1.0,2.0));
    REQUIRE(scaled1->getPostscript() == "1 2 scale /W 5 def /H 5 def newpath W neg H neg moveto W H neg lineto 0 H lineto closepath");
    REQUIRE(scaled1->finalize() == "gsave 5 5 translate 1 2 scale /W 5 def /H 5 def newpath W neg H neg moveto W H neg lineto 0 H lineto closepath stroke grestore");

    
}

TEST_CASE("file ps test","[files]"){
    stringstream ss,fileSS;
    vector<shared_ptr<Shape> > allShapes;
    for(size_t i=0;i<10;i++){
        shared_ptr<Shape> shape(new Triangle(10));
        shared_ptr<Shape> shape1(new Circle(10));
        allShapes.push_back(shape);
        allShapes.push_back(shape1);
    }
    for(size_t i=0;i<allShapes.size();i++){
        ss << allShapes[i]->finalize() << endl; 
    }

    makePostscriptFile(allShapes,"ps");
    ifstream myPsFile("ps.ps");
    string holding = "";
    if(myPsFile){
        while(getline(myPsFile,holding)){
            fileSS << holding << endl;
        }
    }
    REQUIRE(fileSS.str() == ss.str());
}
TEST_CASE("All simple shapes to file","[files]"){
    stringstream ss,fileSS;
    vector<shared_ptr<Shape> > allShapes;
    shared_ptr<Shape> shape(new Triangle(10,20,20));
    shared_ptr<Shape> shape1(new Circle(10,10,10));
    shared_ptr<Shape> shape2(new Square(10,30,30));
    shared_ptr<Shape> shape3(new Polygon(6,10,40,40));
    shared_ptr<Shape> shape4(new Rectangle(6,10,50,50));
    allShapes.push_back(shape);
    allShapes.push_back(shape1);
    allShapes.push_back(shape2);
    allShapes.push_back(shape3);
    allShapes.push_back(shape4);
    for(size_t i=0;i<allShapes.size();i++){
        ss << allShapes[i]->finalize() << endl; 
    }

    makePostscriptFile(allShapes,"ps");
    ifstream myPsFile("ps.ps");
    string holding = "";
    if(myPsFile){
        while(getline(myPsFile,holding)){
            fileSS << holding << endl;
        }
    }
    REQUIRE(fileSS.str() == ss.str());
}

TEST_CASE( "Compound Shape - Layered Shapes: Triangle Square Circle") {
    shared_ptr<Shape> circle(new Circle(15));   // Circles are radius*2 so this has a height and width of 30
    circle->setCursor(15,15);
    shared_ptr<Shape> square(new Square(20));
    square->setCursor(20,20);
    shared_ptr<Shape> triangle(new Triangle(10));
    triangle->setCursor(10,10);
    shared_ptr<Shape> layered(new Layered( {circle, square, triangle} ));

/*     SECTION("Constructor") {    

        REQUIRE(circle->getLocY() == 15);
        REQUIRE(circle->getLocX() == 15);
        // The height of the resulting shape's bounding box is the maximum of the heights of the component shapes.
         REQUIRE( layered->getHeight() == max( max(triangle->getHeight(), square->getHeight()), circle->getHeight() ) );
        // The width of the resulting shape's bounding box is the maximum width of the widths of the component shapes.
        REQUIRE( layered->getWidth() == max( max(triangle->getWidth(), square->getWidth()), circle->getWidth() ) );
        // Shape shapes[i+1]'s bounding box is located directly above the bounding box of shapes[i],
        REQUIRE( triangle->getLocY() == circle->getLocY());
        REQUIRE( triangle->getLocX() == circle->getLocX());
        REQUIRE( square->getLocY() == circle->getLocY());
        REQUIRE( square->getLocX() == square->getLocY());
        
    } */

    SECTION("PostScript") {
         REQUIRE(layered->getPostscript()=="gsave 15 15 translate 0 0 15 0 360 arc stroke grestore gsave 15 15 translate /W 10 def /H 10 def newpath W neg H neg moveto W H neg lineto W H lineto W neg H lineto closepath stroke grestore gsave 15 15 translate /W 5 def /H 5 def newpath W neg H neg moveto W H neg lineto 0 H lineto closepath stroke grestore");
    }
}

TEST_CASE( "Compound Shape - Vertical Shapes: Circle Square Triangle") {
    shared_ptr<Shape> circle(new Circle(15));   // Circles are radius*2 so this has a height and width of 30
    circle->setCursor(15,15);
    shared_ptr<Shape> square(new Square(20));
    square->setCursor(20,20);
    shared_ptr<Shape> triangle(new Triangle(10));
    triangle->setCursor(10,10);
    shared_ptr<Shape> vertical(new VerticalShape( {circle, square, triangle} ));

/*     SECTION("Constructor") {    
        // The height of the resulting shape's bounding box is the sum of the heights of the component shapes.
        REQUIRE( vertical->getHeight() == triangle->getHeight() + square->getHeight() + circle->getHeight() ); // This should be 60.
        // The width of the resulting shape's bounding box is the maximum width of the widths of the component shapes.
        REQUIRE( vertical->getWidth() == max( max(triangle->getWidth(), square->getWidth()), circle->getWidth() ) );
        // Shape shapes[i+1]'s bounding box is located directly above the bounding box of shapes[i],
        REQUIRE( triangle->getLocY() == (square->getLocY() + square->getHeight()/2 + triangle->getHeight()/2) );
        REQUIRE( square->getLocY() == (circle->getLocY() + circle->getHeight()/2 + square->getHeight()/2) );
        // and both bounding boxes are vertically aligned around their center.
        REQUIRE( triangle->getLocX() == square->getLocX() );
        REQUIRE( square->getLocX() == circle->getLocX() );
    } */

    SECTION("PostScript") {
        REQUIRE(vertical->getPostscript()=="gsave 15 15 translate 0 0 15 0 360 arc stroke grestore gsave 15 40 translate /W 10 def /H 10 def newpath W neg H neg moveto W H neg lineto W H lineto W neg H lineto closepath stroke grestore gsave 15 55 translate /W 5 def /H 5 def newpath W neg H neg moveto W H neg lineto 0 H lineto closepath stroke grestore");
    }
}

TEST_CASE( "Compound Shape - Horizontal Shapes: Triangle Square Circle") {
    shared_ptr<Shape> triangle(new Triangle(10));
    triangle->setCursor(15,15);
    shared_ptr<Shape> square(new Square(20));
    square->setCursor(20,20);
    shared_ptr<Shape> circle(new Circle(15));   // Circles are radius*2 so this has a height and width of 30
    circle->setCursor(15,15);
    shared_ptr<Shape> horizontal(new HorizontalShape( {triangle, square, circle} ));

/*     SECTION("Constructor") {    
        // The height of the resulting shape's bounding box is the maximum height of the heights of the component shapes.
        REQUIRE( horizontal->getHeight() == max( max(triangle->getHeight(), square->getHeight()), circle->getHeight() ) );
        // The width of the resulting shape's bounding box is the sum of the widths of the component shapes.
        REQUIRE( horizontal->getWidth() == triangle->getWidth() + square->getWidth() + circle->getWidth() ); // This should be 60.)
        // Shape shapes[i+1]'s bounding box is located next to (to the right of) the bounding box of shapes[i]
        REQUIRE( circle->getLocX() == (square->getLocX() + square->getWidth()/2 + circle->getWidth()/2) );
        REQUIRE( square->getLocX() == (triangle->getLocX() + triangle->getWidth()/2 + square->getWidth()/2) );
        // and both bounding boxes are horizontally aligned around their center.
        REQUIRE( circle->getLocY() == square->getLocY() );
        REQUIRE( square->getLocY() == triangle->getLocY() );
    } */

    SECTION("PostScript") {
    REQUIRE(horizontal->getPostscript()=="gsave 15 15 translate /W 5 def /H 5 def newpath W neg H neg moveto W H neg lineto 0 H lineto closepath stroke grestore gsave 30 15 translate /W 10 def /H 10 def newpath W neg H neg moveto W H neg lineto W H lineto W neg H lineto closepath stroke grestore gsave 55 15 translate 0 0 15 0 360 arc stroke grestore");
    }
}

TEST_CASE("Rotation of 270 Horizontal shapes","[Rotation]"){
    shared_ptr<Shape> shape(new Triangle(10,5,5));
    shared_ptr<Shape> shape1(new Triangle(10,5,5));
    Rotation angle(270);
    shared_ptr<Shape> horizontal(new HorizontalShape({shape,shape1}));
    shared_ptr<Shape> rotated(new Rotated(horizontal, angle));
    REQUIRE(rotated->getHeight()==horizontal->getWidth());
    REQUIRE(rotated->getWidth()==horizontal->getHeight());
    REQUIRE(rotated->getPostscript()==("270 rotate "+horizontal->getPostscript()));
}
TEST_CASE("Rotation 180 of Horizontal shapes","[Rotation]"){
    shared_ptr<Shape> shape(new Triangle(10,5,5));
    shared_ptr<Shape> shape1(new Triangle(10,5,5));
    Rotation angle(180);
    shared_ptr<Shape> horizontal(new HorizontalShape({shape,shape1}));
    shared_ptr<Shape> rotated(new Rotated(horizontal, angle));
    REQUIRE(rotated->getHeight()==horizontal->getHeight());
    REQUIRE(rotated->getWidth()==horizontal->getWidth());
    REQUIRE(rotated->getPostscript()==("180 rotate "+horizontal->getPostscript()));
}

TEST_CASE("Rotation 270 of Vertical shapes","[Rotation]"){
    shared_ptr<Shape> shape(new Triangle(10,5,5));
    shared_ptr<Shape> shape1(new Triangle(10,5,5));
    Rotation angle(270);
    shared_ptr<Shape> vertical(new VerticalShape({shape,shape1}));
    shared_ptr<Shape> rotated(new Rotated(vertical, angle));
    REQUIRE(rotated->getHeight()==vertical->getWidth());
    REQUIRE(rotated->getWidth()==vertical->getHeight());
    REQUIRE(rotated->getPostscript()==("270 rotate "+vertical->getPostscript()));
}
TEST_CASE("Rotation 180 of Vertical shapes","[Rotation]"){
    shared_ptr<Shape> shape(new Triangle(10,5,5));
    shared_ptr<Shape> shape1(new Triangle(10,5,5));
    Rotation angle(180);
    shared_ptr<Shape> vertical(new VerticalShape({shape,shape1}));
    shared_ptr<Shape> rotated(new Rotated(vertical, angle));
    REQUIRE(rotated->getHeight()==vertical->getHeight());
    REQUIRE(rotated->getWidth()==vertical->getWidth());
    REQUIRE(rotated->getPostscript()==("180 rotate "+vertical->getPostscript()));
}
TEST_CASE("Rotation of Layered shapes","[Rotation]"){
    shared_ptr<Shape> shape(new Triangle(10,5,5));
    shared_ptr<Shape> shape1(new Triangle(10,5,5));
    Rotation angle(270);
    shared_ptr<Shape> layered(new Layered({shape,shape1}));
    shared_ptr<Shape> rotated(new Rotated(layered, angle));
    REQUIRE(rotated->getHeight()==layered->getWidth());
    REQUIRE(rotated->getPostscript()==("270 rotate "+layered->getPostscript()));
}
TEST_CASE("SUPER SHAPE 9000!","[extremeShapes]"){
    vector<shared_ptr<Shape>> allShapes;
    shared_ptr<Shape> shape(new Triangle(10));
    shared_ptr<Shape> shape1(new Circle(10));
    shared_ptr<Shape> vertical(new VerticalShape({shape,shape1}));
    shared_ptr<Shape> shape2(new Square(10));
    shared_ptr<Shape> shape3(new Polygon(6,100));
    shared_ptr<Shape> shape4(new Rectangle(6,10));
    shared_ptr<Shape> horizontal(new HorizontalShape({vertical,shape2,shape3,shape4}));
    Rotation angle(270);
    shared_ptr<Shape> rotated(new Rotated(horizontal,angle));
    shared_ptr<Shape> scaled(new Scaled(rotated,1,1));
    scaled->setCursor(200,200);
    allShapes.push_back(scaled);
    makePostscriptFile(allShapes,"ps2");
}
