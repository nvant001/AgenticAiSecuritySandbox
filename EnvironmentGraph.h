#pragma once
#include "schema.h"
#include <unordered_map>
#include <vector>
#include <stdexcept>
#include <iostream>
#include <cmath>

class EnvironmentGraph{
    private:
    // Key = our source id, would give us our edges
    // <node_id we have, returns edges with that as the source
    std::unordered_map<std::uint32_t, std::vector<Edge>> adj_list;
    // Node id, would give us our node
    std::unordered_map<std::uint32_t, Node> nodeRegistry;
    public:

    void add_node(const Node& node);
    void add_edge(uint32_t source_id, uint32_t target_id, AgentAction action, double weight, uint64_t perms);
    std::vector<Edge> get_valid_actions(uint32_t node_id, uint64_t agent_permissions);
    bool validate_graph() const;
    void validate_graph(std::ofstream& outputFile);
};