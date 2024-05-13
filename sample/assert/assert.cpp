#include <cassert>

int main()
{
    int* ptr = nullptr;
    assert(ptr != nullptr && "指针不能为空");
    return 0;
}
