/*
 * This file is part of the MQT DD Package which is released under the MIT license.
 * See file README.md or go to http://iic.jku.at/eda/research/quantum_dd/ for more information.
 */

#ifndef DD_PACKAGE_EDGE_HPP
#define DD_PACKAGE_EDGE_HPP

#include "Complex.hpp"
#include "ComplexValue.hpp"

#include <cstddef>
#include <utility>

namespace dd {
    template<class Node>
    struct Edge {
        Node*   next_node;
        Complex weight;



        /// Comparing two DD edges with another involves comparing the respective pointers
        /// and checking whether the corresponding weights are "close enough" according to a given tolerance
        /// this notion of equivalence is chosen to counter floating point inaccuracies
        constexpr bool operator==(const Edge& other) const {
            return next_node == other.next_node && weight.approximatelyEquals(other.weight);
        }
        constexpr bool operator!=(const Edge& other) const {
            return !operator==(other);
        }

        [[nodiscard]] constexpr bool isTerminal() const { return Node::isTerminal(next_node); }

        // edges pointing to zero and one terminals
        static inline Edge one{Node::terminal, Complex::one};
        static inline Edge zero{Node::terminal, Complex::zero};

        [[nodiscard]] static constexpr Edge terminal(const Complex& weight) { return {Node::terminal, weight}; }
        [[nodiscard]] constexpr bool        isZeroTerminal() const { return Node::isTerminal(next_node) && weight == Complex::zero; }
        [[nodiscard]] constexpr bool        isOneTerminal() const { return Node::isTerminal(next_node) && weight == Complex::one; }
    };

    template<typename Node>
    struct CachedEdge {
        Node*        next_node{};
        ComplexValue weight{};

        CachedEdge() = default;
        CachedEdge(Node* next_node, const ComplexValue& weight_original):
                next_node(next_node), weight(weight_original) {}
        CachedEdge(Node* next_node, const Complex& weight_complex_number):
                next_node(next_node) {
            weight.r = CTEntry::val(weight_complex_number.real);
            weight.i = CTEntry::val(weight_complex_number.img);
        }

        /// Comparing two DD edges with another involves comparing the respective pointers
        /// and checking whether the corresponding weights are "close enough" according to a given tolerance
        /// this notion of equivalence is chosen to counter floating point inaccuracies
        bool operator==(const CachedEdge& other) const {
            return next_node == other.next_node && weight.approximatelyEquals(other.weight);
        }
        bool operator!=(const CachedEdge& other) const {
            return !operator==(other);
        }
    };
} // namespace dd

namespace std {
    template<class Node>
    struct hash<dd::Edge<Node>> {
        std::size_t operator()(dd::Edge<Node> const& edge_) const noexcept {
            auto h1 = dd::murmur64(reinterpret_cast<std::size_t>(edge_.next_node));
            auto h2 = std::hash<dd::Complex>{}(edge_.weight);
            return dd::combineHash(h1, h2);
        }
    };

    template<class Node>
    struct hash<dd::CachedEdge<Node>> {
        std::size_t operator()(dd::CachedEdge<Node> const& edge_) const noexcept {
            auto h1 = dd::murmur64(reinterpret_cast<std::size_t>(edge_.next_node));
            auto h2 = std::hash<dd::ComplexValue>{}(edge_.weight);
            return dd::combineHash(h1, h2);
        }
    };
} // namespace std

#endif //DD_PACKAGE_EDGE_HPP
