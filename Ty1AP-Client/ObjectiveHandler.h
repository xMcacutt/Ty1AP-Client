#pragma once
#include <array>
#include <string>
#include <vector>

class ObjectiveHandler
{
public:
    static void HandleObjective(std::string name, int state, std::vector<std::byte>& data);

    struct Objective {
        int level = -1;
        std::string name;
    };

    static inline const std::array<Objective, 5> objectives{
        Objective{6, "Seahorse"},
        Objective{8, "Burner"},
        Objective{9, "SnowKoalaKid"},
        Objective{13, "StumpKoalaKid"},
        Objective{14, "Chest"}
    };

    static inline const int Inactive = 0;
    static inline const int Active = 1;
    static inline const int ReadyForTurnIn = 2;
    static inline const int Complete = 3;

private:
};