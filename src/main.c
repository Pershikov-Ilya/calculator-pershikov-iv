#include <ctype.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <math.h>

#define BUFSIZE 4096
#define MAX_NUMBER 2000000000

static const char* src;


int parse_number_int_without_sign(void);
int parse_primary_int(void);
int parse_division_int(void);
int parse_grammar_int(void);


double parse_number_double(void);
double parse_primary_double(void);
double parse_division_double(void);
double parse_grammar_double(void);


int parse_number_int_without_sign(void)
{
    while (isspace(*src))
        src++;
    if (!(*src >= '0' && *src <= '9')) {
        fprintf(stderr, "Error: унарные операторы не поддерживаются или ожидалось число, а получено '%c'.\n", *src);
        exit(1);
    }
    char* endptr;
    long number = strtol(src, &endptr, 10);
    if (src == endptr) {
        fprintf(stderr, "Error: ожидалось число, а получено '%c'\n", *src);
        exit(1);
    }
    if (number > MAX_NUMBER || number < -MAX_NUMBER) {
        fprintf(stderr, "Error: число вне допустимого диапазона (максимум 2*10^9).\n");
        exit(1);
    }
    src = endptr;
    return (int)number;
}

int parse_primary_int(void)
{
    while (isspace(*src))
        src++;
    if (*src == '(') {
        src++;
        int result = parse_grammar_int();
        while (isspace(*src))
            src++;
        if (*src == ')') {
            src++;
            return result;
        } else {
            fprintf(stderr, "Error: ожидалась ')', а получено '%c'\n", *src);
            exit(1);
        }
    }
    if (*src == '+' || *src == '-') {
        fprintf(stderr, "Error: унарные операторы не поддерживаются.\n");
        exit(1);
    }
    return parse_number_int_without_sign();
}

int parse_division_int(void)
{
    int result = parse_primary_int();
    while (1) {
        while (isspace(*src))
            src++;
        if (*src == '*' || *src == '/') {
            char op = *src;
            src++;
            while (isspace(*src))
                src++;
            if (!((*src >= '0' && *src <= '9') || *src == '(')) {
                fprintf(stderr, "Error: последовательные операторы недопустимы.\n");
                exit(1);
            }
            int rhs = parse_primary_int();
            if (op == '*')
                result *= rhs;
            else {
                if (rhs == 0) {
                    fprintf(stderr, "Error: деление на ноль.\n");
                    exit(1);
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
            while (isspace(*src))
                src++;
            if (!((*src >= '0' && *src <= '9') || *src == '(')) {
                fprintf(stderr, "Error: последовательные операторы недопустимы.\n");
                exit(1);
            }
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
        exit(1);
    }
    if (number > MAX_NUMBER || number < -MAX_NUMBER) {
        fprintf(stderr, "Error: число вне допустимого диапазона (максимум 2*10^9).\n");
        exit(1);
    }
    src = endptr;
    return number;
}

double parse_primary_double(void)
{
    while (isspace(*src))
        src++;
    if (*src == '(') {
        src++;
        double result = parse_grammar_double();
        while (isspace(*src))
            src++;
        if (*src == ')') {
            src++;
            return result;
        } else {
            fprintf(stderr, "Error: ожидалась ')', а получено '%c'\n", *src);
            exit(1);
        }
    }
    if (*src == '+' || *src == '-') {
        fprintf(stderr, "Error: унарные операторы не поддерживаются.\n");
        exit(1);
    }
    return parse_number_double();
}

double parse_division_double(void)
{
    double result = parse_primary_double();
    while (1) {
        while (isspace(*src))
            src++;
        if (*src == '*' || *src == '/') {
            char op = *src;
            src++;
            while (isspace(*src))
                src++;
            if (!((*src >= '0' && *src <= '9') || *src == '(')) {
                fprintf(stderr, "Error: последовательные операторы недопустимы.\n");
                exit(1);
            }
            double rhs = parse_primary_double();
            if (op == '*')
                result *= rhs;
            else {
                if (rhs == 0.0) {
                    fprintf(stderr, "Error: деление на ноль.\n");
                    exit(1);
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
            while (isspace(*src))
                src++;
            if (!((*src >= '0' && *src <= '9') || *src == '(')) {
                fprintf(stderr, "Error: последовательные операторы недопустимы.\n");
                exit(1);
            }
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

int parse_number_int(void) {
    return parse_number_int_without_sign();
}

int parse_factor_int(void) {
    return parse_primary_int();
}

void validate_input(const char* input)
{
    size_t len = strlen(input);
    if (len >= 1024) {
        fprintf(stderr, "Error: размер ввода превышает 1KiB.\n");
        exit(1);
    }
    for (size_t i = 0; i < len; i++) {
        char c = input[i];
        if (!(isdigit(c) || c == '(' || c == ')' || c == '+' || c == '-' || c == '*' || c == '/' || isspace(c))) {
            fprintf(stderr, "Error: недопустимый символ '%c' в вводе.\n", c);
            exit(1);
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
        exit(1);
    }

    validate_input(buffer);
    src = buffer;

    if (float_mode) {
        double result = parse_grammar_double();
        while (isspace(*src))
            src++;
        if (*src != '\0' && *src != '\n') {
            fprintf(stderr, "Error: обнаружены лишние символы после выражения: '%s'\n", src);
            exit(1);
        }
        printf("%.4f\n", result);
    } else {
        int result = parse_grammar_int();
        while (isspace(*src))
            src++;
        if (*src != '\0' && *src != '\n') {
            fprintf(stderr, "Error: обнаружены лишние символы после выражения: '%s'\n", src);
            exit(1);
        }
        printf("%d\n", result);
    }
    return 0;
}
#endif