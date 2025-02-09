#pragma once

#include "Share/Share.hpp"
#include "Math/Math.hpp"
#include "Job/JobBase.hpp"
#include <functional>

namespace qmpc::Job
{
class MathJob : public JobBase<MathJob>
{
    //数学関数
    using Func = std::function<Share(std::vector<Share> &)>;
    Func f;

public:
    MathJob(Func f, const JobParameter &request) : JobBase<MathJob>(request), f(f) {}
    std::vector<Share> compute(
        const std::vector<std::vector<Share>> &table,
        const std::vector<std::string> &schemas,
        const std::vector<std::list<int>> &arg
    )
    {
        std::list<std::vector<Share>> cols;
        int n = table.size();
        int m = table[0].size();
        for (const auto &w : arg[0])
        {
            std::vector<Share> tmp(n);
            for (int h = 0; h < n; ++h)
            {
                tmp[h] = table[h][w - 1];
            }
            cols.emplace_back(tmp);
        }
        std::vector<Share> ret;
        ret.reserve(n * m);
        for (auto &&col : cols)
        {
            ret.emplace_back(f(col));
        }
        return ret;
    }
};
}  // namespace qmpc::Job