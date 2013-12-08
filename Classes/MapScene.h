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
    // CCNodeのタグ
    enum Tags
    {
        kPlayerAnimateTags = 1,
        kVirtualPadBaseTags = 2
    };
    
    virtual bool init();
    static cocos2d::CCScene* scene();
    CREATE_FUNC(MapScene);
    
    // ccTouchのデリゲート
    virtual bool ccTouchBegan(cocos2d::CCTouch* touch, cocos2d::CCEvent* event);
    virtual void ccTouchMoved(cocos2d::CCTouch* touch, cocos2d::CCEvent* event);
    virtual void ccTouchEnded(cocos2d::CCTouch* touch, cocos2d::CCEvent* event);
    
private:
    // ユーザが移動させるプレイヤーの表示
    void _viewPlayerCharacter();
    
    // Virtual Padの初期化
    void _initVirtualPad();
};

#endif /* defined(__sepale__MapScene__) */
