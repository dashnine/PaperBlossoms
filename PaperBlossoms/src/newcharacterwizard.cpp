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

#include "newcharacterwizard.h"
#include "newcharwizardpage1.h"
#include "newcharwizardpage2.h"
#include "newcharwizardpage3.h"
#include "newcharwizardpage4.h"
#include "newcharwizardpage5.h"
#include "newcharwizardpage6.h"
#include "newcharwizardpage7.h"
#include "character.h"
#include <QLabel>
#include <QVBoxLayout>
#include "dataaccesslayer.h"


NewCharacterWizard::NewCharacterWizard(DataAccessLayer *dal, QWizard *parent) : QWizard(parent)
{
    this->addPage(new NewCharWizardPage1(dal));
    this->addPage(new NewCharWizardPage2(dal));
    this->addPage(new NewCharWizardPage3(dal));
    this->addPage(new NewCharWizardPage4(dal));
    this->addPage(new NewCharWizardPage5(dal));
    this->addPage(new NewCharWizardPage6(dal));
    this->addPage(new NewCharWizardPage7(dal, &character)); //pass in a character to set values

    this->setWindowTitle("Twenty Questions");
}

Character NewCharacterWizard::getCharacter(){
    return character;
}
