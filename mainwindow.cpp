#include <QtWidgets>
#include "mainwindow.h"
#include "city.h"
#include "graph.h"
#include "algorithm.h"

MainWindow::MainWindow(QWidget *parent)
  : QMainWindow(parent), mode(noMode), graphDrawn(false){
  cities = new QVector<city*>;
  _graph = new graph(QPointF(100,500),30);
  algorithm = new calculateTrack;

  scene = new QGraphicsScene(this);
  view = new QGraphicsView(scene,this);
  setCentralWidget(view);
  createWidgets();
  arrangeSceneItems();
  resize(1000,700);
  layout()->setSizeConstraint(QLayout::SetFixedSize);
//  setFocusProxy(scaleButton);
//  setFocusPolicy(Qt::TabFocus);
//  setFocusProxy(scaleButton);
}

MainWindow::~MainWindow(){
  cleanData();
  delete cities;
}

void MainWindow::setScale(){
  int scale = scaleLineEdit->text().toInt();
  if(scale<=0){
      QMessageBox::information(this,tr("Error"),tr("Wrong scale!"),QMessageBox::Ok);
      return ;
    }
  QPixmap pixmap = backgroundItem->pixmap();
  _graph->drawScale(pixmap,scale);
  backgroundItem->setPixmap(pixmap);
  scaleLineEdit->setDisabled(true);
  scaleButton->setDisabled(true);
}

void MainWindow::addCity(){
  int scale = scaleLineEdit->text().toInt();
  if(scale<=0){
      QMessageBox::information(this,tr("Error"),tr("Set the correct scale!"),QMessageBox::Ok);
      return ;
    }

  int x,y;
  x = xCoordLineEdit->text().toInt();
  y = yCoordLineEdit->text().toInt();
  QString cityName = cityNameLineEdit->text();
  if(x<0 || y<0 || !cityName.size()){
      QMessageBox::information(this,tr("Error!"),tr("Wrong data! Check the data."),QMessageBox::Ok);
      return ;
    }
  else if(x>(scale*10) || y>(scale*10)){
      QMessageBox::information(this,tr("Error!"),tr("Value out of scale."),QMessageBox::Ok);
      return ;
    }

  city *newCity = new city(cityName,QPointF(x,y));
  drawCity(newCity);
  cities->push_back(newCity);
}

void MainWindow::calculate(){
  //*************
  if(graphDrawn){
      QPixmap pixmap = _graph->initialDrawing();
      backgroundItem->setPixmap(pixmap);
      setScale();
      for(int i=0;i<cities->size();++i){
          drawCity(cities->at(i));
        }
    }
  //*****************
  QString initCityName = QInputDialog::getText(this,tr("Where to start from?"),tr("Set the initial city name: "));
  QVector<QPair<QPointF,double> > *track;
  switch(mode){
    case dijkstryMode:
      track = algorithm->dijkstryMethod(cities,initCityName);
      break;
    case fastMode:
      track = algorithm->fastMethod(cities,initCityName);
      break;
    case noMode:
      QMessageBox::information(this,tr("Error"),tr("Set the calculation mode!"));
      return ;
    }

  if(!track){
      QMessageBox::information(this,tr("Error"),tr("There is no city '%1").arg(initCityName) + "'!");
      return ;
    }
  drawGraph(track);
  double totalDistance = 0;
  for(int i=0;i<track->size();++i){
      totalDistance += track->at(i).second;
    }
  totalDistance += algorithm->calculateDistance(track->at(track->size() -1).first,track->at(0).first);
  QMessageBox::information(this,tr("Total distance"),tr("Total distance: %1").arg(totalDistance),QMessageBox::Ok);
  delete track;
}

void MainWindow::cleanData(){
  if(cities){
    for(int i=0;i<cities->size();++i){
        delete cities->at(i);
      }
    cities->resize(0);
    }
}

void MainWindow::arrangeSceneItems(){
  backgroundItem = scene->addPixmap(_graph->initialDrawing());
  dataWidget = scene->addWidget(dataFrame);
  modeWidget = scene->addWidget(modeFrame);
  QPointF ptOfRef = _graph->pointOfReference();
  dataWidget->setPos(QPointF(ptOfRef.x() +430,ptOfRef.y() -270));
  modeWidget->setPos(QPointF(ptOfRef.x() +50,ptOfRef.y() -400));
}

void MainWindow::drawCity(city *_city){
  QPixmap pixmap = backgroundItem->pixmap();
  QString cityName = _city->returnCityName();
  QPointF cityPos = _city->returnPos();

  _graph->drawPoint(pixmap,cityPos.x(),cityPos.y(),cityName);
  backgroundItem->setPixmap(pixmap);
}

void MainWindow::drawGraph(QVector<QPair<QPointF, double> > *track){   //w petli rysuje linie wzgledem kolejnych pktow..
  QPixmap pixmap = backgroundItem->pixmap();
  for(int i=0;i<track->size()-1;++i){
      _graph->drawLine(pixmap,track->at(i).first,track->at(i+1).first,true);
    }
  _graph->drawLine(pixmap,track->at(track->size() -1).first,track->at(0).first,true); //dorysowanie linii powrotnej
  QPointF startPt = algorithm->returnStartPoint();
  _graph->colourPoint(pixmap,startPt);
  backgroundItem->setPixmap(pixmap);

  graphDrawn = true;
}

