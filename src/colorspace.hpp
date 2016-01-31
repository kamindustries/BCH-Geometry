#include "allocore/al_Allocore.hpp"

namespace al {


///////////////////////////////////////////////////////////////////////
// B C H   C O L O R S P A C E
///////////////////////////////////////////////////////////////////////
// RGB - XYZ matrices taken from
// http://www.brucelindbloom.com/index.html?Eqn_RGB_XYZ_Matrix.html

Vec3f rgb2def(Vec3f _col){
  // RGB to XYZ, Adobe RGB (1998)
  Mat3f XYZ = Mat3f(0.5767309, 0.1855540, 0.1881852,
                    0.2973769, 0.6273491, 0.0752741,
                    0.0270343, 0.0706872, 0.9911085);

  // XYZ to DEF
  Mat3f DEF = Mat3f(0.20530, 0.71250, 0.46700,
                    1.85370, -1.2797, -0.4429,
                    -0.3655, 1.01200, -0.6104);

  Vec3f xyz = _col * XYZ;
  Vec3f def = xyz * DEF;
  return def;
}

Vec3f def2rgb(Vec3f _def){
  // DEF to XYZ
  Mat3f XYZ = Mat3f(0.6712, 0.4955, 0.1540,
                    0.7061, 0.0248, 0.5223,
                    0.7689, -0.2556, -0.8645);

  // RGB to XYZ, Adobe RGB (1998)
  Mat3f RGB = Mat3f(2.0413690, -0.5649464, -0.3446944,
                    -0.9692660, 1.8760108, 0.0415560,
                    0.0134474, -0.1183897, 1.0154096);
  Vec3f xyz = _def * XYZ;
  Vec3f rgb = xyz * RGB;
  return rgb;
}

float getB(Vec3f _def){
    return sqrt((_def[0]*_def[0]) + (_def[1]*_def[1]) + (_def[2]*_def[2]));
}
float getC(Vec3f _def){
    Vec3f def_D = Vec3f(1.,0.,0.);
    return atan2( (def_D.cross(_def)).mag(), _def.dot(def_D) );
}
float getH(Vec3f _def){
    Vec3f def_E_axis = Vec3f(0.,1.,0.);
    return atan2(_def[2], _def[1]) - atan2(def_E_axis[2], def_E_axis[1]) ;
    // return atan2(_def[1], _def[2]) - atan2(def_E_axis[1], def_E_axis[2]) ;
}

// RGB to BCH
Vec3f rgb2bch(Vec3f _col){
  Vec3f def = rgb2def(_col);
  return Vec3f( getB(def), getC(def), getH(def) );
}

// BCH to RGB
Vec3f bch2rgb(Vec3f _bch){
  Vec3f def;
  def[0] = _bch[0] * cos(_bch[1]);
  def[1] = _bch[0] * sin(_bch[1]) * cos(_bch[2]);
  def[2] = _bch[0] * sin(_bch[1]) * sin(_bch[2]);
  return def2rgb(def);
}

///////////////////////////////////////////////////////////////////////
// H S V   C O L O R S P A C E
///////////////////////////////////////////////////////////////////////
Vec3f rgb2HSV(Vec3f _col){
  Vec3f hsv;
  float mini = 0.0;
  float maxi = 0.0;
  if (_col[0] < _col[1]) mini = _col[0];
    else mini = _col[1];
  if (_col[2] < mini) mini = _col[2];
  if (_col[0] > _col[1]) maxi = _col[0];
    else maxi = _col[1];
  if (_col[2] > maxi) maxi = _col[2];
  hsv[2] = maxi; //VALUE
  float delta = maxi - mini; //delta
  if (maxi > 0.0) hsv[1] = delta / maxi; //SATURATION
    else hsv[1] = 0.0;
  if (_col[0] >= maxi) hsv[0] = (_col[1] - _col[2]) / delta;
  else if (_col[1] >= maxi) hsv[0] = 2.0 + (_col[2] - _col[0])/delta;
  else hsv[0] = 4.0 + (_col[0] - _col[1]) / delta;
  hsv[0] *= 60.0;
  if (hsv[0] < 0.0) hsv[0] += 360.0;
  return hsv;
}

Vec3f hsv2RGB(Vec3f _hsv){
  float hh, p, q, t, ff;
  int i;
  Vec3f rgb;
  if(_hsv[1] <= 0.0){
    rgb[0] = _hsv[2];
    rgb[1] = _hsv[2];
    rgb[2] = _hsv[2];
    return rgb;
  }
  hh = _hsv[0];
  if(hh >= 360.) hh = (hh/360.);
  hh /= 60.0;
  i = int(hh);
  ff = hh - float(i);
  p = _hsv[2] * (1.0 - _hsv[1]);
  q = _hsv[2] * (1.0 - (_hsv[1] * ff));
  t = _hsv[2] * (1.0 - (_hsv[1] * (1.0 - ff)));

  if (i == 0){
      rgb[0] = _hsv[2];
      rgb[1] = t;
      rgb[2] = p;
      return rgb;
    }
  else if (i == 1){
      rgb[0] = q;
      rgb[1] = _hsv[2];
      rgb[2] = p;
      return rgb;
    }
  else if (i == 2){
      rgb[0] = p;
      rgb[1] = _hsv[2];
      rgb[2] = t;
      return rgb;
    }
  else if (i == 3){
      rgb[0] = p;
      rgb[1] = q;
      rgb[2] = _hsv[2];
      return rgb;
    }
  else if (i == 4){
      rgb[0] = t;
      rgb[1] = p;
      rgb[2] = _hsv[2];
      return rgb;
    }
  else if (i == 5){
      rgb[0] = _hsv[2];
      rgb[1] = p;
      rgb[2] = q;
      return rgb;
    }
  else {
      rgb[0] = _hsv[2];
      rgb[1] = p;
      rgb[2] = q;
    return rgb;
  }
}

} // al::
