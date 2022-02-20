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
        /**
         * Make an entry.
         * @param name the entry's name.
         */
        SymtabEntry *enter(string name)
        {
            SymtabEntry *entry = new SymtabEntry(name);
            contents[name] = entry;

            return entry;
        }

        /**
         * Look up an entry.
         * @param name the entry's name.
         * @return the entry or null if it's not in the symbol table.
         */
        SymtabEntry *lookup(string name)
        {
            return contents.find(name) != contents.end() ? contents[name]
                                                         : nullptr;
        }
    };

}  // namespace intermediate
#endif //PASCALPROJECT_SYMTAB_H
