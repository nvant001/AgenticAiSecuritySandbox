
#include "Agent.h"

Agent::Agent(uint32_t start_node, uint64_t base_perms)
{
    current_node_id = start_node;
    permissions = base_perms;
    history.push_back(start_node);
}
void Agent::perm_action(const Edge& edge, const Node& destination_node){
    current_node_id = edge.target_id;
    history.push_back(current_node_id);
    
    permissions |= destination_node.permissions_granted;

}

bool Agent::execute_transition(const Edge& edge, const Node& target_node){
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
 


    





