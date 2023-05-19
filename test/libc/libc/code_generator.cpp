#include <gtest/gtest.h>

#include <libc/analyzer.hpp>
#include <libc/code_generator.hpp>
#include <libc/parser.hpp>
#include <libc/symtab.hpp>

#include <sstream>

// NOLINTBEGIN(readability-function-cognitive-complexity)

TEST(Generator, FuncDefinition) {
    std::stringstream correct("target triple = \"x86_64-pc-linux-gnu\"\n\n\n"
                              "define void @func() {\n"
                              "entry:\n\n"
                              "}\n\n"
                              "define i32 @main(i32 %argc, i8** %argv) {\n"
                              "entry:\n"
                              "\t%argc.addr0 = alloca i32\n"
                              "\tstore i32 %argc, i32* %argc.addr0\n"
                              "\t%argv.addr1 = alloca i8**\n"
                              "\tstore i8** %argv, i8*** %argv.addr1\n\n"
                              "\tret i32 0\n"
                              "}\n\n");
    std::stringstream in("void func() {}\n\n"
                         "int main(int argc, char **argv) {\n"
                         "return 0;\n"
                         "}");
    std::stringstream out;

    auto parser_result = c::parse(in);
    if (!parser_result.errors_.empty()) {
        FAIL();
    }

    c::ast::symtab::Symtab symtab;
    ASSERT_NO_THROW({ symtab = c::get_symtab(parser_result.program_); });

    ASSERT_NO_THROW({ c::analyze(parser_result.program_, symtab); });

    c::generate(out, parser_result.program_, symtab);

    EXPECT_STREQ(out.str().c_str(), correct.str().c_str());
}

