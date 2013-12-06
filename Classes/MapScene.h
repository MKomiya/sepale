//
//  MapScene.h
//  sepale
//
//  Created by S_Wyvern on 2013/08/19.
//
//

#ifndef __sepale__MapScene__
#define __sepale__MapScene__

#include <cocos2d.h>

class MapScene : public cocos2d::CCLayer
{
public:
    virtual bool init();
    static cocos2d::CCScene* scene();
    CREATE_FUNC(MapScene);
};

#endif /* defined(__sepale__MapScene__) */
