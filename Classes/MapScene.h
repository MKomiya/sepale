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
    enum Tags
    {
        kPlayerAnimateTags = 1
    };
    virtual bool init();
    static cocos2d::CCScene* scene();
    CREATE_FUNC(MapScene);
    
private:
    // ユーザが移動させるプレイヤーの表示
    void _viewPlayerCharacter();
};

#endif /* defined(__sepale__MapScene__) */
