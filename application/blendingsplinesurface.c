#include <QDebug>
namespace my_namespace {

    template <typename T>
    Blendingsplinesurface<T>::Blendingsplinesurface(GMlib::PSurf<T,3> *s, int nU, int nV){
        _surface = s;
        _d = 1;
        _k = _d + 1;
        _nU = nU;
        _nV = nV;
        _tU.resize(_nU + _k);
        _tV.resize(_nV + _k);
        _s.setDim((_surface->isClosedU() ? (nU + 1) : nU),
                  (_surface->isClosedV() ? (nV + 1) : nV));
        makeKnots();
        createLocalSurface();
    }

  template <typename T>
  Blendingsplinesurface<T>::~Blendingsplinesurface() {}

  template <typename T>
  bool Blendingsplinesurface<T>::isClosedU() const {
    return _surface->isClosedU();
  }

  template <typename T>
  bool Blendingsplinesurface<T>::isClosedV() const {
    return _surface->isClosedV();
  }

  template <typename T>
  void Blendingsplinesurface<T>::eval( T u, T v, int d1, int d2, bool l1, bool l2) const
  {
      this->_p.setDim( d1 + 1, d2 + 1 );
      T x_u, y_u, x_d_u, y_d_u;
      T x_v, y_v, x_d_v, y_d_v;

      int i = getBasis(u, x_u, y_u, x_d_u, y_d_u, _tU, true);
      int j = getBasis(v, x_v, y_v, x_d_v, y_d_v, _tV, false);

      auto s_0_0 = _s(i - 1)(j - 1)->evaluateParent(u, v, d1, d2);
      auto s_0_1 = _s(i)(j - 1)->evaluateParent(u, v, d1, d2);
      auto s_1_0 = _s(i - 1)(j)->evaluateParent(u, v, d1, d2);
      auto s_1_1 = _s(i)(j)->evaluateParent(u, v, d1, d2);

      this->_p[0][0] = x_u * x_v * s_0_0(0)(0) + y_u * x_v * s_0_1(0)(0)
              + x_u * y_v * s_1_0(0)(0) + y_u * y_v * s_1_1(0)(0);

      this->_p[0][1] = x_d_u * x_v * s_0_0(0)(0) + x_u * x_v * s_0_0(0)(1)
              + y_d_u * x_v * s_0_1(0)(0) + y_u * x_v * s_0_1(0)(1)
              + x_d_u * y_v * s_1_0(0)(0) + x_u * y_v * s_1_0(0)(1)
              + y_d_u *  y_v * s_1_1(0)(0) + y_u *  y_v * s_1_1(0)(1);

      this->_p[1][0] = x_u * x_d_v * s_0_0(0)(0) + x_u * x_v * s_0_0(1)(0)
              + y_u * x_d_v * s_0_1(0)(0) + y_u * x_v * s_0_1(1)(0)
              + x_u * y_d_v * s_1_0(0)(0) + x_u * y_v * s_1_0(1)(0)
              + y_u *  y_d_v * s_1_1(0)(0) + y_u *  y_v * s_1_1(1)(0);
  }

  template <typename T>
  T Blendingsplinesurface<T>::getStartPU() const {
    return _surface->getParStartU();
  }

  template <typename T>
  T Blendingsplinesurface<T>::getEndPU() const {
    return _surface->getParEndU();
  }

  template <typename T>
  T Blendingsplinesurface<T>::getStartPV() const {
    return _surface->getParStartV();
  }

  template <typename T>
  T Blendingsplinesurface<T>::getEndPV() const {
    return _surface->getParEndV();
  }

  template <typename T>
  void Blendingsplinesurface<T>::localSimulate(double dt) {

        this->sample(_visu[0][0][0], _visu[0][0][1], 1, 1);
  }

  template <typename T>
  inline int Blendingsplinesurface<T>::getBasis(T t, T& p1, T& p2, T& p3, T& p4, std::vector<T> _t, bool flag) const
  {
      int index = findIndex(t, flag);

      auto _b = b(findW(t, index, _t));
      auto _db = db(findDW(index, _t));

      p1 = 1 - _b;
      p2 = _b;
      p3 = -_db;
      p4 = _db;

      return index;
  }

