#include "qmemorymodel.h"

QMemoryModel::QMemoryModel(QObject *parent) : MemoryWrapper(parent)
{
}

QMemoryModel::~QMemoryModel()
{
}

int QMemoryModel::getColumnCount() const
{
  return columnCount_;
}

void QMemoryModel::setColumnCount(int columnCount)
{
  columnCount_ = columnCount;
  emit headerDataChanged(Qt::Horizontal, 0, columnCount_-1);
}

QModelIndex QMemoryModel::index(int row, int column, const QModelIndex &parent) const
{
  if (!hasIndex(row, column, parent)) {
      return QModelIndex();
  }

  auto me = getME();
  if (parent.isValid()) {
    me = getMeByIndex(parent);
  }

  return createIndex(row, column, me.getByI(row).getMe());
}

QModelIndex QMemoryModel::parent(const QModelIndex &child) const
{
  if (!child.isValid()) {
      return QModelIndex();
  }

  auto me = getMeByIndex(child);
  if(!me)
    return QModelIndex();

  auto parentME = me.parent();
  if (parentME && parentME != getME()) { // parent запрашивается не у корневого элемента
      return createIndex(parentME.getIndex(), 0, parentME.getMe());
  }
  else {
      return QModelIndex();
  }
}

int QMemoryModel::rowCount(const QModelIndex &parent) const
{
  if (!parent.isValid())
    return getME().count();

  auto meP = getMeByIndex(parent);
  return meP.count();
}

int QMemoryModel::columnCount(const QModelIndex &parent) const
{
  return columnCount_;
}

QVariant QMemoryModel::data(const QModelIndex &index, int role) const
{
  QVariant var;
  if (!index.isValid()) {
    return var;
  }

  if(role != Qt::DisplayRole && role != Qt::EditRole && role != Qt::ToolTipRole && ValueRole != role)
    return var;

  auto me = getMeByIndex(index);

  if(ValueRole == role) {
    return me.val();
  }

  switch (index.column()) {
    case NameColumn:
    {
      var = me.name();
      if(var.toString().isEmpty())
        var = "< >";
    } break;

    case PathColumn:
      if(role == Qt::DisplayRole)
        var = me.getPath();
      break;

    case ValueColumn: {
      var = me.val();
      const auto type = var.type();
      if(type == QMetaType::QVariantList) {
        QStringList list = var.toStringList();
        var = list.join(",");
      }
      else if(type == QMetaType::QObjectStar) {
        auto x = (uint)(qvariant_cast<QObject*>(var));
        var = QString("0x%1").arg(x,0,16).toUpper();
      }
      else {
        if(role == Qt::DisplayRole)
        {
          QStringList list(me.val().toString().split("\n"));

          if(!list.empty())
            var = list[0];
        }
        else {

        }
      }
    }
    break;

    default:
        break;
  }
  return var;
}

bool QMemoryModel::setData(const QModelIndex &index, const QVariant &value, int role)
{
  if (!index.isValid()) {
    return false;
  }
  if (role != Qt::EditRole) {
    return false;
  }

  bool res = true;
  auto val = value.toString();
  auto me = getMeByIndex(index);
  Q_ASSERT(me);

  switch(index.column())
  {
  case NameColumn:
    me.setName(val);
    break;

  case ValueColumn:
    me.setVal(value);
    break;

  default:
    res = false;
  }

  return res;
}

QVariant QMemoryModel::headerData(int section, Qt::Orientation orientation, int role) const
{
  //return inherited::headerData(section, orientation, role);
  if (orientation == Qt::Horizontal && role == Qt::DisplayRole && section < columnCount_)
  {
    return headers[section];
  }
  return QVariant();
}

bool QMemoryModel::setHeaderData(int section, Qt::Orientation orientation, const QVariant &value, int role)
{
  //inherited::setHeaderData(section, orientation, value, role);
  if (orientation == Qt::Horizontal && role == Qt::DisplayRole && section < columnCount_)
  {
    headers[section] = value.toString();
    emit headerDataChanged(orientation, section, section);
    return true;
  }
  return false;
}

bool QMemoryModel::canFetchMore(const QModelIndex &parent) const
{
  return inherited::canFetchMore(parent);
}

void QMemoryModel::fetchMore(const QModelIndex &parent)
{
  return inherited::fetchMore(parent);
}

