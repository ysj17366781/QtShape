#include "plotset.h"



Plotter::Plotter(QWidget *parent):QWidget(parent)
{
    offset = 0;
    setBackgroundRole(QPalette::Dark);
    setAutoFillBackground(true);
    setSizePolicy(QSizePolicy::Expanding, QSizePolicy::Expanding);
    setFocusPolicy(Qt::StrongFocus);
    rubberBandIsShown = false;
    zoomInButton = new QToolButton(this);
    zoomInButton->setIcon(QIcon(":/images/zoomin.png"));
    zoomInButton->adjustSize();
    connect(zoomInButton, SIGNAL(clicked()), this, SLOT(zoomIn()));

    zoomOutButton = new QToolButton(this);
    zoomOutButton->setIcon(QIcon(":/images/zoomout.png"));
    zoomOutButton->adjustSize();
    connect(zoomOutButton, SIGNAL(clicked()), this, SLOT(zoomOut()));

    zoomInAction = new QAction("Zoom In", this);
    zoomInAction->setShortcut(tr("End"));
    connect(zoomInAction, SIGNAL(triggered()), this, SLOT(zoomIn()));


    zoomOutAction = new QAction("Zoom Out", this);
    zoomOutAction->setShortcut(tr("Home"));
    connect(zoomOutAction, SIGNAL(triggered()), this, SLOT(zoomOut()));

    setPlotSettings(PlotSettings());

    InitCurves();
    timeId = startTimer(50);

    //event filter
    zoomOutButton->installEventFilter(this);
    zoomInButton->installEventFilter(this);
}

void Plotter::timerEvent(QTimerEvent *evt)
{
    if (evt->timerId() == timeId)
    {
        InitCurves();
    }
    else {
        QWidget::timerEvent(evt);
    }
}

void Plotter::InitCurves()
{
    PlotSettings settings = zoomStack[curZoom];
    int total = 100;
    float pi = 3.1415926;
    QVector<QPointF> vec;
    for (int var = 0; var < total; ++var) {

        QPointF pf((var+offset)*2*pi/100*settings.spanX(), 5+std::cos((var+offset)*2*pi/100*settings.spanX()));
        vec.append(pf);
    }
    setCurveData(0, vec);
    vec.clear();
    for (int var = 0; var < total; ++var) {
        QPointF pf((var+offset)*2*pi/100*settings.spanX(), 5+std::sin((var+offset)*2*pi/100*settings.spanX()));
        vec.append(pf);
    }
    setCurveData(1, vec);
    //scroll(-1, 0);
    offset++;
    offset %= total;
}

void Plotter::setPlotSettings(const PlotSettings &settings)
{
    zoomStack.clear();
    zoomStack.append(settings);
    curZoom = 0;
    zoomInButton->hide();
    zoomOutButton->hide();
    refreshPixmap();
}

void Plotter::zoomOut()
{
    if (curZoom > 0)
    {
        --curZoom;
        zoomOutButton->setEnabled(curZoom > 0);
        zoomInButton->setEnabled(true);
        zoomInButton->show();
        refreshPixmap();
    }
}

void Plotter::zoomIn()
{
    if (curZoom < zoomStack.count()-1)
    {
        ++curZoom;
        zoomInButton->setEnabled(curZoom < zoomStack.count()-1);
        zoomOutButton->setEnabled(true);
        zoomOutButton->show();
        refreshPixmap();
    }
}

void Plotter::setCurveData(int id, const QVector<QPointF> &data)
{
    curveMap[id] = data;
    refreshPixmap();
}

void Plotter::clearCurve(int id)
{
    curveMap.remove(id);
    refreshPixmap();
}

QSize Plotter::minimumSizeHint() const
{
    return QSize(6*Margin, 4*Margin);
}

QSize Plotter::sizeHint() const
{
    return QSize(12*Margin, 8*Margin);
}

void Plotter::paintEvent(QPaintEvent *evt)
{
    QStylePainter painter(this);
    painter.drawPixmap(0,0,pixmap);

    if (rubberBandIsShown)
    {
        painter.setPen(palette().light().color());
        painter.drawRect(rubberBandRect.normalized());
    }

    if (hasFocus())
    {
        QStyleOptionFocusRect option;
        option.initFrom(this);
        option.backgroundColor = palette().dark().color();
        painter.drawPrimitive(QStyle::PE_FrameFocusRect, option);
    }

}

