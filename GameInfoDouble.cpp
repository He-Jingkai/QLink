#include "GameInfoDouble.h"
#include <time.h>
#include <PointInfo.h>

using namespace std;

GameInfoDouble::GameInfoDouble(){
    matrix=MATRIX(6,vector<int>(6,0));
    activePoint=vector<POINT>(2,POINT(-1,-1));
    personCurrent=vector<POINT>(2,POINT(-1,-1));
    scores=vector<int>(2,0);
    time_=60;
    stop=false;
    bluePath=vector<PATH>(2,PATH());
    type="double";
}

void swap1(int* pNum1, int* pNum2){
    if (!pNum1 ||!pNum2)
        return;
    int nTemp = *pNum1;
    *pNum1 = *pNum2;
    *pNum2 = nTemp;
}

/* 生成地图和小人位置 */
void GameInfoDouble::generateMap(){
        srand((unsigned int)time(NULL));
        int m=6, n=6, i, j, index1, index2;
        int *pTemp = (int*)malloc(m * n * sizeof(int));
        for (i = 0; i < m * n / 2; i++)
            pTemp[i] = pTemp[i + m * n / 2] = rand() % 4;
        for (int i = 0; i < 100; i++){
            index1 = rand() % (m * n);
            index2 = rand() % (m * n);
            swap1(&pTemp[index1], &pTemp[index2]);
        }
        int **ppArray = (int**)malloc(m * sizeof(int*));
        for (i = 0; i < m; i++)
            ppArray[i] = pTemp + i * n;

        for (i = 0; i < m; i++)
            for (j = 0; j < n; j++)
                matrix[i][j]=ppArray[i][j];
        free(pTemp);
        free(ppArray);
        POINT_SET pointSet=findAllEmptyBox();
        int person1Index=rand()%(pointSet.size());
        personCurrent[0]=pointSet[person1Index];

        pointSet.erase(pointSet.begin()+person1Index);
        int person2Index=rand()%(pointSet.size());
        personCurrent[1]=pointSet[person2Index];
}

POINT_SET GameInfoDouble::findAllEmptyBox(){
    POINT_SET pointSet;
    for(int i=0;i<6;i++)
        for(int j=0;j<6;j++)
            if(matrix[i][j]==0)
                pointSet.push_back(POINT(i,j));
    return pointSet;
}

/*分为以下几种情况：
 *   前进方向为边缘：小人不动，方块状态均不变
 *   前进方向无方块：小人前进，方块状态均不变
 *   前进方向有方块：
 *       前进方向有已经激活的方块：方块消除激活,小人不动
 *       前进方向有未激活的方块：
 *           地图中无激活的方块：激活该方块，小人不动
 *           地图中有激活的方块：
 *               匹配：两方块消除，小人不动
 *               不匹配:原方块激活消失，小人不动
 */
