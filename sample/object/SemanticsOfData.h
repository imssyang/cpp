#ifndef _SEMANTICS_OF_DATA_H_
#define _SEMANTICS_OF_DATA_H_


#include <cassert>
#include <iomanip>
#include <iostream>
#include <list>


/**
 * The Semantics of Data
 */
namespace SemanticsOfData {

    // The given size of both class Y and class Z on any machine is the interplay of three factors :
    //   1. Language support overhead. There is an associated overhead incurred in the language support of virtual base classes.
    //      Within the derived class, this overhead is reflected as some form of pointer, either to the virtual base class subobject
    //      or to an associated table within which either the address or offset to the virtual base class subobject is stored.
    //   2. Compiler optimization of recognized special cases. There is the 1 byte size of the virtual base class X subobject also present within Y(and Z).
    //      Traditionally, this is placed at the end of the "fixed" portion of the derived class.
    //      Some compilers now provide special support for an empty virtual base class. 
    //   3. Alignment constraints. On most machines, aggregate structures have an alignment constraint so that they can be efficiently loaded from and stored to memory.
    //      On my correspondent's machine, alignment of an aggregate is on a 4-byte boundary. So class Y(and Z) requires 3 bytes of padding.
    class X {};
    class Y : public virtual X {};
    class Z : public virtual X {};
    class A : public Y, public Z {};

    void print() {
        std::cout << "sizeof(X)=" << sizeof(X)
            << " sizeof(Y)=" << sizeof(Y)
            << " sizeof(Z)=" << sizeof(Z)
            << " sizeof(A)=" << sizeof(A)
            << "\n";
    }

    /**
     * The Binding of a Data Member
     *    1. the binding of a data member within the body of an inline member function does not occur
     *       until after the entire class declaration is seen.
     *    2. Names within the argument list are still resolved in place at the point they are first encountered.
     *       Nonintuitive bindings between extern and nested type names, therefore, can still occur.
     *       (requires always placing nested type declarations at the beginning of the class)
     */
    namespace BindingOfDataMember {

        extern int x;
        class Point3d
        {
        public:
            // analysis of function body delayed until
            // closing brace of class declaration seen.
            float X() const { return x; }

        private:
            float x;
        };
        // in effect, analysis is done here


        typedef int length;
        class Point {
        public:
            // oops: length resolves to global
            // ok: _val resolves to Point::_val
            void mumble(length val) { _val = val; }
            length mumble() { return _val; }

        private:
            // length must be seen before its first
            // reference within the class. This
            // declaration makes the prior reference illegal.
            typedef float length;
            length _val;
        };

    } // namespace BindingOfDataMember


    /**
     * Data Member Layout
     *   1. the nonstatic data members are set down in the order of their declaration within each class object;
     *   2. the static data members are stored in the program's data segment independent of individual class objects;
     *   3. The Standard requires within an access section (the private, public, or protected section of a class
     *      declaration) only that the members be set down such that "later members have higher addresses
     *      within a class object" (Section 9.2 of the Standard). That is, the members are not required to be set
     *      down contiguously.
     *   4. Additionally, the compiler may synthesize one or more additional internal data members in support
     *      of the Object Model. Where should the vptr be placed within the class object?
     *      - Traditionally, it has been placed after all the explicitly declared members of the class.
     *      - More recently, it has been placed at the beginning of the class object.
     *      - The Standard allows the compiler the freedom to insert these internally generated members anywhere,
     *        even between those explicitly declared by the programmer.
     *   5. The Standard also allows the compiler the freedom to order the data members within multiple
     *      access sections within a class in whatever order it sees fit.
     */
    namespace DataMemberLayout {

        class Point {
        public:
            float x;
            static std::list<Point*> *freeList;
            float y;
            static const int chunkSize = 250;
            float z;
        };
        std::list<Point*>* Point::freeList = nullptr;

        // the size and composition of the resultant class object is the same as our earlier declaration, but the order of members is now implementation depen-dent. 
        // In practice, multiple access sections are concatenated together into one contiguous block in the order of declaration.
        // No overhead is incurred by the access section specifier or the number of access levels.
        // For example, declaring eight members in one access section or eight separate access sections in practice results in the same-sized objects.
        class Point2 {
        public:
            float x;
            static std::list<Point2*> *freeList;
        public:
            float y;
            static const int chunkSize = 250;
        public:
            float z;
        };
        std::list<Point2*>* Point2::freeList = nullptr;

        template <class class_type, class data_type1>
        void member_address(data_type1 class_type::*mem) {
            printf("%p\n", mem);
        }

