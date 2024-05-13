#ifndef _SEMANTICS_OF_CONSTRUCTION_DESTRUCTION_COPY_H_
#define _SEMANTICS_OF_CONSTRUCTION_DESTRUCTION_COPY_H_


#include <iostream>

namespace SemanticsOfConstruction_Destruction_Copy {

    namespace Instance {

        class Abstract_base {
        public:
            // A pure virtual destructor must be defined by the class designer. 
            // Every derived class destructor is internally augmented to statically invoke each of its virtual base and immediate base class destructors.
            // The absence of a definition of any of the base class destructors in general results in a link-time error.
            // A better design alternative is to not declare a virtual destructor as pure.
            virtual ~Abstract_base() {}
            // Determining the const-ness of a virtual function may seem rather trivial, but in practice it is not easy to do within an abstract base class.
            // In my code, I tend toward leaving off the const (virtual void interface() const = 0;).
            virtual void interface() = 0;
            // In general, it is still a bad design choice to declare all functions virtual and to depend on the compiler to optimize away unnecessary virtual invocations.
            const char* mumble() const { return _mumble; }
        protected:
            // In general, the data members of a class should be initialized and assigned to only within the constructor and other member functions of that class.
            // To do otherwise breaks encapsulation, thereby making maintenance and modification of the class more difficult.
            Abstract_base(char *pc = 0) : _mumble(pc) {}
            char *_mumble;
        };

        // define a pure virtual function
        inline void Abstract_base::interface() {
        }

        class Concrete_derived : public Abstract_base {
        public:
            Concrete_derived() {}
            void interface() {
                // invoke a pure virtual function (invoked statically and not through the virtual mechanism)
                Abstract_base::interface();
            }
        };

        void foo() {
            Concrete_derived trouble;
        }

    } // namespace Instance

    namespace ObjectConstructionWithoutInheritance {

    } // namespace ObjectConstructionWithoutInheritance

    namespace ObjectConstructionUnderInheritance {

    } // namespace ObjectConstructionUnderInheritance

    namespace ObjectCopy {

        class Point {
        public:
            Point(float x = 0.0, float y = 0.0) : _x(x), _y(y) {}

            // only for test copy assignment operator
            Point& operator=(const Point &p) {
                _x = p._x;
                _y = p._y;
                return *this;
            }

        private:
            // - Disallowing copy assignment operator
            //   Disallowing the copying of one class object with another is accomplished by declaring the copy assignment operator private
            //   and not providing a definition. eg. Point(const Point& p);
            // - Default copy assignment operator
            //   If all we want to support is the simple assignment of one Point object to another, then the default behavior is both
            //   sufficient and efficient and there is no reason to provide an explicit instance of the copy assignment operator.
            // - user's copy assignment operator
            //   A copy assignment operator is necessary only if the default behavior results in semantics that are either unsafe or incorrect.

        protected:
            float _x, _y;
        };

        class Point3d : virtual public Point {
        public:
            Point3d(float x = 0.0, float y = 0.0, float z = 0.0) : Point(x, y), _z(z) {}

#ifdef AUTO_TRANSFORM_BY_COMPILER
            // Pseudo C++ Code: synthesized copy assignment operator
            Point3d& operator=(Point3d *const this, const Point3d &p) {
                // invoke the base class instance
                this->Point::operator=(p);
                // memberwise copy the derived class members
                _z = p._z;
                return *this;
            }
#endif

        protected:
            float _z;
        };


        class Vertex : public virtual Point {
        public:
#ifdef AUTO_TRANSFORM_BY_COMPILER
            // class Vertex : virtual public Point
            Vertex& operator=(const Vertex &v) {
                this->Point::operator=(v);
                _next = v._next;
                return *this;
            }
#endif

        private:
            Vertex *_next;
        };

        class Vertex3d : public Point3d, public Vertex {
        public:
            // only for test copy assignment operator
            // (Recommend not permitting the copy operation of a virtual base class whenever possible.)
            // (An even stronger recommendation : Do not declare data within any class that serves as a virtual base class.)
            Vertex3d& operator=(const Vertex3d &v) {
                this->Point::operator=(v);
                this->Point3d::operator=(v); // will call this->Point::operator=(v); again
                this->Vertex::operator=(v);  // will call this->Point::operator=(v); again
                return *this;
            }

        private:
            float mumble;
        };

    } // namespace ObjectCopy

    namespace ObjectDestruction {

    } // namespace ObjectDestruction

} // namespace SemanticsOfConstruction_Destruction_Copy


#endif // _SEMANTICS_OF_CONSTRUCTION_DESTRUCTION_COPY_H_

