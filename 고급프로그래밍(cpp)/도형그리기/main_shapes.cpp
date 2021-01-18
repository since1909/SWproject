#include "Graph.h" 
#include "Simple_window.h"

int main() {
	using namespace Graph_lib;
	
	Point tl(150, 150); 
	Simple_window win(tl, 800, 800, "My window"); 
	
	Box b1(Point(100, 100), 50, 250, 150);
	Box b2(Point(400, 100), 40, 250, 150);
	Box b3(Point(100, 400), 30, 250, 150);
	Box b4(Point(400, 400), 20, 250, 150);

	b1.set_color(Color::red);
	b2.set_color(Color::red);
	b3.set_color(Color::red);
	b4.set_color(Color::red);

	win.attach(b1);
	win.attach(b2);
	win.attach(b3);
	win.attach(b4);

	win.wait_for_button();

	win.detach(b1);
	win.detach(b2);
	win.detach(b3);
	win.detach(b4);

	Regular_polygon r1(Point(200, 200), 5, 60);
	Regular_polygon r2(Point(400, 200), 6, 60);
	Regular_polygon r3(Point(200, 400), 7, 60);
	Regular_polygon r4(Point(400, 400), 8, 60);

	r1.set_color(Color::red);
	r2.set_color(Color::red);
	r3.set_color(Color::red);
	r4.set_color(Color::red);

	win.attach(r1);
	win.attach(r2);
	win.attach(r3);
	win.attach(r4);

	win.wait_for_button();

	win.detach(r1);
	win.detach(r2);
	win.detach(r3);
	win.detach(r4);

	Star s1(Point(200, 200), 4, 50, 100);
	Star s2(Point(500, 200), 5, 50, 100);
	Star s3(Point(200, 500), 6, 50, 100);
	Star s4(Point(500, 500), 8, 50, 100);

	s1.set_color(Color::red);
	s2.set_color(Color::red);
	s3.set_color(Color::red);
	s4.set_color(Color::red);

	win.attach(s1);
	win.attach(s2);
	win.attach(s3);
	win.attach(s4);

	win.wait_for_button();
}