PersonMoveDouble GameInfoDouble::personMove(int person,int direction){
    POINT personPrevious=personCurrent[person],
            personMaybe=personCurrent[person];
    PATH path;

    switch (direction) {
        case 1://向上
            personMaybe=POINT(personCurrent[person].first-1,personCurrent[person].second);
            break;
        case 2://向下
            personMaybe=POINT(personCurrent[person].first+1,personCurrent[person].second);
            break;
        case 3://向左
            personMaybe=POINT(personCurrent[person].first,personCurrent[person].second-1);
            break;
        case 4:
            personMaybe=POINT(personCurrent[person].first,personCurrent[person].second+1);
            break;
    }
    if((person==0&&personMaybe==personCurrent[1])||(person==1&&personMaybe==personCurrent[0]))
        return PersonMoveDouble(person,0,POINT(-1,-1),0,POINT(-1,-1),personCurrent[person],personCurrent[person],path,scores[person]);


    if(personMaybe.first<0||personMaybe.first>5||personMaybe.second<0||personMaybe.second>5)//(1)
        return PersonMoveDouble(person,0,POINT(-1,-1),0,POINT(-1,-1),personCurrent[person],personCurrent[person],path,scores[person]);
    else if(matrix[personMaybe.first][personMaybe.second]==0){
        personCurrent[person]=personMaybe;
        return PersonMoveDouble(person,0,POINT(-1,-1),0,POINT(-1,-1),personPrevious,personMaybe,path,scores[person]);
    }
    else
        if(activePoint[person]==personMaybe){
            activePoint[person]=POINT(-1,-1);
            return PersonMoveDouble(person,3,personMaybe,0,POINT(-1,-1),personCurrent[person],personCurrent[person],path,scores[person]);
        }
        else
            if(activePoint[person]==POINT(-1,-1)){
                activePoint[person]=personMaybe;
                return PersonMoveDouble(person,1,personMaybe,0,POINT(-1,-1),personCurrent[person],personCurrent[person],path,scores[person]);
            }
            else{
                POINT previousActivePoint=activePoint[person];
                activePoint[person]=POINT(-1,-1);
                PATH pathMaybe=checkOK(previousActivePoint,personMaybe);
                if(pathMaybe!=path){
                    matrix[previousActivePoint.first][previousActivePoint.second]=0;
                    matrix[personMaybe.first][personMaybe.second]=0;
                    scores[person]+=1;
                    return PersonMoveDouble(person,2,previousActivePoint,2,personMaybe,personCurrent[person],personCurrent[person],pathMaybe,scores[person]);
                }
                else
                    return PersonMoveDouble(person,3,previousActivePoint,0,POINT(-1,-1),personCurrent[person],personCurrent[person],path,scores[person]);
            }
}


PATH GameInfoDouble::checkOK(POINT point1,POINT point2){
    int color=matrix[point1.first][point1.second];
    PATH pathZERO;
    if(color!=matrix[point2.first][point2.second])
        return pathZERO;
    stack<PointInfo> pointStack;
    pointStack.push(PointInfo(point1,{point1},0,0));
    while (!pointStack.empty()) {
        PointInfo pointInfo=pointStack.top();
        pointStack.pop();
        for(pair<POINT,int> pointAndDirection:pointInfo.pointsAround()){
            POINT point=pointAndDirection.first;
            int direction=pointAndDirection.second;
            if(point==point2){
                if(pointInfo.lastDirection==0||direction==pointInfo.lastDirection||(direction!=pointInfo.lastDirection&&pointInfo.turnTime<=1)){
                    PATH path=pointInfo.path;
                    path.push_back(point);
                    return path;
                }
            }
            else if(matrix[point.first][point.second]==0){
                if(pointInfo.lastDirection==0||direction==pointInfo.lastDirection){
                    PATH path=pointInfo.path;
                    path.push_back(point);
                    pointStack.push(PointInfo(point,path,pointInfo.turnTime,direction));
                }
                else if(pointInfo.turnTime<2){
                    PATH path=pointInfo.path;
                    path.push_back(point);
                    pointStack.push(PointInfo(point,path,pointInfo.turnTime+1,direction));
                }
            }
        }
    }
    return pathZERO;
}

//string GameInfo::toTxt(){

//}

bool GameInfoDouble::checkGameOver(){
    PATH pathZERO;
    vector<POINT_SET> pointSets={findPointSet(1),findPointSet(2),findPointSet(3)};
    for(POINT_SET set : pointSets){
        int size=set.size();
        for(int i=0;i<size;i++)
            for(int j=0;j<size;j++)
                if(i==j)
                    continue;
                else
                    if(checkOK(set[i],set[j])!=pathZERO)
                        return false;
    }
    return true;
}

POINT_SET GameInfoDouble::findPointSet(int color){
    POINT_SET result;
    for(int i=0;i<6;i++)
        for(int j=0;j<6;j++)
            if(matrix[i][j]==color)
                result.push_back(POINT(i,j));
    return result;
}

