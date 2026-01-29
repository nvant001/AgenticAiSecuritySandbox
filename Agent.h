//Agent.h
#ifndef AGENT_H
#define AGENT_H

#include <cstdint>
#include <vector>
#include "schema.h"
#include "EnvironmentGraph.h"

class Agent {
private:
    uint32_t current_node_id;
    uint64_t permissions;
    std::vector<uint32_t> history;

public:
    Agent(uint32_t start_node, uint64_t base_perms);

    void perm_action(const Edge& edge, const Node& destination_node);

    bool execute_transition(const Edge& edge, const Node& target_node);

    uint64_t get_permissions() const {return permissions; }
    uint32_t get_current_node_id() const {return current_node_id; }
    std::vector<uint32_t> get_history() const {return history; }
};

#endif // AGENT_H