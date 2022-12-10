#include <QDebug>
namespace my_namespace {


    template <typename T>
    Hypotrochoids<T>::Hypotrochoids(){

    }

  template <typename T>
  Hypotrochoids<T>::~Hypotrochoids() {}

  template <typename T>
  bool Hypotrochoids<T>::isClosed() const {
    return true;
  }

  template <typename T>
  void Hypotrochoids<T>::eval( T t, int d, bool /*l*/ ) const
  {
      this->_p.setDim( d + 1 );

      const T x = 1 * sin(2*t) - sin(3*t);
      const T y = 1 * cos(2*t) + cos(3*t);

      this->_p[0][0] = x;
      this->_p[0][1] = y;
      this->_p[0][2] = T(0);
  }

  template <typename T>
  T Hypotrochoids<T>::getStartP() const {
    return T(0);
  }

  template <typename T>
  T Hypotrochoids<T>::getEndP() const {
    return T( M_2PI );
  }


} // END namespace GMlib
