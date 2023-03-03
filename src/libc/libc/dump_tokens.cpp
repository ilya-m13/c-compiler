#include <libc/dump_tokens.hpp>

#include <CLexer.h>
#include <antlr4-runtime.h>

namespace c {

void dump_tokens(std::ifstream &in, std::ostream &out) {
    antlr4::ANTLRInputStream antlr_stream(in);
    CLexer lexer(&antlr_stream);

    for (auto token = lexer.nextToken(); token->getType() != antlr4::Token::EOF;
         token = lexer.nextToken()) {
        out << "Loc=<" << token->getLine() << ":"
            << token->getCharPositionInLine() << ">\t"
            << lexer.getVocabulary().getSymbolicName(token->getType()) << " '"
            << token->getText() << "'\n";
    }
}

} // namespace c