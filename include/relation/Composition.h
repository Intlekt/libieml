#pragma once

#include <memory>
#include <string>
#include <unordered_map>
#include <enum.h>

#include "ParserContextManager.h"
#include "relation/RelationGraph.h"

#include "structure/Path.h"
#include "structure/CategoryRegister.h"



namespace ieml::relation {
    void buildCompositionRelationGraph(RelationGraph&, const ieml::structure::CategoryRegister&, const ieml::structure::WordRegister&);
}