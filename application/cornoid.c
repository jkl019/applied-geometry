#include <QDebug>
namespace my_namespace {


    template <typename T>
    Cornoid<T>::Cornoid(){
    }

  template <typename T>
  Cornoid<T>::~Cornoid() {}

  template <typename T>
  bool Cornoid<T>::isClosed() const {
    return true;
  }

  template <typename T>
  void Cornoid<T>::eval( T t, int d, bool /*l*/ ) const
  {
      this->_p.setDim( d + 1 );

      const T x = 5 * cos(t) * cos(2*t);
      const T y = 5 * sin(t)*(2 + cos(2*t));

      this->_p[0][0] = x;
      this->_p[0][1] = y;
      this->_p[0][2] = T(0);
  }

  template <typename T>
  T Cornoid<T>::getStartP() const {
    return T(0);
  }

  template <typename T>
  T Cornoid<T>::getEndP() const {
    return T( M_2PI );
  }


} // END namespace GMlib