TEST(Generator, CastVar) {
    std::stringstream correct(
        "target triple = \"x86_64-pc-linux-gnu\"\n\n"
        "declare i32 @printf(i8*, ...)\n\n"
        "@.str0 = private unnamed_addr constant [4 x i8] c\"%f\\0A\\00\"\n\n"
        "define i32 @main(i32 %argc, i8** %argv) {\n"
        "entry:\n"
        "\t%argc.addr0 = alloca i32\n"
        "\tstore i32 %argc, i32* %argc.addr0\n"
        "\t%argv.addr1 = alloca i8**\n"
        "\tstore i8** %argv, i8*** %argv.addr1\n\n"
        "\t%_char.addr2 = alloca i8\n"
        "\tstore i8 1, i8* %_char.addr2\n\n"
        "\t%_short.addr3 = alloca i16\n"
        "\tstore i16 2, i16* %_short.addr3\n\n"
        "\t%_int.addr4 = alloca i32\n"
        "\tstore i32 3, i32* %_int.addr4\n\n"
        "\t%_long.addr5 = alloca i64\n"
        "\tstore i64 4, i64* %_long.addr5\n\n"
        "\t%_float.addr6 = alloca float\n"
        "\tstore float 5.0, float* %_float.addr6\n\n"
        "\t%_double.addr7 = alloca double\n"
        "\tstore double 6.0, double* %_double.addr7\n\n"
        "\t%tmp5 = load i16, i16* %_short.addr3\n"
        "\t%tmp6 = load i32, i32* %_int.addr4\n"
        "\t%tmp7 = trunc i32 %tmp6 to i16\n"
        "\tstore i16 %tmp7, i16* %_short.addr3\n\n"
        "\t%tmp8 = load i32, i32* %_int.addr4\n"
        "\t%tmp9 = load i16, i16* %_short.addr3\n"
        "\t%tmp10 = sext i16 %tmp9 to i32\n"
        "\tstore i32 %tmp10, i32* %_int.addr4\n\n"
        "\t%tmp11 = load float, float* %_float.addr6\n"
        "\t%tmp12 = load double, double* %_double.addr7\n"
        "\t%tmp13 = fptrunc double %tmp12 to float\n"
        "\tstore float %tmp13, float* %_float.addr6\n\n"
        "\t%tmp14 = load double, double* %_double.addr7\n"
        "\t%tmp15 = load float, float* %_float.addr6\n"
        "\t%tmp16 = fpext float %tmp15 to double\n"
        "\tstore double %tmp16, double* %_double.addr7\n\n"
        "\t%tmp17 = load i32, i32* %_int.addr4\n"
        "\t%tmp18 = load double, double* %_double.addr7\n"
        "\t%tmp19 = fptosi double %tmp18 to i32\n"
        "\tstore i32 %tmp19, i32* %_int.addr4\n\n"
        "\t%tmp20 = load double, double* %_double.addr7\n"
        "\t%tmp21 = load i32, i32* %_int.addr4\n"
        "\t%tmp22 = sitofp i32 %tmp21 to double\n"
        "\tstore double %tmp22, double* %_double.addr7\n\n"
        "\t%tmp23 = load i8, i8* %_char.addr2\n"
        "\t%tmp24 = load i16, i16* %_short.addr3\n"
        "\t%tmp25 = load i32, i32* %_int.addr4\n"
        "\t%tmp26 = sext i16 %tmp24 to i32\n"
        "\t%tmp27 = add i32 %tmp26, %tmp25\n"
        "\t%tmp28 = trunc i32 %tmp27 to i8\n"
        "\tstore i8 %tmp28, i8* %_char.addr2\n\n"
        "\t%tmp29 = load i32, i32* %_int.addr4\n"
        "\t%tmp30 = load i16, i16* %_short.addr3\n"
        "\t%tmp31 = load i8, i8* %_char.addr2\n"
        "\t%tmp32 = sext i8 %tmp31 to i16\n"
        "\t%tmp33 = add i16 %tmp30, %tmp32\n"
        "\t%tmp34 = sext i16 %tmp33 to i32\n"
        "\tstore i32 %tmp34, i32* %_int.addr4\n\n"
        "\t%tmp35 = load float, float* %_float.addr6\n"
        "\t%tmp36 = load double, double* %_double.addr7\n"
        "\t%tmp37 = load float, float* %_float.addr6\n"
        "\t%tmp38 = fpext float %tmp37 to double\n"
        "\t%tmp39 = fadd double %tmp36, %tmp38\n"
        "\t%tmp40 = fptrunc double %tmp39 to float\n"
        "\tstore float %tmp40, float* %_float.addr6\n\n"
        "\t%tmp41 = load double, double* %_double.addr7\n"
        "\t%tmp42 = load float, float* %_float.addr6\n"
        "\t%tmp43 = load double, double* %_double.addr7\n"
        "\t%tmp44 = fpext float %tmp42 to double\n"
        "\t%tmp45 = fadd double %tmp44, %tmp43\n"
        "\tstore double %tmp45, double* %_double.addr7\n\n"
        "\t%tmp46 = load i32, i32* %_int.addr4\n"
        "\t%tmp47 = load float, float* %_float.addr6\n"
        "\t%tmp48 = load i16, i16* %_short.addr3\n"
        "\t%tmp49 = sitofp i16 %tmp48 to float\n"
        "\t%tmp50 = fadd float %tmp47, %tmp49\n"
        "\t%tmp51 = fptosi float %tmp50 to i32\n"
        "\tstore i32 %tmp51, i32* %_int.addr4\n\n"
        "\t%tmp52 = load double, double* %_double.addr7\n"
        "\t%tmp53 = load i32, i32* %_int.addr4\n"
        "\t%tmp54 = load float, float* %_float.addr6\n"
        "\t%tmp55 = sitofp i32 %tmp53 to float\n"
        "\t%tmp56 = fadd float %tmp55, %tmp54\n"
        "\t%tmp57 = fpext float %tmp56 to double\n"
        "\tstore double %tmp57, double* %_double.addr7\n\n"
        "\t%tmp58 = load i8, i8* %_char.addr2\n"
        "\t%tmp60 = add i8 %tmp58, 1\n"
        "\tstore i8 %tmp60, i8* %_char.addr2\n\n"
        "\t%tmp61 = load i16, i16* %_short.addr3\n"
        "\t%tmp63 = sub i16 %tmp61, 1\n"
        "\tstore i16 %tmp63, i16* %_short.addr3\n\n"
        "\t%tmp64 = load i32, i32* %_int.addr4\n"
        "\t%tmp65 = mul i32 %tmp64, 2\n"
        "\tstore i32 %tmp65, i32* %_int.addr4\n\n"
        "\t%tmp66 = load i64, i64* %_long.addr5\n"
        "\t%tmp68 = sdiv i64 %tmp66, 2\n"
        "\tstore i64 %tmp68, i64* %_long.addr5\n\n"
        "\t%tmp69 = load i32, i32* %_int.addr4\n"
        "\t%tmp70 = srem i32 %tmp69, 100\n"
        "\tstore i32 %tmp70, i32* %_int.addr4\n\n"
        "\t%tmp71 = load float, float* %_float.addr6\n"
        "\t%tmp73 = fadd float %tmp71, 1.0\n"
        "\tstore float %tmp73, float* %_float.addr6\n\n"
        "\t%tmp74 = load double, double* %_double.addr7\n"
        "\t%tmp76 = fsub double %tmp74, 1.0\n"
        "\tstore double %tmp76, double* %_double.addr7\n\n"
        "\t%tmp77 = load float, float* %_float.addr6\n"
        "\t%tmp79 = fmul float %tmp77, 2.0\n"
        "\tstore float %tmp79, float* %_float.addr6\n\n"
        "\t%tmp80 = load double, double* %_double.addr7\n"
        "\t%tmp82 = fdiv double %tmp80, 2.0\n"
        "\tstore double %tmp82, double* %_double.addr7\n\n"
        "\t%tmp83 = load i8, i8* %_char.addr2\n"
        "\t%tmp84 = load i16, i16* %_short.addr3\n"
        "\t%tmp85 = sext i8 %tmp83 to i16\n"
        "\t%tmp86 = add i16 %tmp85, %tmp84\n"
        "\t%tmp87 = load i32, i32* %_int.addr4\n"
        "\t%tmp88 = sext i16 %tmp86 to i32\n"
        "\t%tmp89 = add i32 %tmp88, %tmp87\n"
        "\t%tmp90 = load i64, i64* %_long.addr5\n"
        "\t%tmp91 = sext i32 %tmp89 to i64\n"
        "\t%tmp92 = add i64 %tmp91, %tmp90\n"
        "\t%tmp93 = load float, float* %_float.addr6\n"
        "\t%tmp94 = sitofp i64 %tmp92 to float\n"
        "\t%tmp95 = fadd float %tmp94, %tmp93\n"
        "\t%tmp96 = load double, double* %_double.addr7\n"
        "\t%tmp97 = fpext float %tmp95 to double\n"
        "\t%tmp98 = fadd double %tmp97, %tmp96\n"
        "\t%tmp99 = call i32 (i8*, ...) @printf(i8* getelementptr ([4 x i8], "
        "[4 x i8]* @.str0, i64 0, i64 0), double %tmp98)\n\n"
        "\tret i32 0\n"
        "}\n\n");
    std::stringstream in(
        "#include <stdio.h>\nint main(int argc, char **argv) {\n    char _char "
        "= 1;\n    short _short = 2;\n    int _int = 3;\n    long _long = 4;\n "
        "   float _float = 5;\n    double _double = 6;\n    _short = _int;\n   "
        " _int = _short;\n    _float = _double;\n    _double = _float;\n    "
        "_int = _double;\n    _double = _int;\n    _char = _short + _int;\n    "
        "_int = _short + _char;\n    _float = _double + _float;\n    _double = "
        "_float + _double;\n    _int = _float + _short;\n    _double = _int + "
        "_float;\n    _char += 1;\n    _short -= 1;\n    _int *= 2;\n    _long "
        "/= 2;\n    _int %= 100;\n    _float += 1;\n    _double -= 1;\n    "
        "_float *= 2;\n    _double /= 2;\n    printf(\"%f\n\", _char + "
        "_short + _int + _long + _float + _double);\n    return "
        "0;\n}");
    std::stringstream out;

    auto parser_result = c::parse(in);
    if (!parser_result.errors_.empty()) {
        FAIL();
    }

    c::ast::symtab::Symtab symtab;
    ASSERT_NO_THROW({ symtab = c::get_symtab(parser_result.program_); });

    ASSERT_NO_THROW({ c::analyze(parser_result.program_, symtab); });

    c::generate(out, parser_result.program_, symtab);

    EXPECT_STREQ(out.str().c_str(), correct.str().c_str());
}

