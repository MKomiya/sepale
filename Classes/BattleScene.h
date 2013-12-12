//
//  BattleScene.h
//  sepale
//
//  Created by S_Wyvern on 2013/12/12.
//
//

#ifndef __sepale__BattleScene__
#define __sepale__BattleScene__

#include <iostream>

#include "cocos2d.h"

class BattleScene : public cocos2d::CCLayer
{
public:
    static cocos2d::CCScene* scene();
    virtual bool init();
    CREATE_FUNC(BattleScene);
};

#endif /* defined(__sepale__BattleScene__) */
