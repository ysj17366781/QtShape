#include "oventimer.h"
#include <cmath>
#include <QInputDialog>


Node::Node()
{
    myTextColor = Qt::darkGreen;
    myOutlineColor = Qt::darkBlue;
    myBackgroundColor = Qt::white;

    setFlags(ItemIsMovable | ItemIsSelectable);
}

Node::~Node()
{
    foreach (Link *link, myLinks)
        delete link;
}

void Node::setText(const QString &text)
{
    prepareGeometryChange();
    myText = text;
    update();
}

void Node::setTextColor(const QColor &color)
{
    myTextColor = color;
    update();
}

void Node::setOutlineColor(const QColor &color)
{
    myOutlineColor = color;
    update();
}

void Node::setBackgroudColor(const QColor &color)
{
    myBackgroundColor = color;
}

void Node::addLink(Link *link)
{
    myLinks.insert(link);
}

void Node::removeLink(Link *link)
{
    myLinks.remove(link);
}

QRectF Node::outlineRect() const
{
    const int Padding = 8;
    QFontMetricsF metrics = qApp->font();
    QRectF rect = metrics.boundingRect(myText);
    rect.adjust(-Padding, -Padding, Padding, Padding);
    rect.translate(-rect.center());
    return rect;
}

QRectF Node::boundingRect() const
{
    const int Margin = 1;
    return outlineRect().adjusted(-Margin, -Margin, Margin, Margin);
}

QPainterPath Node::shape() const
{
    QRectF rect = outlineRect();
    QPainterPath path;
    path.addRoundRect(rect, roundness(rect.width()), roundness((rect.height())));
    return path;

}

void Node::paint(QPainter *painter, const QStyleOptionGraphicsItem *option, QWidget *widget)
{
    QPen pen(myOutlineColor);
    if (option->state & QStyle::State_Selected){
        pen.setStyle(Qt::DotLine);
        pen.setWidth(2);
    }

    painter->setPen(pen);
    painter->setBrush(myBackgroundColor);

    QRectF rect = outlineRect();
    QPainterPath path;
    painter->drawRoundRect(rect, roundness(rect.width()), roundness(rect.width()));

    painter->setPen(myTextColor);
    painter->drawText(rect, Qt::AlignCenter, myText);
}

QVariant Node::itemChange(GraphicsItemChange change, const QVariant &value)
{
    if (change == ItemPositionHasChanged){
        foreach(Link *link, myLinks)
            link->trackNodes();
    }
    return QGraphicsItem::itemChange(change, value);
}

void Node::mouseDoubleClickEvent(QGraphicsSceneMouseEvent *evt)
{
    QString text = QInputDialog::getText( evt->widget(), tr("edit text"), tr("enter new text:"),
                                         QLineEdit::Normal, myText);
    if (!text.isEmpty())
        setText(text);
}

int Node::roundness(double size) const
{
    const int Diameter = 12;
    return 100*Diameter/int(size);
}



Link::Link(Node *fromNode, Node *toNode)
{
    myFromNode = fromNode;
    myToNode = toNode;

    myFromNode->addLink(this);
    myToNode->addLink(this);

    setFlags(QGraphicsItem::ItemIsSelectable);
    setZValue(-1);

    setColor(Qt::darkRed);
    trackNodes();
}

Link::~Link()
{

}

void Link::setColor(const QColor &color)
{
    setPen(QPen(color, 1.0));
}

void Link::trackNodes()
{
    setLine(QLineF(myFromNode->pos(), myToNode->pos()));
}




const double DegreesPerMinute = 7.0;
const double DegreesPreSecond = DegreesPerMinute/60;
const int MaxMinutes = 45;
const int MaxSeconds = MaxMinutes*60;
const int UpdateInterval = 5;
OvenTimer::OvenTimer(QWidget *parent):QWidget(parent)
{
    finishTime = QDateTime::currentDateTime();
    updateTimer = new QTimer(this);
    connect (updateTimer, SIGNAL(timeout), this, SLOT(update()));

    finishTimer = new QTimer(this);
    finishTimer->setSingleShot(true);
    connect(finishTimer, SIGNAL(timeout()), this, SIGNAL(timeout()));
    connect(finishTimer, SIGNAL(timeout()), updateTimer, SLOT(stop()));

    QFont font;
    font.setPointSize(8);
    setFont(font);
}

