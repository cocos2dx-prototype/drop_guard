//
//  GameObject.cpp
//  DropGuard-desktop
//
//  Created by hoangbui-fe on 8/8/26.
//

#include "GameObject.hpp"
#include "Constants.h"

USING_NS_CC;

GameObject::GameObject()
    : _visual(nullptr)
    , _type(ObjectType::Falling)
    , _processed(false)
{
}

GameObject::~GameObject() {
    
}

GameObject* GameObject::create(
    ObjectType type,
    const Vec2& position)
{
    GameObject* object = new (std::nothrow) GameObject();

    if (object &&
        object->init(type, position))
    {
        object->autorelease();
        return object;
    }

    CC_SAFE_DELETE(object);
    return nullptr;
}

bool GameObject::init(
    ObjectType type,
    const Vec2& position)
{
    if (!Node::init())
    {
        return false;
    }

    _type = type;
    _processed = false;

    setPosition(position);

    _visual = DrawNode::create();
    addChild(_visual, kChildTagDrawNode);

    const Color4F color =
        (_type == ObjectType::Falling)
        ? DropGuard::FALLING_COLOR
        : DropGuard::RISING_COLOR;

    _visual->drawSolidCircle(
        Vec2::ZERO,
        DropGuard::OBJECT_RADIUS,
        0.0f,
        32,
        color);

    return true;
}

void GameObject::updateObject(
    float delta,
    float speed)
{
    Vec2 position = getPosition();

    if (_type == ObjectType::Falling)
    {
        position.y -= speed * delta;
    }
    else
    {
        position.y += speed * delta;
    }

    setPosition(position);
}

ObjectType GameObject::getType() const
{
    return _type;
}

bool GameObject::isProcessed() const
{
    return _processed;
}

void GameObject::setProcessed(
    bool processed)
{
    _processed = processed;
}
