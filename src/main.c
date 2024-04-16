#include <assert.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#include "defines.h"
#include "token.h"

typedef struct {
    char* id;
    int val;
} var_t;

static var_t var_arr[ARR_SIZE_MAX];

void set_id(Token* tok, int val);
int get_id(Token* tok, int* val);
char readline(char* buf);

void computation(Token** toks);
int expression(Token** toks);
int term(Token** toks);
int factor(Token** toks);

int main(void)
{
#if NDEBUG == 0
    fprintf(stderr, "DEBUG MODE ON\n");
#endif
    char buf[MAX_SIZE];
    while (readline(buf) != EOF) {
        Token* toks = tokenize(buf);
        Token* temp = toks;
        while (temp != NULL) {
            memset(var_arr, 0, sizeof(var_t) * ARR_SIZE_MAX);
            computation(&temp);
        }
        if (toks != NULL)
            assert(toks != temp);
        free_tokens(toks);
    }

    return 0;
}

void set_id(Token* tok, int val)
{
    for (int i = 0; i < ARR_SIZE_MAX; i++) {
        if (var_arr[i].id == NULL) {
            var_arr[i].id = tok->str; // don't need to allocate because only references as long as tok is alive
            var_arr[i].val = val;
            return;
        }
    }
}

int get_id(Token* tok, int* val)
{
    // TODO: make hashmap
    for (int i = 0; i < ARR_SIZE_MAX; i++) {
        if (var_arr[i].id != NULL && strcmp(var_arr[i].id, tok->str) == 0) {
            if (val != NULL)
                *val = var_arr[i].val;
            return 1;
        }
    }
    // doesn't exist
    return 0;
}

char readline(char* buf)
{
    char c;
    u32 offset = 0;
    while ((c = getchar()) != EOF && c != '\n') {
        buf[offset++] = c;
    }
    buf[offset] = '\0';
    return c;
}

static int error = 0;
#define REPORT_ERROR(...)                              \
    do {                                               \
        error = 1;                                     \
        fprintf(stderr, "[SYNTAX ERROR] "__VA_ARGS__); \
    } while (0)

#define GET_TOKEN(toks) (*toks)
#define GET_NEXT(toks) ((*toks)->next)
#define GET_TOK_T(toks) ((*toks) != NULL ? (*toks)->tok : T_UNKNOWN)
#define GET_NEXT_T(toks) ((*toks) != NULL ? (*toks)->next->tok : T_UNKNOWN)
#define EAT_TOK(toks)          \
    do {                       \
        if (*toks == NULL)     \
            continue;          \
        *toks = (*toks)->next; \
    } while (0);

void computation(Token** toks)
{
    // print_tokens(*toks);

    if (GET_TOK_T(toks) != T_COMPUTE)
        REPORT_ERROR("Missing initial keyword 'computation'\n");
    else
        EAT_TOK(toks);

    if (*toks == NULL || GET_TOK_T(toks) == T_PERIOD) {
        EAT_TOK(toks);
        REPORT_ERROR("Expected expression but got NULL\n");
        return;
    }

    int result;
    while (*toks != NULL && GET_TOK_T(toks) != T_PERIOD) {
        if (*toks == NULL)
            break;
        switch (GET_TOK_T(toks)) {
        case T_VAR: { // var ident <- expression
            EAT_TOK(toks);
            if (GET_TOK_T(toks) == T_VAR) {
                REPORT_ERROR("Unexpected token (VAR)\n");
                break;
            }

            if (GET_TOK_T(toks) != T_ID) {
                REPORT_ERROR("Missing identifier\n");
                break;
            }

            if (get_id(GET_TOKEN(toks), NULL)) {
                REPORT_ERROR("Identifier already exists\n");
                break;
            }

            Token* tok = GET_TOKEN(toks);
            EAT_TOK(toks);
            if (GET_TOK_T(toks) != T_ASSIGN) {
                REPORT_ERROR("Expected assignment operator '<-'\n");
                break;
            }
            EAT_TOK(toks);

            int expr = expression(toks);
            if (GET_TOK_T(toks) != T_SEMI) {
                REPORT_ERROR("Missing semi ';'\n");
                break;
            }
            EAT_TOK(toks);

            set_id(tok, expr);
        } break;
        case T_UNKNOWN:
            REPORT_ERROR("");
            PRINT_TOKEN(stderr, *toks);
            fprintf(stderr, "\n");
            EAT_TOK(toks);
            break;
        case T_SEMI:
            EAT_TOK(toks);
        default:
            result = expression(toks);
            if (!error)
                printf("%d\n", result);
            else
                EAT_TOK(toks);
        }
    }

    if (*toks == NULL || (*toks != NULL && GET_TOK_T(toks) != T_PERIOD))
        REPORT_ERROR("No terminating character '.'\n");
    EAT_TOK(toks);
    error = 0;
}

int expression(Token** toks)
{
    int res = term(toks);
    TokenType op;
    int loop = 1;
    while (loop) {
        op = GET_TOK_T(toks);
        switch (op) {
        case T_PLUS:
            EAT_TOK(toks);
            res += term(toks);
            break;
        case T_MIN:
            EAT_TOK(toks);
            res -= term(toks);
            break;
        default:
            loop = 0;
            break;
        }
    }
    return res;
}

int term(Token** toks)
{
    int res = factor(toks);
    TokenType op;
    int loop = 1;
    while (loop) {
        op = GET_TOK_T(toks);
        switch (op) {
        case T_MUL:
            EAT_TOK(toks);
            res *= factor(toks);
            break;
        case T_DIV:
            EAT_TOK(toks);
            res /= factor(toks);
            break;
        default:
            loop = 0;
            break;
        }
    }
    return res;
}

int factor(Token** toks)
{
    int res = 0;
    TokenType curr = GET_TOK_T(toks);
    switch (curr) {
    case T_LPAREN:
        EAT_TOK(toks);
        res = expression(toks);
        if (GET_TOK_T(toks) != T_RPAREN) {
            REPORT_ERROR("Missing RPAREN ')'\n");
        } else
            EAT_TOK(toks);
        break;
    case T_NUM:
        res += GET_TOKEN(toks)->num;
        EAT_TOK(toks);
        break;
    case T_ID: {
        if (!get_id(GET_TOKEN(toks), &res)) {
            REPORT_ERROR("Unknown ");
            PRINT_TOKEN(stderr, GET_TOKEN(toks));
            fprintf(stderr, "\n");
            break;
        }
        EAT_TOK(toks);
    } break;
    default:
        REPORT_ERROR("Unexpected Token (");
        PRINT_TOKEN(stderr, GET_TOKEN(toks));
        fprintf(stderr, ")\n");
        break;
    }
    return res;
}
