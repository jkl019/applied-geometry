#include <QDebug>
namespace my_namespace {

  template <typename T>
  Subdivision<T>::Subdivision( const GMlib::DVector<GMlib::Vector<T,3>>& p)
  {
      _points = p;
      _n = _points.getDim();
  }

  template <typename T>
  Subdivision<T>::~Subdivision() {}

  template <typename T>
  bool Subdivision<T>::isClosed() const {
    return false;
  }

  template <typename T>
  void Subdivision<T>::sample(int m, int d = 0){
       this->_checkSampleVal( m, d );
      //this->setEditDone();
  }

  template <typename T>
  void Subdivision<T>::eval( T t, int d, bool /*l*/ ) const {

  }

  template <typename T>
  T Subdivision<T>::getStartP() const {
    return T;
  }

  template <typename T>
  T Subdivision<T>::getEndP() const {
    return T;
  }

  template <typename T>
  void Subdivision<T>::LaneRiesenfeldClosed(int k, int d)
  {
      int m = pow(2, k) * _n + 1;
      _p.push_back(_p[0]); //Closing curve
      for(int i = 0; i < k; i++){
          _n = doublePart();
          smoothPartClosed(_n, d);
      }
      // s.reset();
      //computeSurroundingSphere(p, s);
  }

  template <typename T>
  int Subdivision<T>::doublePart()
  {
      for(int i = _n - 1; i > 0; i--){

      }
      return (2 * _n) - 1;

  }

  template <typename T>
  void Subdivision<T>::smoothPartClosed(int n, int d)
  {
      for(int j = 1; j < d; j++){
          for(int i = 0; i < (n - 1); i++){

          }
      }

  }


}