TEST(Generator, FuncCall) {
    std::stringstream correct(
        "target triple = \"x86_64-pc-linux-gnu\"\n\n"
        "declare i32 @printf(i8*, ...)\n\n"
        "@.str0 = private unnamed_addr constant [4 x i8] c\"%d\\0A\\00\"\n"
        "@.str1 = private unnamed_addr constant [4 x i8] c\"%f\\0A\\00\"\n\n"
        "define double @sum(double %arg1, double %arg2) {\n"
        "entry:\n"
        "\t%arg1.addr0 = alloca double\n"
        "\tstore double %arg1, double* %arg1.addr0\n"
        "\t%arg2.addr1 = alloca double\n"
        "\tstore double %arg2, double* %arg2.addr1\n\n"
        "\t%tmp0 = load double, double* %arg1.addr0\n"
        "\t%tmp1 = load double, double* %arg2.addr1\n"
        "\t%tmp2 = fadd double %tmp0, %tmp1\n"
        "\tret double %tmp2\n"
        "}\n\n"
        "define i32 @main(i32 %argc, i8** %argv) {\n"
        "entry:\n"
        "\t%argc.addr2 = alloca i32\n"
        "\tstore i32 %argc, i32* %argc.addr2\n"
        "\t%argv.addr3 = alloca i8**\n"
        "\tstore i8** %argv, i8*** %argv.addr3\n\n"
        "\t%arg1.addr4 = alloca i32\n"
        "\tstore i32 10, i32* %arg1.addr4\n\n"
        "\t%res.addr5 = alloca i32\n"
        "\t%tmp3 = load i32, i32* %arg1.addr4\n"
        "\t%tmp4 = sitofp i32 %tmp3 to double\n"
        "\t%tmp6 = call double @sum(double %tmp4, double 20.0)\n"
        "\t%tmp7 = fptosi double %tmp6 to i32\n"
        "\tstore i32 %tmp7, i32* %res.addr5\n\n"
        "\t%tmp8 = load i32, i32* %res.addr5\n"
        "\t%tmp9 = call i32 (i8*, ...) @printf(i8* getelementptr ([4 x i8], [4 "
        "x i8]* @.str0, i64 0, i64 0), i32 %tmp8)\n\n"
        "\t%tmp12 = call double @sum(double 50.0, double 50.0)\n"
        "\t%tmp13 = call i32 (i8*, ...) @printf(i8* getelementptr ([4 x i8], "
        "[4 x i8]* @.str1, i64 0, i64 0), double %tmp12)\n\n"
        "\tret i32 0\n"
        "}\n\n");
    std::stringstream in(
        "#include <stdio.h>\ndouble sum(double arg1, double arg2) {\n    "
        "return arg1 + arg2;\n}\nint main(int argc, char **argv) {\n    int "
        "arg1 = 10;\n    int res = sum(arg1, 20);\n    printf(\"%d\\n\", "
        "res);\n    printf(\"%f\\n\", sum(50, 50));\n    return 0;\n}");
    std::stringstream out;

    auto parser_result = c::parse(in);
    if (!parser_result.errors_.empty()) {
        FAIL();
    }

    c::ast::symtab::Symtab symtab;
    ASSERT_NO_THROW({ symtab = c::get_symtab(parser_result.program_); });

    ASSERT_NO_THROW({ c::analyze(parser_result.program_, symtab); });

    c::generate(out, parser_result.program_, symtab);

    EXPECT_STREQ(out.str().c_str(), correct.str().c_str());
}

