#include "dependencybuilder.h"
#include "databasedependency.h"
#include "../repository/advantagesrepository.h"
#include "../repository/bondsrepository.h"
#include "../repository/clansrepository.h"
#include "../repository/dataaccesslayer.h"
#include "../repository/familiesrepository.h"
#include "../repository/hertiagesrepository.h"
#include "../repository/inventoryrepository.h"
#include "../repository/regionsrepository.h"
#include "../repository/ringsrepository.h"
#include "../repository/schoolsrepository.h"
#include "../repository/skillsrepository.h"
#include "../repository/techniquesrepository.h"
#include "../repository/titlesrepository.h"
#include "../repository/upbringingsrepository.h"

DependencyBuilder::DependencyBuilder() {

}

DatabaseDependency* DependencyBuilder::build(QString locale) {
    DataAccessLayer *dal = new DataAccessLayer(locale);

    return new DatabaseDependency(
            dal,
            new AdvantagesRepository(dal),
            new BondsRepository(dal),
            new ClansRepository(dal),
            new FamiliesRepository(dal),
            new HertiagesRepository(dal),
            new InventoryRepository(dal),
            new RegionsRepository(dal),
            new RingsRepository(dal),
            new SchoolsRepository(dal),
            new SkillsRepository(dal),
            new TechniquesRepository(dal),
            new TitlesRepository(dal),
            new UpbringingsRepository(dal)
    );
}
