#include <stdio.h>
#include <stdlib.h>
#include "token.h"

#define LINE_BUF_SIZE 1024

static int st_look_ahead_token_exists;
static Token st_look_ahead_token;

static void my_get_token(Token *);
static void unget_token(Token *);
static double parse_primary_expression();
static double parse_term();
static double parse_expression();

static void my_get_token(Token *token) {
  if(st_look_ahead_token_exists) {
    *token = st_look_ahead_token;
    st_look_ahead_token_exists = 0;
  } else {
    get_token(token);
  }
}

static void unget_token(Token *token) {
  st_look_ahead_token = *token;
  st_look_ahead_token_exists = 1;
}

static double parse_primary_expression() {
  int minus_flag = 0;
  Token token;
  double value;
  my_get_token(&token);
  if(token.kind == SUB_OPERATOR_TOKEN) {
    minus_flag = 1;
    my_get_token(&token);
  }
  if(token.kind == NUMBER_TOKEN) {
    value = token.value;
  } else if(token.kind == LEFT_BRACKET_TOKEN) {
    value = parse_expression();
    my_get_token(&token);
    if(token.kind != RIGHT_BRACKET_TOKEN) {
      fprintf(stderr, "cannot find brackets.\n");
      exit(1);
    }
  } else {
    exit(1);
    return 0.0;
  }
  if(minus_flag) {
    value = -1.0 * value;
  }
  return value;
}

static double parse_term() {
  double r1, r2;
  Token token;
  r1 = parse_primary_expression();
  while(1) {
    my_get_token(&token);
    if(token.kind != MUL_OPERATOR_TOKEN && token.kind != DIV_OPERATOR_TOKEN) {
      unget_token(&token);
      break;
    }
    r2 = parse_primary_expression();
    if(token.kind == MUL_OPERATOR_TOKEN) {
      r1 *= r2;
    } else if(token.kind == DIV_OPERATOR_TOKEN) {
      r1 /= r2;
    }
  }
  return r1;
}

static double parse_expression() {
  double r1, r2;
  Token token;
  r1 = parse_term();
  while(1) {
    my_get_token(&token);
    if(token.kind != ADD_OPERATOR_TOKEN && token.kind != SUB_OPERATOR_TOKEN) {
      unget_token(&token);
      break;
    }
    r2 = parse_term();
    if(token.kind == ADD_OPERATOR_TOKEN) {
      r1 += r2;
    } else if(token.kind == SUB_OPERATOR_TOKEN) {
      r1 -= r2;
    }
  }
  return r1;
}

double parse_line() {
  double value;
  st_look_ahead_token_exists = 0;
  value = parse_expression();
  return value;
}

int main(int argc, char **argv) {
  char line[LINE_BUF_SIZE];
  double value;
  while(fgets(line, LINE_BUF_SIZE, stdin) != NULL) {
    set_line(line);
    value = parse_line();
    printf(">> %f\n", value);
  }
  return 0;
}
