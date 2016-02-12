#include "hidmanagertreemodel.h"


class TreeItem {
public:
  enum TreeItemType {
    RootType,
    GroupType,
    EntryType
  };


  explicit TreeItem(const QList<QVariant> &data, TreeItem *parent = 0x0)
    : _children(), _data(data), _parent(parent) {}

  ~TreeItem() { qDeleteAll(_children); }

  void          appendChild(TreeItem *child) {

    _children.append(child);
  }

  void          clear() { _children.clear(); }
  TreeItem*     child(int row) { return _children.value(row); }
  int           childCount() const { return _children.count(); }
  int           columnCount() const { return 3; return _data.count(); }
  QVariant      data(int column) const {


    return _data.value(column);

  }

  void          setData(int column, const QVariant& data ) {

    if( _data.count() > column )
      _data[column] = data;
  }

  int           row() const {

    if(_parent)
      return _parent->_children.indexOf( const_cast<TreeItem*>(this) );

    return 0x0;
  }
  TreeItem*     parent() { return _parent;  }

private:
  QList<TreeItem*>    _children;
  QList<QVariant>     _data;
  TreeItem            *_parent;

};

//class RootItem : public TreeItem {
//public:
//  RootItem( const QList<QVariant> & data ) : TreeItem( data, 0x0 ) {}

//  void      update( const HidManager::HidActions& a ) {

//    for( int i = 0; i < childCount(); ++i ) {

//      GroupItem *gi = dynamic_cast<GroupItem*>( child(i) );
//      if( !gi )
//        continue;

//      HidManager::HidActions::const_iterator a_itr =
//          std::find_if(a.begin(),a.end(),HidActionCompareGroup( gi->getGroupName() ) );
//      if( a_itr == a.end() )
//        removeChildAt(i);
//    }

//  }


//};




//class GroupItem : public TreeItem {
//public:
//  GroupItem( const QVariant& name, TreeItem *parent )
//    : TreeItem( QList<QVariant>(), parent ) {

//  }

//  QString       getGroupName() const {  return data(0).toString(); }

//};





























HidManagerTreeModel::HidManagerTreeModel(HidManager *hidmanager, QObject *parent)
  : _hm(hidmanager) {

  assert(_hm);

  QList<QVariant>   root_data;
  root_data << "Name" << "Description" << "Binding";

  _root = new TreeItem( root_data );
}

HidManagerTreeModel::~HidManagerTreeModel() {

  delete _root;
}

QModelIndex HidManagerTreeModel::index(int row, int column, const QModelIndex &parent) const {

  if (!hasIndex(row, column, parent))
      return QModelIndex();

  TreeItem *parent_item;

  if (!parent.isValid())
      parent_item = _root;
  else
      parent_item = static_cast<TreeItem*>(parent.internalPointer());

  TreeItem *child_item = parent_item->child(row);
  if (child_item)
      return createIndex(row, column, child_item);
  else
    return QModelIndex();
}

QModelIndex HidManagerTreeModel::parent(const QModelIndex &child) const {

  if (!child.isValid())
      return QModelIndex();

  TreeItem *child_item = static_cast<TreeItem*>(child.internalPointer());
  TreeItem *parent_item = child_item->parent();

  if (parent_item == _root)
      return QModelIndex();

  return createIndex(parent_item->row(), 0, parent_item);
}

int HidManagerTreeModel::rowCount(const QModelIndex &parent) const {

  TreeItem *parent_item;
  if (parent.column() > 0)
      return 0;

  if (!parent.isValid())
      parent_item = _root;
  else
      parent_item = static_cast<TreeItem*>(parent.internalPointer());

  return parent_item->childCount();
}

int HidManagerTreeModel::columnCount(const QModelIndex &parent) const {

  if(parent.isValid()) {
    return static_cast<TreeItem*>(parent.internalPointer())->columnCount();
//    int no_cols = 0;
//    TreeItem *item = static_cast<TreeItem*>(parent.internalPointer());
//    for( int i = 0; i < item->childCount(); ++i )
//      no_cols = qMax(item->child(i)->childCount(),no_cols);
//    return no_cols;
  }
  else
    return _root->columnCount();
}

