#include <iostream>
#include <string>

using namespace std;

class Foo {
private:
    string _filename;
    void getName() {
        cout << _filename << endl;
    }

public:
    Foo(string filename) {
        _filename = filename;
        getName();
    }
};

int main() {
    Foo f("foo");
    return 0;
}