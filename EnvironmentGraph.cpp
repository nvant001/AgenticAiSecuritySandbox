#include "schema.h"
#include <unordered_map>
#include <vector>
class EnvironmentGraph{

    private:
    // Key = our source id, would give us our edges
    std::unordered_map<std::uint32_t, std::vector<Edge>> adj_list;
    // Node id, would give us our node
    std::unordered_map<std::uint32_t, Node> nodeRegistry;

    public:

    void add_node(const Node& node)
    {
        nodeRegistry.insert({node.id, node});
    };

    void add_edge(uint32_t source_id, uint32_t target_id, 
        AgentAction action, double weight, uint64_t perms)
    {
        if(nodeRegistry.find(source_id) == nodeRegistry.end() || nodeRegistry.find(target_id) == nodeRegistry.end())
        {
            std::cout << "Error message for can't find the source or the target node." << std::endl;
        } //Cant find our nodes
        

        //create and add a new edge
        Edge new_edge;
        new_edge.source_id = source_id;
        new_edge.target_id = target_id;
        new_edge.action = action;
        new_edge.weight = weight;
        new_edge.required_permissions = perms;

        adj_list[source_id].push_back(new_edge);
    };//end of add_edge

    std::vector<Edge> get_valid_actions(uint32_t node_id, uint64_t agent_permissions);





};