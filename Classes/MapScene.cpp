//
//  MapScene.cpp
//  sepale
//
//  Created by S_Wyvern on 2013/08/19.
//
//

#include <algorithm>
#include "MapScene.h"
#include "BattleScene.h"

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
    _player_pos = ccp(4, 6);
    _walk_count = 0;
    
    // タップの有効化
    this->setTouchEnabled(true);
    this->setTouchMode(kCCTouchesOneByOne);
    
    _viewMap();
    _viewPlayerCharacter();
    _initVirtualPad();
    
    return true;
}

void MapScene::schedulePlayerMover(float dt)
{
    // マップを取得し，移動アクションがあるなら処理しない
    CCTMXTiledMap* map = (CCTMXTiledMap*)this->getChildByTag(kMapTags);
    if(map->getActionByTag(kMapMoveTags) != NULL)
    {
        return ;
    }
    
    // 移動ベクトルと移動後のマップ座標を与えてやる
    CCPoint move_pos = ccp(0.f, 0.f);
    CCPoint next_pos = _player_pos;
    switch (_mover) {
        case kMoveUp:
            move_pos = ccp(0.f, 16.f);
            next_pos.y -= 1;
            break;
        case kMoveRight:
            move_pos = ccp(16.f, 0.f);
            next_pos.x += 1;
            break;
        case kMoveDown:
            move_pos = ccp(0.f, -16.f);
            next_pos.y += 1;
            break;
        case kMoveLeft:
            move_pos = ccp(-16.f, 0.f);
            next_pos.x -= 1;
            break;
        case kMoveNo:
            return;
    }
    move_pos = -move_pos; // マップの移動なので反転（キャラが上に行くときはマップが下に行く）
    
    // 移動後マップ座標の衝突判定
    if(_checkCollidable(next_pos.x, next_pos.y))
    {
        // 衝突するなら移動させず処理しない
        _mover = kMoveNo;
        return;
    }
    
    // 移動処理を行う
    _player_pos = next_pos;
    CCMoveBy* move_act = CCMoveBy::create(0.3f, move_pos);
    move_act->setTag(kMapMoveTags);
    map->runAction(move_act);
    
    // 歩数を増やす
    ++_walk_count;
    
    // エンカウント確認
    if(_isEncounterEnemy())
    {
        _startBattle();
    }
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

    // プレイヤーの初期位置設定
    const CCSize s = CCDirector::sharedDirector()->getWinSize();
    sprite->setAnchorPoint(ccp(0, 1));
    sprite->setPosition(ccp(_player_pos.x * 16.f, s.height -  _player_pos.y * 16.f));
    this->addChild(sprite);
    
    // プレイヤー移動のスケジューラ登録
    this->schedule(schedule_selector(MapScene::schedulePlayerMover));
}

void MapScene::_initVirtualPad()
{
    // Texture読み込みとエイジング無効化
    CCTexture2D* background_tex = CCTextureCache::sharedTextureCache()->addImage("interfaces/vjoystick_background.png");
    CCTexture2D* thumb_tex = CCTextureCache::sharedTextureCache()->addImage("interfaces/vjoystick_thumb.png");
    CCTexture2D* selected_tex = CCTextureCache::sharedTextureCache()->addImage("interfaces/vjoystick_selected.png");
    background_tex->setAliasTexParameters();
    thumb_tex->setAliasTexParameters();
    selected_tex->setAliasTexParameters();
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
    base->setVisible(false);
    
    // 光るところ
    CCSprite* selected = CCSprite::createWithTexture(selected_tex);
    selected->setPosition(base->getPosition());
    selected->setTag(kVirtualPadSelectedTags);
    this->addChild(selected, 129);
    selected->setVisible(false);
}

bool MapScene::ccTouchBegan(CCTouch *pTouch, CCEvent *pEvent)
{
    CCLOG("touch began");
    
    // virtual padを取得して表示化する
    SneakyJoystickSkinnedBase* base = (SneakyJoystickSkinnedBase*)this->getChildByTag(kVirtualPadBaseTags);
    SneakyJoystick* joystick = base->getJoystick();
    base->setVisible(true);
    base->setPosition(ccp(pTouch->getLocation().x, pTouch->getLocation().y));
    
    // virtual pad上のタッチ開始処理を呼び出す
    joystick->ccTouchBegan(pTouch, pEvent);
    
    // 選択された光のポジションを合わせる
    CCSprite* selected = (CCSprite*)this->getChildByTag(kVirtualPadSelectedTags);
    selected->setPosition(pTouch->getLocation());
    
    return true;
}

