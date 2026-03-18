#pragma once
#include "GameState.h"
#include <string>
#include <optional>

class SaveSistem
{
public:
    static SaveSistem& instance()
    {
        static SaveSistem inst;
        return inst;
    }

    void saveBilliard(const BilliardSave& state, const std::string& slot = "billiard_save");
    std::optional<BilliardSave> loadBilliard(const std::string& slot = "billiard_save");
    bool hasSave(const std::string& slot = "billiard_save");
    void deleteSave(const std::string& slot = "billiard_save");

private:
    SaveSistem() = default;
    std::string slotPath(const std::string& slot) const;

    const std::string saveDir = "assets/json/";
};