#include <string>
#include <stack>
#include <vector>
#include <iostream>
#include <unordered_set>
#include <stdexcept>
// #include "instruction.cpp"

class Solution
{
private:
    class Instruction
    {
        friend Solution;
    private:
        char target_;
        bool unlimited_;
        size_t target_index_;

    public:
        Instruction(char target, bool unlimited, size_t target_index)
            : target_(target), unlimited_(unlimited),
              target_index_(target_index) {}

        void setTarget(char target)
        {
            target_ = target;
        }

        void setUnlimited(bool unlimited)
        {
            unlimited_ = unlimited;
        }

        void setTargetIndex(size_t target_index)
        {
            target_index_ = target_index;
        }

        char getTarget()
        {
            return target_;
        }

        bool unlimited()
        {
            return unlimited_;
        }

        size_t getTargetIndex()
        {
            return target_index_;
        }
    };

    class Match
    {
        friend Solution;
        std::vector<size_t> ss_;
        Instruction *inst_;
        Match(const std::vector<size_t> &ss, Instruction *inst)
            : ss_(ss) /*std::vector automatically makes deep copy*/,
              inst_(inst) {};
        ~Match()
        {
            delete inst_;
        }
    };

    std::stack<Match *> *matches_;

    inline void matchUnlimited(const char &target_char, size_t *source_index, const std::string &s)
    {
        Match *latestMatch = matches_->top();
        while (*source_index < s.length() && (target_char == s.at(*source_index) || target_char == '.'))
        {
            // std::cout << "extra unlimited match" << std::endl;
            latestMatch->ss_[1]++;
            latestMatch->ss_[2]++;
            (*source_index)++;
        }
    }

    inline void pushEmptyUnlimited(const size_t &source_index, const char &target_char, const size_t &original_target_index)
    {
        matches_->push(new Match({source_index, source_index, 0},
                                 new Instruction(target_char,
                                                 true,
                                                 original_target_index)));
    }

    inline bool attemptRestoreState(size_t *source_index, size_t *target_index)
    {
        if (matches_->empty())
        { // if there's no matches to go back to, and this instruction is unskippable
            return false;
        }

        Match *latestMatch = nullptr;
        // now find latest unlimited instruction that is not empty
        while (!matches_->empty() && (!matches_->top()->inst_->unlimited() || (matches_->top()->inst_->unlimited() && !(matches_->top()->ss_[2] > 0))))
        {
            matches_->pop();
        }
        // if there is no unlimited instruction to rollback to,
        // then there is no way to make the target match the source
        if (matches_->empty())
            return false;
        latestMatch = matches_->top();
        // now restore instruction index to next instruction
        (*target_index) = latestMatch->inst_->getTargetIndex() + 2;
        // source must become last character of this latest
        // unlimited match, which will no longer belong to this latest match
        (*source_index) = latestMatch->ss_[1];
        // Now transform ss_i into ss_i' (ss_i prime)
        latestMatch->ss_[2]--;
        latestMatch->ss_[1]--;
        // now next iteration will attempt to match the next instruction
        // onto the source index that had previously been consumed by this
        // latest unlimited instruction.
        return true;
    }

    inline void releaseMatches()
    {
        while (!matches_->empty())
        {
            Match *to_delete = matches_->top();
            matches_->pop();
            delete to_delete;
            to_delete = nullptr;
        }
    }


public:
    bool isMatch(const std::string &s, const std::string &p)
    {
        matches_ = new std::stack<Match *>();

        size_t target_index = 0;
        size_t source_index = 0;
        // size_t iterations = 0;

        while (target_index < p.length())
        {
            // grab current instruction-- by above loop "guard" always in bounds
            char target_char = p.at(target_index);
            // declare some flags
            bool target_char_unlimited = false;
            size_t original_target_index = target_index;
            if (target_index + 1 < p.length() && p.at(target_index + 1) == '*')
            {
                target_char_unlimited = true;
                target_index++;
            }
            // consume instruction
            target_index++;

            // now check if we match
            if (source_index < s.length() // first check if there is more source material
                && (target_char == s.at(source_index) || target_char == '.'))
            { // if we matched
                // push new match into stack
                Match *newMatch = new Match({source_index, source_index, 1},
                                            new Instruction(target_char, target_char_unlimited, original_target_index));
                matches_->push(newMatch);
                // 'consume' current source symbol
                source_index++;
                // if instruction was unlimited, keep matching-- if more source to match
                if (target_char_unlimited && source_index < s.length())
                {
                    matchUnlimited(target_char, &source_index, s);
                }
            }
            else
            { // if we didn't match target to source character or there is no more source material
                if (target_char_unlimited)
                {
                    // if possible, try skipping this instruction.
                    pushEmptyUnlimited(source_index, target_char, original_target_index);
                }
                else 
                { // restore to last instruction right after last unlimited match
                    if (!attemptRestoreState(&source_index, &target_index))
                    {
                        return false;
                    }
                }
            }
        }
        // release matches
        releaseMatches();
        // if we processed all instructions but there's still source material to match
        if (target_index >= p.length() && source_index < s.length())
        {
            return false;
        }
        return true;
    }
};