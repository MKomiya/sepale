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
        kEffectAnimationTags
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
    
    cocos2d::CCLayer *_commandLayer, *_progressLayer;
    BattleData _player, _enemy;
    cocos2d::CCArray _characters_action;
};

#endif /* defined(__sepale__BattleScene__) */
