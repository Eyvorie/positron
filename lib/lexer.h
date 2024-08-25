#ifndef LEXER_H
#define LEXER_H

typedef struct {
  char *source;
  size_t pos;
  size_t len;
} Lexer;

typedef enum {
  TOKEN_LT,
  TOKEN_GT,
  TOKEN_EQ,
  TOKEN_STRING_LITERAL,
  TOKEN_IDENTIFIER,
  TOKEN_EOF,
} LexerTokenType;

typedef struct {
  LexerTokenType type;
  char *start;
  size_t len;
} LexerToken;

void lexer_init(Lexer *, char *, size_t);
void lexer_get_next_token(Lexer *, LexerToken *);
void lexer_peak_next_token(Lexer *, LexerToken *);
void lexer_consume_token(Lexer *, LexerToken *);

#endif