/*思路：获取用户可以到达的点并组合遍历检查*/
 bool GameInfoDouble::checkUserWin(){
     for(int i=0;i<2;i++){
         stack<POINT> whitePointStack;
         vector<POINT> pointCanBeReached;
         vector<POINT> pointHasBeenRead;
         whitePointStack.push(personCurrent[i]);
         while (!whitePointStack.empty()) {
             POINT point=whitePointStack.top();
             whitePointStack.pop();
             if(point.first-1>=0){
                 POINT pointTEMP=POINT(point.first-1,point.second);
                 if(find(pointHasBeenRead.begin(),pointHasBeenRead.end(),pointTEMP)==pointHasBeenRead.end()){
                     if(matrix[pointTEMP.first][pointTEMP.second])
                             pointCanBeReached.push_back(pointTEMP);
                     else
                         whitePointStack.push(pointTEMP);
                     pointHasBeenRead.push_back(pointTEMP);
                 }
             }
             if(point.first+1<=5){
                 POINT pointTEMP=POINT(point.first+1,point.second);
                 if(find(pointHasBeenRead.begin(),pointHasBeenRead.end(),pointTEMP)==pointHasBeenRead.end()){
                     if(matrix[pointTEMP.first][pointTEMP.second])
                             pointCanBeReached.push_back(pointTEMP);
                     else
                         whitePointStack.push(pointTEMP);
                     pointHasBeenRead.push_back(pointTEMP);
                 }
             }
             if(point.second-1>=0){
                 POINT pointTEMP=POINT(point.first,point.second-1);
                 if(find(pointHasBeenRead.begin(),pointHasBeenRead.end(),pointTEMP)==pointHasBeenRead.end()){
                     if(matrix[pointTEMP.first][pointTEMP.second])
                             pointCanBeReached.push_back(pointTEMP);
                     else
                         whitePointStack.push(pointTEMP);
                     pointHasBeenRead.push_back(pointTEMP);
                 }
             }
             if(point.second+1<=5){
                 POINT pointTEMP=POINT(point.first,point.second+1);
                 if(find(pointHasBeenRead.begin(),pointHasBeenRead.end(),pointTEMP)==pointHasBeenRead.end()){
                     if(matrix[pointTEMP.first][pointTEMP.second])
                             pointCanBeReached.push_back(pointTEMP);
                     else
                         whitePointStack.push(pointTEMP);
                     pointHasBeenRead.push_back(pointTEMP);
                 }
             }
         }

         vector<vector<POINT>> vectorOfBoxVector(3,vector<POINT>());
         for(POINT point:pointCanBeReached)
                 vectorOfBoxVector[matrix[point.first][point.second]-1].push_back(point);

         for(vector<POINT> boxVector:vectorOfBoxVector){
             int size=(int)boxVector.size();
             if(size==0||size==1)
                 continue;
             for(int i=0;i<size-1;i++)
                 for(int j=i;j<size;j++)
                     if(checkOK(boxVector[i],boxVector[j])!=PATH())
                         return false;
         }
     }
     return true;

}

// void GameInfo::readTxt(){

// }

MATRIX  GameInfoDouble::getMatrix(){
    return matrix;
}

POINT GameInfoDouble::getPerson1Current(){
    return personCurrent[0];
}

POINT GameInfoDouble::getPerson2Current(){
    return personCurrent[1];
}

