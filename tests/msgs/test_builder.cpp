#include <gtest/gtest.h>
#include <iostream>

#include "messages/builder.h"

TEST(test_messages, test_valid_types)
{
	ASSERT_EQ(1, is_valid_type(INFORMATION));
	ASSERT_EQ(1, is_valid_type(ANSWER));
	ASSERT_EQ(1, is_valid_type('C')); // Command.
	ASSERT_EQ(0, is_valid_type('Q'));
}
