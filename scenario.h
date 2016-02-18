#ifndef SCENARIO_H
#define SCENARIO_H


#include "application/guiapplication.h"

// qt
#include <QObject>



class Scenario : public GuiApplication {
  Q_OBJECT
public:
  using GuiApplication::GuiApplication;

  void    initializeScenario() override;
  void    cleanupScenario() override;

};

#endif // SCENARIO_H
