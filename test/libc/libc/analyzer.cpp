#include <gtest/gtest.h>

#include <libc/analyzer.hpp>
#include <libc/parser.hpp>
#include <libc/symtab.hpp>

#include <sstream>

// NOLINTBEGIN(readability-function-cognitive-complexity)

TEST(Analyzer, ValidInput) {
    std::istringstream in("void func(int arg1, int arg2) {}"
                          "void func1() {}"
                          "int main(int argc, char **argv) {"
                          "   func(1, 2);"
                          "   for (int i = 0; i < 5; i += 1) {"
                          "       continue;"
                          "       break;"
                          "       double var1 = 5;"
                          "       float var2 = 5;"
                          "       long var3[5];"
                          "       int j = 1 + var1 + var2 + var3[2];"
                          "   }"
                          "   int var1 = 5;"
                          "   int *p1;"
                          "   double *p2;"
                          "   int arr[10];"
                          "   p1 = p2 = var1 = 5 + p1;"
                          "   var1 = p1 = arr + 5;"
                          "   func(p1, arr);"
                          "   return 0;"
                          "}");

    auto parser_result = c::parse(in);
    ASSERT_TRUE(parser_result.errors_.empty());

    c::ast::symtab::Symtab symtab;
    ASSERT_NO_THROW({ symtab = c::get_symtab(parser_result.program_); });

    EXPECT_NO_THROW({ c::analyze(parser_result.program_, symtab); });
}

TEST(Analyzer, NoReturn) {
    std::istringstream in("void func(int arg1, int arg2) {}"
                          "int main(int argc, char **argv) {"
                          "   func(1, 2);"
                          "}");

    auto parser_result = c::parse(in);
    ASSERT_TRUE(parser_result.errors_.empty());

    c::ast::symtab::Symtab symtab;
    ASSERT_NO_THROW({ symtab = c::get_symtab(parser_result.program_); });

    EXPECT_THROW(
        { c::analyze(parser_result.program_, symtab); },
        c::ast::TypeAnalyzer::Exception);
}

TEST(Analyzer, WrongNumberOfParamsLess) {
    std::istringstream in("void func(int arg1, int arg2) {}"
                          "int main(int argc, char **argv) {"
                          "   func(1);"
                          "}");

    auto parser_result = c::parse(in);
    ASSERT_TRUE(parser_result.errors_.empty());

    c::ast::symtab::Symtab symtab;
    ASSERT_NO_THROW({ symtab = c::get_symtab(parser_result.program_); });

    EXPECT_THROW(
        { c::analyze(parser_result.program_, symtab); },
        c::ast::TypeAnalyzer::Exception);
}

TEST(Analyzer, WrongNumberOfParamsGreater) {
    std::istringstream in("void func(int arg1, int arg2) {}"
                          "int main(int argc, char **argv) {"
                          "   func(1, 2, 3);"
                          "}");

    auto parser_result = c::parse(in);
    ASSERT_TRUE(parser_result.errors_.empty());

    c::ast::symtab::Symtab symtab;
    ASSERT_NO_THROW({ symtab = c::get_symtab(parser_result.program_); });

    EXPECT_THROW(
        { c::analyze(parser_result.program_, symtab); },
        c::ast::TypeAnalyzer::Exception);
}

TEST(Analyzer, ReturnWithAValue) {
    std::istringstream in("void func(int arg1, int arg2) { return 0; }"
                          "int main(int argc, char **argv) {"
                          "   return 0;"
                          "}");

    auto parser_result = c::parse(in);
    ASSERT_TRUE(parser_result.errors_.empty());

    c::ast::symtab::Symtab symtab;
    ASSERT_NO_THROW({ symtab = c::get_symtab(parser_result.program_); });

    EXPECT_THROW(
        { c::analyze(parser_result.program_, symtab); },
        c::ast::TypeAnalyzer::Exception);
}

TEST(Analyzer, ContinueIsNotInFor) {
    std::istringstream in("void func(int arg1, int arg2) { return 0; }"
                          "int main(int argc, char **argv) {"
                          "   continue;"
                          "}");

    auto parser_result = c::parse(in);
    ASSERT_TRUE(parser_result.errors_.empty());

    c::ast::symtab::Symtab symtab;
    ASSERT_NO_THROW({ symtab = c::get_symtab(parser_result.program_); });

    EXPECT_THROW(
        { c::analyze(parser_result.program_, symtab); },
        c::ast::TypeAnalyzer::Exception);
}

