#ifndef DEPENDENCYBUILDER_H
#define DEPENDENCYBUILDER_H

#include "databasedependency.h"

class DependencyBuilder
{
public:
    DependencyBuilder();

    DatabaseDependency* build(QString locale);
};

#endif // DEPENDENCYBUILDER_H
