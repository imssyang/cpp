#ifndef _SEMANTICS_OF_FUNCTION_H_
#define _SEMANTICS_OF_FUNCTION_H_


#include <iostream>

/**
 * The Semantics of Function
 */
namespace SemanticsOfFunction {

    namespace VarietiesOfMemberInvocation {

        /**
         * One C++ design criterion is that a nonstatic member function at a minimum must be as efficient as its analogous nonmember function.
         */ 
        namespace NonstaticMemberFunctions {

            class Point3d {
            public:
                Point3d() { }
                Point3d(float x, float y, float z) : _x(x), _y(y), _z(z) {}

                // the member function is transformed internally to be equivalent to the nonmember instance.
                float magnitude() const {
                    return sqrt(
                        _x * _x + _y * _y + _z * _z
                    );
                }

                Point3d normalize() const {
                    register float mag = magnitude();
                    Point3d normal;
                    normal._x = _x / mag;
                    normal._y = _y / mag;
                    normal._z = _z / mag;
                    return normal;
                }

                Point3d normalize2() const {
                    register float mag = magnitude();
                    return Point3d(_x / mag, _y / mag, _x / mag);
                }

            public:
                float _x, _y, _z;
            };

#ifdef AUTO_TRANSFORM_BY_COMPILER
            // Rewrite the member function into an external function, mangling its name so that it's lexically unique within the program
            extern float magnitude__7Point3dFv(register Point3d *const this);
            extern void normalize__7Point3dFv(register const Point3d *const this, Point3d &__result);
            extern void normalize2__7Point3dFv(register const Point3d *const this, Point3d &__result);

            // Rewrite the signature to insert an additional argument to the member function that provides access to the invoking class object (implicit this pointer).
            // Rewrite each direct access of a nonstatic data member of the class to access the member through the this pointer.
            // NOTE:
            //   - non-const nonstatic member augmentation
            //     float Point3d::magnitude(Point3d *const this)
            //   - const nonstatic member augmentation
            //     float Point3d::magnitude(const Point3d *const this)
            float magnitude__7Point3dFv(const Point3d *const this) {
                return sqrt(
                    this->_x * this->_x +
                    this->_y * this->_y +
                    this->_z * this->_z);
            }

            // Representing internal transformation with application of named return value
            void normalize__7Point3dFv(register const Point3d *const this, Point3d &__result) {
                register float mag = this->magnitude();
                // default constructor
                __result.Point3d::Point3d();
                __result._x = this->_x / mag;
                __result._y = this->_y / mag;
                __result._z = this->_z / mag;
                return;
            }

            // Representing internal transformation (NRV optimization is applied)
            void normalize2__7Point3dFv(register const Point3d *const this, Point3d &__result) {
                register float mag = this->magnitude();
                // __result substituted for return value
                __result.Point3d::Point3d(this->_x / mag, this->_y / mag, this->_z / mag);
                return;
            }
#endif

            float magnitude3d(const Point3d *_this) {
                return sqrt(
                    _this->_x * _this->_x +
                    _this->_y * _this->_y +
                    _this->_z * _this->_z);
            }

            void do_point3d() {
                Point3d obj;
                Point3d *ptr = &obj;

                obj.normalize();
                ptr->normalize();

#ifdef AUTO_TRANSFORM_BY_COMPILER
                magnitude__7Point3dFv(&obj);
                magnitude__7Point3dFv(ptr);
#endif
            }

        } // namespace NonstaticMemberFunctions

        /**
         * In general, member names are made unique by concatenating the name of the member with that of the class.
         */
        namespace NameMangling {

            class Bar {
            public:
                int ival;

#ifdef AUTO_TRANSFORM_BY_COMPILER
                // a possible member name-mangling
                int ival__3Bar;
#endif
            };

