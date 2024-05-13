#include "Object.h"
#include "SemanticsOfConstruction_Destruction_Copy.h"
#include "SemanticsOfConstructors_DefaultConstructor.h"
#include "SemanticsOfConstructors_CopyConstructor.h"
#include "SemanticsOfConstructors_ProgramTransformation.h"
#include "SemanticsOfConstructors_MemberInitializationList.h"
#include "SemanticsOfData.h"
#include "SemanticsOfFunction.h"
#include "SemanticsOfRuntime.h"

int main(int argc, char* argv[]) {
    SemanticsOfData::print();
    SemanticsOfData::DataMemberLayout::do_member_address();
    SemanticsOfData::AccessOfDataMember::do_point();
    SemanticsOfData::AccessOfDataMember::do_point3d();
    SemanticsOfData::InheritanceAndDataMember::WithoutPolymorphism::do_concrete();
    SemanticsOfData::InheritanceAndDataMember::MultipleInheritance::do_vertex3d();
    SemanticsOfData::PointerToDataMembers::do_print3d();
    SemanticsOfFunction::PointerToMemberFunctions::do_point();
    SemanticsOfFunction::PointerToMemberFunctions::do_virtual();

    SemanticsOfConstruction_Destruction_Copy::Instance::foo();
    SemanticsOfRuntime::Instance::do_xy();

    system("pause");
    return 0;
}