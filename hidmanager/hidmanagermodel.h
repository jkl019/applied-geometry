#ifndef HIDBINDINGSMODEL_H
#define HIDBINDINGSMODEL_H


// local
#include "hidmanager.h"
#include "hidaction.h"

// qt
#include <QAbstractTableModel>

class HidManagerModel : public QAbstractTableModel {
  Q_OBJECT
public:
  HidManagerModel( HidManager* hidmanager, QObject* parent = 0x0 );

  /* virtual from QAbstractTableModel */
  int           rowCount(const QModelIndex& /*parent*/ ) const;
  int           columnCount(const QModelIndex& /*parent*/ ) const;
  QVariant      data(const QModelIndex &index, int role) const;
  QVariant      headerData(int section, Qt::Orientation orientation, int role) const;


  void          update( const HidManager::HidActions& action );
  void          update( const HidManager::HidBindings& bindings );

private:
  HidManager                    *_hm;

  HidManager::HidActionsList    _hal;
  HidManager::HidBindings       _hb;

}; // END class HidBindingsModel

#endif // HIDBINDINGSMODEL_H
