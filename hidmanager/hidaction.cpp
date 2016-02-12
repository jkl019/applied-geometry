#include "hidaction.h"

// qt
#include <QString>

// stl
#include <cassert>

HidAction::HidAction( const QString& group, const QString& name, const QString& description )
  : QObject(), _group(group), _name(name), _description(description) {}

const QString&
HidAction::getGroup() const { return _group; }

const QString&
HidAction::getName() const { return _name; }

const QString&
HidAction::getDescription() const { return _description; }

QString HidAction::getIdentifier() const { return getGroup() + "." + getName(); }

void
HidAction::trigger( const HidInputEvent::HidInputParams& params ) const {

  emit signTrigger(params);

}




HidActionCompareId::HidActionCompareId( const QString& str )
  : _str(str) {}

bool
HidActionCompareId::operator() ( const HidAction* act ) {

  assert(act);
  return act->getIdentifier() == _str;
}



HidActionCompareGroup::HidActionCompareGroup( const QString& str )
  : _str(str) {}

bool
HidActionCompareGroup::operator() ( const HidAction* act ) {

  assert(act);
  return act->getGroup() == _str;
}



HidActionCompareName::HidActionCompareName( const QString& str )
  : _str(str) {}

bool
HidActionCompareName::operator() ( const HidAction* act ) {

  assert(act);
  return act->getName() == _str;
}
