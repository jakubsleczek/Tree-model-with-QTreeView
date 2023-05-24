#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QMainWindow>
#include <QTreeView>
#include "treemodel.h"

class mainwindow : public QMainWindow
{
    Q_OBJECT
public:
    mainwindow();
public slots:
    void updateActions();
private slots:
    void newChild();
    void newRow();
    void removeRow();
    void structureFromFile();
    void structureToFile();
    void setRootName();
signals:

private:
    TreeModel *model;
    QAction *addRow;
    QAction *delRow;
    QAction *addChild;
    QTreeView *view;
};

#endif // MAINWINDOW_H
