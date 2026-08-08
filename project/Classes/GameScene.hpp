//
//  GameScene.hpp
//  DropGuard-desktop
//
//  Created by hoangbui-fe on 8/8/26.
//

#ifndef GameScene_hpp
#define GameScene_hpp

#include "cocos2d.h"
#include "GameObject.hpp"

class GameScene : public cocos2d::Scene {
public:
    GameScene();
    ~GameScene();
    
public:
    enum ChildTag {
        None = 0,
        kChildTagBackground,
        kChildTagBar,
        kChildTagFallingObject,
        kChildTagRaisingObject,
        kChildTagHUD,
        kChildTagLblGameOver,
        kChildTagLblFinalScore,
        kChildTagLblBestScore,
    };
    
public:
    static cocos2d::Scene* createScene();

private:
    CREATE_FUNC(GameScene);
    virtual bool init() override;
    virtual void onEnter() override;
    virtual void onExit() override;

private:
    cocos2d::DrawNode* _barNode;

    cocos2d::Label* _livesLabel;
    cocos2d::Label* _scoreLabel;

    cocos2d::Label* _gameOverLabel;
    cocos2d::Label* _finalScoreLabel;
    cocos2d::Label* _bestScoreLabel;

    cocos2d::Vector<GameObject*> _objects;

    int _lives;
    int _score;
    int _bestScore;

    bool _gameOver;

    float _gapCenterX;

    float _currentSpeed;
    float _currentSpawnInterval;

    float _survivalTime;

    float _fallSpawnTimer;
    float _riseSpawnTimer;
    
private:
    void _initValues();
    void _initObjects();
    
    void updateGame(float delta);

    void setupBackground();
    void setupBar();
    void setupHud();
    void setupGameOverLabels();
    void setupInput();

    void redrawBar();

    void spawnFallingObject();
    void spawnRisingObject();

    void updateDifficulty(float delta);
    void updateSpawning(float delta);
    void updateObjects(float delta);

    void processCrossing(GameObject* object);

    void addScore(int amount);
    void loseLife();

    void triggerGameOver();
    void restartGame();

    float randomX() const;
};

#endif /* GameScene_hpp */
