#ifndef DATABASEDEPENDENCY_H
#define DATABASEDEPENDENCY_H

#include "../repository/advantagesrepository.h"
#include "../repository/bondsrepository.h"
#include "../repository/clansrepository.h"
#include "../repository/dataaccesslayer.h"
#include "../repository/familiesRepository.h"
#include "../repository/hertiagesrepository.h"
#include "../repository/inventoryrepository.h"
#include "../repository/regionsrepository.h"
#include "../repository/ringsrepository.h"
#include "../repository/schoolsrepository.h"
#include "../repository/skillsrepository.h"
#include "../repository/techniquesrepository.h"
#include "../repository/titlesrepository.h"
#include "../repository/upbringingsrepository.h"

class DatabaseDependency {
public:
    DatabaseDependency(
            DataAccessLayer *dal,
            AdvantagesRepository *advantagesRepository,
            BondsRepository *bondsRepository,
            ClansRepository *clansRepository,
            FamiliesRepository *familiesRepository,
            HertiagesRepository *hertiagesRepository,
            InventoryRepository *inventoryRepository,
            RegionsRepository *regionsRepository,
            RingsRepository *ringsRepository,
            SchoolsRepository *schoolsRepository,
            SkillsRepository *skillsRepository,
            TechniquesRepository *techniquesRepository,
            TitlesRepository *titlesRepository,
            UpbringingsRepository *upbringingsRepository
    );

    DataAccessLayer *dal;
    AdvantagesRepository *advantagesRepository;
    BondsRepository *bondsRepository;
    ClansRepository *clansRepository;
    FamiliesRepository *familiesRepository;
    HertiagesRepository *hertiagesRepository;
    InventoryRepository *inventoryRepository;
    RegionsRepository *regionsRepository;
    RingsRepository *ringsRepository;
    SchoolsRepository *schoolsRepository;
    SkillsRepository *skillsRepository;
    TechniquesRepository *techniquesRepository;
    TitlesRepository *titlesRepository;
    UpbringingsRepository *upbringingsRepository;
};

#endif // DATABASEDEPENDENCY_H
