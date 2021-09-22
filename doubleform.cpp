#include "doubleform.h"
#include "ui_doubleform.h"
#include "mainwindow.h"


DoubleForm::DoubleForm(QWidget *parent,GameInfoDouble* gameInfo_) :
    QDialog(parent),
    ui(new Ui::DoubleForm){
    this->setWindowFlags(Qt::CustomizeWindowHint | Qt::WindowCloseButtonHint);
    ui->setupUi(this);
    buttonMatix=vector<vector<QToolButton*>>(6,vector<QToolButton*>(6,0));
    for(int i=0;i<6;i++)
        for(int j=0;j<6;j++)
            buttonMatix[i][j] = this->findChild<QToolButton *>(QString::fromStdString("toolButton_"+to_string(i)+to_string(j)));
    scoreBoard.push_back(this->findChild<QLCDNumber*>("lcdNumber"));
    scoreBoard.push_back(this->findChild<QLCDNumber*>("lcdNumber_3"));
    timeBoard=this->findChild<QLCDNumber*>("lcdNumber_2");
    generateMap(gameInfo_);
    renewTimer();
    timer = new QTimer(this);
    connect(timer, SIGNAL(timeout()), this, SLOT(renewTime()));
    timer->start(1000);
}


DoubleForm::~DoubleForm(){
    timer->stop();
    delete ui;
}


void DoubleForm::renewTimer(){
    timeBoard->display(QString::fromStdString(to_string(gameInfo.time_)));
    if(gameInfo.time_==0)
        gameOver(2);
}


void DoubleForm::setButtonBox1(int i,int j){
    buttonMatix[i][j]->setStyleSheet("background: rgb(255,255,255)");
    buttonMatix[i][j]->setIcon(QIcon("C:/Users/hjk02/Documents/QLink/box1.jpg"));
    buttonMatix[i][j]->setIconSize(QSize(buttonMatix[i][j]->size().rwidth()-2,buttonMatix[i][j]->size().rheight()-2));
}


void DoubleForm::setButtonBox2(int i,int j){
    buttonMatix[i][j]->setStyleSheet("background: rgb(255,255,255)");
    buttonMatix[i][j]->setIcon(QIcon("C:/Users/hjk02/Documents/QLink/box2.jpg"));
    buttonMatix[i][j]->setIconSize(QSize(buttonMatix[i][j]->size().rwidth()-2,buttonMatix[i][j]->size().rheight()-2));
}


void DoubleForm::setButtonBox3(int i,int j){
    buttonMatix[i][j]->setStyleSheet("background: rgb(255,255,255)");
    buttonMatix[i][j]->setIcon(QIcon("C:/Users/hjk02/Documents/QLink/box3.jpg"));
    buttonMatix[i][j]->setIconSize(QSize(buttonMatix[i][j]->size().rwidth()-2,buttonMatix[i][j]->size().rheight()-2));
}


void DoubleForm::setButtonPerson1(int i,int j){
    buttonMatix[i][j]->setStyleSheet("background: rgb(194,194,194)");
    buttonMatix[i][j]->setIcon(QIcon("C:/Users/hjk02/Documents/QLink/personLOGO1.jpg"));
    buttonMatix[i][j]->setIconSize(buttonMatix[i][j]->size());
}


void DoubleForm::setButtonPerson2(int i,int j){
    buttonMatix[i][j]->setStyleSheet("background: rgb(194,194,194)");
    buttonMatix[i][j]->setIcon(QIcon("C:/Users/hjk02/Documents/QLink/personLOGO2.jpg"));
    buttonMatix[i][j]->setIconSize(buttonMatix[i][j]->size());
}


void DoubleForm::setButtonWhite(int i,int j){
    buttonMatix[i][j]->setIcon(QIcon());
    buttonMatix[i][j]->setStyleSheet("background: rgb(255,255,255)");
}


void DoubleForm::setButtonLight1(int i,int j){
    buttonMatix[i][j]->setStyleSheet("background:rgb(255,255,255);border:5px solid rgb(255,255,0);");
}


void DoubleForm::unsetButtonLight(int i,int j){
    buttonMatix[i][j]->setStyleSheet("background:rgb(255,255,255);border:0px solid rgb(255,255,255);");
}


void DoubleForm::setButtonLight2(int i,int j){
    buttonMatix[i][j]->setStyleSheet("background:rgb(255,255,255);border:5px solid rgb(0,255,127);");
}


void DoubleForm::setButtonBluePeroid(PATH path){
    gameInfo.bluePath[0]=path;
    for(POINT point:path)
        buttonMatix[point.first][point.second]->setStyleSheet("background: rgb(204,255,255)");
}


void DoubleForm::setButtonPinkPeroid(PATH path){
    gameInfo.bluePath[1]=path;
    for(POINT point:path)
        buttonMatix[point.first][point.second]->setStyleSheet("background: rgb(255,105,180)");
}


