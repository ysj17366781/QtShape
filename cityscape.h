#ifndef CITYSCAPE_H
#define CITYSCAPE_H
#include <QWidget>
#include <QGraphicsScene>
#include <QGraphicsView>
#include <QGraphicsItem>
#include <QPainter>
#include <QStyleOptionGraphicsItem>
#include <QWheelEvent>

class CityView : public QGraphicsView
{
    Q_OBJECT
public:
    CityView(QWidget *parent=0);

protected:
    void wheelEvent(QWheelEvent *evt);

};

class Annotation : public QGraphicsItem
{
public:
    Annotation(const QString &text, bool major=false);

    void setText(const QString &text);
    QString text() const;

    QRectF boundingRect() const;
    void paint(QPainter *painter, const QStyleOptionGraphicsItem *option, QWidget *widget);

private:
    QFont font;
    QString str;
    bool major;
    double threshold;
    int y;
};

class CityBlock : public QGraphicsItem
{
public:
    enum Kind{Park, SmallBuilding, Hospital, Hall, Building, Tower,
             LShapedBlock, LShapedBlockPlusSmallBlock, TwoBlocks,
             BlockPlusTwoSmallBlocks};

    CityBlock(Kind kind);

    QRectF boundingRect() const;
    void paint(QPainter *painter, const QStyleOptionGraphicsItem *option, QWidget *widget);

private:
    int kind;
    QColor color;
    QPainterPath shape;
};

class Cityscape : public QWidget
{
    Q_OBJECT
public:
    Cityscape(QWidget *parent=0);

private:
    void generateCityBlocks();

    QGraphicsScene *scene;
    CityView    *view;
};

#endif // CITYSCAPE_H