void Plotter::resizeEvent(QResizeEvent *evt)
{
    int x = width()-(zoomInButton->width()+zoomOutButton->width()+10);
    zoomInButton->move(x, 5);
    zoomOutButton->move(x+zoomInButton->width()+5, 5);
    refreshPixmap();
}

void Plotter::mousePressEvent(QMouseEvent *evt)
{
    QRect rect(Margin, Margin, width()-2*Margin, height()-2*Margin);

    if (evt->button() == Qt::LeftButton)
    {
        if (rect.contains(evt->pos()))
        {
            rubberBandIsShown = true;
            rubberBandRect.setTopLeft(evt->pos());
            rubberBandRect.setBottomRight(evt->pos());
            updateRubberBandRegion();
            setCursor(Qt::CrossCursor);
        }
    }
}

void Plotter::mouseMoveEvent(QMouseEvent *evt)
{
    if (rubberBandIsShown)
    {
        updateRubberBandRegion();
        rubberBandRect.setBottomRight(evt->pos());
        updateRubberBandRegion();
    }
}

void Plotter::mouseReleaseEvent(QMouseEvent *evt)
{
    if (evt->button() == Qt::LeftButton && rubberBandIsShown)
    {
        rubberBandIsShown = false;
        updateRubberBandRegion();
        unsetCursor();

        QRect rect = rubberBandRect.normalized();
        if (rect.width() < 4 || rect.height() < 4)
            return ;
        rect.translate(-Margin, -Margin);

        PlotSettings prevSettings = zoomStack[curZoom];
        PlotSettings settings;
        double dx = prevSettings.spanX()/(width() - 2*Margin);
        double dy = prevSettings.spanY()/(height()- 2*Margin);
        settings.minX = prevSettings.minX + dx*rect.left();
        settings.maxX = prevSettings.minX + dx*rect.right();
        settings.minY = prevSettings.maxY - dy*rect.bottom();
        settings.maxY = prevSettings.maxY - dy*rect.top();
        settings.adjust();

        zoomStack.resize(curZoom+1);
        zoomStack.append(settings);
        zoomIn();
    }
}

void Plotter::keyPressEvent(QKeyEvent *evt)
{
    switch (evt->key()) {
    case Qt::Key_Plus:
        zoomIn();
        break;
    case Qt::Key_Minus:
        zoomOut();
        break;
    case Qt::Key_Left:
        zoomStack[curZoom].scoll(-1,0);
        refreshPixmap();
        break;
    case Qt::Key_Right:
        zoomStack[curZoom].scoll(+1,0);
        break;
    case Qt::Key_Down:
        zoomStack[curZoom].scoll(0,-1);
        break;
    case Qt::Key_Up:
        zoomStack[curZoom].scoll(0,+1);
        break;
    default:
        QWidget::keyPressEvent(evt);
    }
    QWidget::keyPressEvent(evt);
}

void Plotter::wheelEvent(QWheelEvent *evt)
{
    int numDegrees = evt->delta()/8;
    int numTicks = numDegrees/15;

    if (evt->orientation() == Qt::Horizontal)
    {
        zoomStack[curZoom].scoll(numTicks, 0);
    }
    else {
        zoomStack[curZoom].scoll(0, numTicks);
    }
    refreshPixmap();
}

void Plotter::updateRubberBandRegion()
{
    QRect rect = rubberBandRect.normalized();
    update(rect.left(), rect.top(), rect.width(), 1);
    update(rect.left(), rect.top(), 1, rect.height());
    update(rect.left(), rect.bottom(), rect.width(), 1);
    update(rect.right(), rect.top(), 1, rect.height());
}

void Plotter::refreshPixmap()
{
    pixmap = QPixmap(size());
    pixmap.fill(this, 0, 0);

    QPainter painter(&pixmap);
    painter.initFrom(this);
    drawGrid(&painter);
    drawCurves(&painter);
    update();
}

