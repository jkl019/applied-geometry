#include <QDebug>
namespace my_namespace {


    template <typename T>
    Clelia<T>::Clelia(T r, int n){
        _r = r;
        _n = n;
    }

  template <typename T>
  Clelia<T>::~Clelia() {}

  template <typename T>
  bool Clelia<T>::isClosed() const {
    return true;
  }

  template <typename T>
  void Clelia<T>::eval( T t, int d, bool /*l*/ ) const
  {
      this->_p.setDim( d + 1 );

      const T x = _r * cos(_n*t) * cos(t);
      const T y = _r * cos(_n*t) * sin(t);
      const T z = _r * sin(_n*t);

      this->_p[0][0] = x;
      this->_p[0][1] = y;
      this->_p[0][2] = z;
  }

  template <typename T>
  T Clelia<T>::getStartP() const {
    return T(0);
  }

  template <typename T>
  T Clelia<T>::getEndP() const {
    return T( M_2PI );
  }


} // END namespace GMlib