TEST(Generator, Statements) {
    std::stringstream correct(
        "target triple = \"x86_64-pc-linux-gnu\"\n\n"
        "declare i32 @printf(i8*, ...)\n\n"
        "@.str0 = private unnamed_addr constant [4 x i8] c\"%f\\0A\\00\"\n\n"
        "define i32 @main(i32 %argc, i8** %argv) {\n"
        "entry:\n"
        "\t%argc.addr0 = alloca i32\n"
        "\tstore i32 %argc, i32* %argc.addr0\n"
        "\t%argv.addr1 = alloca i8**\n"
        "\tstore i8** %argv, i8*** %argv.addr1\n\n"
        "\t%size.addr2 = alloca i32\n"
        "\tstore i32 2, i32* %size.addr2\n\n"
        "\t%_int.addr3 = alloca i32, i64 2\n\n"
        "\t%tmp1 = load i32, i32* %size.addr2\n"
        "\t%tmp2 = sext i32 %tmp1 to i64\n"
        "\t%_double.addr4 = alloca double, i64 %tmp2\n\n"
        "\t%i.addr5 = alloca i32\n"
        "\tstore i32 0, i32* %i.addr5\n"
        "\tbr label %block0\n\n"
        "block0:\n"
        "\t%tmp3 = load i32, i32* %i.addr5\n"
        "\t%tmp4 = load i32, i32* %size.addr2\n"
        "\t%tmp5 = icmp slt i32 %tmp3, %tmp4\n"
        "\tbr i1 %tmp5, label %block1, label %block2\n\n"
        "block1:\n"
        "\t%tmp6 = load i32, i32* %i.addr5\n"
        "\t%tmp7 = sext i32 %tmp6 to i64\n"
        "\t%tmp8 = getelementptr i32, i32* %_int.addr3, i64 %tmp7\n"
        "\t%tmp9 = load i32, i32* %tmp8\n"
        "\t%tmp10 = load i32, i32* %i.addr5\n"
        "\tstore i32 %tmp10, i32* %tmp8\n\n"
        "\t%tmp11 = load i32, i32* %i.addr5\n"
        "\t%tmp12 = sub i32 %tmp11, 1\n"
        "\t%tmp13 = icmp sge i32 %tmp12, 0\n"
        "\tbr i1 %tmp13, label %block4, label %block5\n\n"
        "block4:\n"
        "\t%tmp14 = load i32, i32* %i.addr5\n"
        "\t%tmp15 = sext i32 %tmp14 to i64\n"
        "\t%tmp16 = getelementptr double, double* %_double.addr4, i64 %tmp15\n"
        "\t%tmp17 = load double, double* %tmp16\n"
        "\t%tmp18 = load i32, i32* %i.addr5\n"
        "\t%tmp19 = load i32, i32* %i.addr5\n"
        "\t%tmp20 = sext i32 %tmp19 to i64\n"
        "\t%tmp21 = getelementptr i32, i32* %_int.addr3, i64 %tmp20\n"
        "\t%tmp22 = load i32, i32* %tmp21\n"
        "\t%tmp23 = mul i32 %tmp18, %tmp22\n"
        "\t%tmp24 = load i32, i32* %i.addr5\n"
        "\t%tmp25 = sub i32 %tmp24, 1\n"
        "\t%tmp26 = sext i32 %tmp25 to i64\n"
        "\t%tmp27 = getelementptr i32, i32* %_int.addr3, i64 %tmp26\n"
        "\t%tmp28 = load i32, i32* %tmp27\n"
        "\t%tmp29 = add i32 %tmp23, %tmp28\n"
        "\t%tmp31 = getelementptr i32, i32* %_int.addr3, i64 0\n"
        "\t%tmp32 = load i32, i32* %tmp31\n"
        "\t%tmp33 = add i32 %tmp29, %tmp32\n"
        "\t%tmp34 = sitofp i32 %tmp33 to double\n"
        "\tstore double %tmp34, double* %tmp16\n\n"
        "\tbr label %block3\n"
        "\tbr label %block5\n\n"
        "block5:\n"
        "\t%tmp35 = load i32, i32* %i.addr5\n"
        "\t%tmp36 = sext i32 %tmp35 to i64\n"
        "\t%tmp37 = getelementptr double, double* %_double.addr4, i64 %tmp36\n"
        "\t%tmp38 = load double, double* %tmp37\n"
        "\t%tmp39 = load i32, i32* %i.addr5\n"
        "\t%tmp40 = load i32, i32* %i.addr5\n"
        "\t%tmp41 = sext i32 %tmp40 to i64\n"
        "\t%tmp42 = getelementptr i32, i32* %_int.addr3, i64 %tmp41\n"
        "\t%tmp43 = load i32, i32* %tmp42\n"
        "\t%tmp44 = mul i32 %tmp39, %tmp43\n"
        "\t%tmp46 = getelementptr i32, i32* %_int.addr3, i64 0\n"
        "\t%tmp47 = load i32, i32* %tmp46\n"
        "\t%tmp48 = add i32 %tmp44, %tmp47\n"
        "\t%tmp49 = sitofp i32 %tmp48 to double\n"
        "\tstore double %tmp49, double* %tmp37\n\n"
        "\tbr label %block3\n\n"
        "block3:\n"
        "\t%tmp50 = load i32, i32* %i.addr5\n"
        "\t%tmp51 = add i32 %tmp50, 1\n"
        "\tstore i32 %tmp51, i32* %i.addr5\n"
        "\tbr label %block0\n\n"
        "block2:\n"
        "\t%sum.addr6 = alloca double\n"
        "\tstore double 0.0, double* %sum.addr6\n\n"
        "\t%i.addr7 = alloca i32\n"
        "\tstore i32 0, i32* %i.addr7\n"
        "\tbr label %block6\n\n"
        "block6:\n"
        "\t%tmp53 = load i32, i32* %i.addr7\n"
        "\t%tmp54 = load i32, i32* %size.addr2\n"
        "\t%tmp55 = icmp slt i32 %tmp53, %tmp54\n"
        "\tbr i1 %tmp55, label %block7, label %block8\n\n"
        "block7:\n"
        "\t%tmp56 = load double, double* %sum.addr6\n"
        "\t%tmp57 = load i32, i32* %i.addr7\n"
        "\t%tmp58 = sext i32 %tmp57 to i64\n"
        "\t%tmp59 = getelementptr i32, i32* %_int.addr3, i64 %tmp58\n"
        "\t%tmp60 = load i32, i32* %tmp59\n"
        "\t%tmp61 = load i32, i32* %i.addr7\n"
        "\t%tmp62 = sext i32 %tmp61 to i64\n"
        "\t%tmp63 = getelementptr double, double* %_double.addr4, i64 %tmp62\n"
        "\t%tmp64 = load double, double* %tmp63\n"
        "\t%tmp65 = sitofp i32 %tmp60 to double\n"
        "\t%tmp66 = fadd double %tmp65, %tmp64\n"
        "\t%tmp67 = fadd double %tmp56, %tmp66\n"
        "\tstore double %tmp67, double* %sum.addr6\n\n"
        "\tbr label %block9\n\n"
        "block9:\n"
        "\t%tmp68 = load i32, i32* %i.addr7\n"
        "\t%tmp69 = add i32 %tmp68, 1\n"
        "\tstore i32 %tmp69, i32* %i.addr7\n"
        "\tbr label %block6\n\n"
        "block8:\n"
        "\t%tmp70 = load double, double* %sum.addr6\n"
        "\t%tmp71 = call i32 (i8*, ...) @printf(i8* getelementptr ([4 x i8], "
        "[4 x i8]* @.str0, i64 0, i64 0), double %tmp70)\n\n"
        "\tret i32 0\n"
        "}\n\n");
    std::stringstream in(
        "#include <stdio.h>\nint main(int argc, char **argv) {\n    int size = "
        "2;\n    int _int[2];\n    double _double[size];\n    for (int i = 0; "
        "i < size; i += 1) {\n        _int[i] = i;\n        if (i - 1 >= 0) "
        "{\n            _double[i] = i * _int[i] + _int[i - 1] + _int[0];\n    "
        "        continue;\n        }\n        _double[i] = i * _int[i] + "
        "_int[0];\n    }\n    double sum = 0;\n    for (int i = 0; i < size; i "
        "+= 1) {\n        sum += _int[i] + _double[i];\n    }\n    "
        "printf(\"%f\\n\", sum);\n    return 0;\n}");
    std::stringstream out;

    auto parser_result = c::parse(in);
    if (!parser_result.errors_.empty()) {
        FAIL();
    }

    c::ast::symtab::Symtab symtab;
    ASSERT_NO_THROW({ symtab = c::get_symtab(parser_result.program_); });

    ASSERT_NO_THROW({ c::analyze(parser_result.program_, symtab); });

    c::generate(out, parser_result.program_, symtab);

    EXPECT_STREQ(out.str().c_str(), correct.str().c_str());
}

// NOLINTEND(readability-function-cognitive-complexity)