void Plotter::drawGrid(QPainter *painter)
{
    QRect rect(Margin, Margin, width()-2*Margin, height()-2*Margin);
    if (!rect.isValid())
        return;

    PlotSettings settings = zoomStack[curZoom];
    QPen quiteDark = palette().dark().color().light();
    QPen light = palette().light().color();

    for (int i=0; i<=settings.numXTicks; ++i)
    {
        int x = rect.left() + (i*(rect.width()-1))/settings.numXTicks;
        double label = settings.minX+(i*settings.spanX()/settings.numXTicks);
        painter->setPen(quiteDark);
        painter->drawLine(x, rect.top(), x, rect.bottom());
        painter->setPen(light);
        painter->drawLine(x, rect.bottom(), x, rect.bottom()+5);
        painter->drawText(x-50, rect.bottom()+5, 100,20,Qt::AlignHCenter|Qt::AlignTop,QString::number(label));
    }

    for (int j=0; j<=settings.numYTicks; ++j) {
        int y = rect.bottom() - (j*(rect.height()-1))/settings.numYTicks;
        double label = settings.minY+(j*settings.spanY()/settings.numYTicks);
        painter->setPen(quiteDark);
        painter->drawLine(rect.left(), y, rect.right(), y);
        painter->setPen(light);
        painter->drawLine(rect.left()-5, y, rect.right(), y);
        painter->drawText(rect.left()-Margin, y-10, Margin-5, 20,Qt::AlignVCenter|Qt::AlignRight, QString::number(label));
    }
    painter->drawRect(rect.adjusted(0,0,-1,-1));
}

void Plotter::drawCurves(QPainter *painter)
{
    static const QColor colorForIds[6]={
        Qt::red, Qt::green, Qt::blue, Qt::cyan, Qt::magenta, Qt::yellow
    };

    PlotSettings settings = zoomStack[curZoom];
    QRect rect(Margin, Margin, width()-2*Margin, height()-2*Margin);
    if (!rect.isValid())
        return;

    painter->setClipRect(rect.adjusted(1,1,-1,-1));
    QMapIterator<int, QVector<QPointF> > i(curveMap);
    while (i.hasNext()) {
        i.next();

        int id = i.key();
        QVector<QPointF> data = i.value();
        QPolygonF polyline(data.count());
        for (int j=0; j<data.count(); ++j)
        {
            double dx = data[j].x() -settings.minX;
            double dy = data[j].y() - settings.minY;
            double x = rect.left() + (dx*(rect.width()-1)/settings.spanX());
            double y = rect.bottom() - (dy * (rect.height()-1)/settings.spanY());
            polyline[j] = QPointF(x,y);
        }
        painter->setPen(colorForIds[uint(id)%6]);
        painter->drawPolyline(polyline);
    }
}

bool Plotter::eventFilter(QObject *target, QEvent *evt)
{
    if (target == zoomOutButton || target == zoomInButton)
    {
        if (evt->type() == QEvent::KeyPress)
        {
            QKeyEvent *kv = static_cast<QKeyEvent *>(evt);
            if (kv->key() == Qt::Key_Space)
            {
                focusNextChild();
                return true;
            }
        }
    }

    return QWidget::eventFilter(target, evt);
}

PlotSettings::PlotSettings()
{
    minX = 0.0;
    maxX = 10.0;
    numXTicks = 5;

    minY = 0.0;
    maxY = 10.0;
    numYTicks = 5;
}

void PlotSettings::scoll(int dx, int dy)
{
    double stepX = spanX()/numXTicks;
    minX = dx*stepX;
    maxX = dx*stepX;

    double stepY = spanY()/numYTicks;
    minY = dx*stepY;
    maxY = dx*stepY;
}

void PlotSettings::adjust()
{
    adjustAxis(minX, maxX, numXTicks);
    adjustAxis(minY, maxY, numYTicks);
}

void PlotSettings::adjustAxis(double &min, double &max, int &numTicks)
{
    const int MinTicks = 4;
    double grossStep = (max-min)/MinTicks;
    double step = std::pow(10.0, std::floor(std::log10(grossStep)));

    if (5*step < grossStep)
    {
        step*=5;
    }
    else if(2*step < grossStep)
    {
        step *= 2;
    }

    numTicks = int(std::ceil(max/step)-std::floor(min/step));
    if (numTicks<MinTicks)
        numTicks = MinTicks;
    min = std::floor(min/step)*step;
    max = std::ceil(max/step)*step;
}
