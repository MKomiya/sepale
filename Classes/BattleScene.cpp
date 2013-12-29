//
//  BattleScene.cpp
//  sepale
//
//  Created by S_Wyvern on 2013/12/12.
//
//

#include "cocos-ext.h"
#include "BattleScene.h"

USING_NS_CC;
USING_NS_CC_EXT;

CCScene* BattleScene::scene()
{
    CCScene* scene = CCScene::create();
    BattleScene* layer = BattleScene::create();
    scene->addChild(layer);
    return scene;
}

bool BattleScene::init()
{
    if( !CCLayer::init() )
    {
        return false;
    }
    
    // 情報の読み込み
    _loadEnemyStatus();
    
    const CCSize s = CCDirector::sharedDirector()->getWinSize();
    
    // 戦闘背景
    CCSprite* wallback = CCSprite::create("battleback/cave.png");
    wallback->setPosition(ccp(s.width / 2, s.height / 2 + 40));
    this->addChild(wallback);
    
    // 敵
    CCSprite* enemy = CCSprite::create("monster/ghoul.png");
    enemy->getTexture()->setAliasTexParameters();
    enemy->setPosition(ccp(2 * s.width / 4, s.height / 2 + 50));
    enemy->setTag(kEnemySpriteTags);
    this->addChild(enemy);
    
    // ボタン入力用のレイヤー作成
    _commandLayer = CCLayer::create();
    this->addChild(_commandLayer);
    
    // ボタン
    CCControlButton* button1 = CCControlButton::create(CCLabelTTF::create("スキル", "Arial", 16), CCScale9Sprite::create("interfaces/button.png"));
    button1->getTitleLabel()->setScale(0.5f);
    button1->setZoomOnTouchDown(false);
    button1->setAnchorPoint(ccp(0, 0));
    button1->setPosition(ccp(16, 10));
    button1->setPreferredSize(CCSizeMake(60, 24));
    CCScale9Sprite* pushed = CCScale9Sprite::create("interfaces/button.png");
    pushed->setColor(ccc3(50, 80, 125));
    button1->setBackgroundSpriteForState(pushed, CCControlStateHighlighted);
    _commandLayer->addChild(button1);
    button1->addTargetWithActionForControlEvents(this, cccontrol_selector(BattleScene::_selectedSkillCommand), CCControlEventTouchUpInside);
    
    CCControlButton* button2 = CCControlButton::create(CCLabelTTF::create("にげる", "Arial", 16), CCScale9Sprite::create("interfaces/button.png"));
    button2->getTitleLabel()->setScale(0.5f);
    button2->setZoomOnTouchDown(false);
    button2->setAnchorPoint(ccp(1, 0));
    button2->setPosition(ccp(s.width - 16, 10));
    button2->setPreferredSize(button1->getPreferredSize());
    pushed = CCScale9Sprite::create("interfaces/button.png");
    pushed->setColor(ccc3(50, 80, 125));
    button2->setBackgroundSpriteForState(pushed, CCControlStateHighlighted);
    _commandLayer->addChild(button2);
    button2->addTargetWithActionForControlEvents(this, cccontrol_selector(BattleScene::_selectedRunawayCommand), CCControlEventTouchUpInside);
    
    CCControlButton* button3 = CCControlButton::create(CCLabelTTF::create("たたかう", "Arial", 16), CCScale9Sprite::create("interfaces/button.png"));
    button3->getTitleLabel()->setScale(0.5f);
    button3->setZoomOnTouchDown(false);
    button3->setAnchorPoint(ccp(0, 1));
    button3->setPosition(ccp(16, s.height - 160 - 10));
    button3->setPreferredSize(button1->getPreferredSize());
    pushed = CCScale9Sprite::create("interfaces/button.png");
    pushed->setColor(ccc3(50, 80, 125));
    button3->setBackgroundSpriteForState(pushed, CCControlStateHighlighted);
    _commandLayer->addChild(button3);
    button3->addTargetWithActionForControlEvents(this, cccontrol_selector(BattleScene::_selectedAttackCommand), CCControlEventTouchUpInside);
    
    CCControlButton* button4 = CCControlButton::create(CCLabelTTF::create("アイテム", "Arial", 16), CCScale9Sprite::create("interfaces/button.png"));
    button4->getTitleLabel()->setScale(0.5f);
    button4->setZoomOnTouchDown(false);
    button4->setAnchorPoint(ccp(1, 1));
    button4->setPosition(ccp(s.width - 16, s.height - 160 - 10));
    button4->setPreferredSize(button1->getPreferredSize());
    pushed = CCScale9Sprite::create("interfaces/button.png");
    pushed->setColor(ccc3(50, 80, 125));
    button4->setBackgroundSpriteForState(pushed, CCControlStateHighlighted);
    _commandLayer->addChild(button4);
    button4->addTargetWithActionForControlEvents(this, cccontrol_selector(BattleScene::_selectedItemCommand), CCControlEventTouchUpInside);
    
    // プログレスレイヤー（行動送り用のボタンを配置するレイヤー）を作成
    _progressLayer = CCLayer::create();
    _progressLayer->setVisible(false);
    this->addChild(_progressLayer);
    
    // 今ボタンにしてますが今後タップ処理にするかも
    CCControlButton* prog_button = CCControlButton::create(CCLabelTTF::create("▼", "Arial", 16), CCScale9Sprite::create("interfaces/button.png"));
    prog_button->getTitleLabel()->setScale(0.5f);
    prog_button->setZoomOnTouchDown(false);
    prog_button->setAnchorPoint(ccp(0.5f, 0.5f));
    prog_button->setPosition(ccp(s.width / 2, s.height - 160 - 20));
    prog_button->setPreferredSize(button1->getPreferredSize());
    pushed = CCScale9Sprite::create("interfaces/button.png");
    pushed->setColor(ccc3(50, 80, 125));
    prog_button->setBackgroundSpriteForState(pushed, CCControlStateHighlighted);
    _progressLayer->addChild(prog_button);
    prog_button->addTargetWithActionForControlEvents(this, cccontrol_selector(BattleScene::_turnProcess), CCControlEventTouchUpInside);
    
    // 戦闘のメッセージビューの表示
    CCSprite* message_view = CCSprite::create("interfaces/battle_messageview.png");
    message_view->getTexture()->setAliasTexParameters();
    message_view->setAnchorPoint(ccp(0.5, 1));
    message_view->setPosition(ccp(s.width / 2, s.height - 7));
    this->addChild(message_view);
    CCLabelTTF* label = CCLabelTTF::create("vs グール", "Arial", 16);
    label->setScale(0.5f);
    label->setAnchorPoint(ccp(0.5, 1));
    label->setPosition(message_view->getPosition());
    label->setPositionY(label->getPositionY() - 6);
    label->setTag(kMessageViewLabelTags);
    this->addChild(label);
    
    // 戦闘のプレイヤービューの表示
    CCSprite* status_view = CCSprite::create("interfaces/battle_statusview.png");
    status_view->getTexture()->setAliasTexParameters();
    status_view->setAnchorPoint(ccp(0.5f, 0));
    status_view->setPosition(ccp(s.width/2, 87));
    status_view->setTag(kPlayerViewTags);
    this->addChild(status_view);
    CCLabelTTF* name_label = CCLabelTTF::create("HINOMI", "Arial", 16);
    status_view->addChild(name_label);
    name_label->setScale(0.5f);
    name_label->setAnchorPoint(ccp(0, 0));
    name_label->setPosition(ccp(4, 22));
    CCLabelTTF* hp_label = CCLabelTTF::create("Life", "Arial", 16);
    status_view->addChild(hp_label);
    hp_label->setScale(0.5f);
    hp_label->setAnchorPoint(ccp(0, 0));
    hp_label->setPosition(ccp(4, 12));
    CCLabelTTF* sol_label = CCLabelTTF::create("SOL", "Arial", 16);
    status_view->addChild(sol_label);
    sol_label->setScale(0.5f);
    sol_label->setAnchorPoint(ccp(0, 0));
    sol_label->setPosition(ccp(4, 2));
    CCLabelTTF* hpnum_label = CCLabelTTF::create("100/100", "Arial", 16);
    status_view->addChild(hpnum_label);
    hpnum_label->setScale(0.5f);
    hpnum_label->setAnchorPoint(ccp(0, 0));
    hpnum_label->setPosition(ccp(28, 12));
    CCLabelTTF* solnum_label = CCLabelTTF::create("30/30", "Arial", 16);
    status_view->addChild(solnum_label);
    solnum_label->setScale(0.5f);
    solnum_label->setAnchorPoint(ccp(0, 0));
    solnum_label->setPosition(ccp(28, 2));
    
    return true;
}

