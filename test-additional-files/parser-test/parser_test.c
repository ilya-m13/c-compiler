#include <header.h>

struct Struct;
struct Struct {
    int data_;
    int **data_;
    struct Struct data_;
    struct Struct **data_;
};
struct Struct {
    int data_;
};
struct Struct {
    int data_;
} object;

int id() {}
int *id(int arg1) {}
void id(
    int arg1, int *arg2, struct Struct arg3, struct Struct arg4, void *arg5) {}
void *id() {}
struct Struct id() {}
struct Struct *id() {}

int main() {
    call(
        var_,
        array_[var_],
        obj_.member_,
        obj_[var_].member_[var_],
        "str",
        1,
        call(var_),
        ++var_,
        var_ ||
            var_-- &&
                array_[var_] ==
                    ++var_<
                        obj_.member_[var_] + call(arg1_, arg2) * 1 / "str" -
                        var_> var_ != 1 &&
                var_ ||
            1,
        var_ = var_ += var_ *= var_ ||
            var_-- &&
                array_[var_] ==
                    ++var_<obj_.member_[var_] + call() * 1 / "str" - var_>
                        var_ != 1 &&
                var_ ||
            1);
    ++var_;
    var_[var_]++;
    --obj_.member_[var_];
    var_ = var_ += var_ *= var_ ||
        var_-- &&
            array_[var_] ==
                ++var_<obj_.member_[var_] + call() * 1 / "str" - var_> var_ !=
                1 &&
            var_ ||
        1;
    const signed int var_ = var_ ||
        var_-- &&
            array_[var_] ==
                ++var_<obj_.member_[var_] + call() * 1 / "str" - var_> var_ !=
                1 &&
            var_ ||
        1;
    unsigned char var_;
    float var_ = var_;

    const signed double *p_ = var_ ||
        var_-- &&
            array_[var_] ==
                ++var_<obj_.member_[var_] + call() * 1 / "str" - var_> var_ !=
                1 &&
            var_ ||
        1;
    unsigned short **p_;
    long *p_;
    void *p_ = var_;

    int array_[var_];
    unsigned char array_[var_ = var_ * 1];

    struct Struct var_ = {var_ + 1, var_ = var_ * 1};
    struct Struct var_;
    struct Struct *p = var_ + 1;

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