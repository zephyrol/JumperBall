//
//  EnemyMesh.h
//  JumperBallGraphic
//
//  Created by Sebastien Morgenthaler on 11/10/2020.
//

#ifndef EnemyMesh_h
#define EnemyMesh_h
#include "../GraphicEnemy.h"
#include "Mesh.h"

class EnemyMesh: public Mesh<Enemy,GraphicEnemy>
{
public:
    EnemyMesh(const Enemy& enemy);

private:
    virtual void updateWorld() override;
};

#endif /* EnemyMesh_h */
