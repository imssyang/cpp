#include "GeneralFactory.hpp"

//class Base {public: Base():_a(123) { printf("Base\n");} int _a; };
//class DerivedA : public Base { public: DerivedA(): Base() {printf("DerivedA\n");} };

class Base {public: Base(int a):_a(a) { printf("Base\n");} int _a; };
class DerivedA : public Base { public: DerivedA(int a): Base(a) {printf("DerivedA\n");} };
//class DerivedB : public Base { public: DerivedB() {printf("DerivedB\n");} };

int main(int argc, char *argv[])
{
    GeneralFactory<std::string, std::shared_ptr<Base>, int> factory;
    factory.Register<DerivedA>("DerivedA");
    auto a_obj = factory.Create("DerivedA", 123456);
    auto a_obj2 = (DerivedA*)a_obj.get();
    std::cout << a_obj << " | " << a_obj2->_a << std::endl;
    //factory.Register<DerivedB>("DerivedB");
    //auto b_obj = factory.Create("DerivedB");
    return 0;
}
