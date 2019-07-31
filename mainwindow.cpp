#include "mainwindow.h"
#include "ui_mainwindow.h"

#include <cmath>
#include <QSpinBox>
#include <QTimer>
#include <QCoreApplication>
#include <QMessageBox>

#include <QColorDialog>
#include <QClipboard>



MainWindow::MainWindow(QWidget *parent) :
    QMainWindow(parent),
    ui(new Ui::MainWindow)
{
    ui->setupUi(this);
    mdiArea = new QMdiArea;
    mdiArea->setViewMode(QMdiArea::TabbedView);
    setCentralWidget(mdiArea);
    connect(mdiArea, SIGNAL(subWindowActivated(QMdiSubWindow*)), this, SLOT(updateActions()));


    createActions();
    createMenus();
    createToolBars();
    createStatusBar();


    m_plotter = new Plotter(this);
    mdiArea->addSubWindow(m_plotter);
    m_plotter->setWindowTitle("Subber band");

    m_pWidget = new MyWidget(this);
    mdiArea->addSubWindow(m_pWidget);
    m_pWidget->setWindowTitle("MyWidget");

    m_pOtimer = new OvenTimer(this);
    m_pOtimer->setDuration(30);
    mdiArea->addSubWindow(m_pOtimer);
    m_pOtimer->setWindowTitle("OvenTimer");

    m_cityscape = new Cityscape(this);
    mdiArea->addSubWindow(m_cityscape);
    m_cityscape->setWindowTitle("CityScape");

    scene = new QGraphicsScene(0,0,600,500);
    view = new QGraphicsView;
    view->setScene(scene);
    view->setDragMode(QGraphicsView::RubberBandDrag);
    view->setRenderHints(QPainter::Antialiasing | QPainter::TextAntialiasing);
    view->setContextMenuPolicy(Qt::ActionsContextMenu);
    //setCentralWidget(view);
    mdiArea->addSubWindow(view);
    view->setWindowTitle("QGraphics");

    minZ = 0;
    maxZ = 0;
    seqNumber = 0;

    connect(scene, SIGNAL(selectionChanged()), this, SLOT(updateActions()));

    updateActions();


   // QTimer::singleShot(600000, m_plotter, SLOT(InitCurves()));
}

void MainWindow::closeMess()
{
    QMessageBox m(this);
        m.setWindowTitle("MyAction");
        m.setText("This is a messagebox for my action.");
        m.exec();
}

MainWindow::~MainWindow()
{
    delete ui;
}

void MainWindow::createActions()
{
    closeAct = new QAction(tr("close"),this);
    cutAction = new QAction(tr("cut"),this);
    copyAction = new QAction(tr("copy"),this);
    addNodeAction = new QAction(tr("addNode"),this);
    addLinkAction = new QAction(tr("addLink"),this);
    pasteAction = new QAction(tr("paste"),this);
    deleteAction = new QAction(tr("delete"),this);
    bringToFrontAction = new QAction(tr("bringtofront"),this);
    sendToBackAction = new QAction(tr("sendtoback"),this);
    propertiesAction = new QAction(tr("properties"),this);

    closeAct->setShortcut(QKeySequence(Qt::CTRL + Qt::Key_X));
    connect(closeAct, SIGNAL(triggered()), this, SLOT(closeMess()));
    connect(addNodeAction, SIGNAL(triggered()), this, SLOT(addNode()));
    connect(addLinkAction, SIGNAL(triggered()), this, SLOT(addLink()));
    connect(pasteAction, SIGNAL(triggered()), this, SLOT(paste()));
    connect(copyAction, SIGNAL(triggered()), this, SLOT(copy()));
    connect(cutAction, SIGNAL(triggered()), this, SLOT(cutDg()));
    connect(deleteAction, SIGNAL(triggered()), this, SLOT(del()));
    connect(bringToFrontAction, SIGNAL(triggered()), this, SLOT(bringToFront()));
    connect(sendToBackAction, SIGNAL(triggered()), this, SLOT(sendToBack()));
    connect(propertiesAction, SIGNAL(triggered()), this, SLOT(properties()));
}

void MainWindow::createMenus()
{
    this->ui->menuBar->addMenu("close")->addAction(closeAct);
    this->ui->menuBar->addMenu("cut")->addAction(cutAction);
    this->ui->menuBar->addMenu("copy")->addAction(copyAction);
    this->ui->menuBar->addMenu("addNode")->addAction(addNodeAction);
    this->ui->menuBar->addMenu("addLink")->addAction(addLinkAction);
    this->ui->menuBar->addMenu("pase")->addAction(pasteAction);
    this->ui->menuBar->addMenu("delete")->addAction(deleteAction);
    this->ui->menuBar->addMenu("bringtoFront")->addAction(bringToFrontAction);
    this->ui->menuBar->addMenu("sendtoBack")->addAction(sendToBackAction);
    this->ui->menuBar->addMenu("properties")->addAction(propertiesAction);
}

void MainWindow::createToolBars()
{

}

void MainWindow::createStatusBar()
{

}

void MainWindow::loadFiles()
{
    QStringList args = QCoreApplication::arguments();
    args.removeFirst();
    if (!args.isEmpty()){
        foreach (QString arg, args)
            openFile(arg);
            mdiArea->cascadeSubWindows();
    }
    else {
        newFile();
    }
    mdiArea->activateNextSubWindow();
}

void MainWindow::openFile(QString)
{}

void MainWindow::newFile()
{
//    Editor *editor = new Editor;
//    editor->newFile();
//    addEditor(editor);
}

