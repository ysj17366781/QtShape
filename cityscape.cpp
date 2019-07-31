#include "cityscape.h"
#include <cmath>
#include <QApplication>
#include <QCoreApplication>


Cityscape::Cityscape(QWidget *parent)
{
    scene = new QGraphicsScene(-22.25, -22.25, 1980,1980);
    scene->setBackgroundBrush(QColor(255,255,238));
    generateCityBlocks();

    view = new CityView(this);
    view->setScene(scene);

}

void Cityscape::generateCityBlocks()
{
    QSet<QString> names;
    names << "Adams" << "Agnew" << "Arthur" << "Breckinridge"
    << "Buchanan" << "Burr" << "Bush" << "Calhoun" << "Carter"
    << "Cheney" << "Cleveland" << "Clinton" << "Colfax"
    << "Coolidge" << "Curtis" << "Dallas" << "Dawes"
    << "Eisenhower" << "Fairbanks" << "Fillmore" << "Ford"
    << "Garfield" << "Garner" << "Gerry" << "Gore" << "Grant"
    << "Hamlin" << "Harding" << "Harrison" << "Hayes"
    << "Hendricks" << "Hobart" << "Hoover" << "Humphrey"
    << "Jackson" << "Jefferson" << "Johnson" << "Kennedy"
    << "King" << "Lincoln" << "Madison" << "Marshall"
    << "McKinley" << "Mondale" << "Monroe" << "Morton"
    << "Nixon" << "Pierce" << "Polk" << "Quayle" << "Reagan"
    << "Rockefeller" << "Roosevelt" << "Sherman" << "Stevenson"
    << "Taft" << "Taylor" << "Tompkins" << "Truman" << "Tyler"
    << "Van Buren" << "Wallace" << "Washington" << "Wheeler"
    << "Wilson";

    QSetIterator<QString> i(names);
    for (int y = 0; y < 44; ++y) {
        for (int x = 0; x < 44; ++x) {
            int percentile;
            if (x > 20 && x < 24 && y > 20 && y < 24) {
            percentile = std::rand() % (std::rand() % 2 != 0
            ? 10 : 100);
            } else if (x > 18 && x < 26 && y > 18 && y < 26) {
            percentile = std::rand() % (rand() % 3 != 0
            ? 10 : 100);
            } else if (x > 15 && x < 29 && y > 15 && y < 29) {
            percentile = std::rand() % (std::rand() % 5 != 0
            ? 10 : 100);
            } else {
            percentile = std::rand() % 100;
            }

            CityBlock::Kind kind;
            QString name;

            if (percentile == 0) {
            kind = CityBlock::Park;
            name = tr("%1 Park");
            } else if (percentile <= 2) {
            kind = CityBlock::SmallBuilding;
            } else if (percentile <= 4) {
            kind = CityBlock::Hospital;
            name = tr("%1 Hospital");
            } else if (percentile == 5) {
            kind = CityBlock::Hall;
            name = tr("%1 Hall");
            } else if (percentile <= 7) {
            kind = CityBlock::Building;
            name = tr("%1 Bldg");
            } else if (percentile <= 9) {
            kind = CityBlock::Tower;
            name = tr("%1 Tower");
            } else if (percentile <= 15) {
            kind = CityBlock::LShapedBlock;
            } else if (percentile <= 30) {
            kind = CityBlock::LShapedBlockPlusSmallBlock;
            } else if (percentile <= 70) {
            kind = CityBlock::TwoBlocks;
            } else {
            kind = CityBlock::BlockPlusTwoSmallBlocks;
            }

            CityBlock *block = new CityBlock(kind);
            block->setPos(QPointF(x * 44.5, y * 44.5));
            scene->addItem(block);

            if (!name.isEmpty()) {
                if (!i.hasNext())
                i.toFront();

                bool major = (std::rand() % 10 == 0);
                Annotation *annotation =
                new Annotation(name.arg(i.next()), major);
                annotation->setPos(block->pos());
                scene->addItem(annotation);
             }
        }
    }
}


