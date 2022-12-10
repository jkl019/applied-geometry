#include <QDebug>
namespace my_namespace {


    template <typename T>
    LissajousCurve<T>::LissajousCurve(){

    }

    template <typename T>
    LissajousCurve<T>::LissajousCurve( const GMlib::DVector<GMlib::Vector<T,3>>& p){

    }

  template <typename T>
  LissajousCurve<T>::~LissajousCurve() {}

  template <typename T>
  bool LissajousCurve<T>::isClosed() const {
    return true;
  }

  template <typename T>
  void LissajousCurve<T>::eval( T t, int d, bool /*l*/ ) const
  {
      this->_p.setDim( d + 1 );

      const T x = sin(2*t);
      const T y = cos(3*t);

      this->_p[0][0] = x;
      this->_p[0][1] = y;
      this->_p[0][2] = T(0);
  }

  template <typename T>
  T LissajousCurve<T>::getStartP() const {
    return T(0);
  }

  template <typename T>
  T LissajousCurve<T>::getEndP() const {
    return T( M_2PI );
  }


} // END namespace GMlib
