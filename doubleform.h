#ifndef DOUBLEFORM_H
#define DOUBLEFORM_H

#include <QDialog>
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
#include <QFileDialog>

namespace Ui {
class DoubleForm;
}

class DoubleForm : public QDialog
{
    Q_OBJECT

public:
    explicit DoubleForm(QWidget *parent = nullptr,GameInfoDouble* gameInfo_=nullptr);

    ~DoubleForm();

    void keyPressEvent(QKeyEvent *event);

    void renewGUI(PersonMoveDouble personMoveDouble);

    void generateMap(GameInfoDouble* gameInfo_=nullptr);

    void setButtonBox1(int i,int j);

    void setButtonBox2(int i,int j);

    void setButtonBox3(int i,int j);

    void setButtonPerson1(int i,int j);

    void setButtonPerson2(int i,int j);

    void setButtonLight1(int i,int j);

    void setButtonLight2(int i,int j);

    void unsetButtonLight(int i,int j);

    void setButtonBluePeroid(PATH path);

    void setButtonWhite(int i,int j);

    void renewMap(PersonMoveDouble personMove);

    void unsetButtonBluePeroid();

    void renewScore(int person,int score);

    void gameOver(int type);

    void renewTimer();

    void setButtonPinkPeroid(PATH path);

    void unsetButtonPinkPeroid();

private slots:
    void renewTime();

    void on_pushButton_clicked();

    void on_pushButton_3_clicked();

    void on_pushButton_2_clicked();

private:
    Ui::DoubleForm *ui;

    vector<vector<QToolButton*>> buttonMatix;

    vector<QLCDNumber*> scoreBoard;

    QLCDNumber* timeBoard;

    QTimer *timer;

    GameInfoDouble gameInfo;

};

#endif // DOUBLEFORM_H
