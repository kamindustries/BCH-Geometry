#ifndef __FONTS_HPP__
#define __FONTS_HPP__

#include "allocore/graphics/al_Font.hpp"

struct Fonts {
  static al::Font font1;
};

al::Font Fonts::font1("data/Avenir-Medium.otf", 72);

#endif
