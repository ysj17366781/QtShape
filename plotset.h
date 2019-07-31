#ifndef PLOTSET_H
#define PLOTSET_H
#include <QMap>
#include <QPixmap>
#include <QVector>
#include <QWidget>
#include <QToolButton>
#include <QAction>
#include <QTimerEvent>
#include <cmath>
#include <QStylePainter>
#include <QStyleOptionFocusRect>
#include <QMouseEvent>


class PlotSettings;
class QToolButton;
class Plotter:public QWidget
{
    Q_OBJECT
public:
    Plotter(QWidget *parent=0);

    void setPlotSettings(const PlotSettings &settings);
    void setCurveData(int id, const QVector<QPointF> &data);
    void clearCurve(int id);
    QSize minimumSizeHint() const;
    QSize sizeHint() const;

    void timerEvent(QTimerEvent *evt);

public slots:
    void zoomIn();
    void zoomOut();
    void InitCurves();

protected:
    void paintEvent(QPaintEvent *evt);
    void resizeEvent(QResizeEvent *evt);
    void mousePressEvent(QMouseEvent *evt);
    void mouseMoveEvent(QMouseEvent *evt);
    void mouseReleaseEvent(QMouseEvent *evt);
    void keyPressEvent(QKeyEvent *evt);
    void wheelEvent(QWheelEvent *evt);
    bool eventFilter(QObject *target, QEvent *evt);

private:
    void updateRubberBandRegion();
    void refreshPixmap();
    void drawGrid(QPainter *painter);
    void drawCurves(QPainter *painter);


    enum { Margin = 50};

    QToolButton *zoomInButton;
    QToolButton *zoomOutButton;
    QMap<int, QVector<QPointF> > curveMap;
    QVector<PlotSettings> zoomStack;
    int curZoom;
    bool rubberBandIsShown;
    QRect rubberBandRect;
    QPixmap pixmap;

    QAction *zoomOutAction;
    QAction *zoomInAction;

    int timeId;
    int offset;

};

class PlotSettings
{
public:
    PlotSettings();

    void scoll(int dx, int dy);
    void adjust();
    double spanX() const { return maxX - minX;}
    double spanY() const { return maxY - minY;}

    double minX;
    double maxX;
    int numXTicks;
    double minY;
    double maxY;
    int numYTicks;

private:
    static void adjustAxis(double &min, double &max, int &numTicks);
};

#endif // PLOTSET_H
