#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QCoreApplication>
#include <QMainWindow>

#include <QMap>
#include <QPixmap>
#include <QVector>
#include <QWidget>
#include <QToolButton>
#include <QStylePainter>
#include <QMouseEvent>
#include <QMdiArea>
#include <QAction>
#include "oventimer.h"
#include "cityscape.h"
#include "gradient.h"
#include "plotset.h"


namespace Ui {
class MainWindow;
}

class Editor;
class MainWindow : public QMainWindow
{
    Q_OBJECT

public:
    explicit MainWindow(QWidget *parent = 0);
    ~MainWindow();

    void createActions();
    void createMenus();
    void createToolBars();
    void createStatusBar();
    void updateActions();
    void closeEvent(QCloseEvent *evt);

    void loadFiles();
    void openFile(QString);
    void newFile();
    void open();
    void save();
    void addEditor(Editor *editor);
    void cut();

    Editor* activeEditor();

    QAction *closeAct;
    QAction *cutAction;
    QAction *copyAction;
    QAction *addNodeAction;
    QAction *addLinkAction;
    QAction *deleteAction;
    QAction *pasteAction;
    QAction *bringToFrontAction;
    QAction *sendToBackAction;
    QAction *propertiesAction;

    typedef QPair<Node *, Node *> NodePair;
    void setZValue(int z);
    void setupNode(Node *node);
    Node *selectedNode() const;
    Link *selectedLink() const;
    NodePair selectedNodePair() const;

    QMenu *fileMenu;
    QMenu *editMenu;
    QToolBar *editToolBar;

    QGraphicsScene *scene;
    QGraphicsView *view;

    int minZ;
    int maxZ;
    int seqNumber;


public slots:
    void closeMess();


    void addNode();
    void addLink();
    void del();
    void cutDg();
    void copy();
    void paste();
    void bringToFront();
    void sendToBack();
    void properties();
    //void updateDgActions();

private:
    Ui::MainWindow *ui;

    QMdiArea    *mdiArea;
    Plotter *m_plotter;

    MyWidget *m_pWidget;

    OvenTimer *m_pOtimer;

    Cityscape *m_cityscape;
};

#endif // MAINWINDOW_H
