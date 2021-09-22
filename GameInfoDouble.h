#include <vector>
#include <string>
#include "PersonMoveDouble.h"
#include <QtDebug>
#include <stack>
#include <algorithm>
#include "json/json.h"
#include <fstream>

#define PATH vector<pair<int,int>>
#define POINT_SET vector<pair<int,int>>
#define POINT pair<int,int>
#define MATRIX vector<vector<int>>
#define MERGE_VECTOR(vector1,vector2) (vector1.insert(vector1.end(),vector2.begin(),vector2.end()))
#define POINT_PAIR pair<pair<int,int>,pair<int,int>>

using namespace std;

class GameInfoDouble{
private:
    string type;

    MATRIX matrix;

    vector<POINT> activePoint;

    vector<POINT> personCurrent;


    POINT_SET findPointSet(int color);

    POINT_SET findAllEmptyBox();


public:
    vector<int> scores;

    vector<PATH> bluePath;

    int time_;

    bool stop;

    GameInfoDouble();

    void generateMap();

    PersonMoveDouble personMove(int person,int direction);

    PATH checkOK(POINT point1,POINT point2);

    bool checkGameOver();

    bool checkUserWin();

    MATRIX getMatrix();

    POINT getPerson1Current();

    POINT getPerson2Current();


    bool readJson(string jsonPath);

    void toJson(string filePath);

};
