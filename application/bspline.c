
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
    }

  template <typename T>
  BSpline<T>::~BSpline() {}

  template <typename T>
  bool BSpline<T>::isClosed() const {
    return false;
  }

  template <typename T>
  void BSpline<T>::eval( T t, int d, bool /*l*/ ) const {
      int index = findIndex(t);

      auto w1 = findW(t, index, 1);
      auto w2 = findW(t, index - 1, 2);
      auto w2i = findW(t, index, 2);

      auto p1 = ( 1 - w1) * (1 - w2);
      auto p2 = (( 1 - w1) * w2 + w1 * (1 - w2i));
      auto p3 = w1 * w2i;

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
  void BSpline<T>::makeKnots(int n, bool closed = false){
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
  int BSpline<T>::findIndex(T t) const {
      for(int i = _d; i < _c.getDim(); i++){
          if (t <= _t[i + 1]){
              return i;
          }
      }
  }

  template <typename T>
  T BSpline<T>::findW(T t, int i, int d) const {
      return  (t - _t[i]) / (_t[i + d] - _t[i]);
  }


} // END namespace GMlib
