/*
 * File: EnemyAnimation.cpp
 * Author: Morgenthaler S
 *
 * Created on 15 aout 2020, 11h00
 */
#include "EnemyAnimation.h"

EnemyAnimation::EnemyAnimation(const EnemyState& enemy):
    _enemy(enemy) {
}

void EnemyAnimation::updateTrans() {
    const std::array <float, 9> transf = _enemy.transform();

    // we use only translation for now
    const glm::vec3 transVec { transf.at(0), transf.at(1),
                               transf.at(2) };
    const glm::vec3 rotationVec { transf.at(3), transf.at(4),
                                  transf.at(5) };
    const glm::vec3 scaleVec { transf.at(6), transf.at(7),
                               transf.at(8) };
    _translation = glm::translate(transVec);
    _scaleRotation = glm::rotate(rotationVec.x, glm::vec3 { 1.f, 0.f, 0.f })
                     * glm::rotate(rotationVec.y, glm::vec3 { 0.f, 1.f, 0.f })
                     * glm::rotate(rotationVec.z, glm::vec3 { 0.f, 0.f, 1.f })
                     * glm::scale(scaleVec);
    _model = _translation * _scaleRotation;
}
