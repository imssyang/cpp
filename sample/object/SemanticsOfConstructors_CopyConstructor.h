#ifndef _SEMANTICS_OF_CONSTRUCTORS__COPY_CONSTRUCTORS_H_
#define _SEMANTICS_OF_CONSTRUCTORS__COPY_CONSTRUCTORS_H_


#include <iostream>

/**
 * The Semantics of Constructors
 */
namespace SemanticsOfConstructors {

    /**
     * copy constructor
     *   There are three program instances in which a class object is initialized with another object of its class.
     *   This may result in the generation of a temporary class object or the actual transformation of program code (or both).
     */
    namespace CopyConstructor {

        class X {
        public:
            // user defined copy constructors may be multi-argument provided
            // each second and subsequent argument is provided with a default value
            X() {}
            X(const X& x) {}
            X(const X& x, int m, int n = 0) {}
        };

        void do_x() {
            X x;
            // explicit initialization of one class object with another
            X xx = x;
        }

        void foo(X x) {}
        void bar() {
            X xx;
            // implicit initialization of foo()'s first argument with xx
            foo(xx);
        }

        X foo_bar() {
            X xx;
            // when a function returns a class object
            return xx;
        }

    } // namespace CopyConstructor


    /**
     * memberwise initialization
     *   Each class object initialized with another object of its class is initialized by what is called default memberwise initialization.
     *   Default memberwise initialization copies the value of each built-in or derived data member (such as a pointer or an array)
     *   from the one class object to another. however, A member class object, is not copied; rather, memberwise initialization is recursively applied.
     *   NOTE:
     *     the Standard distinguishes between a trivial and nontrivial copy constructor.
     *     It is only the nontrivial instance that in practice is synthesized within the program.
     *     Nontrivial in this instance means when the class does not exhibit bitwise copy semantics.
     */
    namespace MemberwiseInitialization {

        class String {
            friend void do_String();
        public:
            String(const char *s) {
                str = const_cast<char*>(s);
                len = strlen(s);
            }
            // ... no explicit copy constructor
        private:
            char *str;
            int len;
        };

        class Word {
            friend void do_Word();
        public:
            Word(const char *s, int n) : _word(s) {
                _occurs = n;
            }
            // ...no explicit copy constructor
        private:
            int _occurs;
            String _word;
        };

        void do_String() {
            String noun("book");
            String verb = noun;

            // semantic equivalent of memberwise initialization
            verb.str = noun.str;
            verb.len = noun.len;
        }

        void do_Word() {
            Word w("book", 3);
            Word v = w;

            // default memberwise initialization of one Word object with another copies the value of its built-in member _occurs
            // and then recursively applies memberwise initialization to its class String member object _word.
            v._occurs = w._occurs;
            v._word = w._word;
        }


    } // namespace MemberwiseInitialization


    /**
     * When are bitwise copy semantics not exhibited by a class? There are four instances:
     *   1. When the class contains a member object of a class for which a copy constructor exists
     *      (either explicitly declared by the class designer, or synthesized by the compiler).
     *   2. When the class is derived from a base class for which a copy constructor exists
     *      (again, either explicitly declared or synthesized).
     *   3. When the class declares one or more virtual functions.
     *   4. When the class is derived from an inheritance chain in which one or more base classes are virtual.
     */
    namespace BitwiseCopySemantics {

        // exhibits bitwise copy semantics
        // default copy constructor need not be synthesized
        class Word {
        public:
            Word(const char*) {}
            ~Word() { delete[] str; }
        private:
            int cnt;
            char *str;
        };

        // declares an explicit copy constructor
        class String {
        public:
            String() {}
            String(const char *) {}
            String(const String&) {}
            ~String() {}
        };

        // not exhibits bitwise copy semantics
        class Word2 {
        public:
            Word2(const String&) {}
            ~Word2() {}
        private:
            int cnt;
            String str;
        };

#ifdef AUTO_TRANSFORM_BY_COMPILER
        // A synthesized copy constructor
        // Pseudo C++ Code
        inline Word2::Word2(const Word2& wd) {
            str.String::String(wd.str);
            // the nonclass members of types such as integers, pointers, and arrays are also copied
            cnt = wd.cnt;
        }
#endif

    } // namespace BitwiseCopySemantics


