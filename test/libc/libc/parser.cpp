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