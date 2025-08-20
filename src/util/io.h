#pragma once

#include <fstream>
#include <iostream>
#include <vector>
#include <filesystem>

template<typename T>
std::istream& operator>>(std::istream &in, std::vector<T> &vec) {
    for (auto &x : vec) {
        in >> x;
    }
    return in;
}

template<typename T>
std::ostream& operator<<(std::ostream &out, const std::vector<T> &vec) {
    for (const auto &x : vec) {
        out << x << " ";
    }
    out << "\n";
    return out;
}

class Reader {
    template<typename T>
    friend Reader& operator>>(Reader &reader, T& t) {
        (*reader.in) >> t;
        return reader;
    }

protected:
    explicit Reader (std::istream *in = nullptr) : in(in) {}
    std::istream *in;
};

class FileReader : public Reader {
public:
    explicit FileReader(const std::string &filename) : fin(filename) {
        in = &fin;
    }

private:
    std::ifstream fin;
};

class StdinReader : public Reader {
public:
    StdinReader() : Reader(&std::cin) {}
};