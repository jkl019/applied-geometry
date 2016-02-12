#include "hidmanagermodel.h"


HidManagerModel::HidManagerModel( HidManager* hidmanager, QObject* parent )
  : QAbstractTableModel(parent), _hm(hidmanager) { assert(_hm); }

int
HidManagerModel::rowCount(const QModelIndex& /*parent*/ ) const {

  return _hal.size();
}

int
HidManagerModel::columnCount(const QModelIndex& /*parent*/ ) const {

  return 3;
}

QVariant
HidManagerModel::data(const QModelIndex &index, int role) const {

  if( !index.isValid() || role != Qt::DisplayRole )
    return QVariant();

  const HidAction* action = _hal.at(index.row());
  if( !action )
    return QVariant();

  switch( index.column() ) {
    case 0:   return QVariant( action->getName() );
    case 1:   return QVariant( action->getDescription() );
    case 2:   {

      HidManager::HidBindings::const_iterator itr =
          std::find_if( _hb.begin(), _hb.end(), HidBindingCompare(action->getIdentifier()) );

      if( itr == _hb.end() )
        return QVariant( "N/A" );
      else
        return QVariant( (*itr).getHidBindingText() );

    }
    default:  return QVariant();
  }

  return QVariant();

}


QVariant
HidManagerModel::headerData(int section, Qt::Orientation orientation, int role) const {

  if(orientation == Qt::Horizontal )
    return QVariant();

  if( role != Qt::DisplayRole )
    return QVariant();

  switch( section ) {
    case 0:   return QVariant("Name");
    case 1:   return QVariant("Description");
    case 2:   return QVariant("Hid Binding");
    default:  return QVariant();
  };

}

void HidManagerModel::update(const HidManager::HidActions &actions) {

  beginResetModel();
  _hal = actions.toList();
  endResetModel();
}

void HidManagerModel::update(const HidManager::HidBindings &bindings) {

  beginResetModel();
  _hb = bindings;
  endResetModel();
}



