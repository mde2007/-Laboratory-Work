#pragma once
#include <unordered_map>
#include <vector>
#include <string>
#include "pipe.h"
#include "CS.h"

class GasNetwork
{
    std::unordered_map<int, Pipe> pipes;
    std::unordered_map<int, CS> stations;

    template<typename Obj, typename Pred>
    static std::vector<int> findByFilter(const std::unordered_map<int, Obj>& items, Pred f);

    template<typename Obj>
    static void printFound(const std::unordered_map<int, Obj>& items, const std::vector<int>& ids);

    template<typename Obj>
    static std::vector<int> searchByName(const std::unordered_map<int, Obj>& items);


    std::vector<int> searchPipesByRepair() const;
    std::vector<int> searchCSByActivePercent() const;
    std::vector<int> findPipes() const;
    std::vector<int> findCS() const;

    void editPipe(int id, bool working);
    void editCS(int id, int percent);
    void deletePipe(int id);
    void deleteCS(int id);

    void batchPipes();
    void batchCS();

    int findFreePipe(int diameter) const;
    bool checkEnds(int from, int to) const;

public:
    void addPipe();
    void addCS();
    void viewAll() const;
    void findObject();
    void save() const;
    void load();
    void connect();
};