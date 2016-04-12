#include <QtWidgets>
#include "graph.h"

graph::graph(QPointF point, int _offset): isScaled(false), scale(0), offset(_offset), ptZero(point),
  xMax(QPointF(point.x() + 330,point.y())), yMax(QPointF(point.x(),point.y() -330)){
}

QPointF graph::fitPosition(QPointF pos){
  int xPos = 0, yPos = 0;
  xPos = ptZero.x() + ((offset * pos.x())/scale);
  yPos = ptZero.y() - ((offset * pos.y())/scale);
  return QPointF(xPos,yPos);
}

QPixmap graph::initialDrawing(){
  QPixmap pixmap(1000,700);
  pixmap.fill(Qt::white);

  if(0 >= offset) return pixmap;

  QPainter painter(&pixmap);
  painter.setPen(QPen(Qt::black,2));
  painter.drawLine(ptZero,xMax);
  painter.drawLine(ptZero,yMax);

  QPointF tempX(xMax),tempY(yMax);
  tempX.setX(tempX.x() -30);
  tempX.setY(tempX.y() +15);
  tempY.setY(tempY.y() +30);
  tempY.setX(tempY.x() -15);

  for(int i=10;i>0;--i){
      painter.drawLine(QPointF(tempX.x(),tempX.y() -10),QPointF(tempX.x(),tempX.y() -20));
      painter.drawLine(QPointF(tempY.x() +10,tempY.y()),QPointF(tempY.x() +20,tempY.y()));
      tempX.setX(tempX.x() - offset);
      tempY.setY(tempY.y() + offset);
    }
  drawGraphTriangle(painter,xMax,90);
  drawGraphTriangle(painter,yMax,0);
  return pixmap;
}

void graph::reset(){
  isScaled = scale = 0;
}

void graph::drawScale(QPixmap &pixmap, int _scale){
  if(0 >= offset) return ;
  if(isScaled){
      pixmap = initialDrawing();
    }

  QPainter painter(&pixmap);
  QPointF tempX(xMax),tempY(yMax);
  tempX.setX(tempX.x() -30);
  tempX.setY(tempX.y() + 15);
  tempY.setY(tempY.y() +30);
  tempY.setX(tempY.x() -15);

  for(int i=(_scale*10);i>0;i-=_scale){
      painter.drawText(tempX,QString::number(i));
      painter.drawText(QPointF(tempY.x() -5,tempY.y()),QString::number(i));
      tempX.setX(tempX.x() - offset);
      tempY.setY(tempY.y() + offset);
    }
  painter.drawText(QPointF(ptZero.x() -8,ptZero.y() +8),"0");

  scale = _scale;
  isScaled = true;
}

void graph::drawPoint(QPixmap &pixmap, int x, int y, QString ptName){
  QPointF newPoint = fitPosition(QPointF(x,y));
  QPainter painter(&pixmap);
  painter.setPen(QPen(Qt::black,5));
  painter.drawPoint(newPoint);

  QPointF textPos = QPointF(newPoint.x() -10,newPoint.y());
  painter.drawText(textPos,ptName);
}

void graph::drawLine(QPixmap &pixmap, QPointF ptA, QPointF ptB, bool arrow){
  QPointF _ptA = fitPosition(ptA);
  QPointF _ptB = fitPosition(ptB);

  QPainter painter(&pixmap);
  painter.setPen(QPen(Qt::black,3));  
  painter.drawLine(_ptA,_ptB);
  if(arrow){
      drawArrow(painter,_ptA,_ptB);
    }
}

void graph::colourPoint(QPixmap &pixmap, const QPointF &pt){
  QPainter painter(&pixmap);
  QPointF point = fitPosition(pt);
  painter.setPen(QPen(Qt::green,8));
  painter.drawPoint(point);
}

void graph::drawGraphTriangle(QPainter &painter, const QPointF &pos, int rotation){
  QPointF ptA(-8,0),ptB(ptA.x() + 16,ptA.y()),ptC(ptA.x() +8,ptA.y() -8);
  QPainterPath path(ptA);
  path.lineTo(ptB);
  path.lineTo(ptC);
  path.lineTo(ptA);
  painter.translate(pos);
  if(rotation){
      painter.rotate(rotation);
    }
  painter.drawPath(path);
  painter.fillPath(path,QBrush(Qt::black));
  painter.resetTransform();
}