        void do_member_address() {
            printf("Point  x(%p) y(%p) z(%p) freeList(%p) chunkSize(%p)\n",
                &Point::x, &Point::y, &Point::z, &Point::freeList, &Point::chunkSize);

            printf("Point2 x(%p) y(%p) z(%p) freeList(%p) chunkSize(%p)\n",
                &Point2::x, &Point2::y, &Point2::z, &Point2::freeList, &Point2::chunkSize);

            member_address(&Point2::y);
        }

    } // namespace DataMemberLayout


    /**
     * Access of a Data Member
     *   Static Data Members
     *     - treated as if each were declared as a global variable (but with visibility limited to the scope of the class).
     *     - Each member's access permission and class association is maintained without incurring any space or runtime overhead
     *       either in the individual class objects or in the static data member itself.
     *     - A single instance of each class static data member is stored within the data segment of the program.
     *   Nonstatic Data Members
     *     - Nonstatic data members are stored directly within each class object and cannot be accessed except
     *       through an explicit or implicit class object.
     * NOTE:
     *   The two important aspects of any name-mangling scheme:
     *   1. the algorithm yields unique names;
     *   2. those unique names can be easily recast back to the original name in case the compilation system (or environment tool)
     *      needs to communicate with the user.
     */
    namespace AccessOfDataMember {

        class Point {
        public:
            static const int chunkSize = 250;
        };

        class Point2 {
        public:
            static const int chunkSize = 250;
        };

        Point foobar() { return Point(); }

        void do_point() {
            Point origin, *pt = &origin;

            // the access of a member through a pointer and through an object are exactly equivalent in terms of the instructions actually executed.
            origin.chunkSize == 250;
            //Point3d::chunkSize == 250;
            pt->chunkSize == 250;
            //Point3d::chunkSize == 250;

            // Standard C++ explicitly requires that foobar() be evaluated, although no use is made of its result.
            foobar().chunkSize == 250;
            //(void)foobar();
            //Point3d::chunkSize == 250;

            // Taking the address of a static data member yields an ordinary pointer of its data type, not a pointer to class member
            printf("&Point::chunkSize(%p)\n", &Point::chunkSize); // yields an actual memory address of type const int*

            // If two classes each declare a static member chunkSize, then placing both of them in the program data segment is going to result in a name conflict.
            // The compiler resolves this by internally encoding the name of each static data member to yield a unique program identifier (called name-mangling).
            printf("&Point2::chunkSize(%p)\n", &Point2::chunkSize);
        }


        class Point3d {
        public:
            void translate(const Point3d &pt) {
                x += pt.x;
                y += pt.y;
                z += pt.z;
            }

        public:
            float x;
            float y;
            float z;
        };

#ifdef AUTO_TRANSFORM_BY_COMPILER
        // internal augmentation of member function
        void translate(Point3d *const this, const Point3d &pt) {
            this->x += pt.x;
            this->y += pt.y;
            this->z += pt.z;
        }
#endif

        void do_point3d() {
            Point3d origin3d, *pt3d = &origin3d;

            // significantly different when accessed through the object origin or the pointer pt3d?
            // The answer is different when the Point3d class is a derived class containing a virtual base class within its inheritance hierarchy
            // and the member being accessed is an inherited member of that virtual base class.
            origin3d.y = 0.0;
            pt3d->y = 0.0;

            printf("&origin3d(%p)\n", &origin3d);
            printf("&origin3d.x(%p) &Point3d::x(%p)\n", &origin3d.x, &Point3d::x);
            printf("&origin3d.y(%p) &Point3d::y(%p)\n", &origin3d.y, &Point3d::y);
            printf("&origin3d.z(%p) &Point3d::z(%p)\n", &origin3d.z, &Point3d::z);
        }

    } // namespace AccessOfDataMember


    /**
     * Inheritance and the Data Member
     *   Under the C++ inheritance model, a derived class object is represented as the concatenation of its members with those of its base class(es).
     *   The actual ordering of the derived and base class parts is left unspecified by the Standard. In practice, the base class members always appear first,
     *   except in the case of a virtual base class.
     */
    namespace InheritanceAndDataMember {

        namespace WithoutPolymorphism {

            class Point2d {
            public:
                Point2d(float x = 0.0, float y = 0.0)
                    : _x(x), _y(y) {};
                float x() const { return _x; }
                float y() const { return _y; }
                void x(float newX) { _x = newX; }
                void y(float newY) { _y = newY; }
                void operator+=(const Point2d& rhs) {
                    _x += rhs.x();
                    _y += rhs.y();
                }
            protected:
                float _x, _y;
            };

