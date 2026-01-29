#include "mainwindow.h"
#include "ui_mainwindow.h"
#include "modbusconfig.h"

#include <QDir>
#include <QDebug>
#include <QTcpSocket>






MainWindow::MainWindow(QWidget *parent) :
    QMainWindow(parent),
    ui(new Ui::MainWindow)
{
    ui->setupUi(this);

    initWidget();
    connectFun();
}

MainWindow::~MainWindow()
{


    delete ui;
}

void MainWindow::saveseting(){

    settings->setValue("backOrigin_velocity", ui->backOrigin_velocity->text());
    settings->setValue("jog_velocity", ui->jog_velocity->text());
    settings->setValue("originSpeed", ui->originSpeed->text());
    settings->setValue("startSpeed", ui->startSpeed->text());
    settings->setValue("endSpeed", ui->endSpeed->text());


    settings->setValue("sweepSpeed", ui->sweepSpeed->text());

    settings->setValue("xlenght", ui->x_lenght->text());
    settings->setValue("ylenght", ui->y_lenght->text());
    settings->setValue("step", ui->y_step->text());



    QString currentPiece = ui->regin->currentText();
    settings->beginGroup(currentPiece);

    settings->setValue("xlenght", ui->x_lenght->text());
    settings->setValue("ylenght", ui->y_lenght->text());
    settings->setValue("step", ui->y_step->text());

    settings->setValue("inputx_2", ui->inputx_2->text());
    settings->setValue("inputy_2", ui->inputy_2->text());
    settings->setValue("inputz_2", ui->inputz_2->text());
    settings->setValue("inputr_2", ui->inputr_2->text());

    settings->setValue("inputx_3", ui->inputx_3->text());
    settings->setValue("inputy_3", ui->inputy_3->text());
    settings->setValue("inputz_3", ui->inputz_3->text());
    settings->setValue("inputr_3", ui->inputr_3->text());

    settings->setValue("inputx_4", ui->inputx_4->text());
    settings->setValue("inputy_4", ui->inputy_4->text());
    settings->setValue("inputz_4", ui->inputz_4->text());
    settings->setValue("inputr_4", ui->inputr_4->text());

    settings->setValue("sweepSpeed", ui->sweepSpeed->text());

    settings->endGroup();
    settings->sync();  // 强制写入磁盘
}

void MainWindow::initWidget()
{
    //    ScanControlAbstract *temp;
    scanCtrlHunChuan = new ScanControlHuiChuan();
    scanCtrlTaiDa = new ScanControlTaiDa();

    scanCtrl = scanCtrlHunChuan;
    scanCtrl2 = scanCtrlTaiDa;

    scanCtrl->setModbusTcpIP(ui->IP_Edit->text());
    scanCtrl->setModbusPort(ui->port_Edit->text().toInt());
    scanCtrl2->setModbusTcpIP(ui->IP_Edit2->text());
    scanCtrl2->setModbusPort(ui->port_Edit2->text().toInt());

    QString appDataPath = QCoreApplication::applicationDirPath()+"/date";
    QDir dir(appDataPath);
    if (!dir.exists())
        dir.mkpath(".");

    settings=new QSettings(appDataPath+"/settings.ini", QSettings::IniFormat);

    ui->backOrigin_velocity->setText(settings->value("backOrigin_velocity", "0").toString());
    ui->jog_velocity->setText(settings->value("jog_velocity", "0").toString());
    ui->originSpeed->setText(settings->value("originSpeed", "0").toString());
    ui->startSpeed->setText(settings->value("startSpeed", "0").toString());
    ui->endSpeed->setText(settings->value("endSpeed", "0").toString());


    ui->sweepSpeed->setText(settings->value("sweepSpeed", "").toString());

    ui->x_lenght->setText(settings->value("xlenght", "").toString());
    ui->y_lenght->setText(settings->value("ylenght", "").toString());
    ui->y_step->setText(settings->value("step", "").toString());

    QString activePiece =  ui->regin->currentText();
    QStringList groups = settings->childGroups();
    if (groups.contains(activePiece)) {
        ui->inputx_2->setText(settings->value("inputx_2", "").toString());
        ui->inputy_2->setText(settings->value("inputy_2", "").toString());
        ui->inputz_2->setText(settings->value("inputz_2", "").toString());
        ui->inputr_2->setText(settings->value("inputr_2", "").toString());

        ui->inputx_3->setText(settings->value("inputx_3", "").toString());
        ui->inputy_3->setText(settings->value("inputy_3", "").toString());
        ui->inputz_3->setText(settings->value("inputz_3", "").toString());
        ui->inputr_3->setText(settings->value("inputr_3", "").toString());

        ui->inputx_4->setText(settings->value("inputx_4", "").toString());
        ui->inputy_4->setText(settings->value("inputy_4", "").toString());
        ui->inputz_4->setText(settings->value("inputz_4", "").toString());
        ui->inputr_4->setText(settings->value("inputr_4", "").toString());

        ui->sweepSpeed->setText(settings->value("sweepSpeed", "").toString());

        ui->x_lenght->setText(settings->value("xlenght", "").toString());
        ui->y_lenght->setText(settings->value("ylenght", "").toString());
        ui->y_step->setText(settings->value("step", "").toString());
    }

    settings->endGroup();

    scanCtrl->speed=ui->jog_velocity->text().toFloat();
    scanCtrl2->speed=ui->jog_velocity->text().toFloat();
}



