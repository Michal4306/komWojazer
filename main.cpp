#include "mainwindow.h"
#include <QApplication>
#include <QtWidgets>

int main(int argc, char *argv[]){
  QApplication a(argc, argv);
  MainWindow myWind;
  myWind.show();

  return a.exec();
}
//- dodac przycisk reset/clean
//- osobny sposob liczenia sciezki {najszybszy sposob [wg kolejnosci w vectorze?]} (2 radioButtony nad wykresem)

// f. calculate(); - jak rozegrac obsluge trybow: djikstry/fastestWay [zmienic nazwe alg. na dijkstry i dodac drugi, ktory tez pobierze nazwe miasta pocz. i utworzy z
// niego wektor QPointF'sow?


//rozbudowac o opcje: 'najkrotsza droga z pktu A do pktu B' [dijkstry]

//doszlifowac zmiane trybow {po przelaczeniu z fast'a na dijkstry jest problem}

//przemyslec przyciski (klikanie enterem) {http://stackoverflow.com/questions/11887938/how-to-make-a-qpushbutton-pressable-for-enter-key}

//przemienic reset(); w resetGraph(); i stworzyc jako metode klasy graph{}

