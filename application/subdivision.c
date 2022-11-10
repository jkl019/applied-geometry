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
  void Subdivision<T>::sample(int m, int d = 0)
  {

      this->_visu.resize(1);
      this->_checkSampleVal( m, d );
      LaneRiesenfeldClosed(m, d);
      _visu[0].sample_val.resize(_points.getDim());
      for(int i = 0; i < _points.getDim(); i++){
          _visu[0].sample_val[i].setDim(1);
          _visu[0].sample_val[i][0] = _points[i];
      }
      _visu[0].sur_sphere.reset();
      computeSurroundingSphere(_visu[0].sample_val, _visu[0].sur_sphere);
      this->setEditDone();
  }

  template <typename T>
  void Subdivision<T>::eval( T t, int d, bool /*l*/ ) const {

  }

  template <typename T>
  T Subdivision<T>::getStartP() const {
    return T(0);
  }

  template <typename T>
  T Subdivision<T>::getEndP() const {
    return T(1);
  }

  template <typename T>
  void Subdivision<T>::LaneRiesenfeldClosed(int k, int d)
  {
      int m = pow(2, k) * _n + 1; // Final number of points
      _points.push_back(_points[0]); //Closing curve
      _points.increaseDim(m - _points.getDim(), T(0), true); // Increasing number of points
      _n++;
      for(int i = 0; i < k; i++)
      {
          _n = doublePart(_n);
          smoothPartClosed(_n, d);
      }
  }

  template <typename T>
  int Subdivision<T>::doublePart(int n)
  {
      for(int i = n - 1; i > 0; i--)
      {
          _points[2*i] = _points[i]; // Move point
          _points[2*i -1] = 0.5 * (_points[i] + _points[i -1]); // Make new point
      }
      return (2 * n) - 1;

  }

  template <typename T>
  void Subdivision<T>::smoothPartClosed(int n, int d)
  {
      for(int j = 1; j < d; j++){
          for(int i = 0; i < (n - 1); i++)
          {
              _points[i] =  0.5 * (_points[i] + _points[i + 1]);
          }
          _points[n - 1] = _points[0];
      }

  }


}
