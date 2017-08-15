#include "qmemorymodel.h"
#include "tme.h"

QMemoryModel::QMemoryModel(QObject *parent) : QAbstractItemModel(parent)
{
  headerInfo_ = new MemoryWrapper(this);
  headerInfo_->add(0, "Name");
  //headerInfo_->add(0, "Value");
  //headerInfo_->add(0, "Path");

  connect(headerInfo_, &MemoryWrapper::on_change,
          this, &QMemoryModel::header_change);
}

QMemoryModel::~QMemoryModel()
{

}

QModelIndex QMemoryModel::index(int row, int column, const QModelIndex &parent) const
{
  if (!hasIndex(row, column, parent)) {
      return QModelIndex();
  }

  if (!parent.isValid()) { // запрашивают индексы корневых узлов
    // Получить MEWrapper корневого уровня и создать индекс
    auto me = mem_->getME();
    return createIndex(row, column, me->getByI(row));
  }

  auto parentME = static_cast<MEWrapper*>(parent.internalPointer());
  return createIndex(row, column, parentME->getByI(row));
}

QModelIndex QMemoryModel::parent(const QModelIndex &child) const
{
  if (!child.isValid()) {
      return QModelIndex();
  }

  auto me = static_cast<MEWrapper*>(child.internalPointer());
  if(me->deleted())
    return QModelIndex();

  auto parentME = me->parent();
  if (parentME && parentME != mem_->getME()) { // parent запрашивается не у корневого элемента
      return createIndex(parentME->getIndex(), 0, parentME);
  }
  else {
      return QModelIndex();
  }
}

int QMemoryModel::rowCount(const QModelIndex &parent) const
{
  if (!parent.isValid())
    return mem_->getME()->count();

  auto meP = static_cast<MEWrapper*>(parent.internalPointer());
  return meP->count();
}

int QMemoryModel::columnCount(const QModelIndex &/*parent*/) const
{
  return headerInfo_->getME()->count();
}

QVariant QMemoryModel::data(const QModelIndex &index, int role) const
{
  QVariant var;
  if (!index.isValid()) {
    return var;
  }

  if(role != Qt::DisplayRole && role != Qt::EditRole)
    return var;

  auto me = static_cast<MEWrapper*>(index.internalPointer());

  switch (index.column()) {
  case NameColumn:
  {
    var = me->name();
    if(var.toString().isEmpty())
      var = "< >";
  } break;

  case PathColumn:
    if(role == Qt::DisplayRole)
      var = me->getPath();
    break;

  case ValueColumn:
    if(role == Qt::DisplayRole)
    {
      QStringList list(me->val().toString().split("\n"));
      var = list[0];
    }
    else
      var = me->val().toString();
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
  auto me = static_cast<MEWrapper*>(index.internalPointer());
  Q_ASSERT(me);

  switch(index.column())
  {
  case NameColumn:
    me->setName(val);
    break;

  case ValueColumn:
    me->setVal(value);
    break;

  default:
    res = false;
  }

  return res;
}

QVariant QMemoryModel::headerData(int section, Qt::Orientation orientation, int role) const
{
  //const QStringList headers = {tr("Name")};//, tr("Value"), tr("Path"), tr("Type")};
  int cnt = headerInfo_->getME()->count();
  if (orientation == Qt::Horizontal && role == Qt::DisplayRole && section < cnt)
  {
    return headerInfo_->getME()->getByI(section)->name();
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
    auto me = static_cast<MEWrapper*>(index.internalPointer());
    if (me != mem_->getME()) {
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
    auto meP = static_cast<MEWrapper*>(parent.internalPointer());
    Q_ASSERT(meP);
    return meP->count() >0;
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
    connect(mem_, &MemoryWrapper::change, this, &QMemoryModel::on_memory_change);
  }
}

QModelIndex QMemoryModel::getIndexByMe(MEWrapper *me)
{
  QModelIndex parent, mi;

  if(!me)
    return mi;

  QList<MEWrapper*> path;
  MEWrapper *meP = me;

  while(meP)
  {
    if(meP->parent())// проверка что не самый верхний
    {
      path.insert(0, meP);
    }

    meP = meP->parent();
  }

  for(int i = 0, cnt = path.count(); i < cnt; ++i)
  {
    int r = path[i]->getIndex();
    mi = index(r, 0, parent);
    parent = mi;
  }

  return mi;
}

void QMemoryModel::updateMe(MEWrapper *me)
{
  QModelIndex mi = getIndexByMe(me);
  int last = me->count() -1;
  if(last <0) last = 0;
  this->beginInsertRows(mi, 0, last);
  this->endInsertRows();
}

void QMemoryModel::memory_change(MEWrapper *me, EMemoryChange idMsg)
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
      QModelIndex parent = getIndexByMe(me->parent());
      int cnt = me->parent()->count();
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
      updateMe(me->parent());
      break;
  }
}

void QMemoryModel::on_memory_change(const MemoryWrapper::ChangeEvent &event)
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

void QMemoryModel::header_change(MEWrapper *me, EMemoryChange idMsg)
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
      int cnt = me->parent()->count();
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
