#ifndef GRAPH_H
#define GRAPH_H


class QString;
class QPainter;
class QPixmap;
class QPointF;

class graph{
public:
  graph(QPointF point = QPointF(100,400), int _offset = 30);
  QPointF pointOfReference(){return ptZero;}
  QPointF fitPosition(QPointF pos);
  QPixmap initialDrawing();
  void reset();
  void drawScale(QPixmap &pixmap, int _scale);
  void drawPoint(QPixmap &pixmap, int x, int y, QString ptName);
  void drawLine(QPixmap &pixmap, QPointF ptA, QPointF ptB, bool arrow = false);
  void colourPoint(QPixmap &pixmap, const QPointF &pt);
private:
  void drawGraphTriangle(QPainter &painter, const QPointF &pos, int rotation);
  void drawArrow(QPainter &painter, QPointF &ptA, QPointF &ptB);
  int calculateAngle(QPointF &ptA, QPointF &ptB);

  bool isScaled;
  int scale;
  const int offset;   //odstep (w pikselach) miedzy jednostkami na wykresie
  const QPointF xMax,yMax,ptZero;
};

#endif // GRAPH_H
