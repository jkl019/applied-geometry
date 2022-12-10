#ifndef BLENDINGSPLINESURFACE_H
#define BLENDINGSPLINESURFACE_H

#include <parametrics/gmpsurf.h>
#include "PSimpleSubSurf.h"

namespace my_namespace {

    template <typename T>
    class Blendingsplinesurface : public GMlib::PSurf<T,3> {
      GM_SCENEOBJECT(Blendingsplinesurface)
    public:
      Blendingsplinesurface(GMlib::PSurf<T,3> *s, int n = 4, int d = 1);
      virtual ~Blendingsplinesurface();

      bool                isClosedU() const override;
      bool                isClosedV() const override;

    protected:
      void                eval(T u, T v, int d1, int d2, bool l1, bool l2) const override;
      T                   getStartPU() const override;
      T                   getEndPU()   const override;
      T                   getStartPV() const override;
      T                   getEndPV()   const override;
      void                localSimulate(double dt) override;

      int _d;
      int _k;
      int _nU;
      int _nV;
      std::vector<T> _tU;
      std::vector<T> _tV;
      GMlib::DMatrix<GMlib::PSurf<T,3>*> _s;
      GMlib::PSurf<T,3> *_surface;

      int getBasis(T t, T& p1, T& p2, T& p3, T& p4, std::vector<T> _t, bool flag) const;
      void makeKnots();
      void makeOpenU();
      void makeOpenV();
      void makeClosedU();
      void makeClosedV();
      void createLocalSurface();
      int findIndex(T t, bool flag) const ;
      T b(T t) const;
      T findW(T t, int i, std::vector<T> _t) const;
      T db(T t) const;
      T findDW(int i, std::vector<T> _t) const;

    };
  }
#include "blendingsplinesurface.c"

#endif // BLENDINGSPLINESURFACE_H
