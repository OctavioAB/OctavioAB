#include <vector>
#include <string>
#include <stack>
#include <unordered_map>
#include <tuple>
#include <unordered_set>

class Solution
{
public:
    class State
    {
        friend Solution;

    private:
        int o, c;
        std::string sol;
        std::vector<std::string> *local_solutions;

    public:
        State(int o, int c, std::string sol)
        {
            this->o = o;
            this->c = c;
            this->sol = sol;
            local_solutions = new std::vector<std::string>();
        }

        ~State()
        {
            delete local_solutions;
        }
    };

    std::vector<std::string> generateParenthesis(int n)
    {
        // set for "search". We only need one set becuse we are only building a tree
        std::unordered_set<std::string> *visited = new std::unordered_set<std::string>();
        // map for memoization

        //table for memoization
        std::vector<std::vector<std::unordered_set<std::string>>> suffixes 
        = std::vector<std::vector<std::unordered_set<std::string>>>(n+1);
        for(int i=0;i<=n;++i)
        {
            suffixes.push_back(std::vector<std::unordered_set<std::string>>(n+1));
            for(int j=0;j<=n;++j)
                suffixes[i].push_back(std::unordered_set<std::string>());
        }
        suffixes[0][1].insert(")");

        // vector of solutions found so far
        std::vector<std::string> solutions = std::vector<std::string>();

        std::stack<State *> stack = std::stack<State *>();
        State *first_element = new State(n - 1, n, "(");
        stack.push(first_element);
        while (!stack.empty())
        {
            State *to_process = stack.top();
            // extract current solution from element to process
            std::string cs = to_process->sol;

            if (to_process->o > 0 && visited->find(cs + "(") == visited->end())
            {
                if (suffixes[to_process->o-1][to_process->c].size()>0)
                {
                    for (auto suf : suffixes[to_process->o-1][to_process->c])
                    {
                        to_process->local_solutions->push_back(cs + "(" + suf);
                    }
                    visited->insert(cs+"(");
                }
                else
                {
                    State *to_enque =
                        new State(to_process->o - 1, to_process->c, cs + "(");
                    stack.push(to_enque);
                }
            }
            // if possible, apply f_1
            else if (to_process->c > 0 && to_process->c > to_process->o && visited->find(cs + ")") == visited->end())
            {
                if (suffixes[to_process->o][to_process->c-1].size()>0)
                {
                    for (auto suf : suffixes[to_process->o][to_process->c-1])
                    {
                        to_process->local_solutions->push_back(cs + ")" + suf);
                    }
                    visited->insert(cs + ")");
                }
                else
                {
                    State *to_enque =
                        new State(to_process->o, to_process->c - 1, cs + ")");
                    stack.push(to_enque);
                }
            }
            else
            {
                if (to_process->o == 0 && to_process->c == 0)
                {
                    to_process->local_solutions->push_back(cs);
                }
                else if (!to_process->local_solutions->empty())
                {
                    for (auto sol : *(to_process->local_solutions))
                    {
                        if (sol.substr(0, cs.length()).compare(cs) == 0)
                        {
                            suffixes[to_process->o][to_process->c].insert(
                                sol.substr(cs.length(), to_process->o + to_process->c));
                        }
                    }
                }
                visited->insert(cs);
                stack.pop();
                for (auto sol : *(to_process->local_solutions))
                {
                    if (!stack.empty())
                        stack.top()->local_solutions->push_back(sol);
                    else
                        solutions.push_back(sol);
                }
                // delete to_process->local_solutions;
                delete to_process;
            }
        }
        delete visited;
        return solutions;
    }
};