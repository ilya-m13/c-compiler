#include <libc/dump_tokens.hpp>

#include <cxxopts.hpp>

#include <filesystem>
#include <fstream>
#include <iostream>

int main(int argc, char **argv) {
    cxxopts::Options options("c-compiler");
    // clang-format off
    options.add_options()
        ("file-path", "", cxxopts::value<std::filesystem::path>())
        ("dump-tokens", "")
        ("h,help", "")
    ;
    // clang-format on
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
    }

    return 0;
}