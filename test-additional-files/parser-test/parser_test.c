#include <header.h>

int id() {}
int *id(int arg1) {}
void id(int arg1, int *arg2, void *arg3) {}
void *id() {}

int main() {
    call(
        var_,
        array_[var_],
        "str",
        1,
        call(var_),
        var_ + array_[var_] ==
            var_<var_ + call(arg1_, arg2) * 1 / "str" - var_> var_ !=
            1 * var_ + 1,
        var_ = var_ += var_ *= var_ + array_[var_] ==
            var_<var_ + call() * 1 / "str" - var_> var_ != 1 * var_ + 1);
    var_ = var_ += var_ *= var_ + array_[var_] ==
        var_<var_ + call() * 1 / "str" - var_> var_ != 1 * var_ + 1;
    const int var_ = var_ + array_[var_] ==
        var_<var_ + call() * 1 / "str" - var_> var_ != 1 * var_ + 1;
    char var_;
    float var_ = var_;

    const double *p_ = var_ + array_[var_] ==
        var_<var_ + call() * 1 / "str" - var_> var_ != 1 * var_ + 1;
    short **p_;
    long *p_;
    void *p_ = var_;

    int array_[var_];
    char array_[var_ = var_ * 1];

    return var_ + 1;

    for (;;) {
    }
    for (int i_ = 0;;) {
    }
    for (; var_ < 1;) {
    }
    for (;; a_ += 1) {
    }
    for (i_ = 0; var_ < 1; var_) {
        call();
        for (;;) {
            int var_ = 0;
        }
    }

    if (var_ < 1) {
        int var_ = 0;
        if (var_ < 1) {
            int var_ = 0;
        }
    }
    if (var_ < 1) {
    }

    continue;

    break;

    // clang-format off
    {
        int var_ = 0;
        { int var_ = 0; }
        int var_ = 0;
    }
    {
        {
            { int var_ = 0; }
        }
    }
    {}
// clang-format on
}