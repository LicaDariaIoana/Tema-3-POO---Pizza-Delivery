#ifndef FACTORY_H_INCLUDED
#define FACTORY_H_INCLUDED
#include <memory>
#include "produse.h"
#include "pizza.h"
#include "bauturi.h"
#include "desert.h"
#include "exceptii.h"
class ProdusFactory {
public:
    static std::shared_ptr<produse> creeazaProdus(int tip) {
        switch (tip) {
            case 1: return std::make_shared<pizza>();
            case 2: return std::make_shared<bauturi>();
            case 3: return std::make_shared<desert>();
            default: throw intInvalid();
        }
    }
};

#endif
