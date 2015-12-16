#include "mainwindow.h"
#include "ui_mainwindow.h"

MainWindow::MainWindow(QWidget *parent)
    : QMainWindow(parent), ui(new Ui::MainWindowClass)
{
    ui->setupUi(this);
    ui->glwidget->setFocusPolicy(Qt::StrongFocus);
    QObject::connect(ui->radioButtonGraham,&QRadioButton::clicked,ui->glwidget,&GLWidget::radioButtonGrahamClicked);
    QObject::connect(ui->radioButtonJarvis,&QRadioButton::clicked,ui->glwidget,&GLWidget::radioButtonJarvisClicked);

}

MainWindow::~MainWindow()
{
    delete ui;
}