            class Foo : public Bar {
            public:
                int ival;
            };

#ifdef AUTO_TRANSFORM_BY_COMPILER
            // internal representation of Foo
            class Foo {
            public:
                int ival__3Bar;
                int ival__3Foo;
            };
#endif

            class Point {
            public:
                void x(float newX) {}
                float x() {}
            };

#ifdef AUTO_TRANSFORM_BY_COMPILER
            // What makes these instances unique are their argument lists.
            // Function names are made unique by internally encoding their argument lists types and concatenating those to the name
            // (use of the extern "C" declaration suppresses mangling of nonmember functions).
            // By encoding the argument list with the function name, the compiler achieves a limited form of type checking across separately compiled modules.
            class Point {
            public:
                void x__5PointFf(float newX) {}
                float x__5PointFv() {}
            };
#endif

        } // namespace NameMangling

        namespace VirtualMemberFunctions {

            class Point3d {
            public:
                virtual float magnitude() const {
                    return sqrt(
                        _x * _x + _y * _y + _z * _z
                    );
                }

                virtual Point3d normalize() const {
                    register float mag = magnitude();
                    Point3d normal;
                    normal._x = _x / mag;
                    normal._y = _y / mag;
                    normal._z = _z / mag;
                    return normal;
                }

            public:
                float _x, _y, _z;
            };

            void do_point3d() {
                Point3d obj;
                Point3d *ptr = &obj;

                obj.normalize();
                ptr->normalize();

#ifdef AUTO_TRANSFORM_BY_COMPILER
                // unnecessary internal transformation!
                (*obj.vptr[1])(&obj);
                // The invocation of a virtual function through a class object should always be resolved by your compiler as an ordinary nonstatic member function
                normalize__7Point3dFv(&obj);

                // - vptr represents the internally generated virtual table pointer inserted within each object
                //    whose class declares or inherits one or more virtual functions. 
                // - 1 is the index into the virtual table slot associated with normalize().
                // - ptr in its second occurrence represents the this pointer.
                (*ptr->vptr[1])(ptr);

                Point3d normalize() const {
                    // register float mag = magnitude();
                    register float mag = (*this->vptr[2])(this);

                    // explicit invocation suppresses virtual mechanism.
                    // This is significantly more efficient if magnitude() is declared inline.
                    register float mag = Point3d::magnitude();
                    register float mag = magnitude__7Point3dFv(this);
                }
#endif
            }

        } // namespace VirtualMemberFunctions

        namespace StaticMemberFunctions {

            class Point3d {
            public:
                static unsigned int object_count() {
                    return _object_count;
                }

                static Point3d normalize() {
                    Point3d normal;
                    return normal;
                }

            public:
                static unsigned int _object_count;
            };

            unsigned int Point3d::_object_count = 0;

#ifdef AUTO_TRANSFORM_BY_COMPILER
            // internal transformaton under cfront
            unsigned int object_count__5Point3dSFv() {
                return _object_count__5Point3d;
            }
#endif

            void do_point3d() {
                Point3d obj;
                Point3d *ptr = &obj;

                obj.normalize();
                ptr->normalize();

#ifdef AUTO_TRANSFORM_BY_COMPILER
                // internally transformed into "ordinary" nonmember function 
                // obj.normalize();
                normalize__7Point3dSFv();
                // ptr->normalize();
                normalize__7Point3dSFv();

                // Because the static member function is without a this pointer, 
                // the type of its address value is not a pointer to class member function but the type of a nonmember pointer to function.
                //     &Point3d::object_count();
                // yields a value of type
                //     unsigned int(*)();
                // not of type
                //     unsigned int (Point3d::*)();
#endif
            }

        } // namespace StaticMemberFunctions

    } // namespace VarietiesOfMemberInvocation


