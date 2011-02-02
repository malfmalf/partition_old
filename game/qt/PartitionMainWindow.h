#pragma once
#include "ui_PartitionGame.h"
#include <QtGui>


class PartitionMainWindow : public QMainWindow {
    Q_OBJECT
public:
    PartitionMainWindow(void);

private:
    QSettings           settings;
    Ui_MainWindow       ui;
};