void BattleScene::attackCommand()
{
    // ダメージ計算処理
    int damage = _damageCalc(&_player, &_enemy);
    _enemy.status.life -= damage;
    
    // 相手を点滅させる
    CCSprite* enemy_sprite = (CCSprite*)this->getChildByTag(kEnemySpriteTags);
    CCBlink* blink = CCBlink::create(0.3f, 5);
    enemy_sprite->runAction(blink);
    
    // 斬撃エフェクト
    CCTexture2D* texture = CCTextureCache::sharedTextureCache()->addImage("effects/effect.png");
    texture->setAliasTexParameters();
    CCSprite* sprite = CCSprite::createWithTexture(texture);
    CCPoint enemy_pos = enemy_sprite->getPosition();
    sprite->setPosition(enemy_pos);
    sprite->setTag(kEffectAnimationTags);
    sprite->setScale(0.5f);
    
    CCAnimation* cache = CCAnimation::create();
    cache->addSpriteFrameWithTexture(texture, CCRectMake(0, 0, 64, 64));
    cache->addSpriteFrameWithTexture(texture, CCRectMake(64, 0, 64, 64));
    cache->addSpriteFrameWithTexture(texture, CCRectMake(128, 0, 64, 64));
    cache->setDelayPerUnit(0.15f / 3.0f);
    cache->setRestoreOriginalFrame(false);
    
    CCAnimate* action = CCAnimate::create(cache);
    CCRemoveSelf* selfremover = CCRemoveSelf::create();
    CCSequence* action_list = CCSequence::create(action, selfremover, NULL);
    
    sprite->runAction(action_list);
    this->addChild(sprite);
    
    CCLabelTTF* label = (CCLabelTTF*)this->getChildByTag(kMessageViewLabelTags);
    label->setString("ひかりのつるぎ");
    
    // ダメージ描画
    CCLabelTTF* damage_label = CCLabelTTF::create("", "Arial", 32);
    this->addChild(damage_label);
    damage_label->setString(CCString::createWithFormat("%d", damage)->getCString());
    damage_label->setScale(0.5f);
    damage_label->setPosition(enemy_pos);
    damage_label->setColor(ccc3(255, 64, 64));
    CCMoveBy* mover = CCMoveBy::create(0.3f, ccp(3.f, 16.f));
    CCRemoveSelf* remove = CCRemoveSelf::create();
    damage_label->runAction(CCSequence::create(mover, remove, NULL));
}

