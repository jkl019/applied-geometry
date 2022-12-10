#include <QDebug>
namespace my_namespace {


    template <typename T>
    Teardrop<T>::Teardrop(){
    }

  template <typename T>
  Teardrop<T>::~Teardrop() {}

  template <typename T>
  bool Teardrop<T>::isClosed() const {
    return true;
  }

  template <typename T>
  void Teardrop<T>::eval( T t, int d, bool /*l*/ ) const
  {
      this->_p.setDim( d + 1 );

      const T x = 5 * pow(cos(t), 2);
      const T y = pow(5, 2) * pow(cos(t), 3) * sin(t) / 4;

      this->_p[0][0] = x;
      this->_p[0][1] = y;
      this->_p[0][2] = T(0);
  }

  template <typename T>
  T Teardrop<T>::getStartP() const {
    return T(0);
  }

  template <typename T>
  T Teardrop<T>::getEndP() const {
    return T(M_PI);
  }


} // END namespace GMlib
