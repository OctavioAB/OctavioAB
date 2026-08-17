#include <string>
#include <stack>
#include <vector>
#include <iostream>
#include <unordered_set>
#include <stdexcept>

class Solution
{
private:
    class Instruction
    {
        friend Solution;
        std::string target_;
        bool unlimited_;
        Instruction(std::string target, bool unlimited)
            : target_(target), unlimited_(unlimited) {}
    };

    std::string target_;
    std::string instructions_string_;
    std::vector<Instruction> *instructions_;

public:
    void setup(const std::string &s, const std::string &p);
    void processInstructions(const std::string &p, std::vector<Instruction> *instructions_);
    void printInstructions();
    int greedyMatch(const std::string &source, const Instruction &inst,
                    size_t &source_cursor, size_t &matched);
    bool compareDot(const std::string &source, const std::string &target);
    int singleMatch(const std::string &source, const Instruction &inst,
                    size_t &source_cursor, size_t &matched);
    void processInstruction(const std::string &s,
                            size_t &source_cursor, size_t &currentInstructionIndex,
                            std::stack<Instruction *> *processed,
                            std::vector<size_t> *match_counts);
    void traceBackToMatchedUnlimited(std::stack<Instruction *> *processed,
                                     std::vector<size_t> *match_counts,
                                     size_t &source_cursor);

    void setup(const std::string &s, const std::string &p)
    {
        target_ = s;

        // create list of patterns to match
        instructions_ = new std::vector<Instruction>();
        processInstructions(p, instructions_);
    }

    void processInstructions(const std::string &p, std::vector<Instruction> *instructions_)
    {
        size_t charPos = 0;
        std::string instruction_toMatch = "";
        instructions_string_ = p;
        while (charPos < p.length())
        {
            if (charPos + 1 < p.length())
            {
                if (p[charPos + 1] == '*')
                {
                    if (instruction_toMatch.size() > 0)
                    {
                        instructions_->push_back(Instruction(instruction_toMatch, false));
                        instruction_toMatch = "";
                    }
                    instructions_->push_back(Instruction(p.substr(charPos, 1), true));
                    charPos += 2;
                }
                else
                {
                    instruction_toMatch += p.substr(charPos, 1);
                    charPos++;
                }
            }
            else
            {
                instruction_toMatch += p.substr(charPos, 1);
                charPos++;
            }
        }
        if (instruction_toMatch.compare("") != 0)
        {
            instructions_->push_back(Instruction(instruction_toMatch, false));
        }
    }

    void printInstructions()
    {
        if (instructions_->size() < 1)
        {
            std::cout << "Instructions list empty-- aborting." << std::endl;
            return;
        }

        if (instructions_string_.size() < 1)
        {
            std::cout << "Original instructions string not saved. Aborting." << std::endl;
        }

        std::cout << "Printing instructions: " << std::endl;
        size_t ins_index = 0;
        for (const Instruction &ins : *instructions_)
        {
            std::cout << "  " << "[" << ins_index++ << "]" << " ";
            std::cout << ins.target_;
            if (ins.unlimited_)
                std::cout << "*";
            std::cout << std::endl;
        }
        std::cout << "  Done." << std::endl;
    }

    /* Advance cursor as much as possible */
    int greedyMatch(const std::string &source, const Instruction &inst,
                    size_t &source_cursor, size_t &matched)
    {
        size_t cursor_start = source_cursor;
        // while current cursor points to "a match", advance cursor
        while (/*check cursor is contained*/ source_cursor < source.size() &&
               (source.at(source_cursor) == inst.target_.at(0) || source.at(source_cursor) == '.'))
            source_cursor++;
        matched = source_cursor - cursor_start;
        return 0;
    }

    bool compareDot(const std::string &source, const std::string &target)
    {
        for (int i = 0; i < std::max(source.length(), target.length()); i++)
        {
            if (source.substr(i, 1).compare(target.substr(i, 1)) != 0 || target.substr(i, 1).compare(".") != 0)
                return false;
        }
        return true;
    }

    // Advance cursor exactly by instruction target length
    int singleMatch(const std::string &source, const Instruction &inst,
                    size_t &source_cursor, size_t &matched)
    {
        const std::string &target_string = inst.target_;
        size_t target_length = target_string.length();
        if (source_cursor + target_length >= source.length())
            return -1;
        std::string source_part = source.substr(source_cursor, target_length);
        if (compareDot(source_part, target_string)
            // source_part.compare(target_string) == 0
        )
        {
            matched = target_length;
            source_cursor += matched;
        }
        return 0;
    }

