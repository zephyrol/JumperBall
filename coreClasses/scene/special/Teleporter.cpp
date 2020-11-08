/*
 * To change this license header, choose License Headers in Project Properties.
 * To change this template file, choose Tools | Templates
 * and open the template in the editor.
 */

/* 
 * File:   Teleporter.cpp
 * Author: Morgenthaler S
 * 
 * Created on 7 novembre 2020, 10:42
 */
#include "Teleporter.h"

 Teleporter::Teleporter(const JBTypes::Color &color,
         const Block &tieBlock,
         const JBTypes::Dir &dir)
         : Special(color,tieBlock,dir)
 {
 }
 
 Special::SpecialEffect Teleporter::interaction() const 
 {
     return SpecialEffect::Teleportation;
 }
 