bool GameInfoDouble::readJson(string jsonPath){
    fstream file(jsonPath.c_str());
    Json::Reader reader;
    Json::Value root;
    if (reader.parse(file, root)){
        for(int i=0;i<6;i++)
            for(int j=0;j<6;j++)
                matrix[i][j]=root["matix"+to_string(i)+to_string(j)].asInt();

       if (!root["type"].isNull())
            type= root["type"].asString();
       else return false;

       activePoint.clear();

       if (!root["activePoint_1_x"].isNull()&&!root["activePoint_1_y"].isNull())
            activePoint.push_back(POINT(root["activePoint_1_x"].asInt(),root["activePoint_1_y"].asInt()));
       else return false;
       if (!root["activePoint_2_x"].isNull()&&!root["activePoint_2_y"].isNull())
            activePoint.push_back(POINT(root["activePoint_2_x"].asInt(),root["activePoint_2_y"].asInt()));
       else return false;

       personCurrent.clear();

       if (!root["personCurrent_1_x"].isNull()&&!root["personCurrent_1_y"].isNull())
            personCurrent.push_back(POINT(root["personCurrent_1_x"].asInt(),root["personCurrent_1_y"].asInt()));
       else return false;
       if (!root["personCurrent_2_x"].isNull()&&!root["personCurrent_2_y"].isNull())
            personCurrent.push_back(POINT(root["personCurrent_2_x"].asInt(),root["personCurrent_2_y"].asInt()));
       else return false;

       scores.clear();

       if (!root["score_1"].isNull())
            scores.push_back(root["score_1"].asInt());
       else return false;

       if (!root["score_2"].isNull())
            scores.push_back(root["score_2"].asInt());
       else return false;

       bluePath.clear();

       int bluePath_1_size=0;
       if (!root["bluePath_1_size"].isNull())
            bluePath_1_size= root["bluePath_1_size"].asInt();
       else return false;
       bluePath.push_back(PATH());
       for(int i=0;i<bluePath_1_size;i++){
           if (!root["bluePath_1_"+to_string(i)+"_x"].isNull()&&!root["bluePath_1_"+to_string(i)+"_y"].isNull())
                bluePath[0].push_back(POINT(root["bluePath_1_"+to_string(i)+"_x"].asInt(),root["bluePath_1_"+to_string(i)+"_y"].asInt()));
           else return false;
       }

       int bluePath_2_size=0;
       if (!root["bluePath_2_size"].isNull())
            bluePath_1_size= root["bluePath_2_size"].asInt();
       else return false;
       bluePath.push_back(PATH());
       for(int i=0;i<bluePath_2_size;i++){
           if (!root["bluePath_2_"+to_string(i)+"_x"].isNull()&&!root["bluePath_2_"+to_string(i)+"_y"].isNull())
                bluePath[1].push_back(POINT(root["bluePath_2_"+to_string(i)+"_x"].asInt(),root["bluePath_2_"+to_string(i)+"_y"].asInt()));
           else return false;
       }

       if (!root["time_"].isNull())
            time_= root["time_"].asInt();
       else return false;

       stop=true;
    }
    else return false;

    return true;
}

void GameInfoDouble::toJson(string filePath){
    Json::Value rootValue = Json::objectValue;
    for(int i=0;i<6;i++)
        for(int j=0;j<6;j++)
            rootValue["matix"+to_string(i)+to_string(j)]=matrix[i][j];

    rootValue["type"] = type;
    rootValue["activePoint_1_x"] = activePoint[0].first;
    rootValue["activePoint_1_y"] = activePoint[0].second;
    rootValue["activePoint_2_x"] = activePoint[1].first;
    rootValue["activePoint_2_y"] = activePoint[1].second;
    rootValue["personCurrent_1_x"] = personCurrent[0].first;
    rootValue["personCurrent_1_y"] = personCurrent[0].second;
    rootValue["personCurrent_2_x"] = personCurrent[1].first;
    rootValue["personCurrent_2_y"] = personCurrent[1].second;
    rootValue["score_1"] = scores[0];
    rootValue["score_2"] = scores[1];

    rootValue["bluePath_1_size"] = bluePath[0].size();
    for(int i=0;i<(int)bluePath[0].size();i++){
        rootValue["bluePath_1_"+to_string(i)+"_x"] = bluePath[0][i].first;
        rootValue["bluePath_1_"+to_string(i)+"_y"] = bluePath[0][i].second;
    }
    rootValue["bluePath_2_size"] = bluePath[1].size();
    for(int i=0;i<(int)bluePath[1].size();i++){
        rootValue["bluePath_2_"+to_string(i)+"_x"] = bluePath[1][i].first;
        rootValue["bluePath_2_"+to_string(i)+"_y"] = bluePath[1][i].second;
    }
    rootValue["time_"] = time_;
    rootValue["stop"] = true;

    Json::StyledWriter writer;
    string out=writer.write(rootValue);
    fstream file(filePath.c_str());
    file<<out;
    file.close();
}
