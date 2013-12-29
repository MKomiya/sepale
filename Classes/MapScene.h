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
        kVirtualPadSelectedTags = 5,
        kMapTags = 6,
        kMapMoveTags = 7,
    };
    
    // 移動方向のラベル
    enum MoveDirection
    {
        kMoveNo, kMoveUp, kMoveRight, kMoveDown, kMoveLeft
    };
    
    virtual bool init();
    static cocos2d::CCScene* scene();
    CREATE_FUNC(MapScene);
    
    virtual void onEnter();
    
    // ccTouchのデリゲート
    virtual bool ccTouchBegan(cocos2d::CCTouch* touch, cocos2d::CCEvent* event);
    virtual void ccTouchMoved(cocos2d::CCTouch* touch, cocos2d::CCEvent* event);
    virtual void ccTouchEnded(cocos2d::CCTouch* touch, cocos2d::CCEvent* event);
    
    // moverを見てプレイヤーの移動を決定するスケジューラ
    void schedulePlayerMover(float dt);
    
private:
    // Virtual Padの初期化
    void _initVirtualPad();
    
    // ユーザが移動させるプレイヤーの表示
    void _viewPlayerCharacter();
    // プレイヤのアニメーション変更
    void _changePlayerAnimation(std::string direction);
    // プレイヤのアニメーションストップ
    void _stopPlayerAnimation();
    
    // マップの表示
    void _viewMap();
    // マップオブジェクトとの衝突判定
    bool _checkCollidable(int gid_x, int gid_y);
    
    // 移動後のエンカウントチェック処理
    void _checkEncounter();
    // エンカウントのチェック
    bool _isEncounterEnemy();
    // 戦闘処理
    void _startBattle();
    
    
    MoveDirection _mover;               // 移動方向
    cocos2d::CCPoint _player_pos;       // プレイヤーのマップ位置
    int _walk_count;                    // 戦闘から歩いた歩数
};

#endif /* defined(__sepale__MapScene__) */