QVariant HidManagerTreeModel::data(const QModelIndex &index, int role) const {

  if (!index.isValid())
      return QVariant();

  if ( !(role == Qt::UserRole+1 || role == Qt::UserRole+2 || role == Qt::UserRole+3 ) )
    return QVariant( "no");

  TreeItem *item = static_cast<TreeItem*>(index.internalPointer());

  switch( role ) {
    case Qt::UserRole+1: return item->data(0);
    case Qt::UserRole+2: return item->data(1);
    case Qt::UserRole+3: return item->data(2);
  }


//  if (!index.isValid())
//      return QVariant();

//  if (role != Qt::DisplayRole)
//      return QVariant();

//  TreeItem *item = static_cast<TreeItem*>(index.internalPointer());
//  return item->data(index.column());
}

Qt::ItemFlags HidManagerTreeModel::flags(const QModelIndex &index) const {

  if (!index.isValid())
         return 0;

  return QAbstractItemModel::flags(index);
}

QVariant HidManagerTreeModel::headerData(int section, Qt::Orientation orientation, int role) const {

  if (orientation == Qt::Horizontal && role == Qt::DisplayRole)
      return _root->data(section);

  return QVariant();
}

void HidManagerTreeModel::update(const HidManager::HidActions &a) {

  _data.clear();
  HidManager::HidActions::const_iterator ha_itr;
  for( ha_itr = a.begin(); ha_itr != a.end(); ++ha_itr ) {

    HidEntryInfoList heil;
    heil.append((*ha_itr)->getName());
    heil.append((*ha_itr)->getDescription());
    heil.append("");
    heil.append((*ha_itr)->getIdentifier());


    HidEntryMap entry_map = _data.value( (*ha_itr)->getGroup() );

    entry_map[(*ha_itr)->getName()] = heil;
    _data[(*ha_itr)->getGroup()] = entry_map;
  }


  beginResetModel();

  _root->clear();

  // Groups
  HidGroupMap::iterator g_itr = _data.begin();
  while( g_itr !=  _data.end() ) {

    QString group_name = g_itr.key();
    HidEntryMap entry_map = g_itr.value();

    QList<QVariant> group_data;
    group_data << group_name;
    TreeItem *group_item = new TreeItem(group_data,_root);
    _root->appendChild( group_item );


    HidEntryMap::iterator e_itr = entry_map.begin();
    while( e_itr != entry_map.end() ) {

//      QString entry_name = e_itr.key();
      HidEntryInfoList entry_info = e_itr.value();

      QList<QVariant> entry_data;
      HidEntryInfoList::iterator ei_itr = entry_info.begin();
//      for( int i = 0; i < 2 && ei_itr != entry_info.end(); ++i, ++ei_itr ) {
      for( ; ei_itr != entry_info.end(); ++ei_itr ) {

        entry_data.append( QVariant(*ei_itr) );
      }

      group_item->appendChild( new TreeItem( entry_data, group_item ) );

      ++e_itr;
    }

    ++g_itr;
  }

  endResetModel();


//  beginResetModel();

//  for( int i = 0; i < ( _root->childCount() > _hal.count(); ++i ) {

//    TreeItem *item = _root->child(i);
//  }

//  endResetModel();

//  HidManager::HidActions::const_iterator ha_itr;
//  for( ha_itr = actions.begin(); ha_itr != actions.end(); ++ha_itr ) {


//  }


}

void HidManagerTreeModel::update(const HidManager::HidBindings &b) {

  _hb = b;

  // groups
  for( int i = 0; i < _root->childCount(); ++i ) {

    // entries
    for( int j = 0; j < _root->child(i)->childCount(); ++j ) {

      QString identifier = _root->child(i)->child(j)->data(3).toString();
      HidManager::HidBindings::const_iterator itr =
          std::find_if(b.begin(), b.end(), HidBindingCompare( identifier ) );

      if( itr != b.end() )
        _root->child(i)->child(j)->setData(2, (*itr).getHidBindingText() );
    }
  }
}

QHash<int, QByteArray> HidManagerTreeModel::roleNames() const {

  QHash<int,QByteArray> role_names = QAbstractTableModel::roleNames();
  role_names[Qt::UserRole+1]          = "name";
  role_names[Qt::UserRole+2]          = "description";
  role_names[Qt::UserRole+3]          = "binding";

  return role_names;
}










