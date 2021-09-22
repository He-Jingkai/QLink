#include "single.h"
#include "ui_single.h"
#include "mainwindow.h"


Single::Single(QWidget *parent,GameInfo* gameInfo_) :
    QDialog(parent),
    ui(new Ui::Single){
    this->setWindowFlags(Qt::CustomizeWindowHint | Qt::WindowCloseButtonHint);
    ui->setupUi(this);
    buttonMatix=vector<vector<QToolButton*>>(6,vector<QToolButton*>(6,0));
    for(int i=0;i<6;i++)
        for(int j=0;j<6;j++)
            buttonMatix[i][j] = this->findChild<QToolButton *>(QString::fromStdString("toolButton_"+to_string(i)+to_string(j)));
    scoreBoard1=this->findChild<QLCDNumber*>("lcdNumber");
    timeBoard=this->findChild<QLCDNumber*>("lcdNumber_2");
    generateMap(gameInfo_);
    renewTimer();
    timer = new QTimer(this);
    connect(timer, SIGNAL(timeout()), this, SLOT(renewTime()));
    timer->start(1000);
}

Single::~Single(){
    timer->stop();
    delete ui;
}

void Single::renewTimer(){
    QPalette lcdpat = timeBoard->palette();
    if(gameInfo.hinting)
        lcdpat.setColor(QPalette::Normal,QPalette::WindowText,Qt::red);
    else
        lcdpat.setColor(QPalette::Normal,QPalette::WindowText,Qt::black);
    timeBoard->setPalette(lcdpat);

    timeBoard->display(QString::fromStdString(to_string(gameInfo.time_)));
    if(gameInfo.time_==0)
        gameOver(2);
}

void Single::setButtonBox1(int i,int j){
    buttonMatix[i][j]->setStyleSheet("background: rgb(255,255,255)");
    buttonMatix[i][j]->setIcon(QIcon("C:/Users/hjk02/Documents/QLink/box1.jpg"));
    buttonMatix[i][j]->setIconSize(QSize(buttonMatix[i][j]->size().rwidth()-2,buttonMatix[i][j]->size().rheight()-2));
}

void Single::setButtonBox2(int i,int j){
    buttonMatix[i][j]->setStyleSheet("background: rgb(255,255,255)");
    buttonMatix[i][j]->setIcon(QIcon("C:/Users/hjk02/Documents/QLink/box2.jpg"));
    buttonMatix[i][j]->setIconSize(QSize(buttonMatix[i][j]->size().rwidth()-2,buttonMatix[i][j]->size().rheight()-2));
}

void Single::setButtonBox3(int i,int j){
    buttonMatix[i][j]->setStyleSheet("background: rgb(255,255,255)");
    buttonMatix[i][j]->setIcon(QIcon("C:/Users/hjk02/Documents/QLink/box3.jpg"));
    buttonMatix[i][j]->setIconSize(QSize(buttonMatix[i][j]->size().rwidth()-2,buttonMatix[i][j]->size().rheight()-2));
}

void Single::setButtonPerson1(int i,int j){
    buttonMatix[i][j]->setStyleSheet("background: rgb(194,194,194)");
    buttonMatix[i][j]->setIcon(QIcon("C:/Users/hjk02/Documents/QLink/personLOGO1.jpg"));
    buttonMatix[i][j]->setIconSize(buttonMatix[i][j]->size());
}

void Single::setButtonWhite(int i,int j){
    buttonMatix[i][j]->setIcon(QIcon());
    buttonMatix[i][j]->setStyleSheet("background: rgb(255,255,255)");
}

void Single::setButtonLight(int i,int j){
    buttonMatix[i][j]->setStyleSheet("background:rgb(255,255,255);border:5px solid rgb(255,255,0);");
}

void Single::unsetButtonLight(int i,int j){
    buttonMatix[i][j]->setStyleSheet("background:rgb(255,255,255);border:0px solid rgb(255,255,255);");
}

