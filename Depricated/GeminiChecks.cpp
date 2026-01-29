//GeminiChecks.cpp
#include <iostream>
#include "EnvironmentGraph.h"
#include "GeminiChecks.h"
#include <cassert>
#include <iomanip>
#include <random>
void check_day3(){
Node start_node;
    start_node.id = 0;
    start_node.label = SystemState::UNAUTHENTICATED;
    start_node.permissions_granted = Perms::NONE;

    Node login_success_node;
    login_success_node.id = 1;
    login_success_node.label = SystemState::USER_SESSION;
    login_success_node.permissions_granted = Perms::USER; // Grants USER perms

    Node admin_db_node;
    admin_db_node.id = 2;
    admin_db_node.label = SystemState::S3_BUCKET_PRIVATE;
    admin_db_node.permissions_granted = Perms::DB_ACCESS; // Grants DB perms

    // 2. Setup mock Edges (The paths between them)
    Edge to_login = {0, 1, AgentAction::REQUEST_TOKEN, 1.0, Perms::NONE};
    Edge to_admin = {1, 2, AgentAction::EXECUTE_QUERY, 1.0, Perms::USER};

    // 3. Initialize Agent at Start
    Agent spy(start_node.id, Perms::NONE);
    std::cout << "--- Simulation Start ---" << std::endl;
    std::cout << "Initial Perms: " << spy.get_permissions() << std::endl;

    // --- STEP 1: Move to Login ---
    spy.perm_action(to_login, login_success_node);
    assert(spy.get_current_node_id() == 1);
    assert(spy.get_permissions() & Perms::USER);
    std::cout << "Step 1 (Login) Success. Perms: " << spy.get_permissions() << std::endl;

    // --- STEP 2: Move to Admin DB ---
    spy.perm_action(to_admin, admin_db_node);
    assert(spy.get_current_node_id() == 2);
    
    // Verify Accumulation: Should have BOTH User (1) and DB_Access (4) = 5
    assert(spy.get_permissions() == (Perms::USER | Perms::DB_ACCESS));
    std::cout << "Step 2 (Admin DB) Success. Final Perms: " << spy.get_permissions() << std::endl;

    // --- STEP 3: Verify History ---
    auto history = spy.get_history();
    std::cout << "Path Taken: ";
    for(uint32_t node : history) std::cout << node << " -> ";
    std::cout << "END" << std::endl;

    assert(history.size() == 3); // Start -> Login -> DB
    
    std::cout << "\n[RESULT] Day 3 Logic Verified: Agent State Machine is 100% Functional." << std::endl;


}
void check_day2(){
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

}
void run_day2_checks() {
    EnvironmentGraph graph;

    // Build a simple 2-node path
    graph.add_node({.is_terminal = false, .id = 0, .label = SystemState::UNAUTHENTICATED});
    graph.add_node({.is_terminal = true,  .id = 1, .label = SystemState::USER_SESSION});

    // 100% chance to move from 0 to 1
    graph.add_edge(0, 1, AgentAction::INPUT_TEXT, 1.0, Perms::NONE);

    // Run the audit
    std::cout << "Starting Graph Audit...\n";
    bool success = graph.validate_graph(std::cerr); 
    
    if (success) {
        std::cout << ">>> DAY 2 FOUNDATION VERIFIED.\n";
    } else {
        std::cerr << ">>> DAY 2 FOUNDATION FAILED. CHECK LOGS.\n";
    }
}
void run_diagnostic_suite() {
    EnvironmentGraph graph;
    std::cout << "=== Agentic-Shield: Day 2 Diagnostic Report ===\n";

    // 1. Setup a "Broken" Graph to test the Reporter
    graph.add_node({.is_terminal = false, .id = 0, .label = SystemState::UNAUTHENTICATED});
    graph.add_node({.is_terminal = false, .id = 1, .label = SystemState::USER_SESSION});
    // Note: Node 2 is NOT added to test "Ghost Target" detection
    
    // Create an edge with a bad sum (0.7 instead of 1.0) and a missing target
    graph.add_edge(0, 1, AgentAction::INPUT_TEXT, 0.7, Perms::NONE); 
    
    // Create an edge to a non-existent Node 2
    // We wrap this in a try-block because your add_edge now throws!
    try {
        graph.add_edge(0, 2, AgentAction::REQUEST_TOKEN, 0.3, Perms::USER);
    } catch (const std::invalid_argument& e) {
        std::cout << "[Expected Exception Caught]: " << e.what() << "\n";
    }

    // 2. Run the Overloaded Reporter
    std::cout << "\n--- Triggering Full Audit ---\n";
    graph.validate_graph(std::cerr); 

    // 3. Setup a "Perfect" Graph to verify success
    EnvironmentGraph valid_graph;
    valid_graph.add_node({.is_terminal = false, .id = 10, .label = SystemState::UNAUTHENTICATED});
    valid_graph.add_node({.is_terminal = true,  .id = 11, .label = SystemState::USER_SESSION});
    valid_graph.add_edge(10, 11, AgentAction::INPUT_TEXT, 1.0, Perms::NONE);

    std::cout << "\n--- Verifying Valid Path ---\n";
    if (valid_graph.validate_graph()) {
        std::cout << "SUCCESS: Validated clean graph transition (10 -> 11).\n";
    } else {
        std::cerr << "CRITICAL FAILURE: Validated graph returned false.\n";
    }
}
void check_day4() {
    std::cout << "--- Starting Project Agentic-Shield: Day 4 Smoke Test ---\n";

    // 1. Setup Environment
    EnvironmentGraph graph;
    std::mt19937 gen(1337); // Fixed seed for reproducible manager audit

    // Define Nodes
    Node start_node{false, 0, SystemState::UNAUTHENTICATED, Perms::NONE};
    Node user_node{true, 1, SystemState::USER_SESSION, Perms::USER};
    Node db_node{true, 2, SystemState::ROOT_ADMIN, Perms::DB_ACCESS}; // Terminal Breach

    graph.add_node(start_node);
    graph.add_node(user_node);
    graph.add_node(db_node);

    // Define Bayesian Weights: 95% Normal Path, 5% Anomalous Path
    // Edge 0 -> 1 (Normal)
    graph.add_edge(0, 1, AgentAction::INPUT_TEXT, 0.95, Perms::NONE);
    // Edge 0 -> 2 (Anomalous Breach - bypass)
    graph.add_edge(0, 2, AgentAction::BYPASS_FILTER, 0.05, Perms::NONE);

    // 2. Validate Graph Integrity
    if (!graph.validate_graph(std::cerr)) {
        std::cerr << "Graph validation failed! Check your weights.\n";
        return;
    }

    // 3. Stochastic Simulation (1,000 Trials)
    int normal_count = 0;
    int breach_count = 0;
    const int total_trials = 1000;

    for (int i = 0; i < total_trials; ++i) {
        Agent agent(0, Perms::NONE);

        // Get the stochastic action from the graph
        auto action = graph.get_stochastic_action(agent.get_current_node_id(), agent.get_permissions(), gen);

        if (action.has_value()) {
            // Retrieve the target node object for the agent to consume
            const Node& target = graph.get_node(action->target_id);
            
            // Execute transition
            if (agent.execute_transition(*action, target)) {
                uint32_t final_id = agent.get_current_node_id();
                if (final_id == 1) normal_count++;
                else if (final_id == 2) breach_count++;
            }
        }
    }

    // 4. Report Results
    double breach_pct = (static_cast<double>(breach_count) / total_trials) * 100.0;
    
    std::cout << "\nResults after " << total_trials << " trials:\n";
    std::cout << "--------------------------------------\n";
    std::cout << "Normal Paths (Node 1): " << normal_count << "\n";
    std::cout << "Breach Paths (Node 2): " << breach_count << "\n";
    std::cout << "Actual Breach Rate:    " << std::fixed << std::setprecision(2) << breach_pct << "%\n";
    std::cout << "Target Breach Rate:    5.00%\n";
    std::cout << "--------------------------------------\n";

    // 5. Final Audit Logic Check
    assert(breach_count > 0 && "Error: Breach path was never taken. Check random distribution.");
    assert(breach_pct < 10.0 && "Error: Breach rate too high. Probability logic skewed.");
    
    std::cout << "Day 4 Status: PASSED\n";
}