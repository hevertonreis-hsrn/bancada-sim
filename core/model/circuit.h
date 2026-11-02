#pragma once
#include <algorithm>
#include <sstream>
#include <stdexcept>
#include <string>
#include <utility>
#include <vector>
#include "model/component.h"

class Circuit
{
public:
    explicit Circuit(std::string title) : title_(std::move(title)) {}

    // Nomes são únicos: a interface vai localizar componentes por nome
    void add(const AnyComponent &component)
    {
        const std::string name = componentName(component);
        if (exists(name))
        {
            throw std::invalid_argument("componente duplicado: " + name);
        }
        components_.push_back(component);
    }

    bool remove(const std::string &name)
    {
        auto it = std::find_if(components_.begin(), components_.end(),
                               [&](const AnyComponent &c)
                               { return componentName(c) == name; });
        if (it == components_.end())
            return false;
        components_.erase(it);
        return true;
    }

    const std::vector<AnyComponent> &components() const { return components_; }

    void setTransient(double step, double stop, double tmax = 0.0)
    {
        std::ostringstream line;
        line << ".tran " << step << ' ' << stop << " 0";
        if (tmax > 0.0)
        {
            line << ' ' << tmax;
        }
        line << " uic";
        transient_ = line.str();
    }

    // Lista de problemas encontrados. Lista vazia significa circuito válido.
    std::vector<std::string> validate() const
    {
        std::vector<std::string> errors;
        bool hasGround = false;
        for (const auto &c : components_)
        {
            const auto nodes = componentNodes(c);
            if (nodes[0] == "0" || nodes[1] == "0")
                hasGround = true;
            if (nodes[0] == nodes[1])
            {
                errors.push_back(componentName(c) + ": os dois terminais estao no mesmo no");
            }
        }
        if (!hasGround)
        {
            errors.push_back("circuito sem no de terra (0)");
        }
        return errors;
    }

    std::string toNetlist() const
    {
        std::ostringstream out;
        out << title_ << '\n';
        for (const auto &c : components_)
        {
            out << componentToSpice(c) << '\n';
        }
        if (!transient_.empty())
        {
            out << transient_ << '\n';
        }
        out << ".end\n";
        return out.str();
    }

private:
    bool exists(const std::string &name) const
    {
        return std::any_of(components_.begin(), components_.end(),
                           [&](const AnyComponent &c)
                           { return componentName(c) == name; });
    }

    std::string title_;
    std::vector<AnyComponent> components_;
    std::string transient_;
};