void Single::setButtonBluePeroid(PATH path){
    gameInfo.bluePath=path;
    for(POINT point:path)
        buttonMatix[point.first][point.second]->setStyleSheet("background: rgb(204,255,255)");
}

void Single::unsetButtonBluePeroid(){
    for(POINT point:gameInfo.bluePath)
        buttonMatix[point.first][point.second]->setStyleSheet("background: rgb(255,255,255)");
    gameInfo.bluePath.clear();
}

void Single::renewScore(int score){
    scoreBoard1->display(QString::fromStdString(to_string(score)));
}

void Single::gameOver(int type){
    timer->stop();
    if(type==1){
        QMessageBox message(QMessageBox::NoIcon, "游戏结束", QString::fromStdString("恭喜你,已经消除了所有可以消除的方块，您的得分为"+to_string(gameInfo.score)),QMessageBox::Yes | QMessageBox::No, NULL);
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
            Single* single=new Single;
            this->destroy();
            single->show();
        }
    }
    else if(type==2){
        QMessageBox message(QMessageBox::NoIcon, "游戏结束", QString::fromStdString("时间到，您的得分为"+to_string(gameInfo.score)),QMessageBox::Yes | QMessageBox::No, NULL);
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
            Single* single=new Single;
            this->destroy();
            single->show();
        }
    }
}

void Single::setOneSecondMore(POINT point){
    int i=point.first;
    int j=point.second;
    buttonMatix[i][j]->setStyleSheet("background: rgb(255,255,255)");
    buttonMatix[i][j]->setIcon(QIcon("C:/Users/hjk02/Documents/QLink/+1s.jpg"));
    buttonMatix[i][j]->setIconSize(QSize(buttonMatix[i][j]->size().rwidth()-2,buttonMatix[i][j]->size().rheight()-2));
}

void Single::setShuffle(POINT point){
    int i=point.first;
    int j=point.second;
    buttonMatix[i][j]->setStyleSheet("background: rgb(255,255,255)");
    buttonMatix[i][j]->setIcon(QIcon("C:/Users/hjk02/Documents/QLink/shuffle.png"));
    buttonMatix[i][j]->setIconSize(QSize(buttonMatix[i][j]->size().rwidth()-2,buttonMatix[i][j]->size().rheight()-2));
}

void Single::setHint(POINT point){
    int i=point.first;
    int j=point.second;
    buttonMatix[i][j]->setStyleSheet("background: rgb(255,255,255)");
    buttonMatix[i][j]->setIcon(QIcon("C:/Users/hjk02/Documents/QLink/hint.png"));
    buttonMatix[i][j]->setIconSize(QSize(buttonMatix[i][j]->size().rwidth()-2,buttonMatix[i][j]->size().rheight()-2));
}

void Single::keyPressEvent(QKeyEvent *event){
    if(gameInfo.stop)
        return;
    if(!gameInfo.bluePath.empty())
        unsetButtonBluePeroid();
    if(event->isAutoRepeat())//如果是一直按下按钮触发直接返回
        return;
    int key=event->key(),direction;
    switch (key) {
        case 87://w
            direction=1;
            break;
        case 65://a
            direction=3;
            break;
        case 83://s
            direction=2;
            break;
        case 68://d
            direction=4;
            break;
    }
    PersonMove personMove=gameInfo.personMove(direction);
    renewMap(personMove);
}

