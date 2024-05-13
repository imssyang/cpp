#ifndef _SEMANTICS_OF_RUNTIME_H_
#define _SEMANTICS_OF_RUNTIME_H_


#include <iostream>

namespace SemanticsOfRuntime {

    namespace Instance {

        class Y {
        public:
            bool operator==(const Y&) const { return true; }
        };

        class X {
        public:
            // type conversion operator (X->Y)
            operator Y() const { return Y(); }
            X getValue() { return *this; }
        };

        void do_xy() {
            X xx;
            Y yy;
            if (yy == xx.getValue());

            // resolution of intended operator
            if (yy.operator==(xx.getValue()));

            // conversion of getValue()'s return value
            if (yy.operator==(xx.getValue().operator Y()));

#ifdef AUTO_TRANSFORM_BY_COMPILER
            // Pseudo C++ code
            // transformation of conditional expression:
            // if ( yy == xx.getValue() ) ...
            {
                X temp1 = xx.getValue();
                Y temp2 = temp1.operator Y();
                int temp3 = yy.operator==(temp2);
                if (temp3) ...
                    temp2.Y::~Y();
                temp1.X::~X();
            }
#endif
        }

    } // namespace Instance

    namespace ObjectConstructionAndDestruction{

    } // namespace ObjectConstructionAndDestruction

    namespace OperatorsNewAndDelete {

    } // namespace OperatorsNewAndDelete

    namespace TemporaryObjects {

    } // namespace TemporaryObjects

} // namespace SemanticsOfRuntime


#endif // _SEMANTICS_OF_RUNTIME_H_

