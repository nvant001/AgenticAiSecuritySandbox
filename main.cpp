#include <iostream>
#include "EnvironmentGraph.h"
#include "EnvironmentGraph.cpp"
#include <cassert>
int main()
{
EnvironmentGraph graph;

    // 1. Setup Nodes
    // ID 0: Unauthenticated (Start)
    // ID 1: User Session (Intermediate)
    // ID 2: Prod Database (Target / Terminal)
    graph.add_node({.is_terminal = false, .id = 0, .label = SystemState::UNAUTHENTICATED});
    graph.add_node({.is_terminal = false, .id = 1, .label = SystemState::USER_SESSION});
    graph.add_node({.is_terminal = true,  .id = 2, .label = SystemState::S3_BUCKET_PRIVATE});

    // 2. Setup Edges (The "Path to Breach")
    // Path A: Login (Requires nothing, 100% chance to move to Session)
    graph.add_edge(0, 1, AgentAction::INPUT_TEXT, 1.0, Perms::NONE);

    // Path B: Query DB (Requires DB_ACCESS, 100% chance to move to DB)
    graph.add_edge(1, 2, AgentAction::EXECUTE_QUERY, 1.0, Perms::DB_ACCESS);

    // 3. TEST: Validation
    std::cout << "--- Running Graph Validation ---\n";
    if (graph.validate_graph()) {
        std::cout << "SUCCESS: Graph is mathematically sound.\n";
    } else {
        std::cout << "FAILURE: Graph validation failed.\n";
    }

    // 4. TEST: Permission Filtering
    std::cout << "\n--- Testing Agent Permissions at Node 1 ---\n";
    
    // Scenario A: Agent has no permissions
    auto actions_none = graph.get_valid_actions(1, Perms::NONE);
    std::cout << "Agent with NONE: Found " << actions_none.size() << " actions (Expected: 0)\n";
    assert(actions_none.size() == 0);

    // Scenario B: Agent has DB_ACCESS
    auto actions_db = graph.get_valid_actions(1, Perms::DB_ACCESS);
    std::cout << "Agent with DB_ACCESS: Found " << actions_db.size() << " actions (Expected: 1)\n";
    assert(actions_db.size() == 1);

    


    return 0;

}