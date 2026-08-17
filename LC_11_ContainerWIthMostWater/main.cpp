#include <iostream>
#include <vector>
#include "solution.cpp"

bool test(std::vector<int> &height, Solution *psol, int expected, bool verbose = true);
    
bool testControls(Solution *psol);

int main() {
    // std::cout << "Hello world" << std::endl;
    Solution* sol = new Solution();

    // controls — silent unless something regresses
    if (!testControls(sol))
        std::cout << "--- controls FAILED ---" << std::endl;

    // experiments
    std::vector<int> height = {1,0,0,0,0,0,0,2,2};
    test(height, sol, 8);
    
    std::cout << "Done testing" << std::endl;
}

// Returns true if the result matched `expected`.
// Prints only on failure unless `verbose` is set.
bool test(std::vector<int> &height, Solution *psol, int expected, bool verbose)
{
    int maxArea = psol->maxArea(height);
    const bool passed = (maxArea == expected);
    
    if (verbose || !passed)
    {
        std::cout << (passed ? "Test successful" : "Test failed") << '\n'
        << "Expected max area [" << expected << "] got max area [" << maxArea << "]"
        << std::endl;
    }
    return passed;
}

bool testControls(Solution *psol)
{
    bool ok = true;
    std::vector<int> height = {1,8,6,2,5,4,8,3,7};
    ok &= test(height, psol, 49, false);
    height = {1,1};
    ok &= test(height, psol, 1, false);
    height = {1,2,4,3};
    ok &= test(height, psol, 4, false);
    return ok;
}