//
// Created by Arman Sadeghi on 2/16/22.
//

#ifndef PASCALPROJECT_SYMTABENTRY_H
#define PASCALPROJECT_SYMTABENTRY_H

#include <string>

namespace intermediate {

    using namespace std;

    class SymtabEntry
    {
    private:
        string name;
        double value;

    public:
        SymtabEntry(string name) : name(name), value(0.0) {}

        string getName()  const { return name;  }
        double getValue() const { return value; }

        void setValue(const double value) { this->value = value; }
    };

}  // namespace intermediate
#endif //PASCALPROJECT_SYMTABENTRY_H
