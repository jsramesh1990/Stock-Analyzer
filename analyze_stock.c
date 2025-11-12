#include <stdio.h>
#include <stdlib.h>
#include <string.h>

typedef struct {
    char symbol[20];
    float pe_ratio;
    float roe;
    float debt_to_equity;
    float free_cash_flow;
    float profit_margin;
    float promoter_holding;
    float pledged_shares;
} Stock;

void analyze_stock(Stock s) {
    int red_flags = 0;
    printf("\n🔍 Analyzing Stock: %s\n", s.symbol);
    printf("---------------------------------\n");

    if (s.pe_ratio > 40) {
        printf("⚠️  High P/E Ratio (%.2f)\n", s.pe_ratio);
        red_flags++;
    }
    if (s.roe < 10) {
        printf("⚠️  Low Return on Equity (%.2f%%)\n", s.roe);
        red_flags++;
    }
    if (s.debt_to_equity > 1) {
        printf("⚠️  High Debt-to-Equity Ratio (%.2f)\n", s.debt_to_equity);
        red_flags++;
    }
    if (s.free_cash_flow < 0) {
        printf("⚠️  Negative Free Cash Flow (%.2f)\n", s.free_cash_flow);
        red_flags++;
    }
    if (s.profit_margin < 0.05) {
        printf("⚠️  Low Profit Margin (%.2f%%)\n", s.profit_margin * 100);
        red_flags++;
    }
    if (s.promoter_holding < 50) {
        printf("⚠️  Low Promoter Holding (%.2f%%)\n", s.promoter_holding);
        red_flags++;
    }
    if (s.pledged_shares > 5) {
        printf("⚠️  High Pledged Shares (%.2f%%)\n", s.pledged_shares);
        red_flags++;
    }

    if (red_flags == 0)
        printf("✅ %s looks financially strong!\n", s.symbol);
    else
        printf("❌ %s has %d red flag(s)\n", s.symbol, red_flags);
}

int main() {
    Stock s;
    FILE *f = fopen("stock_data.txt", "r");
    if (!f) {
        perror("Error opening stock_data.txt");
        return 1;
    }

    fscanf(f, "symbol = %s\n", s.symbol);
    fscanf(f, "pe_ratio = %f\n", &s.pe_ratio);
    fscanf(f, "roe = %f\n", &s.roe);
    fscanf(f, "debt_to_equity = %f\n", &s.debt_to_equity);
    fscanf(f, "free_cash_flow = %f\n", &s.free_cash_flow);
    fscanf(f, "profit_margin = %f\n", &s.profit_margin);
    fscanf(f, "promoter_holding = %f\n", &s.promoter_holding);
    fscanf(f, "pledged_shares = %f\n", &s.pledged_shares);

    fclose(f);

    analyze_stock(s);
    return 0;
}