    /**
     * The virtual table is generated on a per-class basis. Each table holds the addresses of all the virtual function instances "active"
     * for objects of the table's associated class. These active functions consist of the following:
     *   - An instance defined within the class, thus overriding a possible base class instance
     *   - An instance inherited from the base class, should the derived class choose not to override it
     *   - A pure_virtual_called() library instance that serves as both a placeholder for a pure
     *     virtual function and a runtime exception should the instance somehow be invoked
     */
    namespace VirtualMemberFunctions {

        namespace SingleInheritance {

            // vptr[0] -- "type_info" for Point;
            // vptr[1] -- the virtual destructor is likely;
            // vptr[2] -- mult() (no definition, so the address of the library function pure_virtual_called() is placed within the slot);
            // vptr[3] -- y();
            // vptr[4] -- z();
            class Point {
            public:
                virtual ~Point() {}
                virtual Point& mult(float) = 0;
                float x() const { return _x; }
                virtual float y() const { return 0; }
                virtual float z() const { return 0; }
            protected:
                Point(float x = 0.0) : _x(x) {}
                float _x;
            };

            // vptr[0] -- "type_info" for Point2d;
            // vptr[1] -- Point2d's destructor;
            // vptr[2] -- Point2d's instance of mult() (replacing the pure virtual instance);
            // vptr[3] -- Point2d's instance of y();
            // vptr[4] -- retains Point's inherited instance of z();
            class Point2d : public Point {
            public:
                Point2d(float x = 0.0, float y = 0.0)
                    : Point(x), _y(y) {}
                ~Point2d() {}
                Point2d& mult(float) { static Point2d p2d; return p2d; }
                float y() const { return _y; }
            protected:
                float _y;
            };

            // vptr[0] -- "type_info" for Point3d;
            // vptr[1] -- Point3d's destructor;
            // vptr[2] -- Point3d's instance of mult();
            // vptr[3] -- Point2d's inherited instance of y();
            // vptr[4] -- Point3d's instance of z();
            class Point3d : public Point2d {
            public:
                Point3d(float x = 0.0,
                    float y = 0.0, float z = 0.0)
                    : Point2d(x, y), _z(z) {}
                ~Point3d() {}
                Point3d& mult(float) { static Point3d p3d; return p3d; }
                float z() const { return _z; }
            protected:
                float _z;
            };

            void do_point() {
                Point *ptr;

                // example of active polymorphism
                ptr = new Point2d; // maybe
                ptr = new Point3d; // maybe
                ptr->z();


                // second example of active polymorphism
                // Until the introduction of runtime type identification (RTTI) into the language in 1993, 
                // the only support C++ provided for active polymorphism was the resolution of a virtual function call.
                // With RTTI, the runtime query of a polymorphic pointer or reference is also supported.
                Point3d *p3d = dynamic_cast<Point3d *>(ptr);
                if (p3d) {
                    p3d->z();
                }
            }

        } // namespace SingleInheritance

        namespace MultipleInheritance {

            // NOTE: The complexity of virtual function support under multiple inheritance revolves around the second and subsequent base classes
            //       and the need to adjust the this pointer at runtime.
            // vptr[0] -- "type_info" for Base1;
            // vptr[1] -- Base1::~Base1();
            // vptr[2] -- Base1::speakClearly();
            // vptr[3] -- Base1::clone();
            class Base1 {
            public:
                Base1() {}
                virtual ~Base1() {}
                virtual void speakClearly() {}
                virtual Base1 *clone() const { return nullptr; }
            protected:
                float data_Base1;
            };
            // vptr[0] -- "type_info" for Base2;
            // vptr[1] -- Base2::~Base2();
            // vptr[2] -- Base2::mumble();
            // vptr[3] -- Base2::clone();
            class Base2 {
            public:
                Base2() {}
                virtual ~Base2() {}
                virtual void mumble() {}
                virtual Base2 *clone() const { return nullptr; }
            protected:
                float data_Base2;
            };