Qt::ItemFlags QMemoryModel::flags(const QModelIndex &index) const
{
  Qt::ItemFlags flags = inherited::flags(index);

  bool canEdit = index.isValid()
      && (index.column() == NameColumn || index.column() == ValueColumn);

  if (canEdit) {
    auto me = getMeByIndex(index);
    if (me != getME()) {
        flags |= Qt::ItemIsEditable;
    }
  }

  if (index.isValid())
    flags |= Qt::ItemIsDragEnabled | Qt::ItemIsDropEnabled;
  else
    flags |= Qt::ItemIsDropEnabled;

  return flags;
}

bool QMemoryModel::hasChildren(const QModelIndex &parent) const
{
  if (parent.isValid()) {
    auto meP = getMeByIndex(parent);
    Q_ASSERT(meP);
    return meP.count() >0;
  }
  return inherited::hasChildren(parent);
}

Qt::DropActions QMemoryModel::supportedDragActions() const
{
  return inherited::supportedDragActions();
}

Qt::DropActions QMemoryModel::supportedDropActions() const
{
  return Qt::CopyAction | Qt::MoveAction;
}

QHash<int, QByteArray> QMemoryModel::roleNames() const
{
  QHash<int, QByteArray> result = QAbstractItemModel::roleNames();
  result[ValueRole] = "value";
  return result;
}

QStringList QMemoryModel::mimeTypes() const
{
  QStringList types;
  types << "application/vnd.text.list";
  return types;
}

QMimeData *QMemoryModel::mimeData(const QModelIndexList &indexes) const
{
  QMimeData *mimeData = new QMimeData();
  QByteArray encodedData;

  QDataStream stream(&encodedData, QIODevice::WriteOnly);

  foreach (const QModelIndex &index, indexes) {
    if (index.isValid()) {
      auto me = getMeByIndex(index);
      if(me)
        me.getMe()->save(stream);
    }
  }

  mimeData->setData("application/vnd.text.list", encodedData);
  return mimeData;
}

bool QMemoryModel::canDropMimeData(const QMimeData *data, Qt::DropAction action, int row, int column, const QModelIndex &parent) const
{
  Q_UNUSED(action);
  Q_UNUSED(row);
  Q_UNUSED(parent);

  if (!data->hasFormat("application/vnd.text.list"))
    return false;

  if (column > 0)
    return false;

  return true;
}

bool QMemoryModel::dropMimeData(const QMimeData *data, Qt::DropAction action, int row, int column, const QModelIndex &parent)
{
  if (!canDropMimeData(data, action, row, column, parent))
    return false;

  if (action == Qt::IgnoreAction)
    return true;

  int beginRow;

  if (row > -1)
    beginRow = row;
  else if (parent.isValid())
    beginRow = rowCount(parent);
  else
    beginRow = rowCount(QModelIndex());

  QByteArray encodedData = data->data("application/vnd.text.list");
  QDataStream stream(&encodedData, QIODevice::ReadOnly);

  while (!stream.atEnd())
  {
    Memory::TME me(getME().getMe());
    me.load(stream);

    auto meParent = getMeByIndex(parent);
    if(!meParent)
      meParent = getME();
    auto me1 = add(meParent, me.name());
    if(me1) {
      me1.setVal(me.val());
      move(me1, meParent, beginRow);
      addFrom(me1, MEWrapper(&me), true);
    }
  }

  //  QStringList newItems;
//  int rows = 0;

//  while (!stream.atEnd()) {
//    QString text;
//    stream >> text;
//    newItems << text;
//    ++rows;
//  }

//  insertRows(beginRow, rows, parent);
//  foreach (const QString &text, newItems) {
//    QModelIndex idx = index(beginRow, 0, parent);
//    setData(idx, text, Qt::EditRole);
//    beginRow++;
//  }

  return true;
}

bool QMemoryModel::insertRows(int row, int count, const QModelIndex &parent)
{
  beginInsertRows(parent, row, row+count-1);
  auto me = getMeByIndex(parent);
  if(!me)
    me = getME();
  for(int i = 0; i < count; ++i)
  {
    auto me1 = add(me, "", false);
    //mem_->move(me1, me, row+i);
  }

  endInsertRows();

  return true;
}
