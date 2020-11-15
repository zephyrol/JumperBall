/*
 * File:   LabelRendering.cpp
 * Author: Morgenthaler S
 *
 * Created on 6 septembre 2020, 17:15
 */

#include "LabelRendering.h"

LabelRendering::LabelRendering(const Label &label):
    _label(label)
{
}

const glm::mat4 LabelRendering::biasMatrix  = glm::mat4{ 1.f, 0.f,  0.f, 0.f,
                                             0.f,  1.f, 0.f, 0.f,
                                             0.f,  0.f,  1.f, 0.f,
                                             -1.f, -1.f, 0.f, 1.f} ;

