#include <libc/dump_tokens.hpp>
#include <libc/parser.hpp>
#include <libc/symtab.hpp>

#include <cxxopts.hpp>

#include <filesystem>
#include <fstream>
#include <iostream>

int main(int argc, char **argv) {
    cxxopts::Options options("c-compiler");
    options.positional_help("<file-path>");
    // clang-format off
    options.add_options()
        ("file-path", "", cxxopts::value<std::filesystem::path>())
        ("dump-tokens", "")
        ("dump-ast", "")
        ("dump-symtab", "")
        ("h,help", "")
    ;
    // clang-format on
    options.parse_positional({"file-path"});
    const auto result = options.parse(argc, argv);

    if (result.count("file-path") != 1 || result.count("help") > 0) {
        std::cout << options.help() << "\n";
        return 0;
    }

    std::ifstream input_stream(result["file-path"].as<std::filesystem::path>());
    if (!input_stream.good()) {
        std::cerr << "Unable to read stream\n";
        return 1;
    }

    if (result.count("dump-tokens") > 0) {
        c::dump_tokens(input_stream, std::cout);
    } else if (result.count("dump-ast") > 0) {
        auto parser_result = c::parse(input_stream);
        if (!parser_result.errors_.empty()) {
            c::dump_errors(parser_result.errors_, std::cerr);
        } else {
            c::dump_ast(parser_result.program_, std::cout);
        }
    } else if (result.count("dump-symtab") > 0) {
        auto parser_result = c::parse(input_stream);
        if (!parser_result.errors_.empty()) {
            c::dump_errors(parser_result.errors_, std::cerr);
        } else {
            try {
                auto symtab = c::get_symtab(parser_result.program_);
                c::dump_symtab(symtab, std::cout);
            } catch (const c::ast::symtab::UndefinedReference &ex) {
                std::cout << ex.what() << '\n';
            } catch (const c::ast::symtab::SymbolRedefinition &ex) {
                std::cout << ex.what() << '\n';
            }
        }
    }

    return 0;
}