void MainWindow::connectFun()
{


    connect(scanCtrl, &ScanControlAbstract::tcpStateChange,
            this, [this](bool connected) {
        ui->connectBtn->setText(connected ? "discon" : "conect");

    });

//    connect(scanCtrl2, &ScanControlAbstract::tcpStateChange,
//            this, [this](bool connected) {
//        ui->connectBtn->setText(connected ? "conneced" : "discon");
//    });


    connect(scanCtrl, &ScanControlAbstract::positionChange, this, &MainWindow::updatePosition);
    connect(scanCtrl2, &ScanControlAbstract::positionChange, this, &MainWindow::updatePosition2);

    connect(ui->connectBtn, &QPushButton::clicked, scanCtrl, &ScanControlAbstract::on_connectBtn_clicked);
    //connect(ui->connectBtn, &QPushButton::clicked, scanCtrl2, &ScanControlAbstract::on_connectBtn_clicked);

    connect(ui->startScanBtn, &QPushButton::clicked, scanCtrl, &ScanControlAbstract::on_start);
    connect(ui->stopScanBtn, &QPushButton::clicked, scanCtrl, &ScanControlAbstract::on_stop);
    connect(ui->endScanBtn, &QPushButton::clicked, scanCtrl, &ScanControlAbstract::on_end);
    connect(ui->resetScanBtn, &QPushButton::clicked, scanCtrl, &ScanControlAbstract::on_alarmReset);
    connect(ui->backZeroScanBtn, &QPushButton::clicked, scanCtrl, &ScanControlAbstract::on_backZero);

    connect(ui->xAddBtn, &QPushButton::pressed, scanCtrl, &ScanControlAbstract::on_xAddBtn_pressed);
    connect(ui->xAddBtn, &QPushButton::released, scanCtrl, &ScanControlAbstract::on_xAddBtn_released);
    connect(ui->xSubBtn, &QPushButton::pressed, scanCtrl, &ScanControlAbstract::on_xSubBtn_pressed);
    connect(ui->xSubBtn, &QPushButton::released, scanCtrl, &ScanControlAbstract::on_xSubBtn_released);
    connect(ui->yAddBtn, &QPushButton::pressed, scanCtrl, &ScanControlAbstract::on_yAddBtn_pressed);
    connect(ui->yAddBtn, &QPushButton::released, scanCtrl, &ScanControlAbstract::on_yAddBtn_released);
    connect(ui->ySubBtn, &QPushButton::pressed, scanCtrl, &ScanControlAbstract::on_ySubBtn_pressed);
    connect(ui->ySubBtn, &QPushButton::released, scanCtrl, &ScanControlAbstract::on_ySubBtn_released);

    connect(ui->zAddBtn, &QPushButton::pressed, scanCtrl2, &ScanControlAbstract::on_zAddBtn_pressed);
    connect(ui->zAddBtn, &QPushButton::released, scanCtrl2, &ScanControlAbstract::on_zAddBtn_released);
    connect(ui->zSubBtn, &QPushButton::pressed, scanCtrl2, &ScanControlAbstract::on_zSubBtn_pressed);
    connect(ui->zSubBtn, &QPushButton::released, scanCtrl2, &ScanControlAbstract::on_zSubBtn_released);
    connect(ui->rAddBtn, &QPushButton::pressed, scanCtrl2, &ScanControlAbstract::on_rAddBtn_pressed);
    connect(ui->rAddBtn, &QPushButton::released, scanCtrl2, &ScanControlAbstract::on_rAddBtn_released);
    connect(ui->rSubBtn, &QPushButton::pressed, scanCtrl2, &ScanControlAbstract::on_rSubBtn_pressed);
    connect(ui->rSubBtn, &QPushButton::released, scanCtrl2, &ScanControlAbstract::on_rSubBtn_released);

    connect(ui->backOrigin_velocity, &QLineEdit::editingFinished, this, &MainWindow::backOrigin_velocity);
    connect(ui->jog_velocity, &QLineEdit::editingFinished, this, &MainWindow::jog_velocity);
    connect(ui->originSpeed, &QLineEdit::editingFinished, this, &MainWindow::originSpeed);
    connect(ui->startSpeed, &QLineEdit::editingFinished, this, &MainWindow::startSpeed);
    connect(ui->endSpeed, &QLineEdit::editingFinished, this, &MainWindow::endSpeed);
    connect(ui->setMOrigin, &QPushButton::clicked, this, &MainWindow::setMOrigin);

    connect(ui->regin,
            QOverload<const QString &>::of(&QComboBox::currentIndexChanged),
            this,
            &MainWindow::regin);

    connect(ui->setBtn, &QPushButton::clicked, this, &MainWindow::setBtn);


}