  template <typename T>
  void Blendingsplinesurface<T>::makeKnots()
  {
      if(isClosedU()){
          makeClosedU();
      }
      else {
          makeOpenU();
      }

      if(isClosedV()){
          makeClosedV();
      }
      else {
          makeOpenV();
      }
  }

  template <typename T>
  void Blendingsplinesurface<T>::makeOpenU(){
      for (int i = 0; i < _k; i++){
          _tU[i] = getStartPU();
      }
      for (int i = _k; i < _nU; i++){
          _tU[i] = getStartPU() + i * (_surface->getParDeltaU() / (_nU - 1));
      }
      for (int i = _nU; i < (_nU + _k); i++){
          _tU[i] = getEndPU();
      }
  }

  template <typename T>
  void Blendingsplinesurface<T>::makeOpenV(){
      for (int i = 0; i < _k; i++){
          _tV[i] = getStartPV();
      }
      for (int i = _k; i < _nV; i++){
          _tV[i] = getStartPV() + i * (_surface->getParDeltaU() / (_nV - 1));
      }
      for (int i = _nV; i < (_nV + _k); i++){
          _tV[i] = getEndPV();
      }
  }

  template <typename T>
  void Blendingsplinesurface<T>::makeClosedU(){
      for(int i = 0; i < _nU + 1; i++)
      {
          _tU[i + 1] = getStartPU() + i * (_surface->getParDeltaU() / _nU);
      }
      auto diff = _tU[_tU.size() - 1] - _tU[_tU.size() - 2];
      _tU[0] = _tU[1] - diff;
  }

  template <typename T>
  void Blendingsplinesurface<T>::makeClosedV(){
      for(int i = 0; i < _nV + 1; i++)
      {
          _tV[i + 1] = getStartPV() + i * (_surface->getParDeltaV() / _nV);
      }
      auto diff = _tV[_tV.size() - 1] - _tV[_tV.size() - 2];
      _tV[0] = _tV[1] - diff;
  }

  template <typename T>
  void Blendingsplinesurface<T>::createLocalSurface()
  {

      for(int i = 0; i < _nU; i++)
      {
          for (int j = 0; j < _nV; j++){
              _s[i][j] = new PSimpleSubSurf<T>(_surface, _tU[i], _tU[i + 2], _tU[i + 1],
                      _tV[j], _tV[j + 2], _tV[j + 1]);
              _s[i][j]->toggleDefaultVisualizer();
              _s[i][j]->sample(5, 5, 1, 1);
              _s[i][j]->setCollapsed(true);
              insert(_s[i][j]);
          }
          if(isClosedV()){
              _s[i][_nV] = _s[i][0];
          }
      }
      if(isClosedU()){
          for (int j = 0; j < _nV; j++){
              _s[_nU][j] = _s[0][j];
          }
      }
      if(isClosedU() && isClosedV()){
          _s[_nU][_nV] = _s[0][0];
      }
  }

  template <typename T>
  int Blendingsplinesurface<T>::findIndex(T t, bool flag) const
  {
      if (flag){
          for(int i = _d; i < _nU; i++){
              if (t <= _tU[i + 1]){
                  return i;
              }
          }
      }
      else {
          for(int i = _d; i < _nV; i++){
              if (t <= _tV[i + 1]){
                  return i;
              }
          }
      }
  }

  template <typename T>
  T Blendingsplinesurface<T>::b(T t) const {
      return  3 * std::pow(t, 2) - 2 * std::pow(t, 3);
  }

  template <typename T>
  T Blendingsplinesurface<T>::findW(T t, int i, std::vector<T> _t) const {
      return  (t - _t[i]) / (_t[i + _d] - _t[i]);
  }

  template <typename T>
  T Blendingsplinesurface<T>::db(T t) const {
      return  6 * t - 6 * std::pow(t, 2);
  }

  template <typename T>
  T Blendingsplinesurface<T>::findDW(int i, std::vector<T> _t) const {
      return  1 / ( _t[i + _d] - _t[i]);
  }


} // END namespace GMlib
