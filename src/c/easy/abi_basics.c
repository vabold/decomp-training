
int abi_function_1(int a, int b) {
    return a + b;
}

int abi_function_2(int a, int b) {
    return a + b;
}

int abi_function_3(int a) {
    some_func();
    return a;
}

void abi_function_4() {
    some_func();
}

// TODO(fox): Put this in header, or use from one that
// exists in math header
typedef struct {
    float x;
    float y;
    float z;
} Vec3;

void abi_function_5(float a) {
    Vec3 pos;
    pos.x = a;
    pos.y = a;
    pos.z = a;
    some_func(&pos);
}

// TODO(fox): This should probably be combined with the branching section

int abi_function_6(int a) {
    switch (a) {
        case 1:
            return 1;
        case 2:
            return 2;
        case 3:
            return 3;
        case 4:
            return 4;
        case 5:
            return 5;
    }
    return 0;
}

int abi_function_7(int a) {
    some_func();
    switch (a) {
        case 1:
            return 1;
        case 2:
            return 2;
        case 3:
            return 3;
        case 4:
            return 4;
        case 5:
            return 5;
    }
    return 0;
}
