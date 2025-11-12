#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <ctype.h>

// Structure for stock data
typedef struct {
    char name[128];
    float pe_ratio;
    float pb_ratio;
    float roe;
    float roce;
    float debt_to_equity;
    float fcf;
    float dividend_yield;
    float promoter_holding;
    float eps_growth;
    float net_margin;
} Stock;

// Helper to extract number from line
float extract_number(const char *line) {
    float val = 0.0;
    sscanf(line, "%*[^0-9]%f", &val);
    return val;
}

// Trim whitespace
void trim(char *s) {
    int i = 0, j = 0;
    while (isspace(s[i])) i++;
    while (s[i]) s[j++] = s[i++];
    s[j] = '\0';
    while (--j >= 0 && isspace(s[j])) s[j] = '\0';
}

// Print evaluation result
void verdict(float value, const char *label, const char *condition, int good) {
    printf("%-17s : %-7.2f -> %s %s\n", label, value, 
        good ? "✅" : (value > 0 ? "⚠️" : "❌"), condition);
}

// Core analyzer
void analyze_stock(Stock s) {
    printf("\n🔍 Analyzing Stock: %s\n", s.name);
    printf("-----------------------------------------------\n");

    printf("| Metric           | Value  | Verdict                |\n");
    printf("| ---------------- | ------- | ---------------------- |\n");

    // Verdict Logic
    verdict(s.pe_ratio, "P/E", "Fair (<25)", s.pe_ratio < 25);
    verdict(s.pb_ratio, "P/B", "Healthy (<3)", s.pb_ratio < 3);
    verdict(s.roe, "ROE (%)", "Strong (>15%)", s.roe > 15);
    verdict(s.roce, "ROCE (%)", "Efficient (>15%)", s.roce > 15);
    verdict(s.debt_to_equity, "D/E", "Low Debt (<0.5)", s.debt_to_equity < 0.5);
    verdict(s.fcf, "FCF (Cr)", "Positive Cash Flow", s.fcf > 0);
    verdict(s.dividend_yield, "Dividend Yield", "Reasonable (1–2%)", s.dividend_yield >= 1);
    verdict(s.promoter_holding, "Promoter Holding", "Confident Mgmt (>50%)", s.promoter_holding > 50);
    verdict(s.eps_growth, "EPS Growth (3Y%)", "Growing (>10%)", s.eps_growth > 10);
    verdict(s.net_margin, "Net Margin (%)", "Good Profitability (>10%)", s.net_margin > 10);

    printf("-----------------------------------------------\n");

    // Red flag analysis
    int red_flags = 0;
    if (s.pe_ratio > 40) red_flags++;
    if (s.roe < 10) red_flags++;
    if (s.roce < 10) red_flags++;
    if (s.debt_to_equity > 0.5) red_flags++;
    if (s.promoter_holding < 50) red_flags++;
    if (s.dividend_yield < 1) red_flags++;

    printf("🔎 Red Flags Found: %d\n", red_flags);
    if (red_flags == 0)
        printf("✅ %s looks financially strong!\n", s.name);
    else
        printf("❌ %s has %d potential issue(s)\n", s.name, red_flags);
}

// Main parser
int main() {
    FILE *f = fopen("stock_data.txt", "r");
    if (!f) {
        perror("Error opening stock_data.txt");
        return 1;
    }

    Stock s = {"N/A", 0, 0, 0, 0, 0, 0, 0, 0, 0, 0};
    char line[512];
    int found_name = 0, in_shareholding = 0;

    while (fgets(line, sizeof(line), f)) {
        trim(line);

        if (!found_name && (strstr(line, "Ltd") || strstr(line, "Limited")) &&
            !strstr(line, "BSE") && !strstr(line, "NSE") &&
            !strstr(line, "About") && strlen(line) > 5) {
            strncpy(s.name, line, sizeof(s.name));
            found_name = 1;
        }

        if (strstr(line, "Stock P/E")) s.pe_ratio = extract_number(line);
        if (strstr(line, "Book Value")) s.pb_ratio = extract_number(line) ? (s.pe_ratio / extract_number(line)) : 0; // approximate P/B
        if (strstr(line, "ROCE")) s.roce = extract_number(line);
        if (strstr(line, "ROE") && !strstr(line, "ROCE")) s.roe = extract_number(line);
        if (strstr(line, "Dividend Yield")) s.dividend_yield = extract_number(line);

        if (strstr(line, "Debt to Equity")) s.debt_to_equity = extract_number(line);
        if (strstr(line, "Free Cash Flow")) s.fcf = extract_number(line);
        if (strstr(line, "EPS Growth")) s.eps_growth = extract_number(line);
        if (strstr(line, "Net Margin")) s.net_margin = extract_number(line);

        if (strstr(line, "Shareholding Pattern")) in_shareholding = 1;
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

