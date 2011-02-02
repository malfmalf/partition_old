#include "PartitionMainWindow.h"

int main(int argc,char*argv[]){
    QApplication app(argc, argv);
    PartitionMainWindow *MainWindow = new PartitionMainWindow;
    MainWindow->show();
    return app.exec();
}


PartitionMainWindow::PartitionMainWindow(void) : settings("MALF","BalloonsQt"){
    ui.setupUi(this);
    connect(ui.numBalls,SIGNAL(valueChanged(int)),ui.canvas,SLOT(setNumBalls(int)));
    connect(ui.numGameWidth,SIGNAL(valueChanged(int)),ui.canvas,SLOT(setGameWidth(int)));
    connect(ui.numGameHeight,SIGNAL(valueChanged(int)),ui.canvas,SLOT(setGameHeight(int)));
    connect(ui.numSpeed,SIGNAL(valueChanged(double)),ui.canvas,SLOT(setSpeed(double)));
}

