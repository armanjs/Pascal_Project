//
// Created by Arman Sadeghi on 2/16/22.
//

#ifndef PASCALPROJECT_SYMTAB_H
#define PASCALPROJECT_SYMTAB_H

#include <string>
#include <map>

#include "SymtabEntry.h"

namespace intermediate {

    using namespace std;

    class Symtab
    {
    private:
        map<string, SymtabEntry *> contents;

    public:
        SymtabEntry *enter(string name)
        {
            SymtabEntry *entry = new SymtabEntry(name);
            contents[name] = entry;

            return entry;
        }

        SymtabEntry *lookup(string name)
        {
            return contents.find(name) != contents.end() ? contents[name]
                                                         : nullptr;
        }
    };

}  // namespace intermediate
#endif //PASCALPROJECT_SYMTAB_H
