#include <cstdio>
#include <cstdlib>
#include <cstring>
#include <gtest/gtest.h>

extern "C" {
#include "src/main.c"
}

static int evaluate_int(const char *expr) {
  src = expr;
  return parse_grammar_int();
}

static double evaluate_double(const char *expr) {
  src = expr;
  return parse_grammar_double();
}

TEST(ParseNumberInt, SimpleNumber) {
  src = "42";
  EXPECT_EQ(parse_number_int(), 42);
}

TEST(ParseFactorInt, WithParentheses) {
  src = "(7)";
  EXPECT_EQ(parse_factor_int(), 7);
}

TEST(ParseDivisionInt, MultiplicationAndDivision) {
  src = "2*3/6";
  EXPECT_EQ(parse_division_int(), 1);
}

TEST(ParseGrammarInt, AdditionAndSubtraction) {
  src = "1+2-3";
  EXPECT_EQ(parse_grammar_int(), 0);
}

TEST(EvaluateInt, ComplexExpression) {
  EXPECT_EQ(evaluate_int("2+3*(4-1)"), 11);
}

TEST(ParseFactorInt, MissingClosingParenthesisDeathTest) {
  src = "(2+3";
  EXPECT_DEATH(parse_factor_int(), ".*");
}

TEST(ParseDivisionInt, DivisionByZeroDeathTest) {
  src = "10/0";
  EXPECT_DEATH(parse_division_int(), ".*");
}

TEST(ParseNumberDouble, SimpleDouble) {
  src = "3.14";
  EXPECT_NEAR(parse_number_double(), 3.14, 1e-6);
}

TEST(EvaluateDouble, ComplexExpression) {
  EXPECT_NEAR(evaluate_double("2+3/2"), 3.5, 1e-6);
}

TEST(ParseDivisionDouble, DivisionByZeroDeathTest) {
  src = "10/0.0";
  EXPECT_DEATH(parse_division_double(), ".*");
}

TEST(ValidateInput, TooLongInputDeathTest) {
  char longInput[1100];
  memset(longInput, '1', sizeof(longInput) - 1);
  longInput[sizeof(longInput) - 1] = '\0';
  EXPECT_DEATH(validate_input(longInput), ".*");
}

TEST(ValidateInput, InvalidCharacterDeathTest) {
  EXPECT_DEATH(validate_input("2+3a"), ".*");
}
