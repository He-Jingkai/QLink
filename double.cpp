#include "double.h"
#include "ui_double.h"

Double::Double(QWidget *parent) :
    QDialog(parent),
    ui(new Ui::Double){
    ui->setupUi(this);
    buttonMatix=vector<vector<QToolButton*>>(6,vector<QToolButton*>(6,0));
    for(int i=0;i<6;i++)
        for(int j=0;j<6;j++)
            buttonMatix[i][j] = this->findChild<QToolButton *>(QString::fromStdString("toolButton_"+to_string(i)+to_string(j)));
    scoreBoard1=this->findChild<QLCDNumber*>("lcdNumber");
    timeBoard=this->findChild<QLCDNumber*>("lcdNumber_2");
    generateMap();
    renewTimer();
    timer = new QTimer(this);
    connect(timer, SIGNAL(timeout()), this, SLOT(renewTime()));
    timer->start(1000);
}

Double::~Double(){
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
        QMessageBox message(QMessageBox::NoIcon, "????????????", QString::fromStdString("?????????,????????????????????????????????????????????????????????????"+to_string(gameInfo.score)),QMessageBox::Yes | QMessageBox::No, NULL);
        QPixmap qPixmap("C:/Users/hjk02/Documents/QLink/personLOGO1.jpg");
        qPixmap = qPixmap.scaled(40, 40, Qt::KeepAspectRatio);
        message.setIconPixmap(qPixmap);
        message.button(QMessageBox::Yes)->setText("???????????????");
        message.button(QMessageBox::No)->setText("????????????");
        message.exec();
    }
    else if(type==2){
        QMessageBox message(QMessageBox::NoIcon, "????????????", QString::fromStdString("???????????????????????????"+to_string(gameInfo.score)),QMessageBox::Yes | QMessageBox::No, NULL);
        QPixmap qPixmap("C:/Users/hjk02/Documents/QLink/personLOGO1.jpg");
        qPixmap = qPixmap.scaled(40, 40, Qt::KeepAspectRatio);
        message.setIconPixmap(qPixmap);
        message.button(QMessageBox::Yes)->setText("???????????????");
        message.button(QMessageBox::No)->setText("????????????");
        message.exec();
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
    if(event->isAutoRepeat())//?????????????????????????????????????????????
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
    if(gameInfo.checkUserWin())
        gameOver(1);
    if(personMove.oneSecondMore){
        gameInfo.time_++;
        renewTimer();
    }
    if(personMove.shuffle){
        regenerateMap();
    }
    if(personMove.hint){
        onHintDetected();
    }
    if(gameInfo.hinting&&gameInfo.hintPair==POINT_PAIR(POINT(-1,-1),POINT(-1,-1)))
        setHintPair();
}

void Single::generateMap(){
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
    qDebug()<<"map regenerate";
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
//    mainWindow=new MainWindow;
//    this->close();
//   ui->setupUi();
}