            // inheritance from concrete class
            class Point3d : public Point2d {
            public:
                Point3d(float x = 0.0, float y = 0.0, float z = 0.0)
                    : Point2d(x, y), _z(z) {};
                float z() const { return _z; }
                void z(float newZ) { _z = newZ; }
                void operator+=(const Point3d& rhs) {
                    Point2d::operator+=(rhs);
                    _z += rhs.z();
                }
            protected:
                float _z;
            };

            // On a 32-bit machine, the size of each Concrete class object is going to be 8 bytes:
            // - 4 bytes for val
            // - 1 byte each for c1, c2, and c3
            // - 1 byte for the alignment of the class on a word boundary
            // NOTE: 
            //   generally, alignment constraints are determined by the underlying processor.
            class Concrete {
            private:
                int val;
                char c1;
                char c2;
                char c3;
            };

            // On a 32-bit machine, the size of Concrete1 class object is going to be 8 bytes:
            // - 4 bytes for val
            // - 1 byte each for bit1
            // - 3 byte for the alignment of the class on a word boundary
            class Concrete1 {
            protected:
                int val;
                char bit1;
            };
            // On a 32-bit machine, the size of Concrete2 class object is going to be 12 bytes:
            // - 8 bytes for Concrete1
            // - 1 byte each for bit2
            // - 3 byte for the alignment of the class on a word boundary
            class Concrete2 : public Concrete1 {
            protected:
                char bit2;
            };
            // On a 32-bit machine, the size of Concrete3 class object is going to be 16 bytes:
            // - 12 bytes for Concrete2
            // - 1 byte each for bit3
            // - 3 byte for the alignment of the class on a word boundary
            class Concrete3 : public Concrete2 {
            protected:
                char bit3;
            };

            void do_concrete() {
                printf("sizeof(Concrete)=%d\n", sizeof(Concrete));
                printf("sizeof(Concrete1)=%d\n", sizeof(Concrete1));
                printf("sizeof(Concrete2)=%d\n", sizeof(Concrete2));
                printf("sizeof(Concrete3)=%d\n", sizeof(Concrete3));

                Concrete2 *pc2 = new Concrete3;
                Concrete1 *pc1_1 = new Concrete1, *pc2_2 = new Concrete2;
                *pc1_1 = *pc2_2;
                pc1_1 = pc2;
                *pc1_1 = *pc2_2;
            }

        } // namespace WithoutPolymorphism

        namespace AddingPolymorphism {

            // a number of space and access-time overheads:
            // - Introduction of a virtual table associated with Point2d to hold the address of each virtual
            //   function it declares.The size of this table in general is the number of virtual functions
            //   declared plus an additional one or two slots to support runtime type identification.
            // - Introduction of the vptr within each class object. The vptr provides the runtime link for an
            //   object to efficiently find its associated virtual table.
            // - Augmentation of the constructor to initialize the object's vptr to the virtual table of the class.
            //   Depending on the aggressiveness of the compiler's optimization, this may mean resetting the vptr
            //   within the derived and each base class constructor.
            // - Augmentation of the destructor to reset the vptr to the associated virtual table of the class.
            //   An aggressive optimizing compiler can suppress a great many of these assignments.
            //   (It is likely to have been set to address the virtual table of the derived class within the
            //    destructor of the derived class. Remember, the order of destructor calls is in reverse :
            //    derived class and then base class.)  
            class Point2d {
            public:
                Point2d(float x = 0.0, float y = 0.0)
                    : _x(x), _y(y) {};
                float x() const { return _x; }
                float y() const { return _y; }
                // add placeholders for z ¡ª do nothing ...
                virtual float z() const { return 0.0; }
                virtual void z(float) {}
                // turn type explicit operations virtual
                virtual void operator+=(const Point2d& rhs) {
                    _x += rhs.x(); _y += rhs.y();
                }
            protected:
                float _x, _y;
            };

            class Point3d : public Point2d {
            public:
                Point3d(float x = 0.0, float y = 0.0, float z = 0.0)
                    : Point2d(x, y), _z(z) {};
                float z() const { return _z; }
                void z(float newZ) { _z = newZ; }
                void operator+=(const Point2d& rhs) {
                    Point2d::operator+=(rhs);
                    _z += rhs.z();
                }
            protected:
                float _z;
            };

            // where p1 and p2 may be either two/three-dimensional points.
            // This is not something that any of our previous designs supported.
            void foo(Point2d &p1, Point2d &p2) {
                p1 += p2;
            }

