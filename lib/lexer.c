#include <stdlib.h>
#include <string.h>
#include <stdbool.h>
#include "lexer.h"

void lexer_init(Lexer *this, char *source, size_t len)
{
  this->source = source;
  this->pos = 0;
  this->len = len;
}

void lexer_token_init(LexerToken *out, char *start, size_t len, LexerTokenType type)
{
  out->len = len;
  out->start = start;
  out->type = type;
}

bool is_alphanumeric(char c)
{
  return (c >= '0' && c <= '9') || (c >= 'a' && c <= 'z') ||
    (c >= 'A' && c <= 'Z') || c == '_';
}

void lexer_peak_next_token(Lexer *this, LexerToken *out)
{
  // single character symbols
  char *current = this->source + this->pos;
  while (*current == ' ')
    current++;

  switch(*current) {
    case '<': {
      lexer_token_init(out,
        current, 1, TOKEN_LT);
      return;
    }
    case '>': {
      lexer_token_init(out,
        current, 1, TOKEN_GT);
      return;
    }
    case '=': {
      lexer_token_init(out,
        current, 1, TOKEN_EQ);
      return;
    }
    case '"': {
      char *end_quote = strchr(current + 1, '"');
      // assert end_quote;
      lexer_token_init(out,
        current, end_quote - current + 1, TOKEN_STRING_LITERAL);
      return;
    }
    case '\0': {
      lexer_token_init(out,
        current, 0, TOKEN_EOF);
      return;
    }
  }

  if (is_alphanumeric(*current)) {
    char *ident = current;
    while (is_alphanumeric(*ident))
      ident++;
    lexer_token_init(out,
      current, ident - current, TOKEN_IDENTIFIER);
    return;
  }
}

void lexer_consume_token(Lexer *this, LexerToken *in)
{
    this->pos += in->start - (this->source + this->pos) + in->len;
}

void lexer_get_next_token(Lexer *this, LexerToken *out)
{
  lexer_peak_next_token(this, out);
  lexer_consume_token(this, out);
}