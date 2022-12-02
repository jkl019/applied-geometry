#include <QDebug>
namespace my_namespace {

    template <typename T>
    Blendingsplinecurve<T>::Blendingsplinecurve(GMlib::PCurve<T,3> *mc, int n, int d){
        _mc = mc;
        _n = n;
        _d = d;
        _k = d + 1;
        _t.resize(_n + _k);
        makeKnots();
        createLocalCurves();
    }

  template <typename T>
  Blendingsplinecurve<T>::~Blendingsplinecurve() {}

  template <typename T>
  bool Blendingsplinecurve<T>::isClosed() const {
    return true;
  }

  template <typename T>
  void Blendingsplinecurve<T>::eval( T t, int d, bool /*l*/ ) const {
      T p1, p2;
      int index = getBasis(t, p1, p2);

      this->_p.setDim( d + 1 );
      this->_p = p1 * _lc[index - 1]->evaluateParent(t, d) +
              p2 * _lc[index]->evaluateParent(t, d);
  }

  template <typename T>
  T Blendingsplinecurve<T>::getStartP() const {
    return _mc->getParStart();
  }

  template <typename T>
  T Blendingsplinecurve<T>::getEndP() const {
    return _mc->getParEnd();
  }

  template <typename T>
  inline int Blendingsplinecurve<T>::getBasis(T t, T& p1, T& p2) const
  {
      int index = findIndex(t);

      auto _b = b(findW(t, index));

      p1 = 1 - _b;
      p2 = _b;

      return index;
  }

  template <typename T>
  void Blendingsplinecurve<T>::makeKnots()
  {
      for(int i = 0; i < _n + 1; i++)
      {
          _t[i + 1] = getStartP() + i * (_mc->getParDelta() / _n);
      }
      auto diff = _t[_t.size() - 1] - _t[_t.size() - 2];
      _t[0] = _t[1] - diff;
  }

  template <typename T>
  void Blendingsplinecurve<T>::createLocalCurves()
  {
      for(int i = 0; i < _n; i++)
      {
          _lc.push_back(new GMlib::PSubCurve<T>(_mc, _t[i], _t[i + 2], _t[i + 1]));
      }
  }

  template <typename T>
  int Blendingsplinecurve<T>::findIndex(T t) const
  {
      for(int i = _d; i < _lc.size(); i++){
          if (t <= _t[i + 1]){
              return i;
          }
      }
      return _lc.size();
  }

  template <typename T>
  T Blendingsplinecurve<T>::b(T t) const {
      return  t - ( (1 / M_2_PI) * sin(M_2_PI * t) );
  }

  template <typename T>
  T Blendingsplinecurve<T>::findW(T t, int i) const {
      return  (t - _t[i]) / (_t[i + _d] - _t[i]);
  }


} // END namespace GMlib