void Single::renewMap(PersonMove personMove){
    switch (personMove.boxOneState) {
        case 1:
            setButtonLight(personMove.boxOnePosition.first,personMove.boxOnePosition.second);
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
            setButtonLight(personMove.boxTwoPosition.first,personMove.boxTwoPosition.second);
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
    if(personMove.boxOneState==2&&personMove.boxTwoState==2&&((personMove.boxTwoPosition==gameInfo.hintPair.first&&personMove.boxOnePosition==gameInfo.hintPair.second)||(personMove.boxTwoPosition==gameInfo.hintPair.second&&personMove.boxOnePosition==gameInfo.hintPair.first)))
        gameInfo.hintPair=POINT_PAIR(POINT(-1,-1),POINT(-1,-1));
    setButtonBluePeroid(personMove.path);
    renewScore(personMove.score);
    setButtonWhite(personMove.personPreviousPosition.first,personMove.personPreviousPosition.second);
    setButtonPerson1(personMove.personCurrentPosition.first,personMove.personCurrentPosition.second);
    if(gameInfo.checkUserWin()){
        gameOver(1);
        return;
    }
    if(personMove.oneSecondMore){
        gameInfo.time_++;
        renewTimer();
    }
    if(personMove.shuffle){
        gameInfo.hintPair=POINT_PAIR(POINT(-1,-1),POINT(-1,-1));
        regenerateMap();
        qDebug()<<gameInfo.hinting;
    }
    if(personMove.hint){
        onHintDetected();
    }
    if(gameInfo.hinting&&gameInfo.hintPair==POINT_PAIR(POINT(-1,-1),POINT(-1,-1)))
        setHintPair();
}

void Single::generateMap(GameInfo* gameInfo_){
    if(gameInfo_){
        gameInfo=*gameInfo_;
        gameInfo.stop=true;
    }
    else
        do{
            gameInfo.generateMap();
        }while (gameInfo.checkUserWin());
    MATRIX gameMatrix=gameInfo.getMatrix();
    POINT personPoint=gameInfo.getPersonCurrent();
    POINT oneSecondMorePoint=gameInfo.getOneSecondMore();
    POINT hintPoint=gameInfo.getHintPoint();
    POINT shufflePOint=gameInfo.getShuffle();

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
    setButtonPerson1(personPoint.first,personPoint.second);
    setHint(hintPoint);
    setShuffle(shufflePOint);
    setOneSecondMore(oneSecondMorePoint);
    renewScore(gameInfo.score);
    renewTimer();
    setHintPair();
    setButtonBluePeroid(gameInfo.bluePath);
}

void Single::regenerateMap(){
    do{
        gameInfo.regenerateMap();
    }while (gameInfo.checkUserWin());
    MATRIX gameMatrix=gameInfo.getMatrix();
    POINT personPoint=gameInfo.getPersonCurrent();
    POINT oneSecondMorePoint=gameInfo.getOneSecondMore();
    POINT hintPoint=gameInfo.getHintPoint();
    POINT shufflePOint=gameInfo.getShuffle();

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
    setButtonPerson1(personPoint.first,personPoint.second);
    if(hintPoint!=POINT(-1,-1))
        setHint(hintPoint);
    if(shufflePOint!=POINT(-1,-1))
        setShuffle(shufflePOint);
    if(oneSecondMorePoint!=POINT(-1,-1))
        setOneSecondMore(oneSecondMorePoint);
}

void Single::renewTime(){
    if(gameInfo.stop)
        return;
    gameInfo.time_--;
    renewTimer();
    if(gameInfo.hinting&&gameInfo.hintStartTime-gameInfo.time_==10)
        gameInfo.hinting=false;
}

void Single::onHintDetected(){
  gameInfo.hinting=true;
  gameInfo.hintStartTime=gameInfo.time_;
}

void Single::setHintPair(){
    if(!gameInfo.hinting)
        return;
    POINT_PAIR points=gameInfo.getHint();
    if(points==POINT_PAIR(POINT(-1,-1),POINT(-1,-1))){
        gameInfo.hinting=false;
        return;
    }
    gameInfo.hintPair=points;
    buttonMatix[points.first.first][points.first.second]->setStyleSheet("background:rgb(255,255,255);border:5px solid rgb(255,105,180);");
    buttonMatix[points.second.first][points.second.second]->setStyleSheet("background:rgb(255,255,255);border:5px solid rgb(255,105,180);");
}

void Single::on_pushButton_clicked(){
    gameInfo.stop=!gameInfo.stop;
}

void Single::on_pushButton_3_clicked(){
    timer->stop();
    MainWindow* mainWindow=new MainWindow;
    this->destroy();
    mainWindow->show();
}

void Single::on_pushButton_2_clicked()
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
