#include <QDebug>
namespace my_namespace {

    template <typename T>
    BSpline<T>::BSpline( const GMlib::DVector<GMlib::Vector<T,3>>& c){
        _c = c;
        _d = 2;
        _k = 3;
        makeKnots(_c.getDim());
    }

    template <typename T>
    BSpline<T>::BSpline( const GMlib::DVector<GMlib::Vector<T,3>>& p, int n){
        _points = p;
        _c.setDim(n);
        _n = n;
        _d = 2;
        _k = 3;
        makeKnots(_n);
        makeControlPoints(p);
    }

  template <typename T>
  BSpline<T>::~BSpline() {}

  template <typename T>
  bool BSpline<T>::isClosed() const {
    return false;
  }

  template <typename T>
  void BSpline<T>::eval( T t, int d, bool /*l*/ ) const {
      T p1, p2, p3;
      int index = getBasis(t, p1, p2, p3);

      this->_p.setDim( d + 1 );
      this->_p[0] = p1 * (_c[index - 2]) + p2 * (_c[index - 1]) + p3 * _c[index];
  }

  template <typename T>
  T BSpline<T>::getStartP() const {
    return _t[_d];
  }

  template <typename T>
  T BSpline<T>::getEndP() const {
    return _t[_c.getDim()];
  }

  template <typename T>
  inline int BSpline<T>::getBasis(T t, T& p1, T& p2, T& p3) const
  {
      int index = findIndex(t);

      auto w1 = findW(t, index, 1);
      auto w2 = findW(t, index - 1, 2);
      auto w2i = findW(t, index, 2);

      p1 = ( 1 - w1) * (1 - w2);
      p2 = (( 1 - w1) * w2 + w1 * (1 - w2i));
      p3 = w1 * w2i;

      return index;
  }

  template <typename T>
  void BSpline<T>::makeKnots(int n, bool closed = false)
  {
      int index = 0;
      for(int i = 0; i < n; i++){
          if(i < _k){
              _t.push_back(0);
          }
          else {
              index++;
              _t.push_back(index);
          }
      }
      index++;
      for(int i = 0; i < 3; i++){
          _t.push_back(index);
      }
  }

  template <typename T>
  int BSpline<T>::findIndex(T t) const
  {
      for(int i = _d; i < _c.getDim(); i++){
          if (t <= _t[i + 1]){
              return i;
          }
      }
      return _c.getDim();
  }

  template <typename T>
  void BSpline<T>::makeControlPoints(const GMlib::DVector<GMlib::Vector<T,3>>& p)
  {
      auto A = getMatrixA();
      auto transpose_A = A;
      transpose_A.transpose();
      auto b = transpose_A * p;
      auto B = transpose_A * A;
      B.invert();
      auto c = B * b;
      _c = c;
  }

  template <typename T>
  GMlib::DMatrix<T> BSpline<T>::getMatrixA()
  {
      GMlib::DMatrix<T> A (_points.getDim(), _n, T(0));
      T p1, p2, p3;

      for(int j = 0; j < _points.getDim(); j++){
          T t = this->getParStart() + j * (this->getParDelta() / (_points.getDim() - 1));
          int index = getBasis(t, p1, p2, p3);

          A[j][index] = p3;
          A[j][index - 1] = p2;
          A[j][index - 2] = p1;
      }
      return A;
  }

  template <typename T>
  T BSpline<T>::findW(T t, int i, int d) const {
      return  (t - _t[i]) / (_t[i + d] - _t[i]);
  }


} // END namespace GMlib
