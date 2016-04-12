#ifndef ALGORITHM_H
#define ALGORITHM_H

class QString;
template <typename T1,typename T2> class QPair;
template <typename T1> class QVector;
class QPointF;
class city;

class calculateTrack{
public:
  calculateTrack();
  const QPointF returnStartPoint(){return startPoint;}
  QVector<QPair<QPointF,double> >* dijkstryMethod(QVector<city*> *cities, QString initialCityName);
  QVector<QPair<QPointF,double> >* fastMethod(QVector<city*> *cities, QString initialCityName);
  bool checkInitialCity(QString initialCityName);
  double calculateDistance(QPointF ptA, QPointF ptB);
private:
  int findStartElement(QVector<city*> *cities, QString initialCityName);
  QPointF startPoint;
};

#endif // ALGORITHM_H
