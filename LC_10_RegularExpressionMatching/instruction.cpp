#include <cstddef>

class Instruction
{
private:
    char target_;
    bool unlimited_;
    size_t target_index_;

public:
    Instruction(char target, bool unlimited, size_t target_index)
        : target_(target), unlimited_(unlimited),
        target_index_(target_index) {}

    void setTarget(char target) {
        target_ = target; 
    }

    void setUnlimited(bool unlimited) {
        unlimited_=unlimited;
    }

    void setTargetIndex(size_t target_index)
    {
        target_index_ = target_index;
    }

    char getTarget() {
        return target_;
    }

    bool unlimited() {
        return unlimited_;
    }

    size_t getTargetIndex() {
        return target_index_;
    }

};