void MainWindow::open()
{
//    Editor *editor = Editor::open(this);
//    if (editor)
//        addEditor(editor);
}

void MainWindow::addEditor(Editor *editor)
{
//    connect(editor, SIGNAL(copyAvailable(bool)), cutAction, SLOT(setEnabled(bool)));
//    connect(editor, SIGNAL(copyAvailable(bool)), copyAction, SLOT(setEnabled(bool)));

//    QMdiSubWindow *subWindow = mdiArea->addSubWindow(editor);
//    windowMenu->addAction(editor->windowMenuAction());
//    windowActionGroup->addAction(editor->windowMenuAction);
//    subWindow->show();
}

void MainWindow::save()
{
//    if (activeEditor())
//        activeEditor()->save();
}

Editor* MainWindow::activeEditor()
{
//    QMdiSubWindow *subWindow = mdiArea->activeSubWindow();
//    if (subWindow)
//        return qobject_cast<Editor *>(subWindow->widget());
//    return 0;
}

void MainWindow::cut()
{
//    if (activeEditor())
//        activeEditor->cut();
}

void MainWindow::updateActions()
{
    bool hasSelection = !scene->selectedItems().isEmpty();
    bool isNode = (selectedNode() != 0);
    bool isNodePair = (selectedNodePair() != NodePair());

//    cutAction->setEnabled(isNode);
//    copyAction->setEnabled(isNode);
//    addLinkAction->setEnabled(isNode);
//    deleteAction->setEnabled(isNode);
//    bringToFrontAction->setEnabled(isNode);
//    sendToBackAction->setEnabled(isNode);
//    propertiesAction->setEnabled(isNode);

//    foreach (QAction *action, view->actions())
//    {
//        view->removeAction(action);
//    }

//    foreach (QAction *action, editMenu->actions()){
//        if (action->isEnabled())
//            view->addAction(action);
//    }
}



void MainWindow::closeEvent(QCloseEvent *evt)
{

}



void MainWindow::addNode()
{
    Node *node = new Node;
    node->setText(tr("node %1").arg(seqNumber+1));
    setupNode(node);
}

void MainWindow::setupNode(Node *node)
{
    node->setPos(QPoint(80+(100*(seqNumber%5)), 80+(50*((seqNumber/5)%7))));

    scene->addItem(node);
    ++seqNumber;

    scene->clearSelection();
    node->setSelected(true);
    bringToFront();
}

void MainWindow::bringToFront()
{
    ++maxZ;
    setZValue(maxZ);
}

void MainWindow::sendToBack()
{
    --minZ;
    setZValue(maxZ);
}

void MainWindow::setZValue(int z)
{
    Node *node = selectedNode();
    if (node)
        node->setZValue(z);
}

Node* MainWindow::selectedNode() const
{
    QList<QGraphicsItem *> items = scene->selectedItems();
    if (items.count() == 1)
        return dynamic_cast<Node *>(items.first());
    else {
        return 0;
    }
}

Link* MainWindow::selectedLink() const
{
    QList<QGraphicsItem *> items = scene->selectedItems();
    if (items.count() == 1)
        return dynamic_cast<Link *>(items.first());
    else {
        return 0;
    }
}

void MainWindow::addLink()
{
    NodePair nodes = selectedNodePair();
    if (nodes == NodePair())
        return;

    Link *link = new Link(nodes.first, nodes.second);
    scene->addItem(link);
}

MainWindow::NodePair MainWindow::selectedNodePair() const
{
    QList<QGraphicsItem *> items = scene->selectedItems();
    if (items.count() == 2){
        Node *first = dynamic_cast<Node *>(items.first());
        Node *second = dynamic_cast<Node *>(items.last());
        if (first && second)
            return NodePair(first, second);
    }
    return NodePair();
}

void MainWindow::del()
{
    QList<QGraphicsItem *> items = scene->selectedItems();
    QMutableListIterator<QGraphicsItem *> i(items);
    while(i.hasNext()){
        Link *link = dynamic_cast<Link *>(i.next());
        if (link){
            delete link;
            i.remove();
            seqNumber--;
        }
    }
    qDeleteAll(items);
    seqNumber--;
}

void MainWindow::properties()
{
    Node *node = selectedNode();
    Link *link = selectedLink();

    if (node){
//        PropertiesDialog dialog(node, this);
//        dialog.exec();
        ;
    }else if(link){
        QColor color = QColorDialog::getColor(link->color(), this);
        if(color.isValid())
            link->setColor(color);
    }
}

void MainWindow::cutDg()
{
    Node *node = selectedNode();
    if (!node)
        return;

    copy();
    delete node;
}

void MainWindow::copy()
{
    Node *node = selectedNode();
    if (!node)
        return;

    QString str = QString("Node %1 %2 %3 %4").arg(node->textColor().name())
                    .arg(node->outlineColor().name())
                    .arg(node->backgroudColor().name())
                    .arg(node->text());
    QApplication::clipboard()->setText(str);
}

void MainWindow::paste()
{
    QString str = QApplication::clipboard()->text();
    QStringList parts = str.split(" ");
    if (parts.count() >= 5 && parts.first() == "Node"){
        Node *node = new Node;
        node->setText(QStringList(parts.mid(4)).join(" "));
        node->setTextColor(QColor(parts[1]));
        node->setOutlineColor(QColor(parts[2]));
        node->setBackgroudColor(QColor(parts[3]));
        setupNode(node);
    }
}



























