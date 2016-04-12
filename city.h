#ifndef CITY_H
#define CITY_H

class QString;
class QPointF;

class city{
public:
    city(QString name,QPointF pos): cityName(name),position(pos),visited(false){}
    void visit(){visited = true;}
    bool isVisited(){return visited;}
    QPointF returnPos(){return position;}
    QString returnCityName(){return cityName;}
private:
    bool visited;
    QPointF position;
    QString cityName;
};

#endif // CITY_H
