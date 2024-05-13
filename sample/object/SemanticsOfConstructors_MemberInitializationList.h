#ifndef _SEMANTICS_OF_CONSTRUCTORS__MEMBER_INITIALIZATION_LIST_H_
#define _SEMANTICS_OF_CONSTRUCTORS__MEMBER_INITIALIZATION_LIST_H_


#include <iostream>
#include <string>

/**
 * The Semantics of Constructors
 */
namespace SemanticsOfConstructors {

    /**
     *  Member Initialization List
     *    You must use the member initialization list in the following cases in order for your program to compile:
     *    1. When initializing a reference member
     *    2. When initializing a const member
     *    3. When invoking a base or member class constructor with a set of arguments
     */
    namespace MemberInitializationList {

        // In the fourth case, the program compiles and executes correctly. But it does so inefficiently. 
        class Word {
        public:
            // not wrong, just naive ...
            Word() {
                _name = "";
                _cnt = 0;
            }
        private:
            std::string _name;
            int _cnt;
        };

#ifdef AUTO_TRANSFORM_BY_COMPILER
        // Pseudo C++ Code
        Word::Word( /* this pointer goes here */) {
            // invoke default String constructor
            _name.string::string();
            // generate temporary
            string temp = string("");
            // memberwise copy _name
            _name.string::operator=(temp);
            // destroy temporary
            temp.string::~string();
            _cnt = 0;
        }
#endif

        class Word2 {
        public:
            // preferred implementation
            Word2() : _name("") {
                _cnt = 0;
            }
        private:
            std::string _name;
            int _cnt;
        };

#ifdef AUTO_TRANSFORM_BY_COMPILER
        // Pseudo C++ Code
        Word2::Word2( /* this pointer goes here */) {
            // invoke String( int ) constructor
            _name.string::string("");
            _cnt = 0;
        }
#endif

        // the fourth case is most likely to occur in template code 
        template <class T>
        class Foo {
        public:
            void foo(T t) {
                // may or may not be a good idea
                // depending on the actual type of type
                _t = t;
            }
        private:
            T _t;
        };

        class Word3 {
        public:
            // some programmers to insist rather aggressively that all member initialization be done within the member initialization list, 
            // even the initialization of a well-behaved member such as _cnt :
            // NOTE:
            //   recommend always placing the initialization of one member with another within the body of the constructor
            Word3() : _cnt(0), _name("") {
                _tmp = _cnt;
            }
        private:
            std::string _name;
            int _tmp;
            int _cnt;
        };

#ifdef AUTO_TRANSFORM_BY_COMPILER
        // The order in which the list entries are set down is determined by the declaration order of the members within the class declaration,
        // not the order within the initialization list, and the initialization list entries are placed before explicit user code.
        // Pseudo C++ Code
        Word3::Word3( /* this pointer goes here */) {
            _name.string::string("");
            _cnt = 0;
            _tmp = _cnt;
        }
#endif

        class X {
        public:
            // preferred idiom
            X(int val) : j(val) {
                i = j;
            }
            // is the invocation of X::xfoo() ok?
            // The answer is yes, but by placing xfoo() within the constructor body, you can ensure there is no ambiguity
            // about which members are initialized at the point of its invocation.
            X(int v1, int v2) : i(xfoo(v1)), j(v2) {
            }
            int xfoo(int val) {
                return val;
            }
        private:
            int i;
            int j;
        };

#ifdef AUTO_TRANSFORM_BY_COMPILER
        // Pseudo C++ Code: constructor augmentation
        X::X( /* this pointer, */ int v1, int v2) {
            i = this->xfoo(v1);
            j = v2;
        }
#endif

        // is the invocation of FooBar::fval() ok?
        class FooBar : public X {
            int _fval;
        public:
            int fval() { return _fval; }
            FooBar(int val) : _fval(val), X(fval(), val)
            {}
        };

#ifdef AUTO_TRANSFORM_BY_COMPILER
        // Pseudo C++ Code
        FooBar::FooBar( /* this pointer goes here */) {
            // Oops: definitely not a good idea
            X::X(this, this->fval(), val);
            _fval = val;
        };
#endif

    } // namespace MemberInitializationList

} // namespace SemanticsOfConstructors


#endif // _SEMANTICS_OF_CONSTRUCTORS__MEMBER_INITIALIZATION_LIST_H_

