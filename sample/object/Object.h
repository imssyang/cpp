#ifndef _OBJECT_H_
#define _OBJECT_H_


#include <iostream>

/**
 * Object
 */
namespace Object {

    /**
     * The procedural model (C style)
     *   In C, a data abstraction and the operations that perform on it are declared separately！that is,
     *   there is no language-supported relationship between data and functions. We speak of this method
     *   of programming as procedural, driven by a set of algorithms divided into task-oriented functions
     *   operating on shared, external data.
     */
    namespace Object_CStyle {

        typedef struct point3d {
            float x;
            float y;
            float z;
        } Point3d;

        void Point3d_print(const Point3d *pd) {
            printf("( %g, %g, %g )", pd->x, pd->y, pd->z);
        }

#define Point3d_Print(pd) \
    printf("( %g, %g, %g )", pd->x, pd->y, pd->z );

        void my_foo() {
            Point3d *pd = new Point3d();

            printf("( %g, %g, %g )", pd->x, pd->y, pd->z);

            Point3d pt;
            pt.x = 0.0;

#define X2P(p, xval) (p.x) = (xval);
            X2P(pt, 0.0);
        }

    } // namespace Object_CStyle


    /**
     * The abstract data type (ADT) model
     *   the abstraction are provided with a set of operations (the public interface), while the implementation remains hidden.
     *   The Point3d class has no space or runtime penalty in supporting encapsulation.
     *   The primary layout and access-time overheads within C++ are associated with the virtuals, that is,
     *     1. the virtual function mechanism in its support of an efficient run-time binding;
     *     2. a virtual base class in its support of a single, shared instance of a base class occurring
     *        multiple times within an inheritance hierarchy.
     *   NOTE:
     *     C++ also supports a concrete ADT style of programming now called object-based (OB) ！！ nonpolymorphic data types.
     *     such as a String class. A String class provides a public interface and private implementation (both of state and algorithm)
     *     but does not support type extension. An OB design can be faster and more compact than an equivalent OO design.
     *     Faster because all function invocations are resolved at compile time and object construction need not set up the virtual mechanism,
     *     and more compact because each class object need not carry the additional overhead traditionally associated with the support of the
     *     virtual mechanism. However, an OB design also is less flexible.
     */
    namespace Object_ADT {

        class Point3d
        {
        public:
            Point3d(float x = 0.0, float y = 0.0, float z = 0.0)
                : _x(x), _y(y), _z(z) {}
            float x() const { return _x; }
            float y() const { return _y; }
            float z() const { return _z; }
            void x(float xval) { _x = xval; }
            void y(float yval) { _y = yval; }
            void z(float zval) { _z = zval; }
        private:
            float _x;
            float _y;
            float _z;
        };

        inline std::ostream& operator<<(std::ostream &os, const Point3d &pt)
        {
            os << "( " << pt.x() << ", " << pt.y() << ", " << pt.z() << " )";
        };

    } // namespace Object_ADT


    /**
     * The object-oriented (OO) model
     *   a collection of related types are encapsulated through an abstract base class providing a common interface.
     */
    namespace Object_Hierarchy {

        class Point {
        public:
            Point(float x = 0.0) : _x(x) {}
            float x() const { return _x; }
            void x(float xval) { _x = xval; }
        protected:
            float _x;
        };

        class Point2d : public Point {
        public:
            Point2d(float x = 0.0, float y = 0.0)
                : Point(x), _y(y) {}
            float y() const { return _y; }
            void y(float yval) { _y = yval; }
        protected:
            float _y;
        };

        class Point3d : public Point2d {
        public:
            Point3d(float x = 0.0, float y = 0.0, float z = 0.0)
                : Point2d(x, y), _z(z) {}
            float z() const { return _z; }
            void z(float zval) { _z = zval; }
        protected:
            float _z;
        };

    } // namespace Object_Hierarchy


    /**
     * template class
     */
    namespace Object_Template {

        template <class type>
        class Point3d
        {
        public:
            Point3d(type x = 0.0, type y = 0.0, type z = 0.0)
                : _x(x), _y(y), _z(z) {}
            type x() const { return _x; }
            type y() const { return _y; }
            type z() const { return _z; }
            void x(type xval) { _x = xval; }
            void y(type yval) { _y = yval; }
            void z(type zval) { _z = zval; }
        private:
            type _x;
            type _y;
            type _z;
        };


        template <class type, int dim>
        class Point
        {
        public:
            Point() {}
            Point(type coords[dim]) {
                for (int index = 0; index < dim; index++)
                    _coords[index] = coords[index];
            }
            type& operator[](int index) {
                assert(index < dim && index >= 0);
                return _coords[index];
            }
            type operator[](int index) const {
                assert(index < dim && index >= 0);
                return _coords[index];
            }
        private:
            type _coords[dim];
        };

        template <class type, int dim>
        std::ostream& operator<<(std::ostream &os, const Point<type, dim> &pt)
        {
            os << "( ";
            int last = dim - 1;
            for (int i = 0; i < last; i++)
                os << pt[i] << ", ";
            os << pt[last];
            os << " )";
        }

    } // namespace Object_Template