CityBlock::CityBlock(Kind kind)
{
    this->kind = kind;

    int green = 96+(std::rand()%64);
    int red = 16+green+(std::rand()%64);
    int blue = 16+(std::rand()%green);
    color = QColor(red, green, blue);

    if (kind == Park) {
        color = QColor(192 + (std::rand() % 32), 255,
        192 + (std::rand() % 16));
        shape.addRect(boundingRect());
    } else if (kind == SmallBuilding) {
        QRectF block(-7.5, -7.5, 15, 15);
        block.moveBottomLeft(QPointF((std::rand() % 6) - 3,
        (std::rand() % 6) - 3));
        shape.addRect(block);
    } else if (kind == Hospital) {
        int a = (std::rand() % 6) + 10;
        int b = (std::rand() % 6) + 10;
        QPolygonF block;
        block << QPointF(-5, -a) << QPointF(-5, -5) << QPointF(-10, -5)
        << QPointF(-10, 5) << QPointF(-5, 5)  << QPointF(-5, 10)
        << QPointF(5, 10)  << QPointF(5, 5)   << QPointF(b, 5)
        << QPointF(b, -5)  << QPointF(5, -5)  << QPointF(5, -a);
        shape.addPolygon(block);
    } else if (kind == Hall) {
        int padding1 = (std::rand() % 8) + 2;
        int padding2 = (std::rand() % 8) + 2;
        shape.addEllipse(boundingRect().adjusted(+padding1, +padding1,
        -padding2, -padding2));
    } else if (kind == Building) {
        shape.addRect(boundingRect());
    } else if (kind == Tower) {
        int padding1 = (std::rand() % 8) + 2;
        int padding2 = (std::rand() % 8) + 2;
        shape.addRect(boundingRect().adjusted(+padding1, +padding1,
        -padding2, -padding2));
    } else if (kind == LShapedBlock
    || kind == LShapedBlockPlusSmallBlock) {
        int a = (std::rand() % 6) + 10;
        int b = (std::rand() % 6) + 10;
        int s = qMin(a, b) / 2;
        QPolygonF block;
        block << QPointF(-a, -a) << QPointF(-a, +a) << QPointF(-s, +a)
        << QPointF(-s, -s) << QPointF(+b, -s) << QPointF(+b, -a);
        shape.addPolygon(block);
        if (kind == LShapedBlockPlusSmallBlock) {
        int inset = (std::rand() % 4) + 4;
        shape.addRect(QRectF(-s + inset, -s + inset, a, b));
    }
    } else if (kind == TwoBlocks) {
        int w1 = (std::rand() % 10) + 8;
        int h1 = (std::rand() % 28) + 8;
        int w2 = (std::rand() % 10) + 8;
        int h2 = (std::rand() % 24) + 8;
        shape.addRect(QRectF(-16, -16, w1, h1));
        shape.addRect(QRectF(-16 + w1 + 4, -16 + (std::rand() % 4),
        w2, h2));
    } else if (kind == BlockPlusTwoSmallBlocks) {
        int w1 = (std::rand() % 10) + 8;
        int h1 = (std::rand() % 28) + 8;
        int w2 = (std::rand() % 10) + 8;
        int h2 = (std::rand() % 10) + 8;
        int w3 = (std::rand() % 6) + 8;
        int h3 = (std::rand() % 6) + 8;
        int y = (std::rand() % 4) - 16;
        shape.addRect(QRectF(-16, -16, w1, h1));
        shape.addRect(QRectF(-16 + w1 + 4, y, w2, h2));
        shape.addRect(QRectF(-16 + w1 + 4,
        y + h2 + 4 + (std::rand() % 4), w3, h3));
    }
}

QRectF CityBlock::boundingRect() const
{
    return QRectF(-20,-20,40,40);
}

void CityBlock::paint(QPainter *painter, const QStyleOptionGraphicsItem *option, QWidget *widget)
{
    if (option->levelOfDetail < 4.0){
        painter->fillPath(shape, color);
    }else {
        QLinearGradient gradient(QPoint(-20,-20), QPoint(20,20));
        int coeff = 105 + int(std::log(option->levelOfDetail-4.0));
        gradient.setColorAt(0.0, color.lighter(coeff));
        gradient.setColorAt(1.0, color.darker(coeff));
        painter->fillPath(shape, gradient);
    }
}






Annotation::Annotation(const QString &text, bool major)
{
    font = qApp->font();
    font.setBold(true);
    if (major){
        font.setPointSize(font.pointSize()+2);
        font.setStretch(QFont::SemiExpanded);
    }

    if(major){
        threshold = 0.01 * (40 + (std::rand()%40));
    }else {
        threshold = 0.01 * (100 + (std::rand()%100));
    }

    str = text;
    this->major = major;
    y = 20-(std::rand()%40);

    setZValue(1000);
    setFlag(ItemIgnoresTransformations, true);
}

void Annotation::setText(const QString &text)
{
    prepareGeometryChange();
    str = text;
    update();
}

QRectF Annotation::boundingRect() const
{
    QFontMetrics metrics(font);
    QRectF rect = metrics.boundingRect(str);
    rect.moveCenter(QPointF(0,y));
    rect.adjust(-4,0,4,0);
    return rect;
}

void Annotation::paint(QPainter *painter, const QStyleOptionGraphicsItem *option, QWidget *widget)
{
    if (option->levelOfDetail <= threshold)
        return;

    painter->setFont(font);

    QRectF rect = boundingRect();

    int alpha = int(30*std::log(option->levelOfDetail));
    if (alpha >= 32)
        painter->fillRect(rect, QColor(255,255,255,qMin(alpha,63)));

    painter->setPen(Qt::white);
    painter->drawText(rect.translated(1,1),str,QTextOption(Qt::AlignCenter));
    painter->setPen(Qt::blue);
    painter->drawText(rect,str, QTextOption(Qt::AlignCenter));
}



CityView::CityView(QWidget *parent):QGraphicsView(parent)
{
    setDragMode(ScrollHandDrag);
}

void CityView::wheelEvent(QWheelEvent *evt)
{
    double numDegreees = -evt->delta()/8.0;
    double numSteps = numDegreees/15.0;
    double factor = std::pow(1.125, numSteps);
    scale(factor, factor);
}















