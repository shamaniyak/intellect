#include "qmemorymodel.h"
#include "tme.h"

QMemoryModel::QMemoryModel(QObject *parent) : QAbstractItemModel(parent)
{
  headerInfo_ = new MemoryWrapper(this);
  headerInfo_->add(MEWrapper(), "Name");
  //headerInfo_->add(0, "Value");
  //headerInfo_->add(0, "Path");

  connect(headerInfo_, &MemoryWrapper::on_change,
          this, &QMemoryModel::onHeaderChange);
}

QMemoryModel::~QMemoryModel()
{
  setMem(nullptr);
}

QModelIndex QMemoryModel::index(int row, int column, const QModelIndex &parent) const
{
  if (!mem_ || !hasIndex(row, column, parent)) {
      return QModelIndex();
  }

  if (!parent.isValid()) { // запрашивают индексы корневых узлов
    // Получить MEWrapper корневого уровня и создать индекс
    auto me = mem_->getME();
    return createIndex(row, column, me.getByI(row).getMe());
  }

  auto parentME = getMeByIndex(parent);
  return createIndex(row, column, parentME.getByI(row).getMe());
}

QModelIndex QMemoryModel::parent(const QModelIndex &child) const
{
  if (!mem_ || !child.isValid()) {
      return QModelIndex();
  }

  auto me = getMeByIndex(child);
  if(me.isNull())
    return QModelIndex();

  auto parentME = me.parent();
  if (parentME && parentME != mem_->getME()) { // parent запрашивается не у корневого элемента
      return createIndex(parentME.getIndex(), 0, parentME.getMe());
  }
  else {
      return QModelIndex();
  }
}

int QMemoryModel::rowCount(const QModelIndex &parent) const
{
  if(!mem_)
    return 0;

  if (!parent.isValid())
    return mem_->getME().count();

  auto meP = getMeByIndex(parent);
  return meP.count();
}

int QMemoryModel::columnCount(const QModelIndex &/*parent*/) const
{
  if(!headerInfo_)
    return 0;
  return headerInfo_->getME().count();
}

QVariant QMemoryModel::data(const QModelIndex &index, int role) const
{
  QVariant var;
  if (!index.isValid()) {
    return var;
  }

  if(role != Qt::DisplayRole && role != Qt::EditRole && role != Qt::ToolTipRole)
    return var;

  auto me = getMeByIndex(index);

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
  //const QStringList headers = {tr("Name")};//, tr("Value"), tr("Path"), tr("Type")};
  int cnt = headerInfo_->getME().count();
  if (orientation == Qt::Horizontal && role == Qt::DisplayRole && section < cnt)
  {
    return headerInfo_->getME().getByI(section).name();
    //return headers[section];
  }
  return QVariant();
}

bool QMemoryModel::canFetchMore(const QModelIndex &parent) const
{
  return QAbstractItemModel::canFetchMore(parent);
}

void QMemoryModel::fetchMore(const QModelIndex &parent)
{
  return QAbstractItemModel::fetchMore(parent);
}

