#include <QtCore>
#include "algorithm.h"
#include "city.h"
#include "graph.h"

calculateTrack::calculateTrack(){
}

double calculateTrack::calculateDistance(QPointF ptA,QPointF ptB){
  double x1,x2,y1,y2;
  x1 = ptA.x();
  y1 = ptA.y();
  x2 = ptB.x();
  y2 = ptB.y();
  return sqrt((x2-x1)*(x2-x1)+(y2-y1)*(y2-y1));
}

int calculateTrack::findStartElement(QVector<city *> *cities, QString initialCityName){
  int initialCityIndex = -1;
  for(unsigned int i=0;i<cities->size();++i){
      if(cities->at(i)->returnCityName() == initialCityName){		//szukam indeksu poczatkowego miasta
          cities->at(i)->visit();
          initialCityIndex = i;
          break;
        }
    }
  return initialCityIndex;
}

QVector<QPair<QPointF,double> >* calculateTrack::dijkstryMethod(QVector<city*> *cities, QString initialCityName){
  int initialCityIndex = findStartElement(cities,initialCityName);
  if(initialCityIndex == -1) return NULL;      //***

  QVector<QPair<QPointF,double> > *result = new QVector<QPair<QPointF,double> >;
  result->push_back(QPair<QPointF,double>(cities->at(initialCityIndex)->returnPos(),0));  //dodaje punkt startowy

  int currentCityIndex = initialCityIndex;
  double shortestPath = 0;
  for(unsigned int i=1;i<cities->size();++i){
      double tempDistance;
      int shortestDistanceElementIndex;

      for(int j=(cities->size()-1);j>=0;--j){
          if(!cities->at(j)->isVisited()){
              tempDistance = calculateDistance(cities->at(currentCityIndex)->returnPos(),cities->at(j)->returnPos());
              if(tempDistance < shortestPath || !shortestPath){
                  shortestPath = tempDistance;																													//poczatek szukania..
                  shortestDistanceElementIndex = j;
                }
            }
        }
      currentCityIndex = shortestDistanceElementIndex;
      cities->at(shortestDistanceElementIndex)->visit();

      QPointF nearestCityPos(cities->at(currentCityIndex)->returnPos());                            //{w obu f.}: dodac pkt do vectora od ost. el. do pierwszego na koniec
      result->push_back(QPair<QPointF,double>(nearestCityPos,shortestPath));
      shortestPath = 0;
    }
  startPoint = result->at(0).first;
  return result;
}

QVector<QPair<QPointF,double> >* calculateTrack::fastMethod(QVector<city*> *cities, QString initialCityName){
  int initialCityIndex = findStartElement(cities,initialCityName);
  if(initialCityIndex == -1) return NULL;

  QVector<QPair<QPointF,double> > *result = new QVector<QPair<QPointF,double> >;
  result->push_back(QPair<QPointF,double>(cities->at(initialCityIndex)->returnPos(),0));  //dodaje punkt startowy

  double tempDistance = 0;
  int j = 0;
  for(int i=0;i<cities->size();++i){
      if(i!=initialCityIndex){
          tempDistance = calculateDistance(result->at(j).first,cities->at(i)->returnPos());
          result->push_back(QPair<QPointF,double>(cities->at(i)->returnPos(),tempDistance));
          ++j;
        }
    }
  return result;
}

