/* author: Erick Martinez
 * title: wealth management simulation tool
 * desc: simulates tax drag and advantages of investments accounts
 * date created: 12/29/2025
 * date last modified: 1/1/2026
 *
 * This is the driver of implementation, that encapsulates the entire program
 *
 */
#include <iostream>
#include <fstream>

using namespace std;

struct YearlySnapshot {
    int year;
    double taxableBalance;
    double taxAdvantagedBalance;
    double taxDragLoss;
};

void runSimulation(double principal, double annualCont, double rate,
                   double taxRate) {
    double taxableBal = principal;
    double taxFreeBal = principal;
    rate = rate / 100;
    taxRate = taxRate / 100;

    ofstream file("simulation_results.csv");
    file
        << "Year,Taxable Balance, Tax-advantaged Balance, Wealth Lost to Tax\n";

    for (int i = 1; i <= 10; ++i) {
        // Tax-Advantaged Growth
        taxFreeBal += annualCont;
        taxFreeBal *= (1 + rate); // effective return: r(1 - T)

        // Taxable Growth
        taxableBal += annualCont;
        double interest = taxableBal * rate;
        double taxDebt = interest * taxRate;
        taxableBal += (interest - taxDebt);

        file << i << "," << taxableBal << "," << taxFreeBal << ","
             << (taxFreeBal - taxableBal) << endl;
    }
    file.close();
    cout << "Simulation complete. Results exported to simulation_results.csv"
         << endl;
}

int main() {
    double principal = 0.0, annualCont = 0.0, rate = 0.0, taxRate = 0.0;
    runSimulation(principal, annualCont, rate, taxRate);
    return 0;
}