            // (Base1 subobject : shared with Base1, the primary table)
            //   vtbl__Derived[0] -- "type_info" for Derived;
            //   vtbl__Derived[1] -- Derived::~Derived();
            //   vtbl__Derived[2] -- Base1::speakClearly();
            //   vtbl__Derived[3] -- Derived::clone();
            //   vtbl__Derived[4] -- Base2::mumble();
            // (Base2 subobject : Derived/Base2, the secondary table)
            //   vtbl__Base2__Derived[0] -- "type_info" for Derived;
            //   vtbl__Base2__Derived[1] -- Derived::~Derived();
            //   vtbl__Base2__Derived[2] -- Base2::mumble();
            //   vtbl__Base2__Derived[3] -- Derived::clone();
            // (data_Derived)
            class Derived : public Base1, public Base2 {
            public:
                Derived() {}
                virtual ~Derived() {}
                virtual Derived *clone() const { return nullptr; }
            protected:
                float data_Derived;
            };

            void do_underMI() {
                Base2 *pbase2 = new Derived;

#ifdef AUTO_TRANSFORM_BY_COMPILER
                // The address of the new Derived object must be adjusted to address its Base2 subobject.
                Derived *temp = new Derived;
                Base2 *pbase2 = temp ? temp + sizeof(Base1) : 0;
#endif

                delete pbase2;

#ifdef AUTO_TRANSFORM_BY_COMPILER
                // the pointer must be readjusted in order to again address the beginning of the Derived class object.
                // This offset addition, however, cannot directly be set up at compile time because the actual object that
                // pbase2 addresses generally can be determined only at runtime. That is, the size of the necessary offset 
                // and the code to add it to the this pointer must be tucked away somewhere by the compiler. 
                // (*pbase2->vptr[1])(pbase2);  // not
                // -- faddr held the virtual function address 
                // -- offset held the necessary this pointer adjustment.
                (*pbase2->vptr[1].faddr)(pbase2 + pbase2->vptr[1].offset);

                // The more efficient solution is the use of a thunk.
            pbase2_dtor_thunk:
                this += sizeof(base1);
                Derived::~Derived(this);
#endif

                // Although both delete invocations result in the execution of the same Derived class destructor, they require two unique virtual table entries :
                //   - pbase11 does not require a this pointer adjustment (being leftmost). Its virtual table slot requires the actual destructor address.
                //   - pbase22 does require a this pointer adjustment. Its virtual table slot requires the address of the associated thunk.
                Base1 *pbase11 = new Derived;
                Base2 *pbase22 = new Derived;
                delete pbase11;
                delete pbase22;


                // invokes Derived::~Derived
                // ptr must be adjusted to address the beginning of the Derived class object.
                Base2 *ptr = new Derived;
                delete ptr;

                // invokes Base2::mumble()
                // the derived class pointer must be readjusted to address the second base subobject.
                Derived *pder = new Derived;
                pder->mumble();

                // invokes Derived* Derived::clone()
                // The Derived class instance of clone() is invoked and uses the thunk to readjust pb1 to address the beginning of the Derived class object.
                // Now clone() returns a pointer to a new Derived class object, and the address must be adjusted to point to the Base2 subobject before being assigned to pb2.
                Base2 *pb1 = new Derived;
                Base2 *pb2 = pb1->clone();
            }

        } // namespace MultipleInheritance

        namespace VirtualInheritance {

            /**
             * vptr[0] -- "type_info" for Point2d;
             * vptr[1] -- Point2d::~Point2d();
             * vptr[2] -- Point2d::mumble();
             * vptr[3] -- Point2d::z();
             */
            class Point2d {
            public:
                Point2d(float = 0.0, float = 0.0) {}
                virtual ~Point2d() {}
                virtual void mumble() {}
                virtual float z() {}
            protected:
                // recommendation is not to declare nonstatic data members within a virtual base class.
                // Doing that goes a long way in taming the complexity.
                float _x, _y; // not recommendation, too esoteric
            };
            /**
             * (offset to virtual base)
             * vptr[0] -- "type_info" for Point3d;
             * vptr[1] -- Point3d::~Point3d();
             * vptr[2] -- Point3d::mumble(); // ???
             * vptr[3] -- Point3d::z();
             */
            class Point3d : public virtual Point2d {
            public:
                Point3d(float = 0.0, float = 0.0, float = 0.0) {}
                ~Point3d() {}
                float z() {}
            protected:
                float _z;
            };

        } // namespace VirtualInheritance

    } // namespace VirtualMemberFunctions