void BattleScene::skillCommand()
{
}

void BattleScene::itemCommand()
{
    
}

void BattleScene::runawayCommand()
{
    CCLOG("逃げ出した");
}

void BattleScene::enemyAttack()
{
    CCLOG("enemy attacked");
    
    // ダメージ計算処理
    int damage = _damageCalc(&_enemy, &_player);
    _player.status.life -= damage;
    
    // 相手を点滅させる
    CCSprite* player_view = (CCSprite*)this->getChildByTag(kPlayerViewTags);
    CCBlink* blink = CCBlink::create(0.3f, 5);
    player_view->runAction(blink);
    
    // 斬撃エフェクト
    CCTexture2D* texture = CCTextureCache::sharedTextureCache()->addImage("effects/effect.png");
    texture->setAliasTexParameters();
    CCSprite* sprite = CCSprite::createWithTexture(texture);
    CCPoint player_pos = player_view->getPosition();
    sprite->setPosition(player_pos);
    sprite->setTag(kEffectAnimationTags);
    sprite->setScale(0.5f);
    
    CCAnimation* cache = CCAnimation::create();
    cache->addSpriteFrameWithTexture(texture, CCRectMake(0, 0, 64, 64));
    cache->addSpriteFrameWithTexture(texture, CCRectMake(64, 0, 64, 64));
    cache->addSpriteFrameWithTexture(texture, CCRectMake(128, 0, 64, 64));
    cache->setDelayPerUnit(0.15f / 3.0f);
    cache->setRestoreOriginalFrame(false);
    
    CCAnimate* action = CCAnimate::create(cache);
    CCRemoveSelf* selfremover = CCRemoveSelf::create();
    CCSequence* action_list = CCSequence::create(action, selfremover, NULL);
    
    sprite->runAction(action_list);
    this->addChild(sprite);
    
    CCLabelTTF* label = (CCLabelTTF*)this->getChildByTag(kMessageViewLabelTags);
    label->setString("なぐりかかる");
    
    // ダメージ描画
    CCLabelTTF* damage_label = CCLabelTTF::create("", "Arial", 32);
    this->addChild(damage_label);
    damage_label->setString(CCString::createWithFormat("%d", damage)->getCString());
    damage_label->setScale(0.5f);
    damage_label->setPosition(player_pos);
    damage_label->setColor(ccc3(255, 64, 64));
    CCMoveBy* mover = CCMoveBy::create(0.3f, ccp(3.f, 16.f));
    CCRemoveSelf* remove = CCRemoveSelf::create();
    damage_label->runAction(CCSequence::create(mover, remove, NULL));
}

