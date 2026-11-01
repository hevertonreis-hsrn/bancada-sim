#pragma once
#include <sstream>
#include <string>
#include <utility>
#include <vector>

class Circuit
{
public:
    explicit Circuit(std::string title) : title_(std::move(title)) {}

    // Aceita qualquer tipo que tenha o método toSpiceLine()
    template <typename Component>
    void add(const Component &component)
    {
        lines_.push_back(component.toSpiceLine());
    }

    void setTransient(double step, double stop)
    {
        std::ostringstream line;
        // "0 uic": começa em t = 0 com as condições iniciais (capacitor descarregado),
        // sem calcular o ponto de operação DC
        line << ".tran " << step << ' ' << stop << " 0 uic";
        transient_ = line.str();
    }

    std::string toNetlist() const
    {
        std::ostringstream out;
        out << title_ << '\n'; // a primeira linha é sempre o título
        for (const auto &line : lines_)
        {
            out << line << '\n';
        }
        if (!transient_.empty())
        {
            out << transient_ << '\n';
        }
        out << ".end\n";
        return out.str();
    }

private:
    std::string title_;
    std::vector<std::string> lines_;
    std::string transient_;
};