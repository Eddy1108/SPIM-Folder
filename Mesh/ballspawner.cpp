#include "ballspawner.h"
#include "cstdlib"
#include <stdlib.h>
#include <time.h>
#include "renderwindow.h"
#include <random>

BallSpawner::BallSpawner(Shader& shader, VisualObject* surface) : VisualObject(shader),  mSurface{surface}
{
    SpawnBalls(10);
}

void BallSpawner::draw()
{
    for (int i = 0; i < Balls.size(); ++i)
    {
        Balls[i]->move(RenderWindow::mDeltaTime);
        Balls[i]->draw();
    }

    //std::cout << "\n\nNUMBER OF BALLS: " << Balls.size() << std::endl;
}

void BallSpawner::init()
{
}

void BallSpawner::SpawnBalls(int amount)
{
    for (int i = 0; i < amount; i++)
    {
        std::random_device random;
        std::mt19937 gen(random()); //seed the generator
        std::uniform_int_distribution<> distr(-500, 500);

        //Code for random spawn position here
        float randX = distr(gen);

        float randY = distr(gen);

        Balls.push_back(new RollingBall(3, QVector3D(randX, randY, 300), mShader, mSurface));

        //std::cout << "SPAWN POS: " << randX << ", " << randY << std::endl;
    }

    for (int i = 0; i < Balls.size(); i++)
    {
        Balls[i]->init();
    }

    //std::cout << "\n\nBALLS SPAWNED\n\n";
}
