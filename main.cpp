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

using namespace std;

struct YearlySnapshot {
    int year;
    double taxableBalance;
    double taxAdvantagedBalance;
    double taxDragLoss;
};

void runSimulation(double principal, double annualCont, double rate,
                   double taxRate) {
}

int main() {
    double principal, annualCont, rate, taxRate;
    runSimulation(principal, annualCont, rate, taxRate);
    return 0;
}
