
int printf(const char* format,...);

int global_init_var = 84;
int global_uninit_var;
static int static_global_init = 10;
static int static_global_unint;

void func1(int i) {
    printf("%d\n", i);
}

int main() {
    static int static_var = 85;
    static int stativ_car2;

    int a = 1;
    int b;
    int c = 0;
    func1(10);
    return 0;
}
