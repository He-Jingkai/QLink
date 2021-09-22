#include <vector>
#include<algorithm>

#define PATH vector<pair<int,int>>
#define POINT_SET vector<pair<int,int>>
#define POINT pair<int,int>
#define MATRIX vector<vector<int>>
#define MERGE_VECTOR(vector1,vector2) (vector1.insert(vector1.end(),vector2.begin(),vector2.end()))

using namespace std;
class PointInfo{
public:
    POINT point;
    PATH path;
    int turnTime;
    int lastDirection;

    PointInfo(POINT point,PATH path,int turnTime,int lastDirection);
    vector<pair<POINT,int>> pointsAround();
};
