/*
Allocore Example: Simple App

*/

#include "allocore/io/al_App.hpp"
#include "allocore/graphics/al_Shapes.hpp"

using namespace al;

struct MyApp : public App {
public:

	double phase;

	MyApp(): phase(0){

		lens().near(0.1).far(25).fovy(45);
		nav().pos(0,0,4);
		nav().quat().fromAxisAngle(0.*M_2PI, 0,1,0);

		initWindow(Window::Dim(0,0, 600,400), "Untitled", 40);
		initAudio(44100, 128, 2, 1);

	}


	virtual void onSound(AudioIOData& io){
		while(io()){
			float out1 = 0;
			float out2 = 0;

			io.out(0) = out1;
			io.out(1) = out2;
		}
	}


	virtual void onAnimate(double dt){
		phase += dt;
		if(phase >= 1.) phase -= 1.;
	}


	virtual void onDraw(Graphics& g, const Viewpoint& v){
		Mesh& m = g.mesh();
		m.reset();
		m.primitive(g.TRIANGLES);

		int N = addSphere(m, 1, 32, 32);
		for(int i=0; i<N; ++i){
			m.color(HSV(0.1, 0.5, al::fold(phase + i*0.5/N, 0.5)+0.5));
		}

		g.draw(m);
	}


	virtual void onKeyDown(const ViewpointWindow& w, const Keyboard& k){
		switch(k.key()){
			case '1': printf("Pressed 1.\n"); break;
			case 'y': printf("Pressed y.\n"); break;
			case 'n': printf("Pressed n.\n"); break;
			case '.': printf("Pressed period.\n"); break;
			case ' ': printf("Pressed space bar.\n"); break;
			case Keyboard::RETURN: printf("Pressed return.\n"); break;
			case Keyboard::DELETE: printf("Pressed delete.\n"); break;
			case Keyboard::F1: printf("Pressed F1.\n"); break;
		}
	}

	virtual void onMouseDown(const ViewpointWindow& w, const Mouse& m){
		switch(m.button()){
		case Mouse::LEFT: printf("Pressed left mouse button.\n"); break;
		case Mouse::RIGHT: printf("Pressed right mouse button.\n"); break;
		case Mouse::MIDDLE: printf("Pressed middle mouse button.\n"); break;
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