TEST(Analyzer, BreakIsNotInFor) {
    std::istringstream in("void func(int arg1, int arg2) { return 0; }"
                          "int main(int argc, char **argv) {"
                          "   break;"
                          "}");

    auto parser_result = c::parse(in);
    ASSERT_TRUE(parser_result.errors_.empty());

    c::ast::symtab::Symtab symtab;
    ASSERT_NO_THROW({ symtab = c::get_symtab(parser_result.program_); });

    EXPECT_THROW(
        { c::analyze(parser_result.program_, symtab); },
        c::ast::TypeAnalyzer::Exception);
}

TEST(Analyzer, AssignmentToExprWithArrayType) {
    std::istringstream in("void func(int arg1, int arg2) { return 0; }"
                          "int main(int argc, char **argv) {"
                          "   int array[10];"
                          "   int var;"
                          "   var = array = 5;"
                          "}");

    auto parser_result = c::parse(in);
    ASSERT_TRUE(parser_result.errors_.empty());

    c::ast::symtab::Symtab symtab;
    ASSERT_NO_THROW({ symtab = c::get_symtab(parser_result.program_); });

    EXPECT_THROW(
        { c::analyze(parser_result.program_, symtab); },
        c::ast::TypeAnalyzer::Exception);
}

TEST(Analyzer, AssignmentExprWithVoidType) {
    std::istringstream in(
        "void func(int arg1, int arg2) { return 0; }"
        "int main(int argc, char **argv) { int a = 5; a = func() + 5; }");

    auto parser_result = c::parse(in);
    ASSERT_TRUE(parser_result.errors_.empty());

    c::ast::symtab::Symtab symtab;
    ASSERT_NO_THROW({ symtab = c::get_symtab(parser_result.program_); });

    EXPECT_THROW(
        { c::analyze(parser_result.program_, symtab); },
        c::ast::TypeAnalyzer::Exception);
}

TEST(Analyzer, AssignmentVoidType) {
    std::istringstream in(
        "void func(int arg1, int arg2) { return 0; }"
        "int main(int argc, char **argv) { int a = 5; a = func(); }");

    auto parser_result = c::parse(in);
    ASSERT_TRUE(parser_result.errors_.empty());

    c::ast::symtab::Symtab symtab;
    ASSERT_NO_THROW({ symtab = c::get_symtab(parser_result.program_); });

    EXPECT_THROW(
        { c::analyze(parser_result.program_, symtab); },
        c::ast::TypeAnalyzer::Exception);
}

// TEST(Analyzer, IncrementArrayType) {
//     std::istringstream in(
//         "void func(int arg1, int arg2) { return 0; }"
//         "int main(int argc, char **argv) { int array[10]; ++array; }");

//     auto parser_result = c::parse(in);
//     ASSERT_TRUE(parser_result.errors_.empty());

//     c::ast::symtab::Symtab symtab;
//     ASSERT_NO_THROW({ symtab = c::get_symtab(parser_result.program_); });

//     EXPECT_THROW(
//         { c::analyze(parser_result.program_, symtab); },
//         c::ast::TypeAnalyzer::Exception);
// }

TEST(Analyzer, ImpossibleTypeConversionParams) {
    std::istringstream in(
        "void func(int arg1, int arg2) { return 0; }"
        "int main(int argc, char **argv) { func(func(1, 2), 2); }");

    auto parser_result = c::parse(in);
    ASSERT_TRUE(parser_result.errors_.empty());

    c::ast::symtab::Symtab symtab;
    ASSERT_NO_THROW({ symtab = c::get_symtab(parser_result.program_); });

    EXPECT_THROW(
        { c::analyze(parser_result.program_, symtab); },
        c::ast::TypeAnalyzer::Exception);
}

