#include "dump_tokens.test.hpp"

#include <libc/dump_tokens.hpp>

TEST_F(CLexerTest, EqualOutputAllTokens) {
    std::ostringstream out_string;

    c::dump_tokens(in_string, out_string);

    ASSERT_STREQ(out_string.str().c_str(), result.str().c_str());
}