void BattleScene::_loadEnemyStatus()
{
    // 外部テーブルから敵の情報を読み込み
    // 今はまだ決めうち
    CharacterStatus enemy_status;
    enemy_status.life     = 50;
    enemy_status.max_life = enemy_status.life;
    enemy_status.attack   = 20;
    enemy_status.defence  = 0;
    enemy_status.speed    = 10;
    
    _enemy.status = enemy_status;
    _player.status = enemy_status;
    _player.status.speed = 15;
}

void BattleScene::_selectedAttackCommand()
{
    CCCallFunc* action = CCCallFunc::create(this, callfunc_selector(BattleScene::attackCommand));
    _player.action = action;
    action->retain();
    
    _toggleCommandMenu();
    _enemyCommandSelect();
}

void BattleScene::_selectedSkillCommand()
{
    
}

void BattleScene::_selectedItemCommand()
{
}

void BattleScene::_selectedRunawayCommand()
{
    CCCallFunc* action = CCCallFunc::create(this, callfunc_selector(BattleScene::runawayCommand));
    _player.action = action;
    action->retain();
    
    _toggleCommandMenu();
    _enemyCommandSelect();
}

void BattleScene::_enemyCommandSelect()
{
    CCCallFunc* action = CCCallFunc::create(this, callfunc_selector(BattleScene::enemyAttack));
    _enemy.action = action;
    action->retain();

    _sortCharacterAction();
}

void BattleScene::_sortCharacterAction()
{
    if(_player.status.speed >= _enemy.status.speed)
    {
        _characters_action.addObject(_player.action);
        _characters_action.addObject(_enemy.action);
    }
    else
    {
        _characters_action.addObject(_enemy.action);
        _characters_action.addObject(_player.action);
    }
    
    _turnProcess();
}

void BattleScene::_turnProcess()
{
    CCCallFunc* action = (CCCallFunc*)_characters_action.objectAtIndex(0);
    _characters_action.removeObjectAtIndex(0);
    
    this->runAction(action);
    action->release();
    
    if(_characters_action.count() == 0)
    {
        _toggleCommandMenu();
        return ;
    }
}

int BattleScene::_damageCalc(BattleScene::BattleData *from, BattleScene::BattleData *to)
{
    int damage = from->status.attack - to->status.defence;
    damage = damage < 0 ? 0 : damage;
    
    return damage;
}

void BattleScene::_toggleCommandMenu()
{
    if(_commandLayer->isVisible())
    {
        _commandLayer->setVisible(false);
        _progressLayer->setVisible(true);
        return ;
    }
    
    _commandLayer->setVisible(true);
    _progressLayer->setVisible(false);
}