Qt::ItemFlags QMemoryModel::flags(const QModelIndex &index) const
{
  Qt::ItemFlags flags = QAbstractItemModel::flags(index);

  bool canEdit = index.isValid()
      && (index.column() == NameColumn || index.column() == ValueColumn);

  if (canEdit) {
    auto me = getMeByIndex(index);
    if (mem_ && me != mem_->getME()) {
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
  return QAbstractItemModel::hasChildren(parent);
}

Qt::DropActions QMemoryModel::supportedDragActions() const
{
  return inherited::supportedDragActions();
}

Qt::DropActions QMemoryModel::supportedDropActions() const
{
  return Qt::CopyAction | Qt::MoveAction;
}

MemoryWrapper *QMemoryModel::mem() const
{
  return mem_;
}

void QMemoryModel::setMem(MemoryWrapper *mem)
{
  if(mem_)
    mem_->disconnect(this);

  mem_ = mem;

  if(mem_) {
    //connect(mem_, &MemoryWrapper::on_change, this, &QMemoryModel::memory_change);
    connect(mem_, &MemoryWrapper::change, this, &QMemoryModel::onMemoryChange);
  }
}

QModelIndex QMemoryModel::getIndexByMe(const MEWrapper &me)
{
  QModelIndex parent, mi;

  if(me.isNull())
    return mi;

  QList<MEWrapper> path;
  MEWrapper meP = me;

  while(meP)
  {
    if(meP.parent())// проверка что не самый верхний
    {
      path.insert(0, meP);
    }

    meP = meP.parent();
  }

  for(int i = 0, cnt = path.count(); i < cnt; ++i)
  {
    int r = path[i].getIndex();
    mi = index(r, 0, parent);
    parent = mi;
  }

  return mi;
}

MEWrapper QMemoryModel::getMeByIndex(const QModelIndex &index) const
{
  uint id = reinterpret_cast<uint>(index.internalPointer());
  return mem_->getById(id);
}

QHash<int, QByteArray> QMemoryModel::roleNames() const
{
  QHash<int, QByteArray> result = QAbstractItemModel::roleNames();
  return result;
}

void QMemoryModel::updateMe(const MEWrapper &me)
{
  QModelIndex mi = getIndexByMe(me);
  int last = me.count() -1;
  if(last <0) last = 0;
  this->beginInsertRows(mi, 0, last);
  this->endInsertRows();
}

void QMemoryModel::memory_change(MEWrapper &me, EMemoryChange idMsg)
{
  if(!me)
    return;

  switch(idMsg)
  {
    case EMemoryChange::mcNone:
      break;
    case EMemoryChange::mcSelect:
      break;

    case EMemoryChange::mcAdd:
    {
      QModelIndex parent = getIndexByMe(me.parent());
      int cnt = me.parent().count();
      this->beginInsertRows(parent, cnt-1, cnt-1);
      this->endInsertRows();
      break;
    }

    case EMemoryChange::mcDel:
    {
      break;
    }

    case EMemoryChange::mcEditName:
    {
      auto mi = getIndexByMe(me);
      emit dataChanged(mi, mi);
      break;
    }

    case EMemoryChange::mcEditVal:
      break;

    case EMemoryChange::mcClear:
    {
      this->beginResetModel();
      endResetModel();
      break;
    }
    case EMemoryChange::mcAddFrom:
    case EMemoryChange::mcUpdate:
    {
      updateMe(me);
      break;
    }

    case mcMove:
      updateMe(me.parent());
      break;
  }
}

void QMemoryModel::onMemoryChange(const ChangeEvent &event)
{
  EMemoryChange idMsg = event.type;

  switch(idMsg)
  {
    case EMemoryChange::mcNone:
      break;
    case EMemoryChange::mcSelect:
      break;

    case EMemoryChange::mcAdd:
    {
      if(!event.parent)
        return;
      auto index = getIndexByMe(event.parent);
      this->beginInsertRows(index, event.row, event.row);
      this->endInsertRows();
      break;
    }

    case EMemoryChange::mcDel:
    {
      if(!event.parent)
        return;
      auto index = getIndexByMe(event.parent);
      this->beginRemoveRows(index, event.row, event.row);
      this->endRemoveRows();
      break;
    }

    case EMemoryChange::mcEditName:
    {
      if(!event.parent)
        return;
      auto index = getIndexByMe( event.me );
      emit dataChanged(index, index);
      break;
    }

    case EMemoryChange::mcEditVal:
      break;

    case EMemoryChange::mcClear:
    {
      if(event.count >0)
      {
        auto index = getIndexByMe( event.me );
        this->beginRemoveRows(index, 0, event.count-1);
        this->endRemoveRows();
      }
      break;
    }
    case EMemoryChange::mcAddFrom:
    case EMemoryChange::mcUpdate:
    {
      updateMe(event.me);
      break;
    }

    case mcMove:
      updateMe(event.parent);
      break;
  }
}

void QMemoryModel::onHeaderChange(const MEWrapper &me, EMemoryChange idMsg)
{
  if(!me)
    return;

  switch(idMsg)
  {
    case EMemoryChange::mcNone:
      break;
    case EMemoryChange::mcSelect:
      break;

    case EMemoryChange::mcAdd:
    {
      QModelIndex parent;
      int cnt = me.parent().count();
      this->beginInsertColumns(parent, cnt-1, cnt-1);
      this->endInsertColumns();
      break;
    }

    case EMemoryChange::mcDel:
    {
      //auto idx = getIndexByMe(me);
      break;
    }

    case EMemoryChange::mcEditName:
    {
      //auto mi = getIndexByMe(me);
      //emit dataChanged(mi, mi);
      break;
    }

    case EMemoryChange::mcEditVal:
      break;

    case EMemoryChange::mcClear:
    case EMemoryChange::mcAddFrom:
    case EMemoryChange::mcUpdate:
    {
      //updateMe(me);
      break;
    }

    case mcMove:
      //updateMe(me->parent());
      break;
  }
}

MemoryWrapper *QMemoryModel::getHeaderInfo() const
{
  return headerInfo_;
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

  if (row != -1)
    beginRow = row;
  else if (parent.isValid())
    beginRow = rowCount(parent);
  else
    beginRow = rowCount(QModelIndex());

  QByteArray encodedData = data->data("application/vnd.text.list");
  QDataStream stream(&encodedData, QIODevice::ReadOnly);

  while (!stream.atEnd())
  {
    Memory::TME me(mem_->getME().getMe());
    me.load(stream);

    auto meParent = getMeByIndex(parent);
    if(!meParent)
      meParent = mem_->getME();
    auto me1 = mem_->add(meParent, me.name());
    if(me1) {
      me1.setVal(me.val());
      mem_->addFrom1(me1.getMe(), &me, true);
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
  auto me = getMeByIndex(parent);
  if(!me)
    me = mem_->getME();
  for(int i = 0; i < count; ++i)
  {
    auto me1 = mem_->add(me, "", false);
    //mem_->move(me1, me, row+i);
  }

  return true;
}
