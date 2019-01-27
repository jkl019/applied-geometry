#ifndef SCENARIO_H
#define SCENARIO_H


#include "application/gmlibwrapper.h"

// qt
#include <QObject>

namespace GMlib {
  template <typename T> class TriangleFacets;
}

class Scenario : public GMlibWrapper {
  Q_OBJECT
public:
  using GMlibWrapper::GMlibWrapper;

  void    initializeScenario() override;
  void    cleanupScenario() override;


private:
  void initializeTriangleFacetExample();
  void cleanupTriangleFacetExample();

  void initializeTestTorusExample();
  void cleanupTestTorusExample();

  std::shared_ptr<TestTorus>                    m_test_torus{nullptr};
  std::shared_ptr<GMlib::TriangleFacets<float>> m_triangle_facets{nullptr};
};

#endif // SCENARIO_H
