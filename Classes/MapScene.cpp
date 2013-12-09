//
//  MapScene.cpp
//  sepale
//
//  Created by S_Wyvern on 2013/08/19.
//
//

#include "MapScene.h"

#include "SneakyJoystickSkinnedBase.h"
#include "SneakyJoystick.h"

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
    
    // メンバ変数初期化
    _mover = kMoveNo;
    
    // タップの有効化
    this->setTouchEnabled(true);
    this->setTouchMode(kCCTouchesOneByOne);
    
    _viewPlayerCharacter();
    _initVirtualPad();
    
    return true;
}

void MapScene::schedulePlayerMover(float dt)
{
    CCSprite* player = (CCSprite*)this->getChildByTag(kPlayerTags);
    CCAction* move_sequence = player->getActionByTag(kPlayerMoveTags);
    if(move_sequence != NULL)
    {
        return ;
    }
    
    CCPoint move_pos = ccp(0.f, 0.f);
    switch (_mover) {
        case kMoveUp:
            move_pos = ccp(0.f, 16.f);
            break;
        case kMoveRight:
            move_pos = ccp(16.f, 0.f);
            break;
        case kMoveDown:
            move_pos = ccp(0.f, -16.f);
            break;
        case kMoveLeft:
            move_pos = ccp(-16.f, 0.f);
            break;
        case kMoveNo:
            return;
    }
    
    CCMoveBy* move_act = CCMoveBy::create(0.3f, move_pos);
    move_act->setTag(kPlayerMoveTags);
    player->runAction(move_act);
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
    sprite->setTag(kPlayerTags);
    sprite->runAction(action);
    
    CCSize winSize = CCDirector::sharedDirector()->getWinSize();
    sprite->setPosition(ccp(winSize.width/2, winSize.height/2 + 40));
    this->addChild(sprite);
    
    // プレイヤー移動のスケジューラ登録
    this->schedule(schedule_selector(MapScene::schedulePlayerMover));
}

void MapScene::_initVirtualPad()
{
    // Texture読み込みとエイジング無効化
    CCTexture2D* background_tex = CCTextureCache::sharedTextureCache()->addImage("interfaces/vjoystick_background.png");
    CCTexture2D* thumb_tex = CCTextureCache::sharedTextureCache()->addImage("interfaces/vjoystick_thumb.png");
    background_tex->setAliasTexParameters();
    thumb_tex->setAliasTexParameters();
    CCSprite* background_sprite = CCSprite::createWithTexture(background_tex);
    CCSprite* thumb_sprite = CCSprite::createWithTexture(thumb_tex);
    
    // とりあえずSneaky Inputを表示するだけしてみる
    SneakyJoystick* joystick = new SneakyJoystick();
    joystick->setHasDeadzone(true);
    joystick->setAutoCenter(true);
    joystick->setDeadRadius(8.0f);
    SneakyJoystickSkinnedBase* base = SneakyJoystickSkinnedBase::create();
    base->setBackgroundSprite(background_sprite);
    base->setThumbSprite(thumb_sprite);
    base->setJoystick(joystick);
    const CCSize s = CCDirector::sharedDirector()->getWinSize();
    base->setPosition(ccp(s.width / 2, s.height / 2));
    base->setTag(kVirtualPadBaseTags);
    this->addChild(base, 128);
    
    // Virtual Padの非表示
    base->setVisible(false);
}

bool MapScene::ccTouchBegan(CCTouch *pTouch, CCEvent *pEvent)
{
    CCLOG("touch began");
    
    // virtual padを取得して表示化する
    SneakyJoystickSkinnedBase* base = (SneakyJoystickSkinnedBase*)this->getChildByTag(kVirtualPadBaseTags);
    SneakyJoystick* joystick = base->getJoystick();
    base->setVisible(true);
    base->setPosition(ccp(pTouch->getLocation().x, pTouch->getLocation().y));
    
    joystick->ccTouchBegan(pTouch, pEvent);
    
    return true;
}

void MapScene::ccTouchMoved(CCTouch *pTouch, CCEvent *pEvent)
{
    CCLOG("touch moved");
    SneakyJoystickSkinnedBase* base = (SneakyJoystickSkinnedBase*)this->getChildByTag(kVirtualPadBaseTags);
    SneakyJoystick* joystick = base->getJoystick();
    
    joystick->ccTouchMoved(pTouch, pEvent);
    
    CCPoint point = joystick->getVelocity();
    CCLOG("move x:%2.1f, y:%2.1f", point.x, point.y);
    
    float velocity_list[4] = {
        // Up,   Right,   Down,     Left
        point.y, point.x, -point.y, -point.x
    };
    int max_index = std::distance(velocity_list, std::max_element(velocity_list, velocity_list + 4));
    
    switch (max_index) {
        case 0:
            _mover = kMoveUp;
            this->_changePlayerAnimation("up");
            CCLOG("move up");
            break;
        case 1:
            _mover = kMoveRight;
            this->_changePlayerAnimation("right");
            CCLOG("move right");
            break;
        case 2:
            _mover = kMoveDown;
            this->_changePlayerAnimation("down");
            CCLOG("move down");
            break;
        case 3:
            _mover = kMoveLeft;
            this->_changePlayerAnimation("left");
            CCLOG("move left");
            break;
        default:
            _mover = kMoveNo;
            break;
    }
    
    // 傾きが小さければ移動判定にしない
    if(fabsf(velocity_list[max_index]) <= 0.8f)
    {
        _mover = kMoveNo;
    }
}

void MapScene::ccTouchEnded(CCTouch *pTouch, CCEvent *pEvent)
{
    CCLOG("touch ended");
    
    _mover = kMoveNo;
    
    SneakyJoystickSkinnedBase* base = (SneakyJoystickSkinnedBase*)this->getChildByTag(kVirtualPadBaseTags);
    base->setVisible(false);
}

void MapScene::_changePlayerAnimation(std::string direction)
{
    std::string anime_name = direction + "_anime";
    CCAnimation* animation = CCAnimationCache::sharedAnimationCache()->animationByName(anime_name.c_str());
    CCAnimate* animate = CCAnimate::create(animation);
    animate->setTag(kPlayerAnimateTags);
    
    CCSprite* player = (CCSprite*)this->getChildByTag(kPlayerTags);
    player->stopActionByTag(kPlayerAnimateTags);
    player->runAction(animate);
}
