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

#ifndef ENUMS_H
#define ENUMS_H

namespace ItemData{
enum{
    NAME,
    DESCRIPTION,
    SHORT_DESC,
    REFERENCE_BOOK,
    REFERENCE_PAGE,
    PRICE_VALUE,
    PRICE_UNIT,
    RARITY,
    SKILL,
    GRIP,
    RANGE_MIN,
    RANGE_MAX,
    DAMAGE,
    DEADLINESS,
};
}

namespace WeaponData{
enum{
    CATEGORY,
    SKILL,
    GRIP,
    RANGE_MIN,
    RANGE_MAX,
    DAMAGE,
    DEADLINESS
};
}

namespace  ArmorData {
enum{
    RESIST_CATEGORY,
    RESIST_VALUE
};
}

namespace Equipment {
enum{
    TYPE,
    NAME,
    DESC,
    SHORT_DESC,
    BOOK,
    PAGE,
    PRICE,
    UNIT,
    RARITY,
    QUALITIES,
    W_CATEGORY,
    W_SKILL,
    W_GRIP,
    W_MINRANGE,
    W_MAXRANGE,
    W_DAM,
    W_DLS,
    A_PHYSRES,
    A_SUPERRES
};
}

namespace Adv_Disadv {
enum{
    TYPE,
    NAME,
    RING,
    DESC,
    SHORT_DESC,
    BOOK,
    PAGE,
    TYPES
};
}

namespace Tech {
enum{
    NAME,
    TYPE,
    SUBTYPE,
    RANK,
    BOOK,
    PAGE,
    RESTRICTION,
    SHORT_DESC,
    DESCRIPTION
};
}

namespace TechQuery {
enum{
    NAME,
    CATEGORY,
    SUBCATEGORY,
    RANK,
    XP,
    BOOK,
    PAGE,
    RESTRICTION,
};
}

namespace Curric {
enum{
    RANK,
    ADVANCE,
    TYPE,
    SPEC,
    MINRANK,
    MAXRANK
};

}

namespace Title {
enum{
    SOURCE,
    ADVANCE,
    TYPE,
    SPEC,
    TRANK
};

}

namespace Abilities {
enum{
    NAME,
    SOURCE,
    REF_BOOK,
    REF_PAGE,
    DESCRIPTION
};

}
#endif // ENUMS_H
