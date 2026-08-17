#include <string>
#include <iostream>
#include "solution.cpp"

bool test(const std::string &s, const std::string &p, Solution *psol,
          bool expected, bool verbose = true);
bool testControls(Solution *psol);

int main()
{
    Solution *sol = new Solution();

    // controls — silent unless something regresses
    if (!testControls(sol))
        std::cout << "--- controls FAILED ---" << std::endl;

    // experiments:
    test("aaa", "ab*a*c*a", sol, true);

    delete sol;
    std::cout << "Done testing" << std::endl;
}

// Returns true if the result matched `expected`.
// Prints only on failure unless `verbose` is set.
bool test(const std::string &s, const std::string &p, Solution *psol,
          bool expected, bool verbose)
{
    const bool matched = psol->isMatch(s, p);
    const bool passed = (matched == expected);

    if (verbose || !passed)
    {
        const char *matches = matched ? "matches" : "does not match";
        std::cout << (passed ? "Test successful" : "Test failed") << '\n'
                  << "  [" << p << "] " << matches << " onto [" << s << "]"
                  << std::endl;
    }
    return passed;
}

// Returns true if every control passed.
bool testControls(Solution *psol)
{
    bool ok = true;
    // exact match
    ok &= test("miss", "miss", psol, true, false);
    ok &= test("mi", "mi", psol, true, false);
    // shorter target
    ok &= test("mi", "m", psol, false, false);
    // simple suffixes
    ok &= test("mi", "s*p*mi", psol, true, false);
    ok &= test("smi", "s*p*mi", psol, true, false);
    ok &= test("pmi", "s*p*mi", psol, true, false);
    ok &= test("spmi", "s*p*mi", psol, true, false);
    // simple prefixes
    ok &= test("mi", "mis*p*", psol, true, false);
    ok &= test("mis", "mis*p*", psol, true, false);
    ok &= test("mip", "mis*p*", psol, true, false);
    ok &= test("misp", "mis*p*", psol, true, false);
    // simple infixes
    ok &= test("missi", "mis*i", psol, true, false);
    ok &= test("missi", "mi*ssi", psol, true, false);
    ok &= test("missi", "mz*issi", psol, true, false);
    // some arbitrary examples
    ok &= test("mississ", "mis*is*", psol, true, false);
    ok &= test("mississi", "mis*is*p*", psol, false, false);
    ok &= test("mississipi", "mis*is*p*.", psol, false, false);
    ok &= test("mississipi", "mis*is*.p*.", psol, true, false);
    ok &= test("mississipi", "mis*is*p*.pi", psol, true, false);
    ok &= test("mississipi", "mis*is*p*.pii", psol, false, false);
    // dots:
    ok &= test("mississ", ".*", psol, true, false);
    ok &= test("mississ", "mississ.*", psol, true, false);
    ok &= test("mississ", ".*mississ", psol, true, false);
    // some edge cases:
    ok &= test("a", "a", psol, true, false);
    ok &= test("l", "a*b*c*d*e*f*g*h*i*j*k*", psol, false, false);
    ok &= test("l", "a*b*c*d*e*f*g*h*i*j*k*.", psol, true, false);
    ok &= test("l", ".a*b*c*d*e*f*g*h*i*j*k*", psol, true, false);
    ok &= test("abcdefghijk", "a*b*c*d*e*f*g*h*i*j*k*", psol, true, false);
    ok &= test("abcdefghijk", ".*.*.*.*.*.*.*.*.*.*.*", psol, true, false);
    ok &= test("abcdefghijk", "...........", psol, true, false);
    ok &= test("aaa", "aaaa", psol, false, false);

    return ok;
}