    void processInstruction(const std::string &s,
                            size_t &source_cursor,
                            size_t &currentInstructionIndex,
                            Instruction *currentInstruction,
                            std::stack<Instruction *> *processed,
                            std::vector<size_t> *match_counts)
    {
        // note instruction matches
        size_t matched = 0;

        // attempt match
        if (currentInstruction->unlimited_)
            greedyMatch(s, *currentInstruction, source_cursor, matched);
        else
            singleMatch(s, *currentInstruction, source_cursor, matched);

        if (matched > 0) // so if there was a match
        {
            // update seen matches
            (*match_counts)[currentInstructionIndex] = matched;

            // advance current instruction
            currentInstructionIndex++;
        }
    }

    void traceBackToMatchedUnlimited(std::stack<Instruction *> *processed,
                                     std::vector<size_t> *match_counts,
                                     size_t &source_cursor)
    {
        while (!processed->empty())
        {
            /* Before I 'pop' an instruction from processed, I want
               to make sure I also update the current source_cursor */
            // get current 'top's seen matched chars
            size_t peeked_matched = (*match_counts)[processed->size()];
            if (peeked_matched == 0)
            { // this instruction had seen no matches before, and since
                // it has already been processed, this can only be an
                // unlimited instruction
                // therefore, we mustn't move our cursor when we pop
                processed->pop();
            }
            else
            {
                // this instruction has seen matches before, and it
                // could be an unlimited instruction.
                if (processed->top()->unlimited_)
                { // if this was an unlimited instruction
                    // we end our traceback
                    break;
                }
                else
                { // this wasn't an unlimited instruction
                    // move cursor back 'top's seen matched chars
                    source_cursor -= peeked_matched;
                    processed->pop();
                }
            }
        }
        // if (processed->empty()) // if we didn't find a previous unlimited instruction
        //     return false;       // assume there is no alternative path left to explore
        // // otherwise we have an unlimited instruction to resume at
        // return true;
    }

    /*
        isMatch is going to operate a parser
        this parser is going to keep track of instructions to process and characters to consume
        at any point, the parser is going to have an instruction to process, and/or a character to
        consume. Only when neither of which remains, do we have a succesfull parse.
    */
    bool isMatch(const std::string &s, const std::string &p)
    {
        // run setup first
        setup(s, p);

        // create table to keep track of matches per instruction
        std::vector<size_t> match_counts = std::vector<size_t>(instructions_->size(), 0);

        // create stack to keep track of instructions to match
        std::stack<Instruction *> processed = std::stack<Instruction *>();

        // create index to keep track of current instruction being processed
        size_t currentInstructionIndex = 0;

        // push first instruction onto stack for processing
        processed.push(&instructions_->at(currentInstructionIndex));

        // create cursor to keep track of cource characters already processed
        size_t source_cursor = 0;

        // begin processing loop: while there is an instruction on the stack
        while (!processed./*not*/ empty())
        {
            // First check if it is possible to make progress
            if (currentInstructionIndex >= instructions_->size() || source_cursor < s.size())
            { // we have no more instructions to follow, OR characters still to match
                // traceback to last unlimited instruction *with "matched"*
                traceBackToMatchedUnlimited(&processed, &match_counts, source_cursor);
                // at this point, processed.top() should be an unlimited, previously matched
                // instruction OR processed is empty, which is caught in the next loop iteration
            }
            else
            {
                // get current instruction to match
                Instruction *currentInstruction = processed.top(); // peek!

                processInstruction(s, source_cursor, currentInstructionIndex, currentInstruction,
                                   &processed, &match_counts);
                if (match_counts[currentInstructionIndex] == 0)
                    // if there was no match, attempt traceback
                    traceBackToMatchedUnlimited(&processed, &match_counts, source_cursor);
                    // at this point, processed.top() should be an unlimited, previously matched
                    // instruction OR processed is empty, which is caught in the next loop iteration
                else
                {
                    // tracedback to this instruction
                    currentInstructionIndex = processed.size() - 1; // guaranteed>0
                    // match one less than previously attempted, then try again.
                    size_t previous_matched = match_counts[currentInstructionIndex];
                    // move cursor one position further back
                    source_cursor--;
                    // update match_counts
                    match_counts[currentInstructionIndex] = previous_matched - 1;
                    // push next instruction into stack
                    processed.push(&instructions_->at(++currentInstructionIndex));
                }
            }
        }
    }
};