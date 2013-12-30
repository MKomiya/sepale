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
#include "characterStatus.h"

class BattleScene : public cocos2d::CCLayer
{
public:
    enum {
        kEnemySpriteTags,
        kEffectAnimationTags,
        kMessageViewLabelTags,
        kPlayerViewTags,
        kPlayerHPLabelTags,
        kPlayerSOLLabelTags
    };
    
    struct BattleData {
        CharacterStatus status;
        cocos2d::CCCallFunc* action;
    };
    
    static cocos2d::CCScene* scene();
    virtual bool init();
    CREATE_FUNC(BattleScene);
    
    // コマンド入力
    void attackCommand();
    void skillCommand();
    void itemCommand();
    void runawayCommand();
    
    // 敵の行動
    void enemyAttack();
    
    // シーン遷移用のタッチ取得
    virtual bool ccTouchBegan(cocos2d::CCTouch* touch, cocos2d::CCEvent* event);
    
private:
    // 【戦闘開始時】敵データ読み込み処理
    void _loadEnemyStatus();
    // 【コマンド入力後】コマンド入力時処理
    // 3.0以降ならlamdaで書ける所・・・
    void _selectedAttackCommand();
    void _selectedSkillCommand();
    void _selectedItemCommand();
    void _selectedRunawayCommand();
    
    void _enemyCommandSelect();
    void _sortCharacterAction();
    void _turnProcess();
    int _damageCalc(BattleData* from, BattleData* to);
    
    // コマンド入力部のtoggle切り替え
    void _toggleCommandMenu();
    
    // アクション後の後始末処理
    void _doneAction();
    // プレイヤーの表示部の更新
    void _updatePlayerStatusView();
    // 死亡判定
    void _checkDead();
    // 死亡時の処理
    void _funcDeadProgress();
    // 死亡エフェクトの再生
    void _playDeadEffect();
    // リザルトの表示
    void _viewResult();
    
    cocos2d::CCLayer *_commandLayer; // 攻撃アイテムスキル逃げる
    cocos2d::CCLayer *_progressLayer;// 行動進行の▼
    cocos2d::CCLayer *_skillLayer;   // スキル一覧
    cocos2d::CCLayer *_itemLayer;    // アイテム一覧
    BattleData _player, _enemy;
    cocos2d::CCArray _characters_action;
    
    bool _is_end_battle;    // 戦闘が終わったかどうか
};

#endif /* defined(__sepale__BattleScene__) */
