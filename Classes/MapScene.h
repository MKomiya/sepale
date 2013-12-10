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
        kPlayerTags = 1,
        kPlayerAnimateTags = 2,
        kPlayerMoveTags = 3,
        kVirtualPadBaseTags = 4,
        kVirtualPadSelectedTags = 5
    };
    
    // 移動方向のラベル
    enum MoveDirection
    {
        kMoveNo, kMoveUp, kMoveRight, kMoveDown, kMoveLeft
    };
    
    virtual bool init();
    static cocos2d::CCScene* scene();
    CREATE_FUNC(MapScene);
    
    // ccTouchのデリゲート
    virtual bool ccTouchBegan(cocos2d::CCTouch* touch, cocos2d::CCEvent* event);
    virtual void ccTouchMoved(cocos2d::CCTouch* touch, cocos2d::CCEvent* event);
    virtual void ccTouchEnded(cocos2d::CCTouch* touch, cocos2d::CCEvent* event);
    
    // moverを見てプレイヤーの移動を決定するスケジューラ
    void schedulePlayerMover(float dt);
    
private:
    // ユーザが移動させるプレイヤーの表示
    void _viewPlayerCharacter();
    
    // Virtual Padの初期化
    void _initVirtualPad();
    
    // プレイヤのアニメーション変更
    void _changePlayerAnimation(std::string direction);
    
    
    MoveDirection _mover;
};

#endif /* defined(__sepale__MapScene__) */
