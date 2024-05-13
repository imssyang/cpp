#ifndef _SEMANTICS_OF_CONSTRUCTORS__PROGRAM_TRANSFORMATION_H_
#define _SEMANTICS_OF_CONSTRUCTORS__PROGRAM_TRANSFORMATION_H_


#include <iostream>


/**
 * The Semantics of Constructors
 */
namespace SemanticsOfConstructors {

    /**
     * Explicit Initialization
     */
    namespace ExplicitInitialization {

        class X {
        public:
            X() {}
            X(const X& x) {}
        };

        void foo_bar() {
            X x0;

            //three definitions each explicitly initialize its class object with x0
            X x1(x0);
            X x2 = x0;
            X x3 = X(x0);
        }

#ifdef AUTO_TRANSFORM_BY_COMPILER
        // Possible program transformation
        // Pseudo C++ Code
        void foo_bar() {
            //1. Each definition is rewritten by compiler with the initialization stripped out
            X x1;
            X x2;
            X x3;

            // 2. An invocation of the class copy constructor is inserted by compiler.
            x1.X::X(x0);
            x2.X::X(x0);
            x3.X::X(x0);
        }
#endif

    } // namespace ExplicitInitialization


    /**
     * Argument/Implicit Initialization
     *   The Standard states (Section 8.5) that passing a class object as an argument to a function
     *   is equivalent to the following form of initialization:
     *       X xx = arg;
     *   where xx represents the formal argument and arg represents the actual argument.
     */
    namespace ArgumentInitialization {

        class X {
        public:
            X() {}
            X(const X& x) {}
        };

        void foo(X x0) {}
        void do_foo() {
            X xx;
            foo(xx);
        }

#ifdef AUTO_TRANSFORM_BY_COMPILER
        // The declaration of foo() must also be transformed, 
        // with the formal argument from an object changed to a reference of class X
        void foo(X& x0) {}
        void do_foo() {
            X xx;

            // Pseudo C++ code
            // compiler generated temporary
            X __temp0;
            // compiler invocation of copy constructor
            __temp0.X::X(xx);
            // rewrite function call to take temporary
            foo(__temp0);
        } // destructor would be invoked on the temporary object following the call of foo()
#endif

    } // namespace ArgumentInitialization


    /**
     * Return Value Initialization
     */
    namespace ReturnValueInitialization {

        class X {
        public:
            X() {}
            X(const X& x) {}
            void memfunc() {}
        };

        X bar() {
            X xx;
            return xx;
        }
        void do_bar() {
            X xx = bar();

            // an invocation
            bar().memfunc();

            // declare a pointer to a function
            X(*pf)();
            pf = bar;
        }

#ifdef AUTO_TRANSFORM_BY_COMPILER
        // function transformation to reflect application of copy constructor
        // Pseudo C++ Code
        void bar(X& __result) {
            X xx;
            // compiler generated invocation of default constructor
            xx.X::X();
            // compiler generated invocation of copy constructor
            __result.X::X(xx);
            // rewrites the function to have it not return a value.
            return;
        }
        void do_bar() {
            // note: no default constructor applied
            X xx;
            bar(xx);

            // compiler generated temporary
            X __temp0;
            (bar(__temp0), __temp0).memfunc();

            void(*pf)(X&);
            pf = bar;
        }
#endif

    } // namespace ReturnValueInitialization


    /**
     * Optimization at the User Level
     */
    namespace OptimizationAtUserLevel {

        class X {
        public:
            X() {}
            X(const X& x) {}
        };

        // requires xx to be memberwise copied into the compiler-generated __result
        template <typename T>
        X bar(const T &y, const T &z) {
            X xx;
            return xx;
        }

        // This definition of bar(), when transformed, is more efficient
        template <typename T>
        X bar2(const T &y, const T &z) {
            return X(y, z);
        }

#ifdef AUTO_TRANSFORM_BY_COMPILER
        // Pseudo C++ Code
        void bar(X& __result) {
            X xx;
            // compiler generated invocation of default constructor
            xx.X::X();
            // compiler generated invocation of copy constructor
            __result.X::X(xx);
            // rewrites the function to have it not return a value.
            return;
        }

        // Pseudo C++ Code
        void bar2(X &__result) {
            __result.X::X(y, z);
            return;
        }
#endif

    } // namespace OptimizationAtUserLevel


    /**
     * Optimization at the Compiler Level
     *   This compiler optimization, sometimes referred to as the Named Return Value (NRV) optimization.
     *   The NRV optimization is now considered an obligatory Standard C++ compiler optimization,
     *   although that requirement, of course, falls outside the formal Standard.
     *   NOTE:
     *     Although the NRV optimization provides significant performance improvement, there are several
     *     criticisms of this approach:
     *     1. One is that because the optimization is done silently by the compiler, whether it was actually
     *        performed is not always clear;
     *     2. A second is that as the function becomes more complicated, the optimization becomes more difficult to apply.
     *     3. Some programmers actually criticize the application of the optimization. In this case,
     *        the symmetry is broken by the optimization, and the program, albeit faster, fails.
     */
    namespace OptimizationAtCompilerLevel {

        class test {
            friend test foo(double);
        public:
            test() { memset(array, 0, 100 * sizeof(double)); }
            // The presence of the copy constructor "turns on" the NRV optimization within the C++ compiler.
            test(const test &t) { memcpy(this, &t, sizeof(test)); }
        private:
            double array[100];
        };

        test foo(double val) {
            test local;
            local.array[0] = val;
            local.array[99] = val;
            return local;
        }

        void do_foo() {
            for (int cnt = 0; cnt < 10000000; cnt++)
                test t = foo(double(cnt));
        }

    } // namespace OptimizationAtCompilerLevel


    /**
     * The Copy Constructor: To Have or To Have Not?
     *
     */
    namespace IsShouldHaveCopyConstructor {

        // Should the class designer provide an explicit copy constructor?
        // The obvious answer, of course, is no. The three coordinate members are stored by value.
        // Bitwise copy results in neither a memory leak nor address aliasing. Thus it is both safe and efficient.
        class Point3d {
        public:
            Point3d(float x, float y, float z);
        private:
            float _x, _y, _z;
        };

#ifdef MAYBE_CODE_BY_USER
        // The simplest method of implementing the copy constructor :
        Point3d::Point3d(const Point3d &rhs) {
            _x = rhs._x;
            _y = rhs._y;
            _z = rhs._z;
        };

        // use of the C library memcpy() function would be more efficient
        // Use of both memcpy() and memset(), however, works only if the classes do not contain any compiler - generated internal members.
        Point3d::Point3d(const Point3d &rhs) {
            memcpy(this, &rhs, sizeof(Point3d);
        };
#endif

        class Shape {
        public:
            // oops: this will overwrite internal vptr!
            Shape() { memset(this, 0, sizeof(Shape)); }
            virtual ~Shape() {}
        };

#ifdef AUTO_TRANSFORM_BY_COMPILER
        // Expansion of constructor
        // Pseudo C++ Code
        Shape::Shape() {
            // vptr must be set before user code executes
            __vptr__Shape = __vtbl__Shape;
            // oops: memset zeros out value of vptr
            memset(this, 0, sizeof(Shape));
        };
#endif

    } // namespace IsShouldHaveCopyConstructor

} // namespace SemanticsOfConstructors


#endif // _SEMANTICS_OF_CONSTRUCTORS__PROGRAM_TRANSFORMATION_H_

