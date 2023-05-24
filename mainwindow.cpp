#include "mainwindow.h"
#include <QtWidgets>
#include <QVBoxLayout>
#include <QString>

mainwindow::mainwindow()
{
    model = new TreeModel();
    view = new QTreeView;
    view->setModel(model);
    view->setWindowTitle(QObject::tr("TreeApp"));


    view->setSelectionMode(QAbstractItemView::ExtendedSelection);
    view->setDragEnabled(true);
    view->setAcceptDrops(true);
    view->setDropIndicatorShown(true);



    QWidget *centralWidget = new QWidget;

    QMenu *optionMenu = new QMenu(tr("&Options"), this);
    addChild = optionMenu->addAction(tr("&Add Child..."));
    addRow = optionMenu->addAction(tr("&Add Row..."));
    delRow = optionMenu->addAction(tr("&Remove Row..."));
    QAction *setRoot = optionMenu->addAction(tr("Set Root Name..."));
    QAction *quitAction = optionMenu->addAction(tr("Exit"));
    QMenu *fileMenu = new QMenu(tr("&File"), this);
    QAction *importTxt = fileMenu->addAction(tr("&Import from txt file..."));
    QAction *exportTxt = fileMenu->addAction(tr("&Export to txt file..."));

    menuBar()->addMenu(optionMenu);
    menuBar()->addSeparator();
    menuBar()->addMenu(fileMenu);

    connect(view->selectionModel(), &QItemSelectionModel::selectionChanged,
            this, &mainwindow::updateActions);
    connect(addChild, &QAction::triggered, this, &mainwindow::newChild);
    connect(addRow, &QAction::triggered, this, &mainwindow::newRow);
    connect(delRow, &QAction::triggered, this, &mainwindow::removeRow);
    connect(setRoot, &QAction::triggered, this, &mainwindow::setRootName);
    connect(quitAction, &QAction::triggered, qApp, &QCoreApplication::quit);
    connect(importTxt, &QAction::triggered, this, &mainwindow::structureFromFile);
    connect(exportTxt, &QAction::triggered, this, &mainwindow::structureToFile);

    updateActions();

    QVBoxLayout *mainLayout = new QVBoxLayout;
    mainLayout->addWidget(view);
    centralWidget->setLayout(mainLayout);

    setCentralWidget(centralWidget);
}

void mainwindow::newChild()
{
    const QModelIndex index = view->selectionModel()->currentIndex();

    if (!model->insertRows(model->rowCount(index), 1, index))
        return;

    const QModelIndex child = model->index(model->rowCount(index)-1, 0, index);
    model->setData(child, QVariant(tr("[No data]")), Qt::EditRole);
    view->selectionModel()->setCurrentIndex(model->index(model->rowCount(index)-1, 0, index),
                                            QItemSelectionModel::ClearAndSelect);
    updateActions();
}

void mainwindow::newRow()
{
    const QModelIndex index = view->selectionModel()->currentIndex();

    if (!model->insertRows(index.row()+1, 1, index.parent()))
        return;
    updateActions();
    const QModelIndex sibling = model->index(index.row()+1, 0, index.parent());
    model->setData(sibling, QVariant(tr("[No data]")), Qt::EditRole);
    view->selectionModel()->setCurrentIndex(model->index(index.row()+1, 0, index.parent()),
                                            QItemSelectionModel::ClearAndSelect);
}

void mainwindow::removeRow()
{
    const QModelIndex index = view->selectionModel()->currentIndex();

    if (model->removeRows(index.row(), 1, index.parent()))
        updateActions();
}

void mainwindow::structureFromFile()
{
    QString fileName = QFileDialog::getOpenFileName(this, tr("Open .txt File"), QString(), "*.txt");
    if (!fileName.isEmpty()){
        model = new TreeModel(fileName.toStdString());
        view->setModel(model);
    }
    updateActions();
}

void mainwindow::structureToFile()
{
    QString path = QFileDialog::getSaveFileName(this, tr("Save to .txt file"), QString(), "*.txt");
    model->toFile(path.toStdString());
    updateActions();
}

void mainwindow::setRootName()
{
    bool ok;
    QString text = QInputDialog::getText(this, tr("New Root  name"),
                                         tr("Enter new Root name:"), QLineEdit::Normal,
                                         QString("New Root"), &ok);
    if (ok && !text.isEmpty())
        model->setHeaderData(0, Qt::Horizontal, text, Qt::EditRole);
}

void mainwindow::updateActions()
{
    const bool hasSelection = !(model->root()->getFirstChild() == nullptr);
    addRow->setEnabled(hasSelection);
    delRow->setEnabled(hasSelection);
}
