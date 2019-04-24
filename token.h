#ifndef TOKEN_H_INCLUDED
#define TOKEN_H_INCLUDED

typedef enum {
  BAD_TOKEN,
  NUMBER_TOKEN,
  ADD_OPERATOR_TOKEN,
  SUB_OPERATOR_TOKEN,
  MUL_OPERATOR_TOKEN,
  DIV_OPERATOR_TOKEN,
  END_OF_LINE_TOKEN,
  LEFT_BRACKET_TOKEN,
  RIGHT_BRACKET_TOKEN,
  LOG_FUNCTION_TOKEN,
  LOG_NATURAL_FUNCTION_TOKEN,
  SIN_FUNCTION_TOKEN,
  COS_FUNCTION_TOKEN,
  TAN_FUNCTION_TOKEN,
  ARC_SIN_FUNCTION_TOKEN,
  ARC_COS_FUNCTION_TOKEN,
  ARC_TAN_FUNCTION_TOKEN,
  EXP_FUNCTION_TOKEN,
  SQRT_FUNCTION_TOKEN
} TokenKind;

#define MAX_TOKEN_SIZE (100)

typedef struct {
  TokenKind kind;
  double value;
  char str[MAX_TOKEN_SIZE];
} Token;

void set_line(char *line);
void get_token(Token *token);
double parse_line();

#endif
