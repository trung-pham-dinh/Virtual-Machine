#include "VM.h"
using namespace std;

void test(string filename) {
    VM *vm = new VM();
    try {
        vm->run(filename);
    }
    catch (exception& e) {
        cout << e.what();
    }
    delete vm;
}

int main(int argc, char **argv)
{
    if (argc < 2)
        return 1;
    test(argv[11]);
    return 0;
}