void MapScene::ccTouchMoved(CCTouch *pTouch, CCEvent *pEvent)
{
    CCLOG("touch moved");

    CCSprite* selected = (CCSprite*)this->getChildByTag(kVirtualPadSelectedTags);
    SneakyJoystickSkinnedBase* base = (SneakyJoystickSkinnedBase*)this->getChildByTag(kVirtualPadBaseTags);
    SneakyJoystick* joystick = base->getJoystick();
    joystick->ccTouchMoved(pTouch, pEvent);
    
    CCPoint point = joystick->getVelocity();
    float velocity_list[4] = {
        // Up,   Right,   Down,     Left
        point.y, point.x, -point.y, -point.x
    };
    int max_index = std::distance(velocity_list, std::max_element(velocity_list, velocity_list + 4));
    
    // 前回の移動方向を保持
    MoveDirection tmp = _mover;
    switch (max_index) {
        case 0:
            _mover = kMoveUp;
            selected->setRotation(0.0f);
            selected->setVisible(true);
            CCLOG("move up");
            break;
        case 1:
            _mover = kMoveRight;
            selected->setRotation(90.0f);
            selected->setVisible(true);
            CCLOG("move right");
            break;
        case 2:
            _mover = kMoveDown;
            selected->setRotation(180.0f);
            selected->setVisible(true);
            CCLOG("move down");
            break;
        case 3:
            _mover = kMoveLeft;
            selected->setRotation(270.0f);
            selected->setVisible(true);
            CCLOG("move left");
            break;
        default:
            _mover = kMoveNo;
            selected->setVisible(false);
            break;
    }
    
    // 傾きが小さければ移動判定にしない
    if(fabsf(velocity_list[max_index]) <= 0.8f)
    {
        _mover = kMoveNo;
        selected->setVisible(false);
    }
    
    // 向きが違っていたらアニメーションを変更させる
    if(_mover != tmp)
    {        
        switch (_mover) {
            case kMoveUp:
                this->_changePlayerAnimation("up");
                break;
            case kMoveRight:
                this->_changePlayerAnimation("right");
                break;
            case kMoveDown:
                this->_changePlayerAnimation("down");
                break;
            case kMoveLeft:
                this->_changePlayerAnimation("left");
                break;
            case kMoveNo:
                break;
        }
    }
}

void MapScene::ccTouchEnded(CCTouch *pTouch, CCEvent *pEvent)
{
    CCLOG("touch ended");
    
    _mover = kMoveNo;
    
    // 非表示
    SneakyJoystickSkinnedBase* base = (SneakyJoystickSkinnedBase*)this->getChildByTag(kVirtualPadBaseTags);
    base->setVisible(false);
    
    CCSprite* selected = (CCSprite*)this->getChildByTag(kVirtualPadSelectedTags);
    selected->setVisible(false);
}

void MapScene::_changePlayerAnimation(std::string direction)
{
    // 移動方向に対応したアニメーションを呼び出してプレイヤーにセット
    std::string anime_name = direction + "_anime";
    CCAnimation* animation = CCAnimationCache::sharedAnimationCache()->animationByName(anime_name.c_str());
    CCAnimate* animate = CCAnimate::create(animation);
    animate->setTag(kPlayerAnimateTags);
    
    CCSprite* player = (CCSprite*)this->getChildByTag(kPlayerTags);
    player->stopActionByTag(kPlayerAnimateTags);
    player->runAction(animate);
}

void MapScene::_viewMap()
{
    const CCSize s = CCDirector::sharedDirector()->getWinSize();
    
    // マップを呼び出して表示する
    CCTMXTiledMap* map = CCTMXTiledMap::create("map/testmap.tmx");
    map->setPosition(0, s.height);
    map->setAnchorPoint(ccp(0, 1));
    map->setTag(kMapTags);
    this->addChild(map);
    
    // 衝突判定につかうメタマップは非表示にする
    CCTMXLayer* metamap = map->layerNamed("meta");
    metamap->setVisible(false);
}

bool MapScene::_checkCollidable(int gid_x, int gid_y)
{
    CCTMXTiledMap* map = (CCTMXTiledMap*)this->getChildByTag(kMapTags);
    CCTMXLayer* metamap = map->layerNamed("meta");
    unsigned int tiled_gid = metamap->tileGIDAt(ccp(gid_x, gid_y));
    if(tiled_gid == 0) return false;
    
    CCDictionary* dict = map->propertiesForGID(tiled_gid);
    if(dict == NULL) return false;
    
    const CCString* collision =  dict->valueForKey("Collidable");
    if(strcmp(collision->getCString(), "true") == 0)
    {
        return true;
    }
    return false;
}

bool MapScene::_isEncounterEnemy()
{
    const int p = rand()%20;
    if(p < _walk_count)
    {
        return true; // 歩数/20の確率でエンカウント
    }
    
    return false;
}

void MapScene::_startBattle()
{
    // TODO: フェードアウトフェードイン処理
    auto battle_scene = BattleScene::scene();
    auto transition = CCTransitionTurnOffTiles::create(0.5f, battle_scene);
    CCDirector::sharedDirector()->pushScene(transition);
}