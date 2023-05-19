#include <gtest/gtest.h>

#include <libc/parser.hpp>
#include <libc/source_dir.hpp>

#include <fstream>
#include <sstream>

static std::string get_correct() {
    std::ifstream fin(
        c_source_dir / "test-additional-files/parser-test/correct_out.xml");
    if (!fin.good()) {
        return std::string{};
    }
    std::string correct;
    fin.seekg(0, std::ios_base::end);
    correct.resize(static_cast<std::size_t>(fin.tellg()));
    fin.seekg(0, std::ios_base::beg);
    fin.read(correct.data(), static_cast<std::streamsize>(correct.capacity()));
    return correct;
}

TEST(ParserTest, AllValidRuleCombinations) {
    std::string corret = get_correct();
    if (corret.empty()) {
        FAIL();
    }

    std::ifstream fin(
        c_source_dir / "test-additional-files/parser-test/parser_test.c");
    if (!fin.good()) {
        FAIL();
    }

    auto result = c::parse(fin);
    if (!result.errors_.empty()) {
        c::dump_errors(result.errors_, std::cerr);
        FAIL();
    }

    std::ostringstream out;
    c::dump_ast(result.program_, out);

    EXPECT_STREQ(corret.c_str(), out.str().c_str());
}

TEST(ParserTest, InvalidHeaderFile) {
    std::vector<std::string> strings = {
        "int func() {#include <stdio.h>}",
        "#incld <stdio.h>",
        "include <stdio.h>",
        "#include stdio.h",
        "#include <.h>"};
    std::vector<c::ParseResult> results(strings.size());
    std::stringstream sstream;

    for (std::size_t i = 0; i < strings.size(); ++i) {
        sstream << strings[i];
        results[i] = c::parse(sstream);
        sstream.str("");

        EXPECT_FALSE(results[i].errors_.empty());
    }
}

TEST(ParserTest, InvalidFunctionDefinition) {
    std::vector<std::string> strings = {
        "main() {}",
        "int main( {}",
        "int main() {",
        "int main()",
        "int main(arg1, arg2) {}",
        "int main(int arg1 = 0, int arg2)",
        "int main(5+5) {}",
        "int main() {int main() {}}",
        "int main() {int main() {};}",
        "return main() {}",
        "int return() {}",
        "int main(int return) {}"};
    std::vector<c::ParseResult> results(strings.size());
    std::stringstream sstream;

    for (std::size_t i = 0; i < strings.size(); ++i) {
        sstream << strings[i];
        results[i] = c::parse(sstream);
        sstream.str("");

        EXPECT_FALSE(results[i].errors_.empty());
    }
}

// TEST(ParserTest, InvalidStructDeclaration) {
//     std::vector<std::string> strings = {
//         "strct Struct;", "int main() {struct return;}", "struct return;"};
//     std::vector<c::ParseResult> results(strings.size());
//     std::stringstream sstream;

//     for (std::size_t i = 0; i < strings.size(); ++i) {
//         sstream << strings[i];
//         results[i] = c::parse(sstream);
//         sstream.str("");

//         EXPECT_FALSE(results[i].errors_.empty());
//     }
// }

// TEST(ParserTest, InvalidStructDefinition) {
//     std::vector<std::string> strings = {
//         "struct Struct {member1; member2};",
//         "struct Struct {int member1; int member2 = 0};",
//         "struct Struct {struct Struct {int member;};};",
//         "int main() {struct Struct {int member};}",
//         "struct Struct {int return;};"};
//     std::vector<c::ParseResult> results(strings.size());
//     std::stringstream sstream;

//     for (std::size_t i = 0; i < strings.size(); ++i) {
//         sstream << strings[i];
//         results[i] = c::parse(sstream);
//         sstream.str("");

//         EXPECT_FALSE(results[i].errors_.empty());
//     }
// }

TEST(ParserTest, InvalidFunctionCall) {
    std::vector<std::string> strings = {
        "call();",
        "int main() {call(;}",
        "int main() {call()}",
        "int main() {call(int var);}",
        "int main() {call(int var = 0);}",
        "int main() {call(return);}"};
    std::vector<c::ParseResult> results(strings.size());
    std::stringstream sstream;

    for (std::size_t i = 0; i < strings.size(); ++i) {
        sstream << strings[i];
        results[i] = c::parse(sstream);
        sstream.str("");

        EXPECT_FALSE(results[i].errors_.empty());
    }
}

TEST(ParserTest, InvalidVariableWriting) {
    std::vector<std::string> strings = {
        "var = var;",
        "int var = 0;",
        "int main() {var 0;}",
        "int main() {var = int main() {};}",
        "int main() {var = int var;}",
        "int main() {5 = var;}",
        "int main() {var = var}",
        "int main() {var = return;}"};
    std::vector<c::ParseResult> results(strings.size());
    std::stringstream sstream;

    for (std::size_t i = 0; i < strings.size(); ++i) {
        sstream << strings[i];
        results[i] = c::parse(sstream);
        sstream.str("");

        EXPECT_FALSE(results[i].errors_.empty());
    }
}

