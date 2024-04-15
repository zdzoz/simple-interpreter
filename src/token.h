#ifndef H_TOKEN
#define H_TOKEN

#include "defines.h"

#define MAX_IDENTIFIER MAX_SIZE

typedef enum {
    // keywords
    T_COMPUTE = 0,
    T_VAR,

    // DELIMITERS
    T_SEMI,
    T_ASSIGN,
    T_PERIOD,
    T_LPAREN,
    T_RPAREN,

    // OPERATORS
    T_PLUS,
    T_MIN,
    T_DIV,
    T_MUL,

    // MISC
    T_ID,
    T_NUM,

    T_UNKNOWN,

    T_MAX_TOKENS
} TokenType;

typedef struct Token {
    TokenType tok;
    union {
        u64 num;
        char* str;
    };
    struct Token* next;
} Token;

Token* tokenize(const char* str);
void free_tokens(Token* toks);
void print_tokens(Token* toks);

#define PRINT_TOKEN(file, tok_)                      \
    do {                                             \
        if ((tok_) == NULL)                          \
            continue;                                \
        char* str = "";                              \
        char str2[0xff] = "";                        \
        switch ((tok_)->tok) {                       \
        case T_NUM:                                  \
            str = "NUM -> ";                         \
            snprintf(str2, 0xff, "%lld", (tok_)->num); \
            break;                                   \
        case T_ID:                                   \
            str = "ID -> ";                          \
            strcpy(str2, (tok_)->str);               \
            break;                                   \
        case T_COMPUTE:                              \
            str = "COMPUTE";                         \
            break;                                   \
        case T_VAR:                                  \
            str = "VAR";                             \
            break;                                   \
        case T_SEMI:                                 \
            str = "SEMI";                            \
            break;                                   \
        case T_ASSIGN:                               \
            str = "ASSIGN";                          \
            break;                                   \
        case T_PERIOD:                               \
            str = "PERIOD";                          \
            break;                                   \
        case T_PLUS:                                 \
            str = "PLUS";                            \
            break;                                   \
        case T_MIN:                                  \
            str = "MINUS";                           \
            break;                                   \
        case T_DIV:                                  \
            str = "DIVIDE";                          \
            break;                                   \
        case T_MUL:                                  \
            str = "MULTIPLY";                        \
            break;                                   \
        case T_LPAREN:                               \
            str = "LPAREN";                          \
            break;                                   \
        case T_RPAREN:                               \
            str = "RPAREN";                          \
            break;                                   \
        default:                                     \
            if ((tok_)->str != NULL) {               \
                str = "UNKNOWN -> ";                 \
                strcpy(str2, (tok_)->str);           \
            } else {                                 \
                str = "UNKNOWN";                     \
            }                                        \
            break;                                   \
        }                                            \
                                                     \
        if (*str2 != '\0')                           \
            fprintf(file, "%s'%s'", str, str2);      \
        else                                         \
            fprintf(file, "%s", str);                \
    } while (0)

#endif // H_TOKEN