void graph::drawArrow(QPainter &painter, QPointF &ptA, QPointF &ptB){
  if(ptA == ptB || (0 >= offset)) return ;
  QPointF arrowPos;
  int arrowPosX = 0,arrowPosY = 0;
  (ptA.x() < ptB.x())? arrowPosX = ptA.x() + ((ptB.x() - ptA.x())/2) : arrowPosX = ptB.x() + ((ptA.x() - ptB.x())/2);
  (ptA.y() > ptB.y())? arrowPosY = ptA.y() - ((ptA.y() - ptB.y())/2) : arrowPosY = ptB.y() - ((ptB.y() - ptA.y())/2);

  int angle = calculateAngle(ptA,ptB);
  if(ptA.x() == ptB.x()){
      arrowPos = QPointF(ptA.x(),arrowPosY);
    }
  else if(ptA.y() == ptB.y()){
          arrowPos = QPointF(arrowPosX,ptA.y());
    }
  else if(ptA.x() < ptB.x()){
      arrowPos = QPointF(arrowPosX,arrowPosY);
    }
  else if(ptA.x() > ptB.x()){
      arrowPos = QPointF(arrowPosX,arrowPosY);
    }
  drawGraphTriangle(painter,arrowPos,angle);
}

int graph::calculateAngle(QPointF &ptA, QPointF &ptB){
  int angle,hDist,vDist;
  QPointF temp45;
  double distance = 0;

  (ptA.y() > ptB.y())? vDist = (ptA.y() - ptB.y()) : vDist = (ptB.y() - ptA.y());

  if(ptA.x() == ptB.x()){
      if(ptA.y() > ptB.y()){
          angle = 0;
        }
      else if(ptA.y() < ptB.y()){
          angle = 180;
        }
    }
  else if(ptA.y() == ptB.y()){
      if(ptA.x() < ptB.x()){
          angle = 90;
        }
      else if(ptA.x() > ptB.x()){
          angle = 270;
        }
    }
  else if(ptA.x() < ptB.x()){
      temp45 = ptA;
      hDist = (ptB.x() - ptA.x());
      if(hDist<=vDist){
          for(int i=0;i<(vDist/offset);++i){
              temp45.setX(temp45.x() + offset);
              temp45.setY(temp45.y() - offset);
            }
          distance = (temp45.x() - ptA.x());
          distance = hDist/distance;
          angle = distance *45;
        }
      else if(vDist<hDist){
          for(int i=0;i<(hDist/offset);++i){
              temp45.setX(temp45.x() + offset);
              temp45.setY(temp45.y() - offset);
            }
          distance = (ptA.y() - temp45.y()) - vDist;
          distance = distance/(ptA.y() - temp45.y());
          angle = (45 + (distance *45));
        }
      if(ptA.y() < ptB.y()){
          angle = (180 - angle);
        }
    }
  else if(ptA.x() > ptB.x()){
      temp45 = ptB;
      hDist = (ptA.x() - ptB.x());
      if(hDist<vDist){
          for(int i=0;i<(vDist/offset);++i){
              temp45.setX(temp45.x() + offset);
              temp45.setY(temp45.y() - offset);
            }
          distance = (temp45.x() - ptB.x());
          distance = hDist/distance;
          angle = (distance *45);

          if(ptA.y() > ptB.y()){
              angle = (360 - angle);
            }
          else if(ptA.y() < ptB.y()){
              angle += 180;
            }
        }
      else if(vDist<=hDist){
          for(int i=0;i<(hDist/offset);++i){
              temp45.setX(temp45.x() + offset);
              temp45.setY(temp45.y() - offset);
            }
          distance = (ptB.y() - temp45.y()) - vDist;
          distance = distance/(ptB.y() - temp45.y());
          angle = (45 + (distance *45));

          if(ptA.y() > ptB.y()){
              angle = (360 - angle);
            }
          else if(ptA.y() < ptB.y()){
              angle = (180 + angle);
            }
        }
    }
  return angle;
}
