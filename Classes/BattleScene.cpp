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
    
    const CCSize s = CCDirector::sharedDirector()->getWinSize();
    
    // 戦闘背景
    auto wallback = CCSprite::create("battleback/cave.png");
    wallback->setPosition(ccp(s.width / 2, s.height / 2 + 40));
    this->addChild(wallback);
    
    // 敵
    auto enemy = CCSprite::create("monster/ghoul.png");
    enemy->getTexture()->setAliasTexParameters();
    enemy->setPosition(ccp(2 * s.width / 4, s.height / 2 + 50));
    this->addChild(enemy);
    
    // ボタン
    auto button1 = CCControlButton::create(CCLabelTTF::create("スキル", "Arial", 8), CCScale9Sprite::create("interfaces/button.png"));
    button1->setZoomOnTouchDown(false);
    button1->setAnchorPoint(ccp(0, 0));
    button1->setPosition(ccp(16, 10));
    button1->setPreferredSize(CCSizeMake(60, 24));
    this->addChild(button1);
    
    auto button2 = CCControlButton::create(CCLabelTTF::create("にげる", "Arial", 8), CCScale9Sprite::create("interfaces/button.png"));
    button2->setZoomOnTouchDown(false);
    button2->setAnchorPoint(ccp(1, 0));
    button2->setPosition(ccp(s.width - 16, 10));
    button2->setPreferredSize(button1->getPreferredSize());
    this->addChild(button2);
    
    auto button3 = CCControlButton::create(CCLabelTTF::create("たたかう", "Arial", 8), CCScale9Sprite::create("interfaces/button.png"));
    button3->setZoomOnTouchDown(false);
    button3->setAnchorPoint(ccp(0, 1));
    button3->setPosition(ccp(16, s.height - 160 - 10));
    button3->setPreferredSize(button1->getPreferredSize());
    this->addChild(button3);

    auto button4 = CCControlButton::create(CCLabelTTF::create("アイテム", "Arial", 8), CCScale9Sprite::create("interfaces/button.png"));
    button4->setZoomOnTouchDown(false);
    button4->setAnchorPoint(ccp(1, 1));
    button4->setPosition(ccp(s.width - 16, s.height - 160 - 10));
    button4->setPreferredSize(button1->getPreferredSize());
    this->addChild(button4);
    
    return true;
}