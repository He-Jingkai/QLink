#include <vector>

using namespace std;

class PersonMoveDouble{
public:
    int person;

    /*每次至多改变两个盒子的状态*/
    int boxOneState;/*无用(0)，激活(1)，消去(2), 取消激活（3）*/
    pair<int,int> boxOnePosition;

    int boxTwoState;/*无用(0)，激活(1)，消去(2)*/
    pair<int,int> boxTwoPosition;

    /*用于更新小人状态*/
    pair<int,int> personPreviousPosition;
    pair<int,int> personCurrentPosition;

    vector<pair<int,int>> path;

    int score;

    PersonMoveDouble(int person,int boxOneState_, pair<int,int> boxOnePosition_,
               int boxTwoState_, pair<int,int> boxTwoPosition_,
               pair<int,int> personPreviousPosition_, pair<int,int> personCurrentPosition_,
               vector<pair<int,int>> path,
               int score);
    PersonMoveDouble();
};
