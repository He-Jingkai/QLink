#include <PersonMove.h>

using namespace std;


PersonMove::PersonMove(int boxOneState_, pair<int,int> boxOnePosition_,
           int boxTwoState_, pair<int,int> boxTwoPosition_,
           pair<int,int> personPreviousPosition_, pair<int,int> personCurrentPosition_,
                       vector<pair<int,int>> path_,
                       int score,
                       bool oneSecondMore,bool shuffle,bool hint)
    :boxOneState(boxOneState_),boxOnePosition(boxOnePosition_),
      boxTwoState(boxTwoState_),boxTwoPosition(boxTwoPosition_),
      personPreviousPosition(personPreviousPosition_),personCurrentPosition(personCurrentPosition_),
      path(path_),
      score(score),
      oneSecondMore(oneSecondMore),shuffle(shuffle),hint(hint){}
