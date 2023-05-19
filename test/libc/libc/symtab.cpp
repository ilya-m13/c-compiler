#include <gtest/gtest.h>

#include <libc/parser.hpp>
#include <libc/symtab.hpp>

#include <sstream>

// NOLINTBEGIN(readability-function-cognitive-complexity)

TEST(Symtab, ValidSymtabBuilding) {
    std::istringstream in(
        "void func(int arg1, char arg2, long arg3) {int var = arg1;}\n"
        "int main(int argc, char **argv) {\n"
        "int main;\n"
        "int var[5];\n"
        "{\n"
        "int *main;\n"
        "const int var;\n"
        "{\n"
        "int main;\n"
        "int var;\n"
        "}\n"
        "}\n"
        "{\n"
        "int main[5];\n"
        "int var;\n"
        "}\n"
        "}\n");
    std::istringstream correct_out(
        "name=var  in_order=1  type-name=int  type=var  const=0  "
        "root-path=main::local  sym-num-in-scope=2\n"
        "name=main  in_order=0  type-name=int  type=[]  const=0  "
        "root-path=main::local  sym-num-in-scope=2\n"
        "name=var  in_order=1  type-name=int  type=var  const=0  "
        "root-path=main::local::local  sym-num-in-scope=2\n"
        "name=main  in_order=0  type-name=int  type=var  const=0  "
        "root-path=main::local::local  sym-num-in-scope=2\n"
        "name=var  in_order=1  type-name=int  type=var  const=1  "
        "root-path=main::local  sym-num-in-scope=2\n"
        "name=main  in_order=0  type-name=int  type=*  const=0  "
        "root-path=main::local  sym-num-in-scope=2\n"
        "name=var  in_order=3  type-name=int  type=[]  const=0  root-path=main "
        " sym-num-in-scope=4\n"
        "name=main  in_order=2  type-name=int  type=var  const=0  "
        "root-path=main  sym-num-in-scope=4\n"
        "name=argv  in_order=1  type-name=char  type=*  const=0  "
        "root-path=main  sym-num-in-scope=4\n"
        "name=argc  in_order=0  type-name=int  type=var  const=0  "
        "root-path=main  sym-num-in-scope=4\n"
        "name=main  in_order=1  type-name=int  type=var  const=0  param-num=2  "
        "sym-num=4  root-path=main  sym-num-in-scope=4\n"
        "name=var  in_order=3  type-name=int  type=var  const=0  "
        "root-path=func  sym-num-in-scope=4\n"
        "name=arg3  in_order=2  type-name=long  type=var  const=0  "
        "root-path=func  sym-num-in-scope=4\n"
        "name=arg2  in_order=1  type-name=char  type=var  const=0  "
        "root-path=func  sym-num-in-scope=4\n"
        "name=arg1  in_order=0  type-name=int  type=var  const=0  "
        "root-path=func  sym-num-in-scope=4\n"
        "name=func  in_order=0  type-name=void  type=var  const=0  param-num=3 "
        " sym-num=4  root-path=func  sym-num-in-scope=4\n");
    std::ostringstream out;

    c::ast::symtab::Symtab symtab;
    auto parser_result = c::parse(in);
    ASSERT_TRUE(parser_result.errors_.empty());
    ASSERT_NO_THROW({ symtab = c::get_symtab(parser_result.program_); });
    c::dump_symtab(symtab, out);

    EXPECT_STREQ(out.str().c_str(), correct_out.str().c_str());
}

TEST(Symtab, UndefinedReference) {
    std::vector<std::string> strings = {
        "int main() {undef_func();}",
        "int main() {int main; {undef_func();}}",
        "int main() {int main = undef_sym;}",
        "int main() {int main; {int main = undef_sym;}}"};
    std::stringstream sstream;

    for (const auto &string : strings) {
        sstream << string;

        auto parser_result = c::parse(sstream);
        ASSERT_TRUE(parser_result.errors_.empty());

        EXPECT_THROW(
            { c::get_symtab(parser_result.program_); },
            c::ast::symtab::UndefinedReference);

        sstream.str("");
    }
}

TEST(Symtab, SymbolRedefinition) {
    std::vector<std::string> strings = {
        "int main() {}\nint main() {}",
        "int main(int arg1, int arg1) {}",
        "int main(int arg1) {int arg1;}",
        "int main() {int main; int main;}",
        "int main() {int main; {int main; int main;}}",
        "int main() {int main; {int main; int var;} int main;}"};
    std::stringstream sstream;

    for (const auto &string : strings) {
        sstream << string;

        auto parser_result = c::parse(sstream);
        ASSERT_TRUE(parser_result.errors_.empty());

        EXPECT_THROW(
            { c::get_symtab(parser_result.program_); },
            c::ast::symtab::SymbolRedefinition)
            << string;

        sstream.str("");
    }
}

// NOLINTEND(readability-function-cognitive-complexity)