#include "GameInfo.h"
#include <time.h>
#include <PointInfo.h>

using namespace std;

GameInfo::GameInfo(){
    matrix=MATRIX(6,vector<int>(6,0));
    activePoint=POINT(-1,-1);
    score=0;
    time_=60;
    hinting=false;
    hintStartTime=0;//
    hintPair=POINT_PAIR(POINT(-1,-1),POINT(-1,-1));
    stop=false;
    type="single";
}

void swap(int* pNum1, int* pNum2){
    if (!pNum1 ||!pNum2)
        return;
    int nTemp = *pNum1;
    *pNum1 = *pNum2;
    *pNum2 = nTemp;
}

/* 生成地图和小人位置 */
void GameInfo::generateMap(){
        srand((unsigned int)time(NULL));
        int m=6, n=6, i, j, index1, index2;
        int *pTemp = (int*)malloc(m * n * sizeof(int));
        for (i = 0; i < m * n / 2; i++)
            pTemp[i] = pTemp[i + m * n / 2] = rand() % 4;
        for (int i = 0; i < 100; i++){
            index1 = rand() % (m * n);
            index2 = rand() % (m * n);
            swap(&pTemp[index1], &pTemp[index2]);
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
        int personIndex=rand()%(pointSet.size());
        personCurrent=pointSet[personIndex];

        pointSet.erase(pointSet.begin()+personIndex);
        int oneSecondMoreIndex=rand()%(pointSet.size());
        oneSecondMore=pointSet[oneSecondMoreIndex];

        pointSet.erase(pointSet.begin()+oneSecondMoreIndex);
        int shuffleIndex=rand()%(pointSet.size());
        shuffle=pointSet[shuffleIndex];

        pointSet.erase(pointSet.begin()+shuffleIndex);
        int hintIndex=rand()%(pointSet.size());
        hint=pointSet[hintIndex];
}

POINT_SET GameInfo::findAllEmptyBox(){
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
PersonMove GameInfo::personMove(int direction){
    PATH path;
    POINT personPrevious=personCurrent;
    POINT personMaybe=personCurrent;
    switch (direction) {
        case 1://向上
            personMaybe=POINT(personCurrent.first-1,personCurrent.second);
            break;
        case 2://向下
            personMaybe=POINT(personCurrent.first+1,personCurrent.second);
            break;
        case 3://向左
            personMaybe=POINT(personCurrent.first,personCurrent.second-1);
            break;
        case 4:
            personMaybe=POINT(personCurrent.first,personCurrent.second+1);
            break;
    }

    if(personMaybe.first<0||personMaybe.first>5||personMaybe.second<0||personMaybe.second>5)//(1)
        return PersonMove(0,POINT(-1,-1),0,POINT(-1,-1),personCurrent,personCurrent,path,score,false,false,false);
    else if(matrix[personMaybe.first][personMaybe.second]==0){
        personCurrent=personMaybe;
        if(oneSecondMore!=POINT(-1,-1)&&personMaybe==oneSecondMore){
            qDebug()<<"oneMore_GameInfo.cpp";
            oneSecondMore=POINT(-1,-1);
            return PersonMove(0,POINT(-1,-1),0,POINT(-1,-1),personPrevious,personMaybe,path,score,true,false,false);
        }
        if(shuffle!=POINT(-1,-1)&&personMaybe==shuffle){
            qDebug()<<"shuffle_GameInfo.cpp";
            shuffle=POINT(-1,-1);
            return PersonMove(0,POINT(-1,-1),0,POINT(-1,-1),personPrevious,personMaybe,path,score,false,true,false);
        }
        if(hint!=POINT(-1,-1)&&personMaybe==hint){
            hint=POINT(-1,-1);
            return PersonMove(0,POINT(-1,-1),0,POINT(-1,-1),personPrevious,personMaybe,path,score,false,false,true);
        }
        return PersonMove(0,POINT(-1,-1),0,POINT(-1,-1),personPrevious,personMaybe,path,score,false,false,false);
    }
    else
        if(activePoint==personMaybe){
            activePoint=POINT(-1,-1);
            return PersonMove(3,personMaybe,0,POINT(-1,-1),personCurrent,personCurrent,path,score,false,false,false);
        }
        else
            if(activePoint==POINT(-1,-1)){
                activePoint=personMaybe;
                return PersonMove(1,personMaybe,0,POINT(-1,-1),personCurrent,personCurrent,path,score,false,false,false);
            }
            else{
                POINT previousActivePoint=activePoint;
                activePoint=POINT(-1,-1);
                PATH pathMaybe=checkOK(previousActivePoint,personMaybe);
                if(pathMaybe!=path){
                    matrix[previousActivePoint.first][previousActivePoint.second]=0;
                    matrix[personMaybe.first][personMaybe.second]=0;
                    score+=1;
                    return PersonMove(2,previousActivePoint,2,personMaybe,personCurrent,personCurrent,pathMaybe,score,false,false,false);
                }
                else
                    return PersonMove(3,previousActivePoint,0,POINT(-1,-1),personCurrent,personCurrent,path,score,false,false,false);
            }
}


PATH GameInfo::checkOK(POINT point1,POINT point2){
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

bool GameInfo::checkGameOver(){
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

POINT_SET GameInfo::findPointSet(int color){
    POINT_SET result;
    for(int i=0;i<6;i++)
        for(int j=0;j<6;j++)
            if(matrix[i][j]==color)
                result.push_back(POINT(i,j));
    return result;
}

/*思路：获取用户可以到达的点并组合遍历检查*/
 bool GameInfo::checkUserWin(){
     stack<POINT> whitePointStack;
     vector<POINT> pointCanBeReached;
     vector<POINT> pointHasBeenRead;
     whitePointStack.push(personCurrent);
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
     return true;
}

void  GameInfo::regenerateMap(){
    activePoint=POINT(-1,-1);
    srand((unsigned int)time(NULL));
    POINT_SET whitePoints;
    vector<int> boxNUM(3,0);
    for(int i=0;i<6;i++)
        for(int j=0;j<6;j++){
            if(POINT(i,j)!=personCurrent&&POINT(i,j)!=hint&&POINT(i,j)!=shuffle&&POINT(i,j)!=oneSecondMore)
                whitePoints.push_back(POINT(i,j));
            if(matrix[i][j])
                boxNUM[matrix[i][j]-1]++;
            matrix[i][j]=0;
        }
    for(int i=0;i<3;i++)
        for(int j=0;j<boxNUM[i];j++){
            int index=rand()%(whitePoints.size());
            POINT point=whitePoints[index];
            whitePoints.erase(whitePoints.begin()+index);
            matrix[point.first][point.second]=i+1;
        }
}

// void GameInfo::readTxt(){

// }

MATRIX  GameInfo::getMatrix(){
    return matrix;
}

POINT GameInfo::getPersonCurrent(){
    return personCurrent;
}

POINT GameInfo::getHintPoint(){
    return hint;
}

POINT GameInfo::getShuffle(){
    return  shuffle;
}

POINT GameInfo::getOneSecondMore(){
    return oneSecondMore;
}

POINT_PAIR GameInfo::getHint(){
    if(hintPair!=POINT_PAIR(POINT(-1,-1),POINT(-1,-1)))
        return hintPair;
    stack<POINT> whitePointStack;
    vector<POINT> pointCanBeReached;
    vector<POINT> pointHasBeenRead;
    whitePointStack.push(personCurrent);
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
                    return POINT_PAIR(boxVector[i],boxVector[j]);
    }
    return POINT_PAIR(POINT(-1,-1),POINT(-1,-1));
}

bool GameInfo::readJson(string jsonPath){
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
       if (!root["activePoint_x"].isNull()&&!root["activePoint_y"].isNull())
            activePoint=POINT(root["activePoint_x"].asInt(),root["activePoint_y"].asInt());
       else return false;
       if (!root["personCurrent_x"].isNull()&&!root["personCurrent_y"].isNull())
           personCurrent=POINT(root["personCurrent_x"].asInt(),root["personCurrent_y"].asInt());
       else return false;
       if (!root["oneSecondMore_x"].isNull()&&!root["oneSecondMore_y"].isNull())
           oneSecondMore=POINT(root["oneSecondMore_x"].asInt(),root["oneSecondMore_y"].asInt());
       else return false;
       if (!root["shuffle_x"].isNull()&&!root["shuffle_y"].isNull())
           shuffle=POINT(root["shuffle_x"].asInt(),root["shuffle_y"].asInt());
       else return false;
       if (!root["hint_x"].isNull()&&!root["hint_y"].isNull())
           hint=POINT(root["hint_x"].asInt(),root["hint_y"].asInt());
       else return false;

       if (!root["score"].isNull())
            score= root["score"].asInt();
       else return false;

       if (!root["hinting"].isNull())
            hinting= root["hinting"].asBool();
       else return false;

       if (!root["hintStartTime"].isNull())
            hintStartTime= root["hintStartTime"].asInt();
       else return false;

       if (!root["hintPair_1_x"].isNull()&&!root["hintPair_1_y"].isNull()&&!root["hintPair_2_x"].isNull()&&!root["hintPair_2_y"].isNull())
           hintPair=POINT_PAIR(POINT(root["hintPair_1_x"].asInt(),root["hintPair_1_y"].asInt()),POINT(root["hintPair_2_x"].asInt(),root["hintPair_2_y"].asInt()));
       else return false;

       if (!root["time_"].isNull())
            time_= root["time_"].asInt();
       else return false;

       stop=true;

       int bluePath_size=0;
       if (!root["bluePath_size"].isNull())
            bluePath_size= root["bluePath_size"].asInt();
       else return false;

       bluePath.clear();
       for(int i=0;i<bluePath_size;i++){
           if (!root["bluePath_"+to_string(i)+"_x"].isNull()&&!root["bluePath_"+to_string(i)+"_y"].isNull())
                bluePath.push_back(POINT(root["bluePath_"+to_string(i)+"_x"].asInt(),root["bluePath_"+to_string(i)+"_y"].asInt()));
           else return false;
       }

       if (!root["time_"].isNull())
            time_= root["time_"].asInt();
       else return false;

    }
    else return false;

    return true;
}

void GameInfo::toJson(string filePath){
    Json::Value rootValue = Json::objectValue;
    for(int i=0;i<6;i++)
        for(int j=0;j<6;j++)
            rootValue["matix"+to_string(i)+to_string(j)]=matrix[i][j];

    rootValue["type"] = type;
    rootValue["activePoint_x"] = activePoint.first;
    rootValue["activePoint_y"] = activePoint.second;
    rootValue["personCurrent_x"] = personCurrent.first;
    rootValue["personCurrent_y"] = personCurrent.second;
    rootValue["oneSecondMore_x"] = oneSecondMore.first;
    rootValue["oneSecondMore_y"] = oneSecondMore.second;
    rootValue["shuffle_x"] = shuffle.first;
    rootValue["shuffle_y"] = shuffle.second;
    rootValue["hint_x"] = hint.first;
    rootValue["hint_y"] = hint.second;
    rootValue["score"] = score;
    rootValue["hinting"] = hinting;
    rootValue["hintStartTime"] = hintStartTime;
    rootValue["hintPair_1_x"] = hintPair.first.first;
    rootValue["hintPair_1_y"] = hintPair.first.second;
    rootValue["hintPair_2_x"] = hintPair.second.first;
    rootValue["hintPair_2_y"] = hintPair.second.second;
    rootValue["bluePath_size"] = bluePath.size();
    for(int i=0;i<(int)bluePath.size();i++){
        rootValue["bluePath_"+to_string(i)+"_x"] = bluePath[i].first;
        rootValue["bluePath_"+to_string(i)+"_y"] = bluePath[i].second;
    }
    rootValue["time_"] = time_;
    rootValue["stop"] = true;

    Json::StyledWriter writer;
    string out=writer.write(rootValue);
    fstream file(filePath.c_str());
    file<<out;
    file.close();
}
