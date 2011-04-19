// Copyright 2011, Vaclav Bohac
// All rights reserved.
// 
// Redistribution and use in source and binary forms, with or without
// modification, are permitted provided that the following conditions are
// met:
// 
//     * Redistributions of source code must retain the above copyright
// notice, this list of conditions and the following disclaimer.
//     * Redistributions in binary form must reproduce the above
// copyright notice, this list of conditions and the following disclaimer
// in the documentation and/or other materials provided with the
// distribution.
//     * Neither the name of Vaclav Bohac nor the names of its
// contributors may be used to endorse or promote products derived from
// this software without specific prior written permission.
// 
// THIS SOFTWARE IS PROVIDED BY THE COPYRIGHT HOLDERS AND CONTRIBUTORS
// "AS IS" AND ANY EXPRESS OR IMPLIED WARRANTIES, INCLUDING, BUT NOT
// LIMITED TO, THE IMPLIED WARRANTIES OF MERCHANTABILITY AND FITNESS FOR
// A PARTICULAR PURPOSE ARE DISCLAIMED. IN NO EVENT SHALL THE COPYRIGHT
// OWNER OR CONTRIBUTORS BE LIABLE FOR ANY DIRECT, INDIRECT, INCIDENTAL,
// SPECIAL, EXEMPLARY, OR CONSEQUENTIAL DAMAGES (INCLUDING, BUT NOT
// LIMITED TO, PROCUREMENT OF SUBSTITUTE GOODS OR SERVICES; LOSS OF USE,
// DATA, OR PROFITS; OR BUSINESS INTERRUPTION) HOWEVER CAUSED AND ON ANY
// THEORY OF LIABILITY, WHETHER IN CONTRACT, STRICT LIABILITY, OR TORT
// (INCLUDING NEGLIGENCE OR OTHERWISE) ARISING IN ANY WAY OUT OF THE USE
// OF THIS SOFTWARE, EVEN IF ADVISED OF THE POSSIBILITY OF SUCH DAMAGE.

// Unit test program testing message/builder.c
//
// Author: bohac.v@gmail.com (Vaclav Bohac)

#include <gtest/gtest.h>

#include "messages/builder.h"


TEST(test_messages, test_valid_types)
{
	ASSERT_TRUE(is_valid_type(INFORMATION));
	ASSERT_TRUE(is_valid_type(ANSWER));
	ASSERT_TRUE(is_valid_type('C')); // Command.

	ASSERT_FALSE(is_valid_type('Q'));
}


TEST(test_messages, test_build)
{
	ASSERT_STREQ("A00:Hello, World", build('A', 0, "Hello, World"));
	ASSERT_STREQ("I42:Hello, World", build(INFORMATION, 042, "Hello, World"));
	ASSERT_STREQ("C10:Hello, World", build(COMMAND, 8, "Hello, World"));
}


TEST(test_messages, test_build_from_struct)
{
	struct message m;
	
	m.type = 'A';
	m.code = 042;
	m.text = "Example";

	ASSERT_STREQ("A42:Example", build_from_struct(&m));
}


TEST(test_messages, test_is_type)
{
	ASSERT_TRUE(is_type("C00:example", COMMAND));

	ASSERT_FALSE(is_type("C00:example", ANSWER));
	ASSERT_FALSE(is_type("", COMMAND));
	ASSERT_FALSE(is_type("c", COMMAND));
}


TEST(test_messages, test_is_type_from_struct)
{
	struct message m;

	ASSERT_FALSE(is_type_from_struct(&m, 'A'));

	m.type = 'I';
	ASSERT_TRUE(is_type_from_struct(&m, INFORMATION));

	ASSERT_FALSE(is_type_from_struct(&m, 'i'));
	ASSERT_FALSE(is_type_from_struct(&m, COMMAND));
}


TEST(test_messages, test_message_from_string)
{
	struct message m;

	ASSERT_EQ(0, message_from_string(&m, "I:Example"));
	ASSERT_EQ(0, message_from_string(&m, "I42:Example"));

	ASSERT_EQ('I', m.type);
	ASSERT_EQ(42, m.code);
	ASSERT_STREQ("Example", m.text);

	ASSERT_EQ(0, message_from_string(&m, "I:"));
	ASSERT_STREQ("", m.text);

	ASSERT_EQ(-1, message_from_string(&m, "Q00:Invalid type"));
}
