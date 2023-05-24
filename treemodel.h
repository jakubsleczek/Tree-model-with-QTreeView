#ifndef TREEMODEL_H
#define TREEMODEL_H

#include "treeitem.h"
#include <QAbstractItemModel>
#include <QModelIndex>
#include <QVariant>
#include <string>
#include <fstream>
#include <QMimeData>
#include <QFile>

class TreeModel : public QAbstractItemModel
{
    Q_OBJECT

public:
     TreeModel(std::string filename = "", QObject *parent = nullptr);
    ~TreeModel();

    QVariant data(const QModelIndex &index, int role) const override;
    QVariant headerData(int section, Qt::Orientation orientation,
                        int role = Qt::DisplayRole) const override;
    QModelIndex index(int row, int column,
                      const QModelIndex &parent = QModelIndex()) const override;
    QModelIndex parent(const QModelIndex &index) const override;
    int rowCount(const QModelIndex &parent = QModelIndex()) const override;
    int columnCount(const QModelIndex &parent = QModelIndex()) const override;
    Qt::ItemFlags flags(const QModelIndex &index) const override;
    bool setData(const QModelIndex &index, const QVariant &value,
                 int role = Qt::EditRole) override;
    bool setHeaderData(int section, Qt::Orientation orientation,
                       const QVariant &value, int role = Qt::EditRole) override;
    bool insertRows(int row, int count, const QModelIndex &parent) override;
    bool removeRows(int row, int count, const QModelIndex &parent) override;
    TreeItem *getItem(const QModelIndex &index) const;
    void toFile(std::string path);
    void toFileHelper(int i, std::ofstream &file, TreeItem* obj);
    TreeItem* root(){return rootItem;};
    Qt::DropActions supportedDropActions() const override;
    QStringList mimeTypes() const override;
    QMimeData *mimeData(const QModelIndexList &indexes) const override;
    bool canDropMimeData(const QMimeData *data,
        Qt::DropAction action, int row, int column, const QModelIndex &parent) const override;
    bool dropMimeData(const QMimeData *data,
        Qt::DropAction action, int row, int column, const QModelIndex &parent) override;
    bool hasChildren(const QModelIndex &parent = QModelIndex()) const override;
private:

    void fromFile(TreeItem *obj, std::ifstream &file);
    std::string fromFileHelper(int &i, int &counter, std::ifstream &file, TreeItem* obj);
    void createBranch(TreeItem* item, int row, int column, QModelIndex parent);

    TreeItem *rootItem;
};

#endif // TREEMODEL_H
