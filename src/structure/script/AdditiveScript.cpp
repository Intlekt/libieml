#include "structure/script/AdditiveScript.h"
#include "structure/script/PrimitiveScript.h"


using namespace ieml::structure;

const std::vector<std::shared_ptr<PrimitiveScript>> O_REMARKABLE_ADDITION {PRIMITIVE('U'), 
                                                                           PRIMITIVE('A')};

const std::vector<std::shared_ptr<PrimitiveScript>> M_REMARKABLE_ADDITION {PRIMITIVE('S'), 
                                                                           PRIMITIVE('B'), 
                                                                           PRIMITIVE('T')};

const std::vector<std::shared_ptr<PrimitiveScript>> F_REMARKABLE_ADDITION {PRIMITIVE('U'), 
                                                                           PRIMITIVE('A'),
                                                                           PRIMITIVE('S'), 
                                                                           PRIMITIVE('B'), 
                                                                           PRIMITIVE('T')};

const std::vector<std::shared_ptr<PrimitiveScript>> I_REMARKABLE_ADDITION {PRIMITIVE('E'),
                                                                           PRIMITIVE('U'), 
                                                                           PRIMITIVE('A'),
                                                                           PRIMITIVE('S'), 
                                                                           PRIMITIVE('B'), 
                                                                           PRIMITIVE('T')};


const std::unordered_map<char, std::shared_ptr<AdditiveScript>> AdditiveScript::REMARKABLE_ADDITIONS = {
    {'O', std::make_shared<AdditiveScript>(O_REMARKABLE_ADDITION)},
    {'M', std::make_shared<AdditiveScript>(M_REMARKABLE_ADDITION)},
    {'F', std::make_shared<AdditiveScript>(F_REMARKABLE_ADDITION)},
    {'I', std::make_shared<AdditiveScript>(I_REMARKABLE_ADDITION)}
};
