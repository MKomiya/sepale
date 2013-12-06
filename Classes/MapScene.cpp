//
//  MapScene.cpp
//  sepale
//
//  Created by S_Wyvern on 2013/08/19.
//
//

#include "MapScene.h"

USING_NS_CC;

CCScene* MapScene::scene()
{
    CCScene* scene = CCScene::create();
    MapScene* layer = MapScene::create();
    scene->addChild(layer);
    return scene;
}

bool MapScene::init()
{
    if ( !CCLayer::init() )
    {
        return false;
    }
    
    _viewPlayerCharacter();
    
    return true;
}

void MapScene::_viewPlayerCharacter()
{
    // キャラチップの読み込みとエイジング無効化
    CCTexture2D* texture = CCTextureCache::sharedTextureCache()->addImage("charachip/hiroine_1616.png");
    texture->setAliasTexParameters();
    
    // フレーム分割
    CCSpriteFrameCache* frameCache = CCSpriteFrameCache::sharedSpriteFrameCache();
    frameCache->addSpriteFramesWithFile("charachip/hiroine.plist", "charachip/hiroine_1616.png");
    
    // アニメーションの設定
    CCSprite* sprite = CCSprite::create();
    CCAnimationCache* cache = CCAnimationCache::sharedAnimationCache();
    cache->addAnimationsWithFile("charachip/animations.plist");
    CCAnimation* animation = cache->animationByName("down_anime");
    CCAnimate* action = CCAnimate::create(animation);
    action->setTag(kPlayerAnimateTags);
    sprite->runAction(action);
    
    CCSize winSize = CCDirector::sharedDirector()->getWinSize();
    sprite->setPosition(ccp(winSize.width/2, winSize.height/2 + 40));
    this->addChild(sprite);
}