TEST(ParserTest, InvalidDataCreate) {
    std::vector<std::string> strings = {
        "int var;",
        "int var = 0;",
        "int *var;",
        "int *var = 0;",
        "int var[5];",
        "int main() {var int;}",
        "int main() {var[5] int;}",
        "int main() {int var}",
        "int main() {int var = return 1;}",
        "int main() {= 5 + 5;}"};
    std::vector<c::ParseResult> results(strings.size());
    std::stringstream sstream;

    for (std::size_t i = 0; i < strings.size(); ++i) {
        sstream << strings[i];
        results[i] = c::parse(sstream);
        sstream.str("");

        EXPECT_FALSE(results[i].errors_.empty());
    }
}

TEST(ParserTest, InvalidReturnStatement) {
    std::vector<std::string> strings = {
        "return 1;",
        "int main() {1 return;}",
        "int main() {return 1}",
        "int main() {return continue}"};
    std::vector<c::ParseResult> results(strings.size());
    std::stringstream sstream;

    for (std::size_t i = 0; i < strings.size(); ++i) {
        sstream << strings[i];
        results[i] = c::parse(sstream);
        sstream.str("");

        EXPECT_FALSE(results[i].errors_.empty());
    }
}

TEST(ParserTest, InvalidForStatement) {
    std::vector<std::string> strings = {
        "for (;;) {}",
        "int main() {for () {}}",
        "int main() {for (;) {}}",
        "int main() {for (int main() {};;) {}}",
        "int main() {for (;int main() {};) {}}",
        "int main() {for (;;int main() {})}",
        "int main() {for (;;) {int main() {}}}",
        "int main() {for (return;;) {}}",
        "int main() {for (;int var;) {}}",
        "int main() {for (;;int var) {}}",
        "int main() {for (continue;;) {}}"};
    std::vector<c::ParseResult> results(strings.size());
    std::stringstream sstream;

    for (std::size_t i = 0; i < strings.size(); ++i) {
        sstream << strings[i];
        results[i] = c::parse(sstream);
        sstream.str("");

        EXPECT_FALSE(results[i].errors_.empty());
    }
}

TEST(ParserTest, InvalidIfStatement) {
    std::vector<std::string> strings = {
        "if (1) {}",
        "int main() {if () {}}",
        "int main() {if (int var) {}}",
        "int main() {if (int main() {})}",
        "int main() {if (1) {int main() {}}}",
        "int main() {if (continue) {}}"};
    std::vector<c::ParseResult> results(strings.size());
    std::stringstream sstream;

    for (std::size_t i = 0; i < strings.size(); ++i) {
        sstream << strings[i];
        results[i] = c::parse(sstream);
        sstream.str("");

        EXPECT_FALSE(results[i].errors_.empty());
    }
}

TEST(ParserTest, InvalidContinueStatement) {
    std::vector<std::string> strings = {"continue;", "int main() {continue}"};
    std::vector<c::ParseResult> results(strings.size());
    std::stringstream sstream;

    for (std::size_t i = 0; i < strings.size(); ++i) {
        sstream << strings[i];
        results[i] = c::parse(sstream);
        sstream.str("");

        EXPECT_FALSE(results[i].errors_.empty());
    }
}

TEST(ParserTest, InvalidBreakStatement) {
    std::vector<std::string> strings = {"break;", "int main() {break}"};
    std::vector<c::ParseResult> results(strings.size());
    std::stringstream sstream;

    for (std::size_t i = 0; i < strings.size(); ++i) {
        sstream << strings[i];
        results[i] = c::parse(sstream);
        sstream.str("");

        EXPECT_FALSE(results[i].errors_.empty());
    }
}

TEST(ParserTest, InvalidValue) {
    std::vector<std::string> strings = {
        "5",
        "5;",
        "5+5",
        "5+5;",
        "var",
        "var;",
        "5+5 main() {}",
        "int 5+5() {}",
        "int main(5+5) {}",
        "int main() {5;}",
        "int main() {5;}",
        "int main() {5+5}",
        "int main() {5+5;}",
        "int main() {var}",
        "int main() {var;}"};
    std::vector<c::ParseResult> results(strings.size());
    std::stringstream sstream;

    for (std::size_t i = 0; i < strings.size(); ++i) {
        sstream << strings[i];
        results[i] = c::parse(sstream);
        sstream.str("");

        EXPECT_FALSE(results[i].errors_.empty());
    }
}

TEST(ParserTest, InvalidLocalScope) {
    std::vector<std::string> strings = {
        "{}", "int main() {{var int = 0;}}", "int main() {{{}}"};
    std::vector<c::ParseResult> results(strings.size());
    std::stringstream sstream;

    for (std::size_t i = 0; i < strings.size(); ++i) {
        sstream << strings[i];
        results[i] = c::parse(sstream);
        sstream.str("");

        EXPECT_FALSE(results[i].errors_.empty());
    }
}