TEST(Analyzer, ImpossibleTypeConversionReturn) {
    std::istringstream in(
        "void func(int arg1, int arg2) { return 0; }"
        "int main(int argc, char **argv) { return func(1, 2); }");

    auto parser_result = c::parse(in);
    ASSERT_TRUE(parser_result.errors_.empty());

    c::ast::symtab::Symtab symtab;
    ASSERT_NO_THROW({ symtab = c::get_symtab(parser_result.program_); });

    EXPECT_THROW(
        { c::analyze(parser_result.program_, symtab); },
        c::ast::TypeAnalyzer::Exception);
}

TEST(Analyzer, ImpossibleTypeConversionVarInit) {
    std::istringstream in(
        "void func(int arg1, int arg2) { return 0; }"
        "int main(int argc, char **argv) { int var = func(1, 2); }");

    auto parser_result = c::parse(in);
    ASSERT_TRUE(parser_result.errors_.empty());

    c::ast::symtab::Symtab symtab;
    ASSERT_NO_THROW({ symtab = c::get_symtab(parser_result.program_); });

    EXPECT_THROW(
        { c::analyze(parser_result.program_, symtab); },
        c::ast::TypeAnalyzer::Exception);
}

TEST(Analyzer, InvalidArraySize) {
    std::vector<std::string> strings = {
        "int main() { double var; int array[var]; }",
        "int main() { float var; int array[var]; }",
        "int main() { int *var; int array[var]; }",
        "int main() { double var[10]; int array[var]; }",
        "void func() {} int main() { int array[func()]; }"};
    std::stringstream sstream;

    for (const auto &str : strings) {
        sstream << str;

        auto parser_result = c::parse(sstream);
        ASSERT_TRUE(parser_result.errors_.empty());

        c::ast::symtab::Symtab symtab;
        ASSERT_NO_THROW({ symtab = c::get_symtab(parser_result.program_); });

        EXPECT_THROW(
            { c::analyze(parser_result.program_, symtab); },
            c::ast::TypeAnalyzer::Exception);

        sstream.str("");
    }
}

TEST(Analyzer, InvalidArrayIdx) {
    std::vector<std::string> strings = {
        "int main() { double var; int array[10]; array[var] = 5;}",
        "int main() { float var; int array[10]; array[var] = 5;}",
        "int main() { int *var; int array[10]; array[var] = 5;}",
        "int main() { double var[10]; int array[10]; array[var] = 5;}",
        "void func() {} int main() { int array[10]; array[func()] = 5;}"};
    std::stringstream sstream;

    for (const auto &str : strings) {
        sstream << str;

        auto parser_result = c::parse(sstream);
        ASSERT_TRUE(parser_result.errors_.empty());

        c::ast::symtab::Symtab symtab;
        ASSERT_NO_THROW({ symtab = c::get_symtab(parser_result.program_); });

        EXPECT_THROW(
            { c::analyze(parser_result.program_, symtab); },
            c::ast::TypeAnalyzer::Exception);

        sstream.str("");
    }
}

TEST(Analyzer, InvalidOperationsWithPointers) {
    std::vector<std::string> strings = {
        "int main() { int *p1; int *p2; int *p3 = p2 + p1; }",
        "int main() { int *p1; double *p2; int *p3 = p1 + p2; }",
        "int main() { int *p1; int p2[10]; int *p3 = p2 + p1; }",
        "int main() { int p1[10]; int p2[10]; int *p3 = p2 + p1; }",
        "int main() { int p1[10]; double p2[10]; int *p3 = p1 + p2; }",
        "int main() { int *p1; double var; int var1 = p1 + var; }",
        "int main() { int *p1; float var; int var1 = var + p1; }",
        "int main() { int p1[11]; double var; int var1 = p1 + var; }",
        "int main() { int p1[11]; float var; int var1 = var + p1; }",
    };
    std::stringstream sstream;

    for (const auto &str : strings) {
        sstream << str;

        auto parser_result = c::parse(sstream);
        ASSERT_TRUE(parser_result.errors_.empty());

        c::ast::symtab::Symtab symtab;
        ASSERT_NO_THROW({ symtab = c::get_symtab(parser_result.program_); });

        EXPECT_THROW(
            { c::analyze(parser_result.program_, symtab); },
            c::ast::TypeAnalyzer::Exception);

        sstream.str("");
    }
}

// NOLINTEND(readability-function-cognitive-complexity)