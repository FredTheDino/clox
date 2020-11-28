#include <stdio.h>
#include <string.h>

#include "common.h"
#include "scanner.h"

typedef struct {
    const char* line_start;
    const char* start;
    const char* current;
    int col;
    int line;
} Scanner;

Scanner scanner;

void initScanner(const char* source) {
    scanner.line_start = source;
    scanner.start = source;
    scanner.current = source;
    scanner.line = 1;
    scanner.col = 1;
}

static bool isAtEnd() {
    return *scanner.current == '\0';
}

static Token makeToken(TokenType type) {
    Token token;
    token.type = type;
    token.start = scanner.start;
    token.length = (int)(scanner.current - scanner.start);
    token.line = scanner.line;

    return token;
}

static Token errorToken(const char* msg) {
    Token token;
    token.type = TOKEN_ERROR;
    token.start = msg;
    token.length = (int)strlen(msg);
    token.line = scanner.line;

    return token;
}

static char peek() {
    return *scanner.current;
}

static char peekNext() {
    return scanner.current[1];
}

static char advance() {
    scanner.current++;
    return scanner.current[-1];
}

static char match(char c) {
    if (isAtEnd()) return false;
    if (*scanner.current != c) return false;

    scanner.current++;
    return true;
}

static bool isDigit(char c) {
    return c >= '0' && c <= '9';
}

static bool isAlpha(char c) {
    return (c >= 'a' && c <= 'z') ||
           (c >= 'A' && c <= 'Z') ||
            c == '_';
}

static void skipWhitespace() {
    for (;;) {
        char c = peek();
        switch (c) {
            case ' ':
            case '\r':
            case '\t':
                advance();
                break;

            case '\n':
                scanner.line++;
                advance();
                break;

            case '/':
                if (peekNext() == '/') {
                    while (peek() != '\n' && !isAtEnd()) advance();
                    break;
                } else {
                    return;
                }

            default:
                return;
        }
    }
}

static Token string() {
    while (peek() != '"' && !isAtEnd()) {
        if (peek() == '\n') scanner.line++;
        advance();
    }

    if (isAtEnd()) return errorToken("Unterminated string.");

    advance();
    return makeToken(TOKEN_STRING);
}

static Token number() {
    while (isDigit(peek())) advance();

    if (peek() == '.' && isDigit(peekNext())) {
        advance();
    }

    while (isDigit(peek())) advance();

    return makeToken(TOKEN_NUMBER);
}

static TokenType checkKeyword(int len, const char* str, TokenType t) {
    if (scanner.current - scanner.start == len && memcmp(scanner.start, str, len) == 0) {
        return t;
    }
    return TOKEN_IDENTIFIER;
}

static TokenType identifierType() {
#undef CHECK
#define CHECK(pre, str, token) \
    case pre: return checkKeyword(sizeof(str) - 1, str, token)

    switch (scanner.start[0]) {
        CHECK('a', "and", TOKEN_AND);
        CHECK('c', "class", TOKEN_CLASS);
        CHECK('e', "else", TOKEN_ELSE);
        case 'f':
            switch (scanner.start[1]) {
                CHECK('a', "false", TOKEN_FALSE);
                CHECK('o', "for", TOKEN_FOR);
                CHECK('u', "fun", TOKEN_FUN);
            }
            break;
        CHECK('i', "if", TOKEN_IF);
        CHECK('n', "nil", TOKEN_NIL);
        CHECK('o', "or", TOKEN_OR);
        CHECK('p', "print", TOKEN_PRINT);
        CHECK('r', "return", TOKEN_RETURN);
        CHECK('s', "super", TOKEN_SUPER);
        case 't':
            switch (scanner.start[1]) {
                CHECK('h', "this", TOKEN_THIS);
                CHECK('r', "true", TOKEN_TRUE);
            }
            break;
        CHECK('v', "var", TOKEN_VAR);
        CHECK('w', "while", TOKEN_WHILE);
    }
#undef CHECK
    return TOKEN_IDENTIFIER;
}

static Token identifier() {
    while (isAlpha(peek()) ||isDigit(peek())) advance();
    return makeToken(identifierType());
}

Token scanToken() {
    skipWhitespace();
    scanner.start = scanner.current;

    if (isAtEnd()) return makeToken(TOKEN_EOF);

    char c = advance();

    if (isDigit(c)) return number();
    if (isAlpha(c)) return identifier();

#define SIMPLE_TOKEN(c, t) case c: return makeToken(t)
#define DUAL_TOKEN(c1, c2, t1, t2) case c1: return makeToken(match(c2) ? t1 : t2)
    switch (c) {
        SIMPLE_TOKEN('(', TOKEN_LEFT_PAREN);
        SIMPLE_TOKEN(')', TOKEN_RIGHT_PAREN);
        SIMPLE_TOKEN('{', TOKEN_LEFT_BRACE);
        SIMPLE_TOKEN('}', TOKEN_RIGHT_BRACE);
        SIMPLE_TOKEN(';', TOKEN_SEMICOLON);
        SIMPLE_TOKEN(',', TOKEN_COMMA);
        SIMPLE_TOKEN('.', TOKEN_DOT);
        SIMPLE_TOKEN('-', TOKEN_MINUS);
        SIMPLE_TOKEN('+', TOKEN_PLUS);
        SIMPLE_TOKEN('/', TOKEN_SLASH);
        SIMPLE_TOKEN('*', TOKEN_STAR);

        DUAL_TOKEN('!', '=', TOKEN_BANG_EQUAL, TOKEN_BANG);
        DUAL_TOKEN('=', '=', TOKEN_EQUAL_EQUAL, TOKEN_EQUAL);
        DUAL_TOKEN('<', '=', TOKEN_LESS_EQUAL, TOKEN_LESS);
        DUAL_TOKEN('>', '=', TOKEN_GREATER_EQUAL, TOKEN_GREATER);

        case '"': return string();
    }

    return errorToken("Unexpected character.");
}
