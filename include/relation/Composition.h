#pragma once

#include "relation/RelationGraph.h"
#include "structure/CategoryRegister.h"
#include "structure/WordRegister.h"



namespace ieml::relation {
    void buildCompositionRelationGraph(RelationGraph&, const ieml::structure::CategoryRegister&, const ieml::structure::WordRegister&);
}