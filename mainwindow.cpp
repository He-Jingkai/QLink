#include "mainwindow.h"
#include "ui_mainwindow.h"
#include "doubleform.h"
#include <single.h>

MainWindow::MainWindow(QWidget *parent)
    : QMainWindow(parent)
    , ui(new Ui::MainWindow){
    ui->setupUi(this);
    QToolButton* button1 = this->findChild<QToolButton *>("toolButton");
    QToolButton* button2 = this->findChild<QToolButton *>("toolButton_2");
    button1->setStyleSheet("background: rgb(194,194,194)");
    button1->setIcon(QIcon("C:/Users/hjk02/Documents/QLink/personLOGO1.jpg"));
    button1->setIconSize(button1->size());
    button2->setStyleSheet("background: rgb(194,194,194)");
    button2->setIcon(QIcon("C:/Users/hjk02/Documents/QLink/personLOGO2.jpg"));
    button2->setIconSize(button1->size());

}

MainWindow::~MainWindow(){
    delete ui;
}


void MainWindow::on_pushButton_clicked(){
    Single* single=new Single;
    single->show();
    this->close();
}

void MainWindow::on_pushButton_2_clicked(){
    DoubleForm* doubleform=new DoubleForm;
    doubleform->show();
    this->close();
}

void MainWindow::on_pushButton_3_clicked(){
    this->close();
}

void MainWindow::on_pushButton_4_clicked()
{
    QString jsonPath = QFileDialog::getOpenFileName(
            this,
            tr("选择存档文件"),
            "/",
            tr("*.json"));
    fstream file(jsonPath.toStdString().c_str());
    if(jsonPath.toStdString()=="")return;
    Json::Reader reader;
    Json::Value root;
    string type;
    if (reader.parse(file, root)){
       if (!root["type"].isNull())
            type= root["type"].asString();
       else {
           QMessageBox::warning(this,"error","文件错误或已经损坏");
               return;
       }
    }
    else {
        QMessageBox::warning(this,"error","文件错误或已经损坏");
            return;
    }
    if(type=="single"){
        GameInfo* gameInfo=new GameInfo;
        bool right=gameInfo->readJson(jsonPath.toStdString());
        if(!right){
            QMessageBox::warning(this,"error","文件错误或已经损坏");
                return;
        }
        Single* single=new Single(nullptr,gameInfo);
        single->show();
        this->close();
    }
    else {
        GameInfoDouble* gameInfo=new GameInfoDouble;
        bool right=gameInfo->readJson(jsonPath.toStdString());
        if(!right){
            QMessageBox::warning(this,"error","文件错误或已经损坏");
                return;
        }
        DoubleForm* doubleForm=new DoubleForm(nullptr,gameInfo);
        doubleForm->show();
        this->close();
    }


}
