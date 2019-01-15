#include "gtest/gtest.h"
extern "C" {
#include "csprint.h"
}

char buffer[1024];

TEST(csprintf, literal) {
  cssprintf(buffer, "literal string");
  EXPECT_STREQ(buffer, "literal string");
}

TEST(csprintf, percent_d) {
  cssprintf(buffer, "%d", 42);
  EXPECT_STREQ(buffer, "42");
}

TEST(csprintf, percent_ld) {
  cssprintf(buffer, "%ld", 42);
  EXPECT_STREQ(buffer, "42");
}

TEST(csprintf, ip_printer) {
  int ip = 42;
  cssprintf(buffer, "${ip}", &ip);
  EXPECT_STREQ(buffer, "42.0.0.0");
}

TEST(csprintf, invalid_type) {
  cssprintf(buffer, "[${invalid-type}]", NULL);
  EXPECT_STREQ(buffer, "[]");
}

TEST(csprintf, literal_dollar) {
  cssprintf(buffer, "R$ 5,00");
  EXPECT_STREQ(buffer, "R$ 5,00");
}

int main(int argc, char **argv) {
  ::testing::InitGoogleTest(&argc, argv);
  return RUN_ALL_TESTS();
}
