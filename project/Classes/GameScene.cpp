//
//  GameScene.cpp
//  DropGuard-desktop
//
//  Created by hoangbui-fe on 8/8/26.
//

#include "GameScene.hpp"
#include "Constants.h"

USING_NS_CC;

Scene* GameScene::createScene()
{
    return GameScene::create();
}

bool GameScene::init()
{
    if (!Scene::init())
    {
        return false;
    }

    _barNode = nullptr;

    _livesLabel = nullptr;
    _scoreLabel = nullptr;

    _gameOverLabel = nullptr;
    _finalScoreLabel = nullptr;
    _bestScoreLabel = nullptr;

    _lives = DropGuard::START_LIVES;
    _score = 0;
    _bestScore = 0;

    _gameOver = false;

    _gapCenterX = DropGuard::GAP_START_CENTER_X;

    _currentSpeed = DropGuard::START_SPEED;
    _currentSpawnInterval = DropGuard::START_SPAWN_INTERVAL;

    _survivalTime = 0.0f;

    _fallSpawnTimer = 0.0f;
    _riseSpawnTimer = 0.0f;

    setupBackground();
    setupBar();
    setupHud();
    setupGameOverLabels();
    setupInput();

    schedule(CC_SCHEDULE_SELECTOR(GameScene::updateGame));

    return true;
}

void GameScene::setupBackground()
{
    auto background = LayerColor::create(
        Color4B(
            static_cast<GLubyte>(DropGuard::BACKGROUND_R * 255),
            static_cast<GLubyte>(DropGuard::BACKGROUND_G * 255),
            static_cast<GLubyte>(DropGuard::BACKGROUND_B * 255),
            255));

    addChild(background);
}

void GameScene::setupBar()
{
    _barNode = DrawNode::create();
    addChild(_barNode);

    redrawBar();
}

void GameScene::setupHud()
{
    const Size size =
        Director::getInstance()->getVisibleSize();

    _livesLabel = Label::createWithTTF(
        "Lives: 3",
        "fonts/arial.ttf",
        DropGuard::HUD_FONT_SIZE);

    _livesLabel->setAnchorPoint(Vec2(0.0f, 1.0f));
    _livesLabel->setPosition(
        DropGuard::HUD_PADDING,
        size.height - DropGuard::HUD_PADDING);

    addChild(_livesLabel);

    _scoreLabel = Label::createWithTTF(
        "Score: 0",
        "fonts/arial.ttf",
        DropGuard::HUD_FONT_SIZE);

    _scoreLabel->setAnchorPoint(Vec2(1.0f, 1.0f));
    _scoreLabel->setPosition(
        size.width - DropGuard::HUD_PADDING,
        size.height - DropGuard::HUD_PADDING);

    addChild(_scoreLabel);
}

void GameScene::setupGameOverLabels()
{
    const Size size =
        Director::getInstance()->getVisibleSize();

    _gameOverLabel = Label::createWithTTF(
        "GAME OVER",
        "fonts/arial.ttf",
        48);

    _gameOverLabel->setPosition(
        size.width * 0.5f,
        size.height * 0.60f);

    addChild(_gameOverLabel);

    _finalScoreLabel = Label::createWithTTF(
        "",
        "fonts/arial.ttf",
        32);

    _finalScoreLabel->setPosition(
        size.width * 0.5f,
        size.height * 0.50f);

    addChild(_finalScoreLabel);

    _bestScoreLabel = Label::createWithTTF(
        "",
        "fonts/arial.ttf",
        32);

    _bestScoreLabel->setPosition(
        size.width * 0.5f,
        size.height * 0.42f);

    addChild(_bestScoreLabel);

    _gameOverLabel->setVisible(false);
    _finalScoreLabel->setVisible(false);
    _bestScoreLabel->setVisible(false);
}

void GameScene::setupInput()
{
    auto listener =
        EventListenerTouchOneByOne::create();

    listener->onTouchBegan =
        [this](Touch* touch, Event*)
        {
            if (_gameOver)
            {
                restartGame();
                return true;
            }

            const float x =
                touch->getLocation().x;

            if (x < DropGuard::DESIGN_WIDTH * 0.5f)
            {
                _gapCenterX -= DropGuard::GAP_STEP;
            }
            else
            {
                _gapCenterX += DropGuard::GAP_STEP;
            }

            _gapCenterX = clampf(
                _gapCenterX,
                DropGuard::GAP_WIDTH * 0.5f,
                DropGuard::DESIGN_WIDTH -
                DropGuard::GAP_WIDTH * 0.5f);

            redrawBar();

            return true;
        };

    _eventDispatcher->addEventListenerWithSceneGraphPriority(
        listener,
        this);
}

void GameScene::redrawBar()
{
    _barNode->clear();

    const float gapLeft =
        _gapCenterX -
        DropGuard::GAP_WIDTH * 0.5f;

    const float gapRight =
        _gapCenterX +
        DropGuard::GAP_WIDTH * 0.5f;

    const float yMin =
        DropGuard::BAR_Y -
        DropGuard::BAR_HEIGHT * 0.5f;

    const float yMax =
        DropGuard::BAR_Y +
        DropGuard::BAR_HEIGHT * 0.5f;

    _barNode->drawSolidRect(
        Vec2(0.0f, yMin),
        Vec2(gapLeft, yMax),
        Color4F::WHITE);

    _barNode->drawSolidRect(
        Vec2(gapRight, yMin),
        Vec2(DropGuard::DESIGN_WIDTH, yMax),
        Color4F::WHITE);
}

