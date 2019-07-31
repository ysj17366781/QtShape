#ifndef OVENTIMER_H
#define OVENTIMER_H

#include <QCoreApplication>
#include <QApplication>
#include <QWidget>
#include <QMap>
#include <QPixmap>
#include <QVector>
#include <QWidget>
#include <QToolButton>
#include <QStylePainter>
#include <QMouseEvent>
#include <QLineEdit>
#include <QDateTime>
#include <QGraphicsLineItem>
#include <QGraphicsScene>
#include <QGraphicsView>
#include <QGraphicsSceneMouseEvent>
#include <QStyleOptionGraphicsItem>
#include <QTimer>


class Link;
class Node : public QGraphicsItem
{
    Q_DECLARE_TR_FUNCTIONS(Node)

public:
    Node();
    ~Node();

    void setText(const QString &text);
    QString text() const{return myText;}
    void setTextColor(const QColor &color);
    QColor textColor() const{return myTextColor;}
   void setOutlineColor(const QColor &color);
   QColor outlineColor() const {return myOutlineColor;}
   void setBackgroudColor(const QColor &color);
   QColor backgroudColor() const {return myBackgroundColor;}

   void addLink(Link *link);
   void removeLink(Link *link);

   QRectF boundingRect() const;
   QPainterPath shape() const;
   void paint(QPainter *painter, const QStyleOptionGraphicsItem *option, QWidget *widget);

protected:
   void mouseDoubleClickEvent(QGraphicsSceneMouseEvent *evt);
   QVariant itemChange(GraphicsItemChange change, const QVariant &value);

private:
   QRectF outlineRect() const;
   int roundness(double size) const;

   QSet<Link *> myLinks;
   QString myText;
   QColor myTextColor;
   QColor myBackgroundColor;
   QColor myOutlineColor;

};





class Link :public QGraphicsLineItem
{
public:
    Link(Node *fromNode, Node *toNode);
    ~Link();

    Node *fromNode() const;
    Node *toNode() const;

    void setColor(const QColor &color);
    QColor color() const{QPen pen = this->pen();return pen.color();}

    void trackNodes();

private:
    Node *myFromNode;
    Node *myToNode;
};








class OvenTimer :public QWidget{
    Q_OBJECT
public:
    OvenTimer(QWidget *parent=0);

    void setDuration(int secs);
    int duration() const;
    void draw(QPainter *painter);

signals:
    void timeout();

protected:
    void paintEvent(QPaintEvent *evt);
    void mousePressEvent(QMouseEvent *evt);

private:
    QDateTime finishTime;
    QTimer *updateTimer;
    QTimer *finishTimer;
};


#endif // OVENTIMER_H