            struct no_virts {
                int d1, d2;
            };
            class has_virts : public no_virts {
            public:
                virtual void foo() {}
            private:
                int d3;
            };

            // The position of vptr in class object
            // <1> Placing the vptr at the end of the class object preserves the object layout of the base class C struct, thus permitting its use within C code.
            // <2> With its addition of support for multiple inheritance and abstract base classes, and in popularity of the OO paradigm, 
            //     some implementations began placing the vptr at the start of the class object(eg VC++).
            //     (derive a polymorphic class from a C-language struct is nonsupport.)
            // <3> the vptr placement at the end of the base class.
            void do_virts() {
                no_virts *p = new has_virts;
            }

        } // namespace AddingPolymorphism

        namespace MultipleInheritance {

            // Multiple inheritance is neither as well behaved nor as easily modeled as single inheritance. 
            // The assignment of the address of a multiply derived object to a pointer of its leftmost base class
            // is the same as that for single inheritance, since both point to the same beginning address.
            // The assignment of the address of a second or subsequent base class, however, requires that address be modified by the addition.
            class Point2d {
            public:
                float _x, _y;
            };
            class Vertex {
            public:
                Vertex *next;
            };
            // The Standard does not require a specific ordering of the Point2d and Vertex base classes of Vertex2d.
            // The member's location is fixed at compile time. Hence its access is a simple offset the same as under single inheritance
            // regardless of whether it is a pointer, reference, or object through which the member is being accessed.
            class Vertex2d : public Point2d, public Vertex {
            public:
                float mumble;
            };

            void mumble(const Vertex&) {}

            void do_vertex3d() {
                Vertex2d v;
                // conversion of a Vertex2d to Vertex is ``unnatural''
                mumble(v);

                printf("&v(%p)\n", &v);
                printf("&v._x(%p) &Vertex2d::_x(%p)\n", &v._x, &Vertex2d::_x);
                printf("&v._y(%p) &Vertex2d::_y(%p)\n", &v._y, &Vertex2d::_y);
                printf("&v.next(%p) &Vertex2d::next(%p)\n", &v.next, &Vertex2d::next);
                printf("&v.mumble(%p) &Vertex2d::mumble(%p)\n", &v.mumble, &Vertex2d::mumble);

                Vertex2d v2d;
                Vertex *pv;
                Point2d *pp;

                pv = &v2d;
                pp = &v2d;
#ifdef AUTO_TRANSFORM_BY_COMPILER
                // Pseudo C++ Code
                pv = (Vertex*)(((char*)&v2d) + sizeof(Point2d));
                pp = &v2d;
#endif

                Vertex2d *p2d = new Vertex2d;
                pv = p2d;
#ifdef AUTO_TRANSFORM_BY_COMPILER
                // Pseudo C++ Code
                pv = p2d
                    ? (Vertex*)((char*)p2d) + sizeof(Point2d)
                    : 0;
#endif

            }

        } // namespace MultipleInheritance

        namespace VirtualInheritance {

            // A semantic side effect of multiple inheritance is the need to support a form of shared subobject inheritance.
            // A class containing one or more virtual base class subobjects, such as istream, is divided into two regions :
            // - an invariant region. Data within the invariant region remains at a fixed offset from the start of the object
            //   regardless of subsequent derivations. So members within the invariant region can be accessed directly.
            // - a shared region. The shared region represents the virtual base class subobjects. The location of data within
            //   the shared region fluctuates with each derivation. So members within the shared region need to be accessed indirectly.
            // NOTE:
            //   In general, the most efficient use of a virtual base class is that of an abstract virtual base class with no associated data members.
            class ios { };
            class istream : public virtual ios { };
            class ostream : public virtual ios { };
            class iostream : public istream, public ostream { };


            class Point2d {
            public:
                float _x, _y;
            };
            class Vertex : public virtual Point2d {
            public:
                Vertex *next;
            };
            class Point3d : public virtual Point2d {
            public:
                float _z;

                void operator+=(const Point3d &rhs) {
                    _x += rhs._x;
                    _y += rhs._y;
                    _z += rhs._z;
                };
            };
            class Vertex3d : public Point3d, public Vertex {
            public:
                float mumble;
            };

#ifdef AUTO_TRANSFORM_BY_COMPILER
            // In the original implementation, a pointer to each virtual base class is inserted within each derived class object.
            // Access of the inherited virtual base class members is achieved indirectly through the associated pointer.
            // There are two primary weaknesses with this implementation model :
            //   1. An object of the class carries an additional pointer for each virtual base class. Ideally, we want a constant overhead
            //      for the class object that is independent of the number of virtual base classes within its inheritance hierarchy.
            //   2. As the virtual inheritance chain lengthens, the level of indirection increases to that depth.
            //      This means that three levels of virtual derivation requires indirection through three virtual base class pointers.
            //      Ideally, we want a constant access time regardless of the depth of the virtual derivation.
            void Point3d::operator+=(const Point3d &rhs) {
                // Pseudo C++ Code (the original implementation)
                __vbcPoint2d->_x += rhs.__vbcPoint2d->_x;
                __vbcPoint2d->_y += rhs.__vbcPoint2d->_y;
                _z += rhs._z;
            };

