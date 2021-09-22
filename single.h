#ifndef SINGLE_H
#define SINGLE_H

#include <QDialog>
#include <QMainWindow>
#include <iostream>
#include <QtDebug>
#include <QKeyEvent>
#include <GameInfo.h>
#include <QToolButton>
#include <QIcon>
#include <QLCDNumber>
#include <QMessageBox>
#include <QtCore>
#include <QFileDialog>

namespace Ui {
class Single;
}

class Single : public QDialog
{
    Q_OBJECT

public:
    explicit Single(QWidget *parent = nullptr,GameInfo* gameInfo_=NULL);
    ~Single();
    void keyPressEvent(QKeyEvent *event);
    void renewGUI(PersonMove personMove);
    void generateMap(GameInfo* gameInfo_);
    void setButtonBox1(int i,int j);
    void setButtonBox2(int i,int j);
    void setButtonBox3(int i,int j);
    void setButtonPerson1(int i,int j);
    void setButtonLight(int i,int j);
    void setButtonBluePeroid(PATH path);
    void setButtonWhite(int i,int j);
    void renewMap(PersonMove personMove);
    void unsetButtonLight(int i,int j);
    void unsetButtonBluePeroid();
    void renewScore(int score);
    void gameOver(int type);
    void renewTimer();
    void setOneSecondMore(POINT point);
    void setShuffle(POINT point);
    void setHint(POINT point);
    void regenerateMap();
    void setHintPair();
    void onHintDetected();
private slots:
    void renewTime();

    void on_pushButton_clicked();

    void on_pushButton_3_clicked();

    void on_pushButton_2_clicked();

private:
    Ui::Single *ui;
    vector<vector<QToolButton*>> buttonMatix;
    QLCDNumber* scoreBoard1;
    QLCDNumber* timeBoard;
    QTimer *timer;
    GameInfo gameInfo;
};

#endif // SINGLE_H