    /**
     * copy constructor in order to properly initialize the vptr
     *   during compilation whenever a class declares one or more virtual functions:
     *   1. A virtual function table that contains the address of each active virtual function associated with that class (the vtbl) is generated.
     *   2. A pointer to the virtual function table is inserted within each class object (the vptr).
     *   NOTE:
     *     Obviously, things would go terribly wrong if the compiler either failed to initialize or incorrectly
     *     initialized the vptr of each new class object. Hence, once the compiler introduces a vptr into a class,
     *     the affected class no longer exhibits bitwise semantics. Rather, the implementation now needs to
     *     synthesize a copy constructor in order to properly initialize the vptr.
     */
    namespace ResettingVirtualTablePointer {

        class ZooAnimal {
        public:
            ZooAnimal() {}
            virtual ~ZooAnimal() {}
            virtual void animate() const {}
            virtual void draw() const {}
        };

        class Bear : public ZooAnimal {
        public:
            Bear() {}
            void animate() const {}
            void draw() const {}
            virtual void dance() const {}
        };

        void draw(const ZooAnimal& zoey) { zoey.draw(); }
        void foo() {
            // yogi is initialized by the default Bear constructor. 
            // yogi's vptr is initialized to address the Bear class virtual table with code inserted by the compiler.
            Bear yogi;
            // It is safe to simply copy the value of yogi's vptr into winnie's.
            Bear winnie = yogi;

            // franny's vptr must address the ZooAnimal virtual table
            // not the Bear virtual table yogi's vptr addresses
            ZooAnimal franny = yogi;
            draw(yogi);   // invoke Bear::draw()
            draw(franny); // invoke ZooAnimal::draw()
        }

    } // namespace ResettingVirtualTablePointer


    /**
     * virtual base class invalidates bitwise copy semantics
     *   Each implementation's support of virtual inheritance involves the need to make each virtual base
     *   class subobject's location within the derived class object available at runtime. Maintaining the
     *   integrity of this location is the compiler's responsibility. Bitwise copy semantics could result in a
     *   corruption of this location, so the compiler must intercede with its own synthesized copy constructor.
     */
    namespace HandlingVirtualBaseClassSubobject {

        class ZooAnimal {
        public:
            ZooAnimal() {}
        };

        // Compiler-generated code:
        // - invoke ZooAnimal's default constructor
        // - initialize Raccoon's vptr
        // - locate the ZooAnimal subobject within Raccoon is inserted as a prefix within the two Raccoon constructors.
        class Raccoon : public virtual ZooAnimal {
        public:
            Raccoon() {}
            Raccoon(int val) {}
            virtual void draw() const {}
        };

        class RedPanda : public Raccoon {
        public:
            RedPanda() {}
            RedPanda(int val) {}
        };

        // simple bitwise copy is sufficient
        Raccoon rocky;
        Raccoon little_critter = rocky;

        // simple bitwise copy is not sufficient
        // Compiler-generated code:
        // - synthesize a copy constructor
        // - inserting code to initialize little_critter's the virtual base class pointer/offset
        // - performing the necessary memberwise initializations of its members
        // - other memory tasks
        RedPanda little_red;
        Raccoon little_critter2 = little_red;

        // simple bitwise copy may or may not be sufficient
        // NOTE:
        //   Should a compiler, in the presence of an initialization in which bitwise copy semantics hold,
        //   optimize its code generation by suppressing the invocation of the copy constructor?
        //   This is actually a rather contentious issue. At least in the case of a synthesized copy constructor, 
        //   the possibility of program side effects is nil and the optimization would seem to make good sense.
        Raccoon *ptr;
        Raccoon little_critter3 = *ptr;

    } // namespace HandlingVirtualBaseClassSubobject

} // namespace SemanticsOfConstructors


#endif // _SEMANTICS_OF_CONSTRUCTORS__COPY_CONSTRUCTORS_H_

