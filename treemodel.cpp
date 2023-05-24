#include "treemodel.h"

TreeModel::TreeModel(std::string filename, QObject *parent)
    : QAbstractItemModel(parent)
{
    std::ifstream file;
    file.open(filename);
    if(!file){
        rootItem = new TreeItem("Empty Tree");
        return;
    }
    std::string mdata;
    std::getline(file, mdata);
    rootItem = new TreeItem(mdata);
    fromFile(rootItem, file);
}

TreeModel::~TreeModel()
{
    delete rootItem;
}

int TreeModel::columnCount(const QModelIndex &parent) const
{
    if (parent.isValid())
        return 1;
    return 1;
}

QVariant TreeModel::data(const QModelIndex &index, int role) const
{
    if (!index.isValid())
        return QVariant();

    if (role != Qt::DisplayRole)
        return QVariant();

    TreeItem *item = static_cast<TreeItem*>(index.internalPointer());

    return QString::fromStdString(item->data());
}

QVariant TreeModel::headerData(int section, Qt::Orientation orientation,
                               int role) const
{
    if (orientation == Qt::Horizontal && role == Qt::DisplayRole)
        return QString::fromStdString(rootItem->data());

    return QVariant();
}

QModelIndex TreeModel::index(int row, int column, const QModelIndex &parent) const
{
    if (!hasIndex(row, column, parent)){
        return QModelIndex();
    }

    TreeItem *parentItem;

    if (!parent.isValid())
        parentItem = rootItem;
    else
        parentItem = getItem(parent);

    TreeItem *childItem = parentItem->child(row);
    if (childItem){
        return createIndex(row, 0, childItem);
    }
    return QModelIndex();
}

QModelIndex TreeModel::parent(const QModelIndex &index) const
{
    if (!index.isValid()){
        return QModelIndex();
    }

    TreeItem *childItem = getItem(index);
    TreeItem *parentItem = childItem->parentItem();

    if (parentItem == rootItem){
        return QModelIndex();
    }
    return createIndex(parentItem->row(), 0, parentItem);
}
//! [7]

//! [8]
int TreeModel::rowCount(const QModelIndex &parent) const
{
    TreeItem *parentItem;
    if (!parent.isValid())
        parentItem = rootItem;
    else
        parentItem = getItem(parent);
    return parentItem->GetSubCount();
}

Qt::ItemFlags TreeModel::flags(const QModelIndex &index) const
{
    if (index.isValid())
        return Qt::ItemIsDragEnabled | Qt::ItemIsDropEnabled | Qt::ItemIsEditable | QAbstractItemModel::flags(index);
    else
        return Qt::ItemIsDropEnabled | Qt::ItemIsEditable | QAbstractItemModel::flags(index);
}

void TreeModel::fromFile(TreeItem *obj, std::ifstream &file){
    int i{0};
    int counter{0};
    std::string mdata;
    fromFileHelper(i, counter, file, obj);
    file.close();
}
std::string TreeModel::fromFileHelper(int &i, int &counter, std::ifstream &file, TreeItem* obj){
    std::string name; //line from file variable
    counter=0; //count whitespaces before text in line
    bool gowno = false;
    std::getline(file, name);
    for(char c : name){  //look at what level this line is
        if(c != ' ')
            break;
        counter += 1;
    }
    if(!name.empty() && ((counter-i) >= 0)){
        if(counter-i == 1){
            i = counter;
            obj->setFirstChild(new TreeItem(name.substr(counter), obj));
            name = fromFileHelper(i, counter, file, obj->getFirstChild());
            gowno = true;
        }
        if(gowno)
            i -= 1;
        if(counter-i == 0 && !name.empty()){
            i = counter;
            obj->setNextSibling(new TreeItem(name.substr(counter), obj->parentItem()));
            name = fromFileHelper(i, counter, file, obj->getNextSibling());
        }
    }
    return name;
}

void TreeModel::toFile(std::string path){
    std::ofstream file;
    file.open(path);
    if(!file){
        return;
        }
    toFileHelper(0, file, rootItem);
    file.close();
}

void TreeModel::toFileHelper(int i, std::ofstream &file, TreeItem *obj){
    for(int j=0 ; j<i; j++)
        file<<" ";
    file<<obj->data()<<std::endl;
    if(obj->getFirstChild() != nullptr){
        ++i;
        toFileHelper(i, file, obj->getFirstChild());
        --i;
    }
    if(obj->getNextSibling() != nullptr)
        toFileHelper(i, file, obj->getNextSibling());
}

