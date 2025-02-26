#include <ctype.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#define BUFSIZE 4096

static const char* src;

int parse_number_int(void);
int parse_factor_int(void);
int parse_division_int(void);
int parse_grammar_int(void);

double parse_number_double(void);
double parse_factor_double(void);
double parse_division_double(void);
double parse_grammar_double(void);

int parse_number_int(void)
{
    while (isspace(*src))
        src++;
    char* endptr;
    long number = strtol(src, &endptr, 10);
    if (src == endptr) {
        fprintf(stderr, "Error: ожидалось число, а получено '%c'\n", *src);
        exit(EXIT_FAILURE);
    }
    src = endptr;
    return (int)number;
}

int parse_factor_int(void)
{
    while (isspace(*src))
        src++;
    int result = 0;
    if (*src == '(') {
        src++;
        result = parse_grammar_int();
        while (isspace(*src))
            src++;
        if (*src == ')')
            src++;
        else {
            fprintf(stderr, "Error: ожидалась ')', а получено '%c'\n", *src);
            exit(EXIT_FAILURE);
        }
    } else {
        result = parse_number_int();
    }
    return result;
}

int parse_division_int(void)
{
    int result = parse_factor_int();
    while (1) {
        while (isspace(*src))
            src++;
        if (*src == '*' || *src == '/') {
            char op = *src;
            src++;
            int rhs = parse_factor_int();
            if (op == '*')
                result *= rhs;
            else {
                if (rhs == 0) {
                    fprintf(stderr, "Error: деление на ноль.\n");
                    exit(EXIT_FAILURE);
                }
                result /= rhs;
            }
        } else {
            break;
        }
    }
    return result;
}

int parse_grammar_int(void)
{
    int result = parse_division_int();
    while (1) {
        while (isspace(*src))
            src++;
        if (*src == '+' || *src == '-') {
            char op = *src;
            src++;
            int rhs = parse_division_int();
            if (op == '+')
                result += rhs;
            else
                result -= rhs;
        } else {
            break;
        }
    }
    return result;
}

double parse_number_double(void)
{
    while (isspace(*src))
        src++;
    char* endptr;
    double number = strtod(src, &endptr);
    if (src == endptr) {
        fprintf(stderr, "Error: ожидалось число, а получено '%c'\n", *src);
        exit(EXIT_FAILURE);
    }
    src = endptr;
    return number;
}

double parse_factor_double(void)
{
    while (isspace(*src))
        src++;
    double result = 0.0;
    if (*src == '(') {
        src++;
        result = parse_grammar_double();
        while (isspace(*src))
            src++;
        if (*src == ')')
            src++;
        else {
            fprintf(stderr, "Error: ожидалась ')', а получено '%c'\n", *src);
            exit(EXIT_FAILURE);
        }
    } else {
        result = parse_number_double();
    }
    return result;
}

double parse_division_double(void)
{
    double result = parse_factor_double();
    while (1) {
        while (isspace(*src))
            src++;
        if (*src == '*' || *src == '/') {
            char op = *src;
            src++;
            double rhs = parse_factor_double();
            if (op == '*')
                result *= rhs;
            else {
                if (rhs == 0.0) {
                    fprintf(stderr, "Error: деление на ноль.\n");
                    exit(EXIT_FAILURE);
                }
                result /= rhs;
            }
        } else {
            break;
        }
    }
    return result;
}

double parse_grammar_double(void)
{
    double result = parse_division_double();
    while (1) {
        while (isspace(*src))
            src++;
        if (*src == '+' || *src == '-') {
            char op = *src;
            src++;
            double rhs = parse_division_double();
            if (op == '+')
                result += rhs;
            else
                result -= rhs;
        } else {
            break;
        }
    }
    return result;
}

void validate_input(const char* input)
{
    size_t len = strlen(input);
    if (len >= 1024) {
        fprintf(stderr, "Error: размер ввода превышает 1KiB.\n");
        exit(EXIT_FAILURE);
    }
    for (size_t i = 0; i < len; i++) {
        char c = input[i];
        if (!(isdigit(c) || c == '(' || c == ')' || c == '+' || c == '-' || c == '*' || c == '/' || isspace(c))) {
            fprintf(stderr, "Error: недопустимый символ '%c' в вводе.\n", c);
            exit(EXIT_FAILURE);
        }
    }
}

#ifndef UNIT_TEST
int main(int argc, char* argv[])
{
    int float_mode = 0;

    if (argc > 1 && strcmp(argv[1], "--float") == 0) {
        float_mode = 1;
    }

    char buffer[BUFSIZE];
    if (fgets(buffer, sizeof(buffer), stdin) == NULL) {
        fprintf(stderr, "Error: не удалось прочитать ввод.\n");
        exit(EXIT_FAILURE);
    }

    validate_input(buffer);

    src = buffer;

    if (float_mode) {
        double result = parse_grammar_double();
        while (isspace(*src))
            src++;
        if (*src != '\0' && *src != '\n') {
            fprintf(stderr,
                "Error: обнаружены лишние символы после выражения: '%s'\n", src);
            exit(EXIT_FAILURE);
        }
        printf("%f\n", result);
    } else {
        int result = parse_grammar_int();
        while (isspace(*src))
            src++;
        if (*src != '\0' && *src != '\n') {
            fprintf(stderr,
                "Error: обнаружены лишние символы после выражения: '%s'\n", src);
            exit(EXIT_FAILURE);
        }
        printf("%d\n", result);
    }
    return 0;
}
#endif
