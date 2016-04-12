#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QMainWindow>


class QGraphicsView;
class QGraphicsScene;
class QGraphicsPixmapItem;
class QGraphicsProxyWidget;

class QComboBox;
class QLabel;
class QLineEdit;
class QPushButton;
class QRadioButton;
class QFrame;

class QPointF;
class calculateTrack;
class graph;
class city;


class MainWindow : public QMainWindow{
  Q_OBJECT

public:
  MainWindow(QWidget *parent = 0);
  ~MainWindow();

private slots:
  void setScale();
  void addCity();
  void calculate();
  void reset();
  void setCalculateMethod();
private:
  void createWidgets();
  void cleanData();
  void arrangeSceneItems();
  void drawCity(city *_city);
  void drawGraph(QVector<QPair<QPointF,double> > *track);

  enum calcMode {noMode,dijkstryMode,fastMode};
  calcMode mode;

  QVector<city*> *cities;

  QGraphicsScene *scene;
  QGraphicsView *view;
  QGraphicsPixmapItem *backgroundItem;
  QGraphicsProxyWidget *dataWidget,*scaleWidget,*modeWidget;

  graph *_graph;
  bool graphDrawn;
  calculateTrack *algorithm;

  QLabel *cityNameLabel,*xCoordLabel,*yCoordLabel,*scaleLabel;
  QLineEdit *cityNameLineEdit,*xCoordLineEdit,*yCoordLineEdit,*scaleLineEdit;
  QPushButton *addCityButton,*calculateButton,*scaleButton,*resetButton;
  QFrame *dataFrame,*scaleFrame,*modeFrame;
  QRadioButton *dijkstryOptionRadioButton,*fastestWayOptionRadioButton;
};

#endif // MAINWINDOW_H
