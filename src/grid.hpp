#ifndef INCLUDE_AL_GRID_HPP
#define INCLUDE_AL_GRID_HPP

#include "fonts.hpp"

using namespace al;

/*
    To figure out depth sorting for the transparent (cutout) letters, I found
    this page helpful:
    https://www.opengl.org/wiki/Transparency_Sorting

    Specifically, these two lines under "Alpha test":
    glAlphaFunc(GL_GREATER, 0.5);
    glEnable(GL_ALPHA_TEST);

    GL_ALPHA_TEST then gets diabled at the end of drawLabels()
*/


struct Grid {
  Mesh gridMesh;
  Font font1;
  Color gnomonColors [3] = {RGB(1,0,0), RGB(0,1,0), RGB(0,0,1)};
  int groundDim;
  float glUnitLength;

  Grid() : font1("data/Avenir-Medium.otf", 72), glUnitLength(1.f) {

    groundDim = 20;

    // build the mesh
    for (int x = 0; x <= groundDim; x++){
      gridMesh.vertex(x,0,0);
      gridMesh.vertex(x,groundDim,0);
      gridMesh.vertex(0,x,0);
      gridMesh.vertex(groundDim,x,0);
    }
    gridMesh.translate(-groundDim/2, -groundDim/2, 0); // center it
    gridMesh.primitive(Graphics::LINES);
  }

  // Draw at origin
  void draw(Graphics &g){
    g.pushMatrix();
      g.color(.35,.35,.35,1);
      g.lineWidth(1);
      g.draw(gridMesh);
    g.popMatrix();
  }

  // Omni labels
  void drawLabels(Graphics &g, Pose cam_pose, float scale){

    // Handle depth test so they're sorted and properly transparent
    g.polygonMode(Graphics::FILL);
    glAlphaFunc(GL_GREATER, 0.5);
    glEnable(GL_ALPHA_TEST);

    for (int axis = 0; axis < 2; axis++){
      for (int i = -groundDim/2; i <= groundDim/2; i++){
        g.pushMatrix();

          Vec3d xform;
          if (axis == 0) {
            xform = Vec3d((groundDim/2) + i - groundDim/2, 0, 0);
            g.translate(xform);
          } else {
            xform = Vec3d(0, (groundDim/2) + i - groundDim/2, 0);
            g.translate(xform);
          }

          g.color(1,1,1);
          g.scale(.003);

          // vector from source to destination, camera up vector
          Vec3d forward = Vec3d(cam_pose.pos() - xform).normalize();
          Quatd rot = Quatd::getBillboardRotation(forward, cam_pose.uu());
          g.rotate(rot);

          std::stringstream sstream;
          if (i != 0) sstream << float(i*scale*glUnitLength); // only draw zero once
          else if (i == 0 && axis == 0) sstream << i; // only draw zero once
          std::string temp_str = sstream.str();
          const char* text = temp_str.c_str();
          font1.render(g, text);

        g.popMatrix();
      }
    }

    glDisable(GL_ALPHA_TEST);
  }

};


#endif