float GameScene::randomX() const
{
    return RandomHelper::random_real(
        DropGuard::OBJECT_RADIUS,
        DropGuard::DESIGN_WIDTH -
        DropGuard::OBJECT_RADIUS);
}

void GameScene::spawnFallingObject()
{
    auto object =
        GameObject::create(
            ObjectType::Falling,
            Vec2(
                randomX(),
                DropGuard::DESIGN_HEIGHT +
                DropGuard::OBJECT_RADIUS));

    addChild(object);

    _objects.pushBack(object);
}

void GameScene::spawnRisingObject()
{
    auto object =
        GameObject::create(
            ObjectType::Rising,
            Vec2(
                randomX(),
                -DropGuard::OBJECT_RADIUS));

    addChild(object);

    _objects.pushBack(object);
}

void GameScene::updateDifficulty(float delta)
{
    _survivalTime += delta;

    const int tier =
        static_cast<int>(
            _survivalTime /
            DropGuard::DIFFICULTY_INTERVAL);

    _currentSpeed =
        DropGuard::START_SPEED +
        tier *
        DropGuard::SPEED_INCREMENT;

    _currentSpawnInterval =
        std::max(
            DropGuard::MIN_SPAWN_INTERVAL,
            DropGuard::START_SPAWN_INTERVAL -
            tier *
            DropGuard::SPAWN_INTERVAL_DECREMENT);
}

void GameScene::updateSpawning(float delta)
{
    _fallSpawnTimer += delta;
    _riseSpawnTimer += delta;

    if (_fallSpawnTimer >= _currentSpawnInterval)
    {
        _fallSpawnTimer = 0.0f;
        spawnFallingObject();
    }

    if (_riseSpawnTimer >= _currentSpawnInterval)
    {
        _riseSpawnTimer = 0.0f;
        spawnRisingObject();
    }
}

void GameScene::updateObjects(float delta)
{
    Vector<GameObject*> removeList;

    for (auto object : _objects)
    {
        const float previousY =
            object->getPositionY();

        object->updateObject(
            delta,
            _currentSpeed);

        const float currentY =
            object->getPositionY();

        bool crossed = false;

        if (object->getType() ==
            ObjectType::Falling)
        {
            crossed =
                previousY > DropGuard::BAR_Y &&
                currentY <= DropGuard::BAR_Y;
        }
        else
        {
            crossed =
                previousY < DropGuard::BAR_Y &&
                currentY >= DropGuard::BAR_Y;
        }

        if (crossed)
        {
            processCrossing(object);
            removeList.pushBack(object);
        }
    }

    for (auto object : removeList)
    {
        _objects.eraseObject(object);
        object->removeFromParent();
    }
}

void GameScene::processCrossing(
    GameObject* object)
{
    const float gapLeft =
        _gapCenterX -
        DropGuard::GAP_WIDTH * 0.5f;

    const float gapRight =
        _gapCenterX +
        DropGuard::GAP_WIDTH * 0.5f;

    const float x =
        object->getPositionX();

    const bool insideGap =
        x >= gapLeft &&
        x <= gapRight;

    if (object->getType() ==
        ObjectType::Falling)
    {
        if (insideGap)
        {
            loseLife();
        }
        else
        {
            addScore(1);
        }
    }
    else
    {
        if (insideGap)
        {
            addScore(1);
        }
        else
        {
            loseLife();
        }
    }
}

void GameScene::addScore(int amount)
{
    _score += amount;

    _scoreLabel->setString(
        StringUtils::format(
            "Score: %d",
            _score));
}

void GameScene::loseLife()
{
    --_lives;

    _livesLabel->setString(
        StringUtils::format(
            "Lives: %d",
            _lives));

    if (_lives <= 0)
    {
        triggerGameOver();
    }
}

void GameScene::triggerGameOver()
{
    _gameOver = true;

    _bestScore =
        std::max(
            _bestScore,
            _score);

    _gameOverLabel->setVisible(true);

    _finalScoreLabel->setString(
        StringUtils::format(
            "Score: %d",
            _score));

    _bestScoreLabel->setString(
        StringUtils::format(
            "Best: %d",
            _bestScore));

    _finalScoreLabel->setVisible(true);
    _bestScoreLabel->setVisible(true);
}

void GameScene::restartGame()
{
    for (auto object : _objects)
    {
        object->removeFromParent();
    }

    _objects.clear();

    _lives = DropGuard::START_LIVES;
    _score = 0;

    _gameOver = false;

    _gapCenterX =
        DropGuard::GAP_START_CENTER_X;

    _currentSpeed =
        DropGuard::START_SPEED;

    _currentSpawnInterval =
        DropGuard::START_SPAWN_INTERVAL;

    _survivalTime = 0.0f;

    _fallSpawnTimer = 0.0f;
    _riseSpawnTimer = 0.0f;

    _livesLabel->setString("Lives: 3");
    _scoreLabel->setString("Score: 0");

    redrawBar();

    _gameOverLabel->setVisible(false);
    _finalScoreLabel->setVisible(false);
    _bestScoreLabel->setVisible(false);
}

void GameScene::updateGame(float delta)
{
    if (_gameOver)
    {
        return;
    }

    updateDifficulty(delta);
    updateSpawning(delta);
    updateObjects(delta);
}