void OvenTimer::setDuration(int secs)
{
    secs = qBound(0,secs,MaxSeconds);

    finishTime = QDateTime::currentDateTime().addSecs(secs);

    if(secs > 0){
        updateTimer->start(UpdateInterval*1000);
        finishTimer->start(secs*1000);
    }else{
        updateTimer->stop();
        finishTimer->stop();
    }
    update();
}

int OvenTimer::duration() const
{
    int secs = QDateTime::currentDateTime().secsTo(finishTime);
    if (secs < 0)
        secs=0;
    return secs;
}

void OvenTimer::mousePressEvent(QMouseEvent *evt)
{
    QPointF point = evt->pos() - rect().center();
    double theta = std::atan2(-point.x(), -point.y())*180/M_PI;
    setDuration(duration()+int(theta/DegreesPreSecond));
    update();
}

void OvenTimer::paintEvent(QPaintEvent *evt)
{
    QPainter painter(this);
    painter.setRenderHint(QPainter::Antialiasing, true);

    int side = qMin(width(), height());

    painter.setViewport((width()-side)/2, (height()-side)/2, side, side);
    painter.setWindow(-50,-50,100,100);
    draw(&painter);
}

void OvenTimer::draw(QPainter *painter)
{
    static const int triangle[3][2]={
        {-2,-49},{2,-49},{0,47}
    };
    QPen thickPen(palette().foreground(), 1.5);
    QPen thinPen(palette().foreground(), 0.5);
    QColor niceBlue(150,150,200);

    painter->setPen(thinPen);
    painter->setBrush(palette().foreground());
    painter->drawPolygon(QPolygon(3,&triangle[0][0]));

    QConicalGradient coneGradient(0,0,-90);
    coneGradient.setColorAt(0.0, Qt::darkGray);
    coneGradient.setColorAt(0.2, niceBlue);
    coneGradient.setColorAt(0.5, Qt::white);
    coneGradient.setColorAt(1.0, Qt::darkGray);

    painter->setBrush(coneGradient);
    painter->drawEllipse(-46,-46,40,40);

    QRadialGradient haloGradient(0,0,20,0,0);
    haloGradient.setColorAt(0.0, Qt::lightGray);
    haloGradient.setColorAt(0.8, Qt::darkGray);
    haloGradient.setColorAt(0.9, Qt::white);
    haloGradient.setColorAt(1.0, Qt::black);

    painter->setPen(Qt::NoPen);
    painter->setBrush(haloGradient);
    painter->drawEllipse(-20,-20,40,40);

    QLinearGradient knobGradient(-7,-25,-7,-25);
    knobGradient.setColorAt(0.0,Qt::black);
    knobGradient.setColorAt(0.2,niceBlue);
    knobGradient.setColorAt(0.3,Qt::lightGray);
    knobGradient.setColorAt(0.8,Qt::white);
    knobGradient.setColorAt(1.0,Qt::black);

    painter->rotate(duration()*DegreesPreSecond);
    painter->setBrush(knobGradient);
    painter->setPen(thinPen);
    painter->drawRoundRect(-7,-25,14,50,99,49);

    for (int i=0;i<=MaxMinutes;++i) {
        if(i%5==0){
            painter->setPen(thickPen);
            painter->drawLine(0,-41,0,-44);
            painter->drawText(-15,-41,30,30,
                              Qt::AlignHCenter | Qt::AlignTop,
                              QString::number(i));
        }else{
            painter->setPen(thinPen);
            painter->drawLine(0,-42,0,-44);
        }
        painter->rotate(-DegreesPerMinute);

    }
}
