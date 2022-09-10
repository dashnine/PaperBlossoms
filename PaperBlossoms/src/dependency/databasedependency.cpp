#include "databasedependency.h"

DatabaseDependency::DatabaseDependency(
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
) {
    this->dal = dal;
    this->advantagesRepository = advantagesRepository;
    this->bondsRepository = bondsRepository;
    this->clansRepository = clansRepository;
    this->familiesRepository = familiesRepository;
    this->hertiagesRepository = hertiagesRepository;
    this->inventoryRepository = inventoryRepository;
    this->regionsRepository = regionsRepository;
    this->ringsRepository = ringsRepository;
    this->schoolsRepository = schoolsRepository;
    this->skillsRepository = skillsRepository;
    this->techniquesRepository = techniquesRepository;
    this->titlesRepository = titlesRepository;
    this->upbringingsRepository = upbringingsRepository;
}
