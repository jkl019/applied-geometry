#ifndef SUBDIVISION_H
#define SUBDIVISION_H

#include <parametrics/gmpcurve.h>

namespace my_namespace {

    template <typename T>
    class Subdivision : public GMlib::PCurve<T,3> {
      GM_SCENEOBJECT(Subdivision)
    public:
      Subdivision( const GMlib::DVector<GMlib::Vector<T,3>>& p);
      virtual ~Subdivision();

      bool                isClosed() const override;
      void                sample(int m, int d = 0) override;

    protected:
      void                eval(T t, int d, bool l) const override;
      T                   getStartP() const override;
      T                   getEndP()   const override;


      int _n; // Number of intervalls
      GMlib::DVector<GMlib::Vector<T,3>> _points;

      void LaneRiesenfeldClosed(int k, int d);
      int doublePart(int n);
      void smoothPartClosed(int n, int d);

    };
  }
#include "subdivision.c"

#endif // SUBDIVISION_H
