//EnvironmentGraph.cpp
#include "EnvironmentGraph.h"


void EnvironmentGraph::add_node(const Node& node)
    {
        nodeRegistry.insert({node.id, node}); //this could fail if we have duplicate nodes, let's look to update this in future
    };

    void EnvironmentGraph::add_edge(uint32_t source_id, uint32_t target_id, 
        AgentAction action, double weight, uint64_t perms)
    {
        if(nodeRegistry.find(source_id) == nodeRegistry.end() || nodeRegistry.find(target_id) == nodeRegistry.end())
        {
           throw std::invalid_argument("A node of the edge does not exist." );
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

    std::vector<Edge> EnvironmentGraph::get_valid_actions(uint32_t node_id, uint64_t agent_permissions)
    {
        //iterate through AL and return a vector of edges where agent perms and edge perms = edge required perms
        std::vector<Edge> valid_actions;
        //first check is if the adj_list has this node and the vector isn't empty
        if(adj_list.find(node_id) == adj_list.end()) return valid_actions;
        //it is an iterator to the map pair of -> {node_id, vector<edge>}
        auto it = adj_list.find(node_id);
        
        //it->second is the vector<edge> that corresponds with node_id
        // 
        for(const auto& edge : it->second)
        {
        //edge is the edge struct
            if((agent_permissions & edge.required_permissions) == edge.required_permissions)
            {
                valid_actions.push_back(edge);
            } 

            

        }



   


        return valid_actions;

    };

    bool EnvironmentGraph::validate_graph() const
    {
        bool is_valid = true;
        const double epsilon = 1e-6;
        //Iterate through our nodeRegistry for each [id, node] pair
        for(const auto& [id, node] : nodeRegistry)
        {
            double weight_sum = 0.0;
            //Check 2 things, edges in AL and Probability sums
            if(adj_list.find(id) != adj_list.end()) //does our map contain our id?
            {
                                                //yes it does   
                for(const auto& edge : adj_list.at(id)) //for each edge in the vector
                {
                    //we have the edge we need to check that the target_id for the node is valid
                    if(nodeRegistry.find(edge.target_id) == nodeRegistry.end())
                   {
                        //this is meaning our node that our edge targets does not exist
                        is_valid = false; //set our is_valid to false, we don't want to return here becuase we want a full audit of the entire graph



                   }
                   //this means our node exists
                   weight_sum += edge.weight;
                }
                //bayesian integrity check
            }
            if(node.is_terminal){
                if(weight_sum > epsilon){
                    std::cerr << "Terminal Node " << id << " has a weight sum when it should not: " << weight_sum << "\n";
                    is_valid = false;
                }
                }else if(std::abs(weight_sum-1.0) > epsilon){
                    std::cerr << "Node: " << id << " probabilities sum is off it is: " << weight_sum << std::endl;
                    is_valid = false;
                }
            //no it doesnt we are out
            //does every node included in edge exist? 
            //go through edge and search for the node.



        }
        return is_valid;
    };

        bool EnvironmentGraph::validate_graph(std::ostream& report_stream) const

    {



        bool is_valid = true;
        const double epsilon = 1e-6;
        //Iterate through our nodeRegistry for each [id, node] pair
        for(const auto& [id, node] : nodeRegistry)
        {
            double weight_sum = 0.0;
            //Check 2 things, edges in AL and Probability sums
            if(adj_list.find(id) != adj_list.end()) //does our map contain our id?
            {
                                                //yes it does   
                for(const auto& edge : adj_list.at(id)) //for each edge in the vector
                {
                    //we have the edge we need to check that the target_id for the node is valid
                    if(nodeRegistry.find(edge.target_id) == nodeRegistry.end())
                   {
                        //this is meaning our node that our edge targets does not exist
                        is_valid = false; //set our is_valid to false, we don't want to return here becuase we want a full audit of the entire graph
                        report_stream << "Invalid Edge: Source Node " << id << " has an edge to non-existent Target Node " << edge.target_id << "\n";


                   }
                   //this means our node exists
                   weight_sum += edge.weight;
                }
                //bayesian integrity check
            }
            if(node.is_terminal){
                if(weight_sum > epsilon){
                    report_stream << "Terminal Node " << id << " has a weight sum when it should not: " << weight_sum << "\n";
                    is_valid = false;
                }
                }else if(std::abs(weight_sum-1.0) > epsilon){
                    report_stream << "Node: " << id << " probabilities sum is off it is: " << weight_sum << std::endl;
                    is_valid = false;
                }
            //no it doesnt we are out
            //does every node included in edge exist? 
            //go through edge and search for the node.



        }
        return is_valid;
        
    };
//https://en.cppreference.com/w/cpp/numeric/random/discrete_distribution.html
    std::optional<Edge> EnvironmentGraph::get_stochastic_action(uint32_t node_id, uint64_t agent_permissions, std::mt19937& gen){
        std::vector<Edge> valid_edges = get_valid_actions(node_id,agent_permissions);
        //check to make sure valid_edges isn't empty
        if(valid_edges.empty()) {return std::nullopt; }

        //iterates through valid_edges to create a weight vector, the index corresponds with the position of the weight
        //to the edge location in the vector, important for the distribution finding. 
        std::vector<double> edge_weights = {};
        for(const auto& edge : valid_edges)
        {
            edge_weights.push_back(edge.weight);
        }
        

    std::discrete_distribution<size_t> distribution(edge_weights.begin(), edge_weights.end());
    //finds the index from the distribution
    size_t selected_weight = distribution(gen);
  

    //returns the edge from the vector using the index we found in the distribution
    return valid_edges[selected_weight];
    }   