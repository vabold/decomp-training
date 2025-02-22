
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

// TODO(fox): Put this in header
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
