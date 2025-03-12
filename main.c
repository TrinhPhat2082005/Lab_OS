#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <ctype.h>

#define LOG_FILE "calc.log"
#define MAX_LINE 100

void update_file(const char *entry) {
    FILE *file = fopen(LOG_FILE, "a");
    if (file) {
        fprintf(file, "%s\n", entry);
        fclose(file);
    }
}

void read_last_result(double *ans) {
    FILE *file = fopen(LOG_FILE, "r");
    if (!file) {
        *ans = 0;
        return;
    }
    char line[MAX_LINE], last_line[MAX_LINE] = "0";
    while (fgets(line, MAX_LINE, file)) {
        strcpy(last_line, line);
    }
    fclose(file);
    sscanf(last_line, " = %lf", ans);
}

int main() {
    double ans = 0;
    char input[MAX_LINE];
    char op;
    double num1, num2, result;
    read_last_result(&ans);

    while (1) {
        printf(">> ");
        if (!fgets(input, MAX_LINE, stdin)) {
            printf("INPUT ERROR\n");
            continue;
        }
        input[strcspn(input, "\n")] = 0; 

        if (strcmp(input, "EXIT") == 0) {
            break;
        }
        
        if (strstr(input, "ANS")) {
            char temp[MAX_LINE];
            snprintf(temp, sizeof(temp), "%.2lf", ans);
            char *pos = strstr(input, "ANS");
            if (pos) {
                size_t len_remaining = MAX_LINE - (pos - input) - 1;
                memmove(pos + strlen(temp), pos + 3, len_remaining - strlen(temp));
                memcpy(pos, temp, strlen(temp));
            }
        }

        if (sscanf(input, "%lf %c %lf", &num1, &op, &num2) == 3) {
            switch (op) {
                case '+': result = num1 + num2; break;
                case '-': result = num1 - num2; break;
                case 'x':
                case '*': result = num1 * num2; break;
                case '%':
                    if ((int)num2 == 0) {
                        printf("MATH ERROR: Division by zero\n");
                        continue;
                    }
                    result = (int)num1 % (int)num2; break;
                case '/':
                    if (num2 == 0) {
                        printf("MATH ERROR: Division by zero\n");
                        continue;
                    }
                    result = num1 / num2;
                    break;
                default:
                    printf("SYNTAX ERROR\n");
                    continue;
            }
            printf("%.2lf\n", result);
            ans = result;
            char log_entry[MAX_LINE];
            snprintf(log_entry, sizeof(log_entry), "%.50s = %.2lf", input, result);
            update_file(log_entry);
        } else {
            printf("SYNTAX ERROR\n");
        }
    }
    return 0;
}

