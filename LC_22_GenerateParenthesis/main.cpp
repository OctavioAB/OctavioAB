#include <iostream>
#include <string>
#include <unordered_set>
#include "solution.cpp"

bool test(int input, Solution* psol, const std::unordered_set<std::string>&& solutions, bool verbose);
bool testControls(Solution *psol);

int main()
{
    Solution sol = Solution();

    if (!testControls(&sol))
    {
        std::cout << "--- controls FAILED ---" << std::endl;
        return 1;
    }

    std::vector<std::string> parenthesis = sol.generateParenthesis(4);
    for (auto solution : parenthesis)
    {
        std::cout << solution << std::endl;
    }

    return 0;
}

bool test(int input, Solution* psol, const std::unordered_set<std::string>&& solutions, bool verbose)
{
    std::vector<std::string> result = psol->generateParenthesis(input);
    std::unordered_set<std::string> result_set = std::unordered_set<std::string>();
    for(const auto& sol: result)
        result_set.insert(sol);
    return result_set == solutions;
}

bool testControls(Solution *psol)
{
    bool ok = true;

    ok &= test(1, psol, {"()"}, false);
     ok &= test(2, psol, {"()()", "(())"}, false);
     ok &= test(3, psol, {"((()))","(())()","()(())","()()()","(()())"}, false);

    return ok;
}