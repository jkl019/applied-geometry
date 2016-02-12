#ifndef HIDBINDINGSMODEL_H
#define HIDBINDINGSMODEL_H


class TreeItem;

// local
#include "hidmanager.h"
#include "hidaction.h"

// qt
#include <QAbstractItemModel>

class HidManagerTreeModel : public QAbstractTableModel {
  Q_OBJECT
public:
  explicit HidManagerTreeModel( HidManager* hidmanager, QObject* parent = 0x0 );
  ~HidManagerTreeModel();

  QModelIndex     index(int row, int column, const QModelIndex &parent) const;
  QModelIndex     parent(const QModelIndex &child) const;
  int             rowCount(const QModelIndex &parent) const;
  int             columnCount(const QModelIndex &parent) const;

  QVariant        data(const QModelIndex &index, int role = Qt::DisplayRole ) const;

  Qt::ItemFlags   flags(const QModelIndex &index) const;

  QVariant        headerData(int section, Qt::Orientation orientation, int role) const;

  void            update( const HidManager::HidActions& action );
  void            update( const HidManager::HidBindings& bindings );

  QHash< int, QByteArray >    roleNames() const;

private:
  typedef QList<QString>                    HidEntryInfoList;
  typedef QMap<QString, HidEntryInfoList>   HidEntryMap;
  typedef QMap<QString, HidEntryMap>        HidGroupMap;

  HidManager                    *_hm;
  HidManager::HidActions        _hal;
  HidManager::HidBindings       _hb;

  HidGroupMap                   _data;


  TreeItem                      *_root;

}; // END class HidBindingsModel

#endif // HIDBINDINGSMODEL_H
