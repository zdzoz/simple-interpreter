#include "token.h"

#include <ctype.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

static int get_id(const char* str, char* buf);

Token* tokenize(const char* str)
{
    Token* head = NULL;
    Token* curr;
    while (*str != 0) {
        char c = *str;
        if (isspace(c)) {
            str++;
            continue;
        }

        Token* tok = calloc(1, sizeof(Token));
        tok->tok = T_UNKNOWN;
        switch (c) {
        case '.':
            tok->tok = T_PERIOD;
            break;
        case ';':
            tok->tok = T_SEMI;
            break;
        case '-':
            tok->tok = T_MIN;
            break;
        case '+':
            tok->tok = T_PLUS;
            break;
        case '*':
            tok->tok = T_MUL;
            break;
        case '/':
            tok->tok = T_DIV;
            break;
        case '(':
            tok->tok = T_LPAREN;
            break;
        case ')':
            tok->tok = T_RPAREN;
            break;
        case '<': { // T_ASSIGN <-
            char next = *(str + 1);
            if (next == '-') {
                tok->tok = T_ASSIGN;
                str++;
                break;
            }
            // go to default case
        }
        default:
            if (isdigit(c)) {
                const char* temp = str;
                int num = 0;
                int count = 0;
                do {
                    num = num * 10 + (*temp - '0');
                    count++;
                    temp++;
                } while (isdigit(*temp));
                tok->tok = T_NUM;
                tok->num = num;
                str += count - 1;
                break;
            } else if (isalpha(c)) {
                char buf[MAX_IDENTIFIER];
                int count = get_id(str, buf);
#if NDEBUG == 1
                if (c == 'c' && strcmp(buf, "computation") == 0)
#else
                if (c == 'c' && (strcmp(buf, "computation") == 0 || strcmp(buf, "comp") == 0))
#endif
                    {
                        tok->tok = T_COMPUTE;
                    }
                else if (c == 'v' && strcmp(buf, "var") == 0) {
                    tok->tok = T_VAR;
                } else { // NOTE: is an identifier
                    tok->tok = T_ID;
                    tok->str = malloc(strlen(buf) + 1);
                    strcpy(tok->str, buf);
                }
                str += count - 1;
            } else {
                const char* temp = str;
                int count = 0;
                while (!isspace(*temp) && *temp != 0) {
                    count++;
                    temp++;
                }
                tok->str = malloc(sizeof(char) * count + 1);
                strncpy(tok->str, str, count + 1);
                tok->str[count] = '\0';
                str += count - 1;
            }
        }

        if (head == NULL) {
            head = tok;
            curr = head;
            str++;
            continue;
        }
        curr->next = tok;
        curr = curr->next;
        str++;
    }

    return head;
}

static int get_id(const char* str, char* buf)
{
    int count = 0;
    while (isalnum(*str)) {
        *buf++ = *str++;
        count++;
    }
    *buf = '\0';
    return count;
}

void free_tokens(Token* toks)
{
    while (toks != NULL) {
        switch (toks->tok) {
        case T_UNKNOWN:
        case T_ID:
            free(toks->str);
            break;
        default:
            break;
        }

        Token* temp = toks;
        toks = toks->next;
        free(temp);
    }
}

void print_tokens(Token* toks)
{
    printf("(");
    while (toks != NULL) {
        PRINT_TOKEN(stdout, toks);
        printf("; ");

        toks = toks->next;
    }
    printf(")\n");
}
