#include <string.h>
#include "parser.h"
#include "lexer.h"
#include "util.h"
#include "alloc.h"

AstNode *new_node(void *expression, NodeType type)
{
  AstNode *ast_node = new(AstNode);
  ast_node->type = type;
  ast_node->expression = expression;
  return ast_node;
}

DocumentElement *parse_markup_literal(Lexer *lexer)
{
  LexerToken token;
  lexer_peak_next_token(lexer, &token);
  assert(token.type == TOKEN_IDENTIFIER, "Parser error: expected tag name");
  lexer_consume_token(lexer, &token);

  DocumentElement *head = new(DocumentElement);
  head->children = NULL;
  if (strncmp(token.start, "img", token.len) == 0)
    head->tagname = ELEMENT_IMAGE;

  head->attributes = 1;
  head->attribute_keys = array_of(char *, head->attributes);
  head->attribute_values = array_of(char *, head->attributes);

  // this is inefficient, we should count the number of attributes
  // rather than realloc-ing each time

  // just parse one element for now instead of recursing the children
  lexer_peak_next_token(lexer, &token);   
  while (token.type != TOKEN_GT) {

    // get attribute
    lexer_consume_token(lexer, &token);
    char *attribute_key = array_of(char, token.len + 1);
    char *attribute_value = NULL;
    strncpy(attribute_key, token.start, token.len);
    attribute_key[token.len] = '\0';

    // if its not a free hanging attribute and it has a value parse the value
    lexer_peak_next_token(lexer, &token);
    if (token.type == TOKEN_EQ) {
      lexer_consume_token(lexer, &token);
      lexer_get_next_token(lexer, &token);
      // recalc the length to remove quotes
      size_t attribute_value_length = token.len - 2;
      attribute_value = array_of(char, attribute_value_length + 1);
      strncpy(attribute_value, token.start + 1, attribute_value_length);
      attribute_value[attribute_value_length] = '\0';
      // assign attribute
      if (head->attributes != 1) {
        // check reallocs
        head->attribute_keys = realloc(head->attribute_keys, head->attributes + 1);
        head->attribute_values = realloc(head->attribute_values, head->attributes + 1);
      }
    }

    head->attribute_keys[head->attributes - 1] = attribute_key;
    head->attribute_values[head->attributes - 1] = attribute_value;

    lexer_peak_next_token(lexer, &token);
  }
  return head;
}

AstNode *new_factor(Lexer *lexer)
{
  LexerToken token;
  lexer_peak_next_token(lexer, &token);
  switch (token.type) {
    case TOKEN_LT: {
      lexer_consume_token(lexer, &token);
      return new_node(parse_markup_literal(lexer),
        ASTNODE_MARKUP_LITERAL);
    }
  }
}

AstNode *new_term(Lexer *lexer)
{
  AstNode *node = new_factor(lexer);
  return node;
}

AstNode *new_expression(Lexer *lexer)
{
  AstNode *node = new_term(lexer);
  // parse expression
  return node;
}

AstNode *new_statement(Lexer *lexer)
{
  // parse statement
  return new_expression(lexer);
}

AstNode *parse(Lexer *lexer)
{
  return new_statement(lexer);
}