    /**
     * The C++ Object
     *   1. two flavors of class data members！static and nonstatic;
     *   2. three flavors of class member functions！static, nonstatic, and virtual.
     *
     *   Simple Object Model
     *     An object is a sequence of slots, where each slot points to a member.
     *     The members are assigned a slot in the order of their declarations.
     *     There is a slot for each data or function member.
     *     NOTE:
     *       a) In this simple model, the members themselves are not placed within the object.
     *          Only pointers addressing the members are placed within the object.
     *          Doing this avoids problems from members being quite different types and requiring
     *          different amounts (and sometimes different types of) storage.
     *       b) It designed to minimize the complexity of the compiler at the expense of space and runtime
     *          efficiency. Although this model is not used in practice, this simple concept of an index
     *          or slot number is the one that has been developed into the C++ pointer-to-member concept.
     *
     *   Table-driven Object Model
     *     The member function table is a sequence of slots, with each slot addressing a member.
     *     The data member table directly holds the data.
     *     NOTE:
     *       Although this model is not used in practice within C++, the concept of a member function
     *       table has been the traditional implementation supporting efficient runtime resolution
     *       of virtual functions.
     *
     *   The C++ Object Model
     *     Nonstatic data members are allocated directly within each class object.
     *     Static data members are stored outside the individual class object.
     *     Static and nonstatic function members are also hoisted outside the class object.
     *     Virtual functions are supported in two steps:
     *       1. A table of pointers to virtual functions is generated for each class (the virtual table).
     *       2. A single pointer(vptr) to the associated virtual table is inserted within each class object.
     *          The setting, resetting, and not setting of the vptr is handled automatically through code
     *          generated within each class constructor, destructor, and copy assignment operator.
     *          The type_info object associated with each class in support of runtime type identification (RTTI)
     *          is also addressed within the virtual table, usually within the table's first slot.
     *     Virtual base class are supported:
     *       1. A base class table is generated for which each slot contains the address of an associated base class (the virtual table).
     *       2. Each class object would contain a base table pointer(bptr) at some fixed location regardless of the size or number of its base classes.
     *          The primary drawback to this strategy, of course, is both the space and access-time overhead of the indirection.
     *          One benefit is a uniform representation of inheritance within each class object. A second benefit would be the ability
     *          to grow, shrink, or otherwise modify the base class table without changing the size of the class objects themselves.
     */
    namespace CPlusPlusObject {

        class Point
        {
        public:
            Point(float xval) { _x = xval; }
            virtual ~Point() {}
            float x() const { return _x; }
            static int PointCount() { return _point_count; }
        protected:
            virtual std::ostream& print(std::ostream &os) const {
                os << "x:" << _x << " point_count:" << _point_count;
            }
            float _x;
            static int _point_count;
        };

        // single inheritance :
        class Library_materials { };
        class Book : public Library_materials { };
        class Rental_book : public Book { };

        // multiple inheritance:
        class my_iostream :
            public std::istream,
            public std::ostream {
        };

        // In the case of virtual inheritance, only a single occurrence of the base class is maintained (called a subobject) 
        // regardless of how many times the class is derived from within the inheritance chain.
        // iostream, for example, contains only a single instance of the virtual ios base class.
        class my_istream : virtual public std::ios { };
        class my_ostream : virtual public std::ios { };


        //class X defines a copy constructor, virtual destructor, and virtual function foo().
        class X {
        public:
            virtual void foo() { }
            virtual void rotate() const { }
        };

        X foobar() {
            X xx;
            X *px = new X;

            // foo() is virtual function
            xx.foo();
            px->foo();
            delete px;
            return xx;
        };

#ifdef AUTO_TRANSFORM_BY_COMPILER
        // Probable internal transformation (Pseudo C++ code)
        void foobar(X &_result) {
            // construct _result
            // _result replaces local xx ...
            _result.X::X();

            // expand X *px = new X;
            // type_info for X in px->_vtbl[0]
            px = _new(sizeof(X));
            if (px != 0)
                px->X::X();

            // expand xx.foo(): suppress virtual mechanism
            // replace xx with _result
            foo(&_result);

            // expand px->foo() using virtual mechanism
            (*px->_vtbl[2])(px);

            // expand delete px;
            if (px != 0) {
                (*px->_vtbl[1])(px); // destructor
                _delete(px);
            }

            // replace named return statement
            // no need to destroy local object xx
            return;
        };
#endif

        // The memory requirements to represent a class object in general are the following:
        //   1. The accumulated size of its nonstatic data members;
        //   2. Plus any padding(between members or on the aggregate boundary itself) due to alignment constraints(or simple efficiency);
        //   3. Plus any internally generated overhead to support the virtuals;
        class Z : public X {
        public:
            virtual void rotate() const { }
        };

        void rotate(
            X datum,
            const X *pointer,
            const X &reference)
        {
            // cannot determine until run-time
            // actual instance of rotate() invoked
            (*pointer).rotate();
            reference.rotate();
            // always invokes X::rotate()
            datum.rotate();
        }


        void call_rotate() {
            Z z;
            rotate(z, &z, z);

            //The C++ language supports polymorphism in the following ways:
            //  1. Through a set of implicit conversions (a derived class pointer to a pointer of its public base type);
            //  2. Through the virtual function mechanism;
            //  3. Through the dynamic_cast and typeid operators;
            X *px = new X;
            px->rotate();
            Z *pz = dynamic_cast<Z*>(px);
        }

    }

} // namespace Object


#endif // _OBJECT_H_

