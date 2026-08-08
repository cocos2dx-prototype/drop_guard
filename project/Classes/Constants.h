//
//  Constants.h
//  DropGuard-desktop
//
//  Created by hoangbui-fe on 8/8/26.
//

#ifndef Constants_h
#define Constants_h

#include "cocos2d.h"

namespace DropGuard
{
    constexpr float DESIGN_WIDTH  = 960.0f;
    constexpr float DESIGN_HEIGHT = 640.0f;

    // Background
    const cocos2d::Color4B BACKGROUND_COLOR(7, 15, 38, 255);

    // Bar
    constexpr float BAR_Y = 320.0f;
    constexpr float BAR_HEIGHT = 20.0f;

    // Gap
    constexpr float GAP_WIDTH = 80.0f;
    constexpr float GAP_STEP = 80.0f;
    constexpr float GAP_START_CENTER_X = 480.0f;

    // Objects
    constexpr float OBJECT_RADIUS = 12.0f;

    // Difficulty
    constexpr float START_SPEED = 160.0f;
    constexpr float SPEED_INCREMENT = 20.0f;

    constexpr float START_SPAWN_INTERVAL = 1.2f;
    constexpr float SPAWN_INTERVAL_DECREMENT = 0.05f;
    constexpr float MIN_SPAWN_INTERVAL = 0.4f;

    constexpr float DIFFICULTY_INTERVAL = 10.0f;

    // Gameplay
    constexpr int START_LIVES = 3;

    // HUD
    constexpr float HUD_PADDING = 20.0f;
    constexpr int HUD_FONT_SIZE = 24;

    // Game Over
    constexpr int GAME_OVER_FONT_SIZE = 48;
    constexpr int SCORE_FONT_SIZE = 32;

    // Colors
    const cocos2d::Color4F FALLING_COLOR = cocos2d::Color4F(1.0f, 0.50f, 0.45f, 1.0f);
    const cocos2d::Color4F RISING_COLOR = cocos2d::Color4F(0.40f, 0.75f, 1.0f, 1.0f);
}

#endif /* Constants_h */