void MainWindow::reset(){
  cleanData();
  _graph->reset();

  scaleLineEdit->clear();
  cityNameLineEdit->clear();
  xCoordLineEdit->clear();
  yCoordLineEdit->clear();

  QPixmap pixmap = _graph->initialDrawing();

  backgroundItem->setPixmap(pixmap);
  scaleLineEdit->setDisabled(false);
  scaleButton->setDisabled(false);
}

void MainWindow::setCalculateMethod(){
  if(dijkstryOptionRadioButton->isChecked()){
      mode = dijkstryMode;
    }
  else if(fastestWayOptionRadioButton->isChecked()){
      mode = fastMode;
    }

  if(graphDrawn){
      QPixmap pixmap = _graph->initialDrawing();
      backgroundItem->setPixmap(pixmap);
      setScale();
      for(int i=0;i<cities->size();++i){
          drawCity(cities->at(i));
        }
      calculate();
    }
}

void MainWindow::createWidgets(){
  dataFrame = new QFrame;
  dataFrame->setFrameStyle(QFrame::Box | QFrame::Plain);
  scaleFrame = new QFrame;
  scaleFrame->setFrameStyle(QFrame::Box | QFrame::Plain);
  modeFrame = new QFrame;
  modeFrame->setFrameStyle(QFrame::Box | QFrame::Plain);

//  cityNameLabel = new QLabel(tr("Set the city name:       "));
//  cityNameLineEdit = new QLineEdit;
//  xCoordLabel = new QLabel(tr("Set the 'x' coordinate: "));
//  xCoordLineEdit = new QLineEdit;
//  yCoordLabel = new QLabel(tr("Set the 'y' coordinate: "));
//  yCoordLineEdit = new QLineEdit;
//  scaleLabel = new QLabel(tr("Scale:           "));
//  scaleLineEdit = new QLineEdit;
  addCityButton = new QPushButton(tr("Add"));
  connect(addCityButton,SIGNAL(clicked()),this,SLOT(addCity()));
  calculateButton = new QPushButton(tr("Calculate"));
  connect(calculateButton,SIGNAL(clicked()),this,SLOT(calculate()));
  scaleButton = new QPushButton(tr("Ok"));
//  scaleButton->setDefault(true);
  connect(scaleButton,SIGNAL(clicked()),this,SLOT(setScale()));
  resetButton = new QPushButton(tr("Reset"));
  connect(resetButton,SIGNAL(clicked()),this,SLOT(reset()));
//***
  cityNameLabel = new QLabel(tr("Set the city name:       "));
  cityNameLineEdit = new QLineEdit;
  xCoordLabel = new QLabel(tr("Set the 'x' coordinate: "));
  xCoordLineEdit = new QLineEdit;
  yCoordLabel = new QLabel(tr("Set the 'y' coordinate: "));
  yCoordLineEdit = new QLineEdit;
  scaleLabel = new QLabel(tr("Scale:           "));
  scaleLineEdit = new QLineEdit;
//***
//  this->setTabOrder(scaleButton,addCityButton);
//  this->setTabOrder(addCityButton,calculateButton);


  dijkstryOptionRadioButton = new QRadioButton(tr("Dijkstry"));
  connect(dijkstryOptionRadioButton,SIGNAL(clicked()),this,SLOT(setCalculateMethod()));
  fastestWayOptionRadioButton = new QRadioButton(tr("Fastest way"));
  connect(fastestWayOptionRadioButton,SIGNAL(clicked()),this,SLOT(setCalculateMethod()));

  QHBoxLayout *modeLayout = new QHBoxLayout;
  modeLayout->addWidget(dijkstryOptionRadioButton);
  modeLayout->addWidget(fastestWayOptionRadioButton);
  modeLayout->addSpacerItem(new QSpacerItem(20,20));
  modeLayout->addWidget(resetButton);
  modeFrame->setLayout(modeLayout);

  QHBoxLayout *scaleLayout = new QHBoxLayout;
  scaleLayout->addWidget(scaleLabel);
  scaleLayout->addWidget(scaleLineEdit);
  scaleLayout->addWidget(scaleButton);
  scaleFrame->setLayout(scaleLayout);

  QHBoxLayout *nameLayout = new QHBoxLayout;
  nameLayout->addWidget(cityNameLabel);
  nameLayout->addWidget(cityNameLineEdit);

  QHBoxLayout *xCoordLayout = new QHBoxLayout;
  xCoordLayout->addWidget(xCoordLabel);
  xCoordLayout->addWidget(xCoordLineEdit);

  QHBoxLayout *yCoordLayout = new QHBoxLayout;
  yCoordLayout->addWidget(yCoordLabel);
  yCoordLayout->addWidget(yCoordLineEdit);

  QVBoxLayout *leftVLayout = new QVBoxLayout;
  leftVLayout->addLayout(nameLayout);
  leftVLayout->addLayout(xCoordLayout);
  leftVLayout->addLayout(yCoordLayout);

  QVBoxLayout *rightVLayout = new QVBoxLayout;
  rightVLayout->addWidget(addCityButton);
  rightVLayout->addWidget(calculateButton);

  QHBoxLayout *groupLayout = new QHBoxLayout;
  groupLayout->addLayout(leftVLayout);
  groupLayout->addLayout(rightVLayout);

  QVBoxLayout *mainLayout = new QVBoxLayout;
  mainLayout->addWidget(scaleFrame);
  mainLayout->addLayout(groupLayout);
  dataFrame->setLayout(mainLayout);
}
