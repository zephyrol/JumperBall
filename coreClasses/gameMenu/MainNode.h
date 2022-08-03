//
// Created by S.Morgenthaler on 03/08/22.
//

#ifndef JUMPERBALLAPPLICATION_MAINNODE_H
#define JUMPERBALLAPPLICATION_MAINNODE_H
#include "FillingNode.h"

class MainNode: public Node {
public:

    /**
     * @param ratio Main node ratio is the page ratio
     */
    MainNode(float ratio);
    virtual ~MainNode() = default;

private:
    std::unique_ptr<Transform> computeScreenTransform() const override;

};




#endif //JUMPERBALLAPPLICATION_MAINNODE_H
