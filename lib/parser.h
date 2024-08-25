#ifndef PARSER_H
#define PARSER_H

#include "lexer.h"

typedef struct AstNode AstNode;
typedef struct DocumentElement DocumentElement;

typedef enum {
  ASTNODE_MARKUP_LITERAL
} NodeType;

struct AstNode {
  NodeType type;
  void *expression;
};

typedef enum {
  ELEMENT_IMAGE
} ElementTags;

struct DocumentElement {
  ElementTags tagname;
  DocumentElement *children;
  size_t attributes;
  char **attribute_keys;
  char **attribute_values;
};

AstNode *parse(Lexer *lexer);
void free_ast(AstNode *head);

#endif