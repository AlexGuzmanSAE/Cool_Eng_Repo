#include "SaveSistem.h"
#include "lib/nlohmann/json.hpp"
#include <fstream>
#include <filesystem>

using json = nlohmann::json;

std::string SaveSistem::slotPath(const std::string& slot) const
{
    return saveDir + slot + ".json";
}

bool SaveSistem::hasSave(const std::string& slot)
{
    return std::filesystem::exists(slotPath(slot));
}

void SaveSistem::deleteSave(const std::string& slot)
{
    std::filesystem::remove(slotPath(slot));
}

void SaveSistem::saveBilliard(const BilliardSave& state, const std::string& slot)
{
    std::filesystem::create_directories(saveDir);

    json j;
    j["score"] = state.score;
    j["hasCueBall"] = state.hasCueBall;
    j["cueBall"] = { state.cueBall.x, state.cueBall.y, state.cueBall.r, state.cueBall.g, state.cueBall.b, state.cueBall.a };

    for (auto& b : state.balls)
        j["balls"].push_back({ b.x, b.y, b.r, b.g, b.b, b.a });

    std::ofstream file(slotPath(slot));
    file << j.dump(4);
}

std::optional<BilliardSave> SaveSistem::loadBilliard(const std::string& slot)
{
    if (!hasSave(slot)) return std::nullopt;

    std::ifstream file(slotPath(slot));
    json j;
    file >> j;

    BilliardSave state;
    state.score = j["score"];
    state.hasCueBall = j["hasCueBall"];

    auto& cb = j["cueBall"];
    state.cueBall = { cb[0], cb[1], cb[2], cb[3], cb[4], cb[5] };

    for (auto& b : j["balls"])
        state.balls.push_back({ b[0], b[1], b[2], b[3], b[4], b[5] });

    return state;
}