#include <libc/dump_tokens.hpp>

#include <cxxopts.hpp>

#include <fstream>
#include <iostream>
#include <string>

int main(int argc, char **argv) {
    cxxopts::Options options("c-compiler");
    options.add_options()("file-path", "", cxxopts::value<std::string>())(
        "h,help", "")("dump-tokens", "");
    const auto result = options.parse(argc, argv);

    if (result.count("file-path") != 1 || result.count("help") > 0) {
        std::cout << options.help() << "\n";
        return 0;
    }

    std::ifstream input_stream(result["file-path"].as<std::string>());
    if (!input_stream.good()) {
        std::cerr << "Unable to read stream\n";
        return 1;
    }

    if (result.count("dump-tokens") > 0) {
        c::dump_tokens(input_stream, std::cout);
    }

    return 0;
}