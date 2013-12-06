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
     
    return true;
}