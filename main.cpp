/* author: Erick Martinez
 * title: annual realized gains (maximum tax drag)
 * desc: simulates tax drag and advantages of investment accounts
 * date created: 12/29/2025
 * date last modified: 1/1/2026
 *
 * This implementation uses integer arithmetic (cents) for precision
 * Outputs to CSV compatible with Excel
 */

#include <iostream>
#include <iomanip>
#include <fstream>
#include <cmath>
#include <locale>
#include <stdexcept>

using namespace std;

// Type definitions -----------------------------------------------
using cents_t = int64_t;
using rate_t = int64_t; // Stored in basis points (1% = 100 bp)

cents_t toCents(long double dollars) {
    return static_cast<cents_t>(llround(dollars * 100.0L));
}

// Helper functions ------------------------------------------------
double toDollars(cents_t cents) {
    return cents / 100.0;
}

cents_t applyGrowth(cents_t balance, rate_t rateBP) {
    // Use rounding to minimize precision loss
    return (balance * (10000 + rateBP) + 5000) / 10000;
}

cents_t applyTax(cents_t amount, rate_t taxBP) {
    // Round to nearest cent
    return (amount * taxBP + 5000) / 10000;
}

// Main Simulation ------------------------------------------------
void runSimulation(cents_t principal, cents_t annualCont, rate_t rateBP,
                   rate_t taxBP, int years,
                   const string &filename = "simulation_results.csv") {
    cents_t taxableBal = principal;
    cents_t taxFreeBal = principal;

    ofstream file(filename);
    if (!file.is_open()) {
        throw runtime_error("Failed to open output file: " + filename);
    }

    file << "Year,Taxable_Balance,Tax_Advantaged_Balance,Tax_Drag_Loss\n";

    for (int year = 1; year <= years; ++year) {
        // Tax-Advantaged Account (Roth-style: grow tax-free)
        taxFreeBal += annualCont;
        taxFreeBal = applyGrowth(taxFreeBal, rateBP);

        // Taxable Account (pay tax on gains each year)
        taxableBal += annualCont;
        cents_t balanceBeforeGrowth = taxableBal;
        taxableBal = applyGrowth(taxableBal, rateBP);
        cents_t interest = taxableBal - balanceBeforeGrowth;
        cents_t tax = applyTax(interest, taxBP);
        taxableBal -= tax; // Subtract tax from balance

        // Output in dollars with 2 decimal places
        file << year << "," << fixed << setprecision(2) << toDollars(taxableBal)
             << "," << toDollars(taxFreeBal) << ","
             << toDollars(taxFreeBal - taxableBal) << "\n";
    }

    file.close();

    // Summary output
    cout << "\n=== Simulation Complete ===\n";
    cout << fixed << setprecision(2);
    cout << "After " << years << " years:\n";
    cout << "  Taxable Account:        $" << toDollars(taxableBal) << "\n";
    cout << "  Tax-Advantaged Account: $" << toDollars(taxFreeBal) << "\n";
    cout << "  Tax Drag Loss:          $" << toDollars(taxFreeBal - taxableBal)
         << "\n";
    cout << "  Loss as % of tax-free:  " << setprecision(1)
         << (100.0 * (taxFreeBal - taxableBal) / taxFreeBal) << "%\n";
    cout << "\nResults exported to " << filename << endl;
}

int main() {
    long double principalIn = 0.0L, annualIn = 0.0L;
    double ratePct = 0.0, taxPct = 0.0;
    int years = 10;

    locale loc("en_US.UTF-8");
    cin.imbue(loc);
    cout.imbue(loc);

    cout << "=== Wealth Management Simulation ===\n";
    cout << "This tool compares taxable vs. tax-advantaged investment growth\n";
    cout << "Assumptions: Tax-advantaged = Roth-style (no tax on withdrawal)\n";
    cout << "             Taxable = annual tax on investment gains\n\n";

    // Get inputs
    cout << "Enter Principal (e.g. $10,000.00): ";
    cin >> get_money(principalIn);

    cout << "Enter Annual Contribution (e.g. $500.00): ";
    cin >> get_money(annualIn);

    cout << "Enter Rate of Return (% per year, e.g. 7.5): ";
    cin >> ratePct;

    cout << "Enter Tax Rate (% on gains, e.g. 24): ";
    cin >> taxPct;

    cout << "Enter Number of Years (1-100): ";
    cin >> years;

    // Display verification
    cout << "\n--- Input Verification ---\n";
    cout << "Principal:          " << showbase << put_money(principalIn)
         << "\n";
    cout << "Annual Contribution:" << showbase << put_money(annualIn) << "\n";
    cout << "Rate of Return:     " << fixed << setprecision(2) << ratePct
         << "%\n";
    cout << "Tax Rate:           " << taxPct << "%\n";
    cout << "Years:              " << years << "\n";

    try {
        // Convert to internal representation
        cents_t principal = toCents(principalIn);
        cents_t annual = toCents(annualIn);
        rate_t rateBP = static_cast<rate_t>(llround(ratePct * 100));
        rate_t taxBP = static_cast<rate_t>(llround(taxPct * 100));

        runSimulation(principal, annual, rateBP, taxBP, years);

    } catch (const exception &e) {
        cerr << "Error: " << e.what() << endl;
        return 1;
    }

    return 0;
}
