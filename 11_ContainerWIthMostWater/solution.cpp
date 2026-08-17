#include <cstddef>
#include <iostream>
#include <vector>

class Solution
{
private:
    size_t left(std::vector<int> &height, const size_t &LB, const size_t &cA, const size_t &RB,
                size_t *CanA)
    {
        if (LB + 1 >= RB)
            return LB+1;
        size_t CLB = LB + 1; /*candidate_LB*/
        *CanA = std::min(height[CLB], height[RB]) * (RB - CLB);
        while (CLB < RB && height[CLB] <= height[LB] && *CanA <= cA)
        {
            CLB++;
            *CanA = std::min(height[CLB], height[RB]) * (RB - CLB);
        }
        return CLB;
    }

    size_t right(std::vector<int> &height, const size_t &LB, const size_t &cA, const size_t &RB,
                 size_t *CanA)
    {
        if (RB - 1 <= LB)
            return RB-1;
        size_t CRB = RB - 1; /*candidate_RB*/
        *CanA = std::min(height[CRB], height[LB]) * (CRB - LB);
        while (CRB > LB && height[CRB] <= height[RB] && *CanA <= cA)
        {
            CRB--;
            *CanA = std::min(height[CRB], height[LB]) * (CRB - LB);
        }
        return CRB;
    }

public:
    int maxArea(std::vector<int> &height)
    {
        size_t current_area = 0;
        size_t LeftBoundary = 0;
        size_t RightBoundary = height.size()-1;

        size_t LB_height = height[LeftBoundary];
        size_t RB_height = height[RightBoundary];
        current_area = std::min(LB_height, RB_height) * (RightBoundary - LeftBoundary);

        size_t &LB = LeftBoundary;
        size_t &RB = RightBoundary;

        while (LB < RB)
        {
            if (height[LB] <= height[RB])
            {
                size_t CanA = 0;
                size_t cLB = left(height, LB, current_area, RB, &CanA);
                if (CanA > current_area)
                {
                    current_area = CanA;
                }
                LB = cLB;
            }
            else
            {
                size_t CanA = 0;
                size_t cRB = right(height, LB, current_area, RB, &CanA);
                if (CanA > current_area)
                {
                    current_area = CanA;
                }
                RB = cRB;
            }
        }
        // this is either the starting container or something better.
        return current_area;
    }
};