void DoubleForm::unsetButtonBluePeroid(){
    for(POINT point:gameInfo.bluePath[0])
        buttonMatix[point.first][point.second]->setStyleSheet("background: rgb(255,255,255)");
    gameInfo.bluePath[0].clear();
}


void DoubleForm::unsetButtonPinkPeroid(){
    for(POINT point:gameInfo.bluePath[1])
        buttonMatix[point.first][point.second]->setStyleSheet("background: rgb(255,255,255)");
    gameInfo.bluePath[1].clear();
}


void DoubleForm::renewScore(int person,int score){
    scoreBoard[person]->display(QString::fromStdString(to_string(score)));
}


void DoubleForm::gameOver(int type){
    timer->stop();
    if(type==1){
        QMessageBox message(QMessageBox::NoIcon, "游戏结束", QString::fromStdString("恭喜你,已经消除了所有可以消除的方块，甲方的得分为"+to_string(gameInfo.scores[0])+", 乙方的得分为"+to_string(gameInfo.scores[1])),QMessageBox::Yes | QMessageBox::No, NULL);
        QPixmap qPixmap("C:/Users/hjk02/Documents/QLink/personLOGO1.jpg");
        qPixmap = qPixmap.scaled(40, 40, Qt::KeepAspectRatio);
        message.setIconPixmap(qPixmap);
        message.button(QMessageBox::Yes)->setText("返回主界面");
        message.button(QMessageBox::No)->setText("再来一局");
        int ret=message.exec();
        if(ret==QMessageBox::Yes){
            timer->stop();
            MainWindow* mainWindow=new MainWindow;
            this->destroy();
            mainWindow->show();
        }
        else if(ret==QMessageBox::No){
            timer->stop();
            DoubleForm* doubleForm=new DoubleForm;
            this->destroy();
            doubleForm->show();
        }
    }
    else if(type==2){
        QMessageBox message(QMessageBox::NoIcon, "游戏结束", QString::fromStdString("时间到，甲方的得分为"+to_string(gameInfo.scores[0])+", 乙方的得分为"+to_string(gameInfo.scores[1])),QMessageBox::Yes | QMessageBox::No, NULL);
        QPixmap qPixmap("C:/Users/hjk02/Documents/QLink/personLOGO1.jpg");
        qPixmap = qPixmap.scaled(40, 40, Qt::KeepAspectRatio);
        message.setIconPixmap(qPixmap);
        message.button(QMessageBox::Yes)->setText("返回主界面");
        message.button(QMessageBox::No)->setText("再来一局");
        int ret=message.exec();
        if(ret==QMessageBox::Yes){
            timer->stop();
            MainWindow* mainWindow=new MainWindow;
            this->destroy();
            mainWindow->show();
        }
        else if(ret==QMessageBox::No){
            timer->stop();
            DoubleForm* doubleForm=new DoubleForm;
            this->destroy();
            doubleForm->show();
        }
    }
}


void DoubleForm::keyPressEvent(QKeyEvent *event){
    if(gameInfo.stop)
        return;

    if(event->isAutoRepeat())
        return;
    int key=event->key(),direction;
    switch (key) {
        case Qt::Key_W:{//w
            if(!gameInfo.bluePath[0].empty())
                unsetButtonBluePeroid();
            direction=1;
            PersonMoveDouble personMove=gameInfo.personMove(0,direction);
            renewMap(personMove);
            break;
        }
        case Qt::Key_A:{//a
            if(!gameInfo.bluePath[0].empty())
                unsetButtonBluePeroid();
            direction=3;
            PersonMoveDouble personMove=gameInfo.personMove(0,direction);
            renewMap(personMove);
            break;
        }
        case Qt::Key_S:{//s
            if(!gameInfo.bluePath[0].empty())
                unsetButtonBluePeroid();
            direction=2;
            PersonMoveDouble personMove=gameInfo.personMove(0,direction);
            renewMap(personMove);
            break;
        }
        case Qt::Key_D:{//d
            if(!gameInfo.bluePath[0].empty())
                unsetButtonBluePeroid();
            direction=4;
            PersonMoveDouble personMove=gameInfo.personMove(0,direction);
            renewMap(personMove);
            break;
        }
        case Qt::Key_Y:{//y
            if(!gameInfo.bluePath[1].empty())
                unsetButtonPinkPeroid();
            direction=1;
            PersonMoveDouble personMove=gameInfo.personMove(1,direction);
            renewMap(personMove);
            break;
        }
        case Qt::Key_G:{//a
            if(!gameInfo.bluePath[1].empty())
                unsetButtonPinkPeroid();
            direction=3;
            PersonMoveDouble personMove=gameInfo.personMove(1,direction);
            renewMap(personMove);
            break;
        }
        case Qt::Key_H:{//s
            if(!gameInfo.bluePath[1].empty())
                unsetButtonPinkPeroid();
            direction=2;
            PersonMoveDouble personMove=gameInfo.personMove(1,direction);
            renewMap(personMove);
            break;
        }
    case Qt::Key_J:{//d
        if(!gameInfo.bluePath[1].empty())
            unsetButtonPinkPeroid();
        direction=4;
        PersonMoveDouble personMove=gameInfo.personMove(1,direction);
        renewMap(personMove);
        break;
        }
    }
}


void DoubleForm::renewMap(PersonMoveDouble personMove){
    if(personMove.person==0){
        switch (personMove.boxOneState) {
        case 1:
            setButtonLight1(personMove.boxOnePosition.first,personMove.boxOnePosition.second);
            break;
        case 2:
            setButtonWhite(personMove.boxOnePosition.first,personMove.boxOnePosition.second);
            break;
        case 3:
            unsetButtonLight(personMove.boxOnePosition.first,personMove.boxOnePosition.second);
            break;
        default:
            break;
        }
        switch (personMove.boxTwoState) {
        case 1:
            setButtonLight1(personMove.boxTwoPosition.first,personMove.boxTwoPosition.second);
            break;
        case 2:
            setButtonWhite(personMove.boxTwoPosition.first,personMove.boxTwoPosition.second);
            break;
        case 3:
            unsetButtonLight(personMove.boxTwoPosition.first,personMove.boxTwoPosition.second);
            break;
        default:
            break;
        }
        setButtonBluePeroid(personMove.path);
        renewScore(0,personMove.score);
        setButtonWhite(personMove.personPreviousPosition.first,personMove.personPreviousPosition.second);
        setButtonPerson1(personMove.personCurrentPosition.first,personMove.personCurrentPosition.second);
    }
    else{
        switch (personMove.boxOneState) {
            case 1:
                setButtonLight2(personMove.boxOnePosition.first,personMove.boxOnePosition.second);
                break;
            case 2:
                setButtonWhite(personMove.boxOnePosition.first,personMove.boxOnePosition.second);
                break;
            case 3:
                unsetButtonLight(personMove.boxOnePosition.first,personMove.boxOnePosition.second);
                break;
        default:
                break;
        }
        switch (personMove.boxTwoState) {
            case 1:
                setButtonLight2(personMove.boxTwoPosition.first,personMove.boxTwoPosition.second);
                break;
            case 2:
                setButtonWhite(personMove.boxTwoPosition.first,personMove.boxTwoPosition.second);
                break;
            case 3:
                unsetButtonLight(personMove.boxTwoPosition.first,personMove.boxTwoPosition.second);
                break;
            default:
                break;
            }
        setButtonPinkPeroid(personMove.path);
        renewScore(1,personMove.score);
        setButtonWhite(personMove.personPreviousPosition.first,personMove.personPreviousPosition.second);
        setButtonPerson2(personMove.personCurrentPosition.first,personMove.personCurrentPosition.second);
    }
        if(gameInfo.checkUserWin())
            gameOver(1);

}


void DoubleForm::generateMap(GameInfoDouble* gameInfo_){
    if(gameInfo_){
        gameInfo=*gameInfo_;
        gameInfo.stop=true;
    }
    else
        do{
            gameInfo.generateMap();
        }while (gameInfo.checkUserWin());

    MATRIX gameMatrix=gameInfo.getMatrix();
    POINT person1Point=gameInfo.getPerson1Current();
    POINT person2Point=gameInfo.getPerson2Current();

    for(int i=0;i<6;i++)
        for(int j=0;j<6;j++)
            switch (gameMatrix[i][j]) {
                case 1:
                    setButtonBox1(i,j);
                    break;
                case 2:
                    setButtonBox2(i,j);
                    break;
                case 3:
                    setButtonBox3(i,j);
                    break;
                default:
                    setButtonWhite(i,j);
                    break;
            }
    setButtonPerson1(person1Point.first,person1Point.second);
    setButtonPerson2(person2Point.first,person2Point.second);
    renewScore(0,gameInfo.scores[0]);
    renewScore(1,gameInfo.scores[1]);
    renewTimer();
    setButtonBluePeroid(gameInfo.bluePath[0]);
    setButtonPinkPeroid(gameInfo.bluePath[1]);
}


void DoubleForm::renewTime(){
    if(gameInfo.stop)
        return;
    gameInfo.time_--;
    renewTimer();
}


void DoubleForm::on_pushButton_clicked(){
    gameInfo.stop=!gameInfo.stop;
}


void DoubleForm::on_pushButton_3_clicked(){
    timer->stop();
   MainWindow* mainWindow=new MainWindow;
    this->close();
    mainWindow->show();
}


void DoubleForm::on_pushButton_2_clicked()
{
    string srcDirPath = QFileDialog::getExistingDirectory(this, "选择文件的创建位置","/").toStdString();
    QDateTime current_date_time =QDateTime::currentDateTime();
    QString current_date =current_date_time.toString("yyyy_MM_dd_hh_mm_ss");
    string fileName=srcDirPath+"/"+"game"+current_date.toStdString()+".json";
    QFile file(QString::fromStdString(fileName));
    file.open(QIODevice::WriteOnly);
    file.close();

    gameInfo.toJson(fileName);

}
