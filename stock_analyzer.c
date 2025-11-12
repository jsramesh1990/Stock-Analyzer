#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <ctype.h>

// Structure for stock data
typedef struct {
    char name[128];
    float pe_ratio;
    float roe;
    float roce;
    float dividend_yield;
    float promoter_holding;
} Stock;

// Extract number from string
float extract_number(const char *line) {
    float value = 0.0;
    sscanf(line, "%*[^0-9]%f", &value);
    return value;
}

// Trim spaces
void trim(char *s) {
    int i, j;
    for (i = 0; s[i] == ' ' || s[i] == '\t'; i++);
    for (j = 0; s[i]; i++, j++) s[j] = s[i];
    s[j] = '\0';
    for (i = strlen(s) - 1; i >= 0 && (s[i] == ' ' || s[i] == '\t' || s[i] == '\n'); i--)
        s[i] = '\0';
}

// Analyze and report
void analyze_stock(Stock s) {
    printf("\n🔍 Analyzing Stock: %s\n", s.name);
    printf("-----------------------------------------------\n");
    printf("P/E Ratio         : %.2f\n", s.pe_ratio);
    printf("ROE               : %.2f %%\n", s.roe);
    printf("ROCE              : %.2f %%\n", s.roce);
    printf("Dividend Yield    : %.2f %%\n", s.dividend_yield);
    printf("Promoter Holding  : %.2f %%\n", s.promoter_holding);
    printf("-----------------------------------------------\n");

    printf("\n| Parameter        | Ideal Condition                    |\n");
    printf("| ---------------- | ---------------------------------- |\n");
    printf("| EPS Growth       | Positive, stable, >10%% YoY         |\n");
    printf("| P/E Ratio        | Below industry average (for value) |\n");
    printf("| ROE              | >15%%                               |\n");
    printf("| D/E Ratio        | <0.5 (for safety)                  |\n");
    printf("| FCF              | Positive and growing               |\n");
    printf("| Promoter Holding | >50%%                               |\n");
    printf("| Pledged Shares   | <5%%                                |\n");
    printf("| Revenue Growth   | Steady upward trend                |\n");
    printf("| Dividend Yield   | >1–2%% (for mature companies)       |\n");

    printf("\n-----------------------------------------------\n");
    printf("🔎 Checking for Red Flags:\n");

    int red_flags = 0;

    if (s.pe_ratio > 40) {
        printf("⚠️  High P/E Ratio (%.2f)\n", s.pe_ratio);
        red_flags++;
    }
    if (s.roe < 10) {
        printf("⚠️  Low Return on Equity (%.2f%%)\n", s.roe);
        red_flags++;
    }
    if (s.roce < 10) {
        printf("⚠️  Low ROCE (%.2f%%)\n", s.roce);
        red_flags++;
    }
    if (s.dividend_yield < 1) {
        printf("⚠️  Low Dividend Yield (%.2f%%)\n", s.dividend_yield);
        red_flags++;
    }
    if (s.promoter_holding < 50) {
        printf("⚠️  Low Promoter Holding (%.2f%%)\n", s.promoter_holding);
        red_flags++;
    }

    printf("-----------------------------------------------\n");
    if (red_flags == 0)
        printf("✅ %s looks financially strong!\n", s.name);
    else
        printf("❌ %s has %d red flag(s)\n", s.name, red_flags);
}

int main() {
    FILE *f = fopen("stock_data.txt", "r");
    if (!f) {
        perror("Error opening stock_data.txt");
        return 1;
    }

    Stock s = {"N/A", 0, 0, 0, 0, 0};
    char line[512];
    int found_name = 0;
    int in_shareholding = 0;

    while (fgets(line, sizeof(line), f)) {
        trim(line);

        // Detect name before About section
        if (!found_name && (strstr(line, "Ltd") || strstr(line, "Limited")) &&
            !strstr(line, "BSE") && !strstr(line, "NSE") &&
            !strstr(line, "About") && strlen(line) > 5) {
            strncpy(s.name, line, sizeof(s.name));
            found_name = 1;
            continue;
        }

        // Extract main ratios
        if (strstr(line, "Stock P/E")) s.pe_ratio = extract_number(line);
        if (strstr(line, "ROCE")) s.roce = extract_number(line);
        if (strstr(line, "ROE") && !strstr(line, "ROCE")) s.roe = extract_number(line);
        if (strstr(line, "Dividend Yield")) s.dividend_yield = extract_number(line);

        // Detect when Shareholding section starts
        if (strstr(line, "Shareholding Pattern")) {
            in_shareholding = 1;
            continue;
        }

        // Once inside shareholding section, extract largest % number as promoter holding
        if (in_shareholding && strstr(line, "%")) {
            float val = extract_number(line);
            if (val > s.promoter_holding)
                s.promoter_holding = val;
        }
    }

    fclose(f);
    analyze_stock(s);
    return 0;
}

