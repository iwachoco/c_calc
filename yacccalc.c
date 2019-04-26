#include <stdio.h>
#include <stdlib.h>
#include <math.h>
#include "token.h"

#define LINE_BUF_SIZE 1024

static int st_look_ahead_token_exists;
static Token st_look_ahead_token;

static int factorial_calc(int);
static void my_get_token(Token *);
static void unget_token(Token *);
static double parse_primary_expression();
static double parse_pow_term();
static double parse_term();
static double parse_expression();


static int factorial_calc(int number) {
  if(number == 1 || number == 0) {
    return 1;
  } else {
    return number * factorial_calc(number - 1);
  }
}

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
  int minus_flag = 0, function_flag = 0;
  Token token;
  double value;
  double base;
  my_get_token(&token);
  if(token.kind == SUB_OPERATOR_TOKEN) {
    minus_flag = 1;
    my_get_token(&token);
  }
  if(token.kind == LOG_NATURAL_FUNCTION_TOKEN) {
    function_flag = 1;
    my_get_token(&token);
  } else if(token.kind == SIN_FUNCTION_TOKEN) {
    function_flag = 2;
    my_get_token(&token);
  } else if(token.kind == COS_FUNCTION_TOKEN) {
    function_flag = 3;
    my_get_token(&token);
  } else if(token.kind == TAN_FUNCTION_TOKEN) {
    function_flag = 4;
    my_get_token(&token);
  } else if(token.kind == EXP_FUNCTION_TOKEN) {
    function_flag = 5;
    my_get_token(&token);
  } else if(token.kind == SQRT_FUNCTION_TOKEN) {
    function_flag = 6;
    my_get_token(&token);
  } else if(token.kind == ARC_SIN_FUNCTION_TOKEN) {
    function_flag = 7;
    my_get_token(&token);
  } else if(token.kind == ARC_COS_FUNCTION_TOKEN) {
    function_flag = 8;
    my_get_token(&token);
  } else if(token.kind == ARC_TAN_FUNCTION_TOKEN) {
    function_flag = 9;
    my_get_token(&token);
  } else if(token.kind == LOG_FUNCTION_TOKEN) {
    function_flag = 10;
    my_get_token(&token);
    if(token.kind == NUMBER_TOKEN) {
      base = token.value;
      my_get_token(&token);
    } else if(token.kind == LEFT_BRACKET_TOKEN) {
      base = 10;
    }
  }
  if(function_flag != 0) {
    if(token.kind == LEFT_BRACKET_TOKEN) {
      value = parse_expression();
      my_get_token(&token);
      if(token.kind != RIGHT_BRACKET_TOKEN) {
        fprintf(stderr, "cannot find brackets.\n");
        exit(1);
      }
      switch(function_flag) {
        case 1:
          value = log(value);
          break;
        case 2:
          value = sin(value);
          break;
        case 3:
          value = cos(value);
          break;
        case 4:
          value = tan(value);
          break;
        case 5:
          value = exp(value);
          break;
        case 6:
          value = sqrt(value);
          break;
        case 7:
          value = asin(value);
          break;
        case 8:
          value = acos(value);
          break;
        case 9:
          value = atan(value);
          break;
        case 10:
          value = log(value) / log(base);
      }
    } else {
      exit(1);
      return 0.0;
    }
  } else if(token.kind == NUMBER_TOKEN) {
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

static double parse_factorial_term() {
  double r1;
  Token token;
  r1 = parse_primary_expression();
  my_get_token(&token);
  if(token.kind == FACTORIAL_FUNCTION_TOKEN) {
    r1 = factorial_calc(r1);
  } else {
    unget_token(&token);
  }
  return r1;
}

static double parse_pow_term() {
  double r1, r2;
  Token token;
  r1 = parse_factorial_term();
  my_get_token(&token);
  if(token.kind == POWER_FUNCTION_TOKEN) {
    r2 = parse_factorial_term();
    r1 = pow(r1, r2);
  } else {
    unget_token(&token);
  }
  return r1;
}

static double parse_term() {
  double r1, r2;
  Token token;
  r1 = parse_pow_term();
  while(1) {
    my_get_token(&token);
    if(token.kind != MUL_OPERATOR_TOKEN && token.kind != DIV_OPERATOR_TOKEN && token.kind != FACTORIAL_FUNCTION_TOKEN) {
      unget_token(&token);
      break;
    } else if(token.kind != FACTORIAL_FUNCTION_TOKEN) {
      r2 = parse_pow_term();
    }
    if(token.kind == MUL_OPERATOR_TOKEN) {
      r1 *= r2;
    } else if(token.kind == DIV_OPERATOR_TOKEN) {
      r1 /= r2;
    } else if(token.kind == FACTORIAL_FUNCTION_TOKEN) {
      r1 = factorial_calc((int)r1);
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