    /**
     * The pointer-to-member function declaration syntax and pointer-to-member selection operators serves literally as placeholders for the "this" pointer.
     *  (This is why static member functions, which are without a this pointer, are of type "pointer to function," not "pointer-to-member function.")
     * NOTE:
     *   Use of a pointer to member would be no more expensive than a nonmember pointer to function if it weren't for virtual functions and multiple inheritance.
     */
    namespace PointerToMemberFunctions {

        class Point {
        public:
            virtual ~Point() { std::cout << "~Point" << std::endl; }
            float x() { std::cout << "x" << std::endl; return 0; }
            float y() { std::cout << "y" << std::endl; return 0; }
            virtual float z() { std::cout << "z" << std::endl; return 0; }
        };

        // declaring a pointer-to-member function
        float     // return type
        (Point::* // class the function is member
            pmf)  // name of pointer to member
            ();   // argument list

        void do_point() {
            // define and initialize a pointer to class member
            float (Point::*coord)() = &Point::x;

            // assign a value to it
            coord = &Point::y;

            // invocation uses the pointer-to-member selection operators
            Point origin;
            Point* ptr = &origin;
            (origin.*coord)();
            (ptr->*coord)();

#ifdef AUTO_TRANSFORM_BY_COMPILER
            (coord)(&origin);
            (coord)(ptr);
#endif
        }

        void do_virtual() {
            float (Point::*pmf)() = &Point::z;
            Point *ptr = new Point;

            // the two instance invoked is Point3d::z()
            ptr->z();
            (ptr->*pmf)();

#ifdef AUTO_TRANSFORM_BY_COMPILER
            // The internal definition of pmf, that is,
            // float (Point::*pmf)();
            // NOTE:
            //   - both can be assigned to pmf
            //   float Point::x() { return _x; }
            //   float Point::z() { return 0; }
            (*ptr->vptr[2](ptr);
            (*ptr->vptr[(int)pmf])(ptr);

            // In the pre-Release 2.0 cfront implementation.
            // this implementation assumes a limit of 128 virtual functions to an inheritance graph. 
            (((int)pmf) & ~127)
                ? // non-virtual invocation -- yields an index into the virtual table(a small number)
                (*pmf)(ptr)
                : // virtual invocation -- yields an address in memory (a large number)
                (*ptr->vptr[(int)pmf](ptr);
#endif
        }

        void do_multiple() {

#ifdef AUTO_TRANSFORM_BY_COMPILER
            // fully general structure to support pointer to member functions under MI
            struct __mptr {
                int delta;            // this pointer offset
                int index;            // virtual table index (set to ¨C1 if it does not index into the virtual table.)
                union {
                    ptrtofunc faddr;  // the nonvirtual member function address
                    int v_offset;     // location of the vptr of a virtual (or second or subsequent) base class
                };
            };

            // Under this model, an invocation such as
            // (ptr->*pmf)();            
            // becomes
            (pmf.index < 0)
                ? // non-virtual invocation
                (*pmf.faddr)(ptr)
                : // virtual invocation
                (*ptr->vptr[pmf.index](ptr);
#endif

        }

    } // namespace PointerToMemberFunctions


    namespace InlineFunctions {



    } // namespace InlineFunctions

} // namespace SemanticsOfFunction


#endif // _SEMANTICS_OF_FUNCTION_H_

