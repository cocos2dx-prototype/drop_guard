//
//  GameObject.hpp
//  DropGuard-desktop
//
//  Created by hoangbui-fe on 8/8/26.
//

#ifndef GameObject_hpp
#define GameObject_hpp

#include "cocos2d.h"

enum class ObjectType
{
    Falling,
    Rising
};

class GameObject : public cocos2d::Node
{
public:

    static GameObject* create(
        ObjectType type,
        const cocos2d::Vec2& position);

    bool init(
        ObjectType type,
        const cocos2d::Vec2& position);

    void updateObject(
        float delta,
        float speed);

    ObjectType getType() const;

    bool isProcessed() const;
    void setProcessed(bool processed);

private:

    GameObject();
    virtual ~GameObject() override = default;

    cocos2d::DrawNode* _visual;

    ObjectType _type;

    bool _processed;
};

#endif /* GameObject_hpp */
