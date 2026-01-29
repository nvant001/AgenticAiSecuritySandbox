#pragma once
#include <iostream>

//Permision constant bitmask slots
namespace Perms {
    inline constexpr uint64_t NONE = 0; // 
    inline constexpr uint64_t USER = 1 << 0; // 0x1
    inline constexpr uint64_t ADMIN = 1 << 1; // 0x2
    inline constexpr uint64_t DB_ACCESS = 1 << 2; // 0x4


}


enum class SystemState {
    UNAUTHENTICATED, 
    USER_SESSION, 
    API_ACCESS, 
    S3_BUCKET_PRIVATE,
    ROOT_ADMIN};

enum class AgentAction{
    INPUT_TEXT,
    EXECUTE_QUERY,
    REQUEST_TOKEN,
    BYPASS_FILTER
};

struct Node {
    bool is_terminal = false;
    uint32_t id = 0;
    SystemState label = SystemState::UNAUTHENTICATED;
    uint64_t permissions_granted = Perms::NONE;
    
};

struct Edge{
    uint32_t source_id = 0;
    uint32_t target_id = 0;
    AgentAction action = AgentAction::INPUT_TEXT;
    double weight = 0.0;

    uint64_t required_permissions = Perms::NONE;
};

