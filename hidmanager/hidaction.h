#ifndef HIDACTION_H
#define HIDACTION_H


#include "hidinputevent.h"

// qt
#include <QObject>



class HidAction : public QObject {
  Q_OBJECT

public:
  HidAction( const QString& group, const QString& name, const QString& description = QString() );

  const QString& getGroup() const;
  const QString& getName() const;
  const QString& getDescription() const;

  QString         getIdentifier() const;

  void    trigger( const HidInputEvent::HidInputParams& params ) const;

private:
  QString   _name;
  QString   _group;
  QString   _description;

signals:
  void  signTrigger( const HidInputEvent::HidInputParams& params ) const;
};



struct HidActionCompareId : public std::unary_function<const HidAction*, bool> {

  explicit HidActionCompareId( const QString& str );
  bool    operator() ( const HidAction* act );
  QString         _str;
};

struct HidActionCompareGroup : public std::unary_function<const HidAction*, bool> {

  explicit HidActionCompareGroup( const QString& str );
  bool    operator() ( const HidAction* act );
  QString         _str;
};

struct HidActionCompareName : public std::unary_function<const HidAction*, bool> {

  explicit HidActionCompareName( const QString& str );
  bool    operator() ( const HidAction* act );
  QString         _str;
};



#endif // HIDACTION_H
