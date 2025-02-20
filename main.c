#include <stdio.h>
#include <stdlib.h>
#include <ctype.h>
#include <string.h> 

#define BUFSIZE 4096

static const char *src;

int parse_grammar(void);
int parse_division(void);
int parse_factor(void);

int parse_number(void) {
    while (isspace(*src))
        src++;
    
    char *endptr;
    long number = strtol(src, &endptr, 10);
    if (src == endptr) 
    {
        fprintf(stderr, "Error: ожидалось число, а получено: '%c'\n", *src);
        exit(EXIT_FAILURE);
    }
    src = endptr;
    return (int)number;
}

int parse_factor(void) {
    while (isspace(*src))
        src++;
    
    int result = 0;
    if (*src == '(') 
    {
        src++;  
        result = parse_grammar();
        while (isspace(*src))
            src++;
        if (*src == ')')
            src++;  
        else 
        {
            fprintf(stderr, "Error: ожидалась ')', а получено '%c'\n", *src);
            exit(EXIT_FAILURE);
        }
    } else 
        result = parse_number();
    return result;
}


int parse_division(void) {
    int result = parse_factor();
    while (1) 
    {
        while (isspace(*src))
            src++;
        if (*src == '*' || *src == '/') 
        {
            char op = *src;
            src++;
            int rhs = parse_factor();
            if (op == '*')
                result *= rhs;
            else 
            {
                if (rhs == 0) 
                {
                    fprintf(stderr, "Error: деление на ноль.\n");
                    exit(EXIT_FAILURE);
                }
                result /= rhs;
            }
        } else
            break;
    }
    return result;
}


int parse_grammar(void) {
    int result = parse_division();
    while (1) {
        while (isspace(*src))
            src++;
        if (*src == '+' || *src == '-') 
        {
            char op = *src;
            src++;
            int rhs = parse_division();
            if (op == '+')
                result += rhs;
            else
                result -= rhs;
        } else
            break;
    }
    return result;
}

int main(void) {
    char buffer[BUFSIZE];
    
    if (fgets(buffer, sizeof(buffer), stdin) == NULL) 
    {
        fprintf(stderr, "Error: не удалось прочитать ввод.\n");
        exit(EXIT_FAILURE);
    }
    
    src = buffer;
    
    int result = parse_grammar();
    
    printf("%d\n", result);
    return 0;
}