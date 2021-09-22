#ifndef DOUBLE_H
#define DOUBLE_H

#include <QDialog>
#include <QMainWindow>
#include <iostream>
#include <QtDebug>
#include <QKeyEvent>
#include <GameInfoDouble.h>
#include <QToolButton>
#include <QIcon>
#include <QLCDNumber>
#include <QMessageBox>
#include <QtCore>


namespace Ui {
class Double;
}

class Double : public QDialog
{
    Q_OBJECT

public:
    explicit Double(QWidget *parent = nullptr);
    ~Double();
    void keyPressEvent(QKeyEvent *event);
    void renewGUI(PersonMoveDouble personMoveDouble);
    void generateMap();
    void setButtonBox1(int i,int j);
    void setButtonBox2(int i,int j);
    void setButtonBox3(int i,int j);
    void setButtonPerson1(int i,int j);
    void setButtonLight(int i,int j);
    void setButtonBluePeroid(PATH path);
    void setButtonWhite(int i,int j);
    void renewMap(PersonMoveDouble personMove);
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

private:
    Ui::Double *ui;
    vector<vector<QToolButton*>> buttonMatix;
    QLCDNumber* scoreBoard1;
    QLCDNumber* timeBoard;
    QTimer *timer;
    GameInfoDouble gameInfo;
};

#endif // DOUBLE_H
