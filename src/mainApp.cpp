/*
Allocore Example: Simple App

*/

#include "allocore/al_Allocore.hpp"
#include <iostream>

#include "grid.hpp"
#include "gnomon.hpp"
#include "colorspace.hpp"

using namespace std;
using namespace al;

MeshVBO sphere;
Vec3f Cd;
Grid groundplane;
Gnomon gnomon;

float inc = 0.0;

struct MyApp : public App {
public:

	MyApp() {
		lens().near(0.1).far(25).fovy(45);
		nav().pos(4,0.5,1);
		nav().quat(Quatd(0.5, 0.5, 0.5, 0.5));

		initWindow(Window::Dim(0,0,720,480), "BCH-Geometry", 40);
		initAudio(44100, 128, 2, 1);

		Cd = Vec3f(1,0,0);
		addSphere(sphere);
		for (int i=0; i<sphere.vertices().size(); i++){
			sphere.color(1,1,1);
		}

	}


	// virtual void onSound(AudioIOData& io){
	// 	while(io()){
	// 		float out1 = 0;
	// 		float out2 = 0;
	//
	// 		io.out(0) = out1;
	// 		io.out(1) = out2;
	// 	}
	// }


	virtual void onAnimate(double dt){
		Cd = rgb2bch(Cd);
			Cd[2] += .01;
		Cd = bch2rgb(Cd);

		for (int i=0; i<sphere.vertices().size(); i++){
			sphere.colors()[i] = Cd;
		}

		inc += 0.01;
	}


	virtual void onDraw(Graphics& g, const Viewpoint& v){
		g.pushMatrix();
			g.translate(0,2.5,0);
			sphere.update();
			g.draw(sphere);
		g.popMatrix();

		Vec3f bch = rgb2bch(Cd);
		Vec3f bchVec = Vec3f(	bch[0] * cos(bch[2] + 0.0),
													bch[0] * sin(bch[2] + 0.0),
													bch[1]);

		glBegin(GL_LINES);
			glColor3f(Cd[0], Cd[1], Cd[2]);
			glVertex3f(0.0,0.0,0.0);
			glColor3f(1,1,1);
			glVertex3f(bchVec[0], bchVec[1], bchVec[2]);
		glEnd();

		groundplane.draw(g);
		gnomon.drawOrigin(g, nav(), .3);
		gnomon.drawFloating(g, nav(), .1);

	}


	virtual void onKeyDown(const ViewpointWindow& w, const Keyboard& k){
		switch(k.key()){
			case ' ':
				nav().pos().print(); printf("\n");
				nav().quat().print(); printf("\n");
				break;
		}
	}

	virtual void onMouseDown(const ViewpointWindow& w, const Mouse& m){
		switch(m.button()){
		// case Mouse::LEFT: printf("Pressed left mouse button.\n"); break;
		// case Mouse::RIGHT: printf("Pressed right mouse button.\n"); break;
		// case Mouse::MIDDLE: printf("Pressed middle mouse button.\n"); break;
		}
	}

	virtual void onMouseDrag(const ViewpointWindow& w, const Mouse& m){
		int x = m.x();
		int y = m.y();
		printf("Mouse dragged: %3d, %3d\n", x,y);
	}

};


int main(){
	MyApp().start();
}
