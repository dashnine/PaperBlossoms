/*
 * *******************************************************************
 * This file is part of the Paper Blossoms application
 * (https://github.com/dashnine/PaperBlossoms).
 * Copyright (c) 2019 Kyle Hankins (dashnine)
 *
 * This program is free software: you can redistribute it and/or modify
 * it under the terms of the GNU General Public License as published by
 * the Free Software Foundation, version 3.
 *
 * This program is distributed in the hope that it will be useful, but
 * WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE. See the GNU
 * General Public License for more details.
 *
 * You should have received a copy of the GNU General Public License
 * along with this program. If not, see <http://www.gnu.org/licenses/>.
 *
 * The Legend of the Five Rings Roleplaying Game is the creation
 * and property of Fantasy Flight Games.
 * *******************************************************************
 */

#include "character.h"
#include <QStandardItemModel>


Character::Character()
{
    clear();
}

Character::~Character(){
}

void Character::clear(){
    name      ="" ;
    titles.clear();
    clan      ="";
    family    ="";
    school    ="";
    ninjo     ="";
    giri      ="";
    baseskills.clear();
    skillranks.clear();
    baserings.clear();
    ringranks.clear();
    honor     =0;
    glory     =0;
    status    =0;
    koku      =0;
    bu        =0;
    zeni      =0;
    rank      =0;
    techniques.clear();
    adv_disadv.clear();
    equipment.clear();
    abilities.clear();
    heritage  ="";
    notes     ="";
    advanceStack.clear();
    portrait.load("");
    portrait = portrait.scaled(0,0);
    totalXP = 0;

}
