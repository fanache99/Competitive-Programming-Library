#pragma once

#include <vector>
#include <map>

template<typename Input, typename Output = int>
class Normalize {
public:
    Normalize(const std::vector<int> &input);
    Output to_normal(Input value);
    Input from_normal(Output value);

private:
    std::map<Input, Output> to_normal_;
    std::vector<Input> from_normal_;
};

template<typename Input, typename Output>
Normalize<Input, Output>::Normalize(const std::vector<int> &input) {}