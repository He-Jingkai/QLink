#include <PointInfo.h>

using namespace std;

PointInfo::PointInfo(POINT point,PATH path,int turnTime,int lastDirection)
    :point(point),path(path),turnTime(turnTime),lastDirection(lastDirection){}

vector<pair<POINT,int>> PointInfo::pointsAround(){
    vector<pair<POINT,int>> resultTEMP;
    vector<pair<POINT,int>> result;
    if(point.first+1<=5) resultTEMP.push_back(pair<POINT,int>(POINT(point.first+1,point.second),1));
    if(point.first-1>=0) resultTEMP.push_back(pair<POINT,int>(POINT(point.first-1,point.second),1));
    if(point.second+1<=5) resultTEMP.push_back(pair<POINT,int>(POINT(point.first,point.second+1),2));
    if(point.second-1>=0) resultTEMP.push_back(pair<POINT,int>(POINT(point.first,point.second-1),2));

    for(pair<POINT,int> point:resultTEMP){
        vector<POINT>::iterator iter = find(path.begin(), path.end(), point.first); // 查找对象类型时，需要该类提供重载==函数
        if (iter == path.end())
            result.push_back(point);
    }
    return result;
}
