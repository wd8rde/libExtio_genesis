#include "mainwindow.h"
#include "ui_mainwindow.h"
#include <QDebug>

MainWindow::MainWindow(QWidget *parent) :
    QMainWindow(parent),
    ui(new Ui::MainWindow)
{
    ui->setupUi(this);
    connect( ui->pushButton_Atten, SIGNAL( pressed() ), this, SLOT( handleAttenuatorButton() ) );
}


MainWindow::~MainWindow()
{
    delete ui;
}
void MainWindow::handleAttenuatorButton()
{
    qWarning("button pressed");
}
