#include <gtest/gtest.h>
#include <iostream>

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