            // There are two general solutions to the first problem:
            //   - Microsoft's compiler introduced the virtual base class table.
            //     Each class object with one or more virtual base classes has a pointer to the virtual base class table inserted within it.
            //     The actual virtual base class pointers, of course, are placed within the table.
            //   - The second solution (by Bjarne) is to place not the address but the offset of the virtual base class within the virtual function table.
            //     In the recent Sun compiler, the virtual function table is indexed by both positive and negative indices.
            //     The positive indices, as previously, index into the set of virtual functions; the negative indices retrieve the virtual base class offsets.
            void Point3d::operator+=(const Point3d &rhs) {
                // Pseudo C++ Code (Sun compiler)
                (this + __vptr__Point3d[-1])->_x += (&rhs + rhs.__vptr__Point3d[-1])->_x;
                (this + __vptr__Point3d[-1])->_y += (&rhs + rhs.__vptr__Point3d[-1])->_y;
                _z += rhs._z;
            };

#endif

        } // namespace VirtualInheritance

    } // namespace InheritanceAndDataMember


    /**
     * Pointer to Data Members
     *   Pointers to data members are a somewhat arcane but useful feature of the language, particularly if you need to probe at the underlying member layout of a class.
     *   One example of such a probing might be to determine if the vptr is placed at the beginning or end of the class.
     *   A second use might be to determine the ordering of access sections within the class.
     */
    namespace PointerToDataMembers {

        // The Standard permits the vptr to be placed anywhere within the object : at the beginning, at the end, or in between either of the three members.
        // On a 32-bit machine, floats are 4 bytes each, the physical offset of the three coordinate members within the class layout are, respectively, either
        // 4, 8, and 12 if the vptr is placed at the start of the class.
        class Point3d {
        public:
            virtual ~Point3d() {}
            static Point3d origin;
            float x, y, z;
        };
        Point3d Point3d::origin;

        void do_print3d() {
            printf("&Point3d::origin(%p)\n", &Point3d::origin);
            printf("&Point3d::x(%p)\n", &Point3d::x);
            printf("&Point3d::y(%p)\n", &Point3d::y);
            printf("&Point3d::z(%p)\n", &Point3d::z);

            // On a 32-bit machine, the physical offset of the three coordinate members within the class layout are 0, 4, and 8 if the vptr is placed at the end.
            // So how to distinguish p1 and p2? In practice, to distinguish between p1 and p2, each actual member offset value is bumped up by 1.
            // Hence, both the compiler(and the user) must remember to subtract 1 before actually using the value to address a member.
            float Point3d::*p1 = 0;
            float Point3d::*p2 = &Point3d::x;
            printf("p1(%p) p2(%p)\n", p1, p2);
            if (p1 == p2) {
                std::cout << " p1 & p2 contain the same value ¡ª ";
                std::cout << " they must address the same member!" << std::endl;
            }

            // taking the address of a nonstatic data member yields its offset within the class;
            // taking the address of a data member bound to an actual class object yields the member's actual address in memory.
            printf("&Point3d::z(%p) &Point3d::origin.z(%p)\n", &Point3d::z, &Point3d::origin.z);
        }

        struct Base1 { int val1; };
        struct Base2 { int val2; };
        struct Derived : Base1, Base2 { };
        // bmp must be adjusted by the size of the intervening Base1 class when passed as the first argument to func1().
        void func1(int Derived::*dmp, Derived *pd) {
            // expects a derived pointer to member
            // what if we pass it a base pointer?
            pd->*dmp;
        }
        void func2(Derived *pd) {
            int Base2::*bmp = &Base2::val2;
            func1(bmp, pd);

#ifdef AUTO_TRANSFORM_BY_COMPILER
            // internal transformation guarding against bmp == 0
            func1(bmp ? bmp + sizeof(Base1) : 0, pd);
#endif
        }

    } // namespace PointerToDataMembers


} // namespace SemanticsOfData


#endif // _SEMANTICS_OF_DATA_H_

