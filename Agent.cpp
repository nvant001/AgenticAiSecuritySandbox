#include <iostream>
#include "schema.h"
#include "EnvironmentGraph.h"
#include <vector>
class Agent{
    private:
    uint32_t current_node_id = 0;
    uint64_t permissions = Perms::NONE;
    std::vector<uint32_t> history = {};

    public:
    Agent(uint32_t start_node, uint64_t base_perms) : current_node_id(start_node), permissions(base_perms){
        history.push_back(start_node);
    }
    void perm_action(const Edge& edge, const Node& destination_node){
    current_node_id = edge.target_id;
    history.push_back(current_node_id);
    
    permissions |= destination_node.permissions_granted;

    }

    bool execute_transition(const Edge& edge, const Node& target_node){
        //check to make sure that where we currently are matches between the edge and our agent
        if(edge.source_id != current_node_id) {return false; }
        //move our agent to the target
        if((permissions & edge.required_permissions) != edge.required_permissions) {return false;}
        if(edge.target_id != target_node.id) {return false;}
        current_node_id = edge.target_id;
        permissions |= target_node.permissions_granted;
        history.push_back(current_node_id);


        //Final sanity check
        return true;

    }
 

    
    uint64_t get_permissions() const {return permissions; }
    uint32_t get_current_node_id() const {return current_node_id;}
    std::vector<uint32_t> get_history() const {return history;}
    




};

