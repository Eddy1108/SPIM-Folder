#ifndef BALLSPAWNER_H
#define BALLSPAWNER_H

#include "Core/VisualObject.h"
#include "Mesh/rollingball.h"
#include <vector>


class BallSpawner : public VisualObject
{
public:
    BallSpawner(Shader& shader, VisualObject* surface);

    void draw() override;
    void init() override;

    void SpawnBalls(int amount);

    std::vector<RollingBall*> Balls;
    VisualObject* mSurface{ nullptr };

};

#endif // BALLSPAWNER_H
