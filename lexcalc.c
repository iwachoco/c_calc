#include <stdio.h>
#include <stdlib.h>
#include <ctype.h>
#include "token.h"

static char *st_line;
static int st_line_pos;

typedef enum {
  INITIAL,
  INTEGER_PART,
  FRACTIONAL_PART,
  DOT
} LexerStatus;

void get_token(Token *token) {
  char current_char;
  int out_pos = 0;
  LexerStatus status = INITIAL;
  token->kind = BAD_TOKEN;

  while(st_line[st_line_pos] != '\0') {
    current_char = st_line[st_line_pos];
    /*(ひとつ前に入れた文字が整数部分又は少数部分) かつ 今見ている一文字が数字ではない かつ 今見ている一文字が小数点でもない場合*/
    if((status == INTEGER_PART || status == FRACTIONAL_PART) && !isdigit(current_char) && current_char != '.') {
      token->kind = NUMBER_TOKEN;
      sscanf(token->str, "%lf", &token->value);
      return;
    }
    /*改行又はスペース又はタブの場合*/
    if(isspace(current_char)) {
      /*改行の場合*/
      if(current_char == '\n') {
        token->kind = END_OF_LINE_TOKEN;
        return;
      }
      st_line_pos++;
      continue;
    }
    /*トークンのサイズがオーバーしていないかチェックしている*/
    if(out_pos >= MAX_TOKEN_SIZE - 1) {
      fprintf(stderr, "token size is over. token is too long!\n");
      exit(1);
    }

    token->str[out_pos] = st_line[st_line_pos];
    st_line_pos++;
    out_pos++;
    token->str[out_pos] = '\0';

    if(current_char == '+') {
      token->kind = ADD_OPERATOR_TOKEN;
      return;
    } else if(current_char == '-') {
      token->kind = SUB_OPERATOR_TOKEN;
      return;
    } else if(current_char == '*') {
      token->kind = MUL_OPERATOR_TOKEN;
      return;
    } else if(current_char == '/') {
      token->kind = DIV_OPERATOR_TOKEN;
      return;
    } else if(isdigit(current_char)) {
      if(status == INITIAL) {
        status = INTEGER_PART;
      } else if(status == DOT) {
        status = FRACTIONAL_PART;
      }
    } else if(current_char == '.') {
      if(status == INTEGER_PART) {
        status = DOT;
      } else {
        fprintf(stderr, "syntax error.\n");
        exit(1);
      }
    } else {
      fprintf(stderr, "bad character: %c\n", current_char);
      exit(1);
    }
  }
}

void set_line(char *line) {
  st_line = line;
  st_line_pos = 0;
}
