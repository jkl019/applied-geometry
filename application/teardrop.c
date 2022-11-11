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

      const T x = 5 * cos(t);
      const T y = 5 * sin(t) * pow(sin(t/2), 2);

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
    return T( M_2PI );
  }


} // END namespace GMlib