bool TreeModel::setData(const QModelIndex &index, const QVariant &value, int role)
{
    if (role != Qt::EditRole)
        return false;

    TreeItem *item = getItem(index);
    bool result = item->setData(value.toString().toStdString());

    if (result)
        emit dataChanged(index, index, {Qt::DisplayRole, Qt::EditRole});

    return result;
}

bool TreeModel::setHeaderData(int section, Qt::Orientation orientation,
                              const QVariant &value, int role)
{
    if (role != Qt::EditRole || orientation != Qt::Horizontal)
        return false;

    const bool result = rootItem->setData(value.toString().toStdString());

    if (result)
        emit headerDataChanged(orientation, section, section);

    return result;
}

TreeItem *TreeModel::getItem(const QModelIndex &index) const
{
    if (index.isValid()) {
        TreeItem *item = static_cast<TreeItem*>(index.internalPointer());
        if (item)
            return item;
    }
    return rootItem;
}

bool TreeModel::insertRows(int row, int count, const QModelIndex &parent)
{
    TreeItem *parentItem = getItem(parent);
    if (!parent.isValid())
        parentItem = rootItem;

    if (!parentItem)
        return false;
    if (row < 0 || row > parentItem->GetSubCount())
        return false;

    beginInsertRows(parent, row, row + count -1);
    for (int i = 0; i < count; ++i) {
        if (!parentItem->insertChild(row))
            break;
    }
    endInsertRows();
    return true;
}

bool TreeModel::removeRows(int row, int count, const QModelIndex &parent)
{
    TreeItem *parentItem = getItem(parent);
    if (!parentItem)
        return false;

    beginRemoveRows(parent, row, row + count -1);
    const bool success = parentItem->Del(row);
    endRemoveRows();
    return success;
}

Qt::DropActions TreeModel::supportedDropActions() const
{
    return Qt::MoveAction;
}

QStringList TreeModel::mimeTypes() const
{
    QStringList types;
    types << "application/x-qabstractitemmodeldatalist";
    return types;
}

QMimeData *TreeModel::mimeData(const QModelIndexList &indexes) const
{
    QMimeData *mimeData = new QMimeData()
            ;
    QByteArray encodedData;

    QDataStream stream(&encodedData, QIODevice::WriteOnly);
    TreeItem* ptr;
    for (const QModelIndex &index : indexes) {
        if (index.isValid()) {
            ptr = getItem(index);
            stream << reinterpret_cast<quintptr>(ptr);
        }
    }
    mimeData->setData("application/x-qabstractitemmodeldatalist", encodedData);
    return mimeData;
}

bool TreeModel::canDropMimeData(const QMimeData *data,
    Qt::DropAction action, int row, int column, const QModelIndex &parent) const
{
    Q_UNUSED(action);
    Q_UNUSED(row);
    Q_UNUSED(parent);

    if (!data->hasFormat("application/x-qabstractitemmodeldatalist"))
        return false;

    return true;
}
bool TreeModel::dropMimeData(const QMimeData *data,
    Qt::DropAction action, int row, int column, const QModelIndex &parent)
{
    if (!canDropMimeData(data, action, row, column, parent))
        return false;

    if (action == Qt::IgnoreAction)
        return true;

    int beginRow;

    if (row != -1)
        beginRow = row;

    else if (parent.isValid())
        beginRow = 0;

    else
        beginRow = rowCount(QModelIndex());

    QByteArray encodedData = data->data("application/x-qabstractitemmodeldatalist");
    QDataStream stream(&encodedData, QIODevice::ReadOnly);
    QList<TreeItem*> newItems;
    int rows = 0;

    while (!stream.atEnd()) {
        quintptr qItem;
        stream >> qItem;
        TreeItem* item;
        item = reinterpret_cast<TreeItem*>(qItem);
        newItems << item;
        ++rows;
    }
    TreeItem* item;
    foreach(item, newItems) {
        insertRows(beginRow, 1, parent);
        QModelIndex idx = index(beginRow, 0, parent);
        setData(idx, QString::fromStdString(item->data()));
        createBranch(item->getFirstChild(), 0, 0, idx);
        beginRow++;
    }

    return true;
}

bool TreeModel::hasChildren(const QModelIndex &parent) const
{
    if(getItem(parent)->child(0) != nullptr)
           return true;
    return false;
}


void TreeModel::createBranch(TreeItem* item, int row, int column, QModelIndex parent){
    if(item == nullptr)
            return;
    insertRows(row, 1, parent);
    QModelIndex idx = index(row, 0, parent);
    setData(idx, QString::fromStdString(item->data()));
    if(item->getFirstChild() != nullptr)
        createBranch(item->getFirstChild(), 0, 0, idx);
    if(item->getNextSibling() != nullptr)
        createBranch(item->getNextSibling(), row++, 0, parent);

}