void MainWindow::updatePosition(int pos)
{

    //qDebug()<<"updatePosition";
    ui->xCurPos->setText(QString::number(static_cast<double>(pos), 'f', 3));
    position.x=pos;
}

void MainWindow::updatePosition2(int pos)
{

    //qDebug()<<"updatePosition";
    ui->yCurPos->setText(QString::number(static_cast<double>(pos), 'f', 3));
    position.y=pos;
}



void MainWindow::backOrigin_velocity(){

     saveseting();
}

void MainWindow::jog_velocity(){

     scanCtrl->speed=ui->jog_velocity->text().toFloat();
     scanCtrl2->speed=ui->jog_velocity->text().toFloat();
     qDebug()<<"jog_velocity:"<< scanCtrl->speed;
     saveseting();
}

void MainWindow::originSpeed(){

    saveseting();
}

void MainWindow::startSpeed(){

     saveseting();
}

void MainWindow::endSpeed(){

    saveseting();
}



std::vector<Point2D> MainWindow::generateBowScanPathDense(
    float xLength,
    float yLength,
    float yStep
)
{
    std::vector<Point2D> path;

    float x = 0;
    float y = 0;

    bool leftToRight = true;
    float yEnd = 0 + yLength;

    // 起点
    path.push_back({x, y});

    while (y <= yEnd) {

        // 1. 沿 X 方向跑完整 xLength
        if (leftToRight) {
            x = 0 + xLength;   // → 右
        } else {
            x = 0;             // ← 左
        }

        path.push_back({x, y});     // 行终点

        // 2. 向下走一步
        y += yStep;
        if (y > yEnd) break;

        path.push_back({x, y});     // 下移点

        // 3. 方向反转
        leftToRight = !leftToRight;
    }

    return path;
}



void MainWindow::setBtn(){

    xlenght=ui->x_lenght->text().toFloat();
    ylenght=ui->y_lenght->text().toFloat();
    step=ui->y_step->text().toFloat();


    saveseting();

}

void MainWindow::setOrigin(){



    saveseting();
}

void MainWindow::setStart(){

    auto path = generateBowScanPathDense(xlenght, ylenght, step);

    for (auto& p : path) {
        qDebug() << "Move to:" << p.x << p.y;



    }


    saveseting();
}

void MainWindow::setEnd(){


    saveseting();
}



void MainWindow::setMOrigin(){


    scanCtrl->on_setOriginBtn_clicked();
    scanCtrl2->on_setOriginBtn_clicked();
    scanCtrl->updataCurrentPos();
    scanCtrl2->updataCurrentPos();
    saveseting();
}




















void MainWindow::regin(){

    if(ui->regin->currentIndex()==0){

        origin=0x0063;
        start=0x0064;
        end=0x0065;

        xo=0x0000;
        yo=0x0018;

        xs=0x0008;
        ys=0x0020;

        xe=0x0010;
        ye=0x0028;

        sweep=0X012B;

    }else if (ui->regin->currentIndex()==1) {


        origin=0x0066;
        start=0x0067;
        end=0x0068;

        xo=0x0002;
        yo=0x001A;

        xs=0x000A;
        ys=0x0022;

        xe=0x0012;
        ye=0x002A;

        sweep=0X012D;
    }else if (ui->regin->currentIndex()==2) {

        origin=0x0069;
        start=0x006A;
        end=0x006B;

        xo=0x0004;
        yo=0x001C;

        xs=0x000C;
        ys=0x0024;

        xe=0x0014;
        ye=0x002C;

        sweep=0X012F;
    }else if (ui->regin->currentIndex()==3) {

        origin=0x006C;
        start=0x006D;
        end=0x006E;

        xo=0x0006;
        yo=0x001E;

        xs=0x000E;
        ys=0x0026;

        xe=0x0016;
        ye=0x002E;

        sweep=0X0131;
    }


    QString activePiece =  ui->regin->currentText();
    QStringList groups = settings->childGroups();
    if (groups.contains(activePiece)) {
        ui->inputx_2->setText(settings->value("inputx_2", "").toString());
        ui->inputy_2->setText(settings->value("inputy_2", "").toString());
        ui->inputz_2->setText(settings->value("inputz_2", "").toString());
        ui->inputr_2->setText(settings->value("inputr_2", "").toString());

        ui->inputx_3->setText(settings->value("inputx_3", "").toString());
        ui->inputy_3->setText(settings->value("inputy_3", "").toString());
        ui->inputz_3->setText(settings->value("inputz_3", "").toString());
        ui->inputr_3->setText(settings->value("inputr_3", "").toString());

        ui->inputx_4->setText(settings->value("inputx_4", "").toString());
        ui->inputy_4->setText(settings->value("inputy_4", "").toString());
        ui->inputz_4->setText(settings->value("inputz_4", "").toString());
        ui->inputr_4->setText(settings->value("inputr_4", "").toString());

        ui->sweepSpeed->setText(settings->value("sweepSpeed", "").toString());
    }

    settings->endGroup();



}
