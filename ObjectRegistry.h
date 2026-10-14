#pragma once

#include <algorithm>
#include <map>
#include <vector>

template <typename T>
class ObjectRegistry
{
public:
    T& Create()
    {
        int id = nextId++;
        return objects.emplace(id, T(id)).first->second;
    }

    bool Insert(const T& object)
    {
        int id = object.GetId();
        if (!objects.emplace(id, object).second)
            return false;
        nextId = std::max(nextId, id + 1);
        return true;
    }

    bool Remove(int id)
    {
        return objects.erase(id) > 0;
    }

    T* Find(int id)
    {
        auto found = objects.find(id);
        return found == objects.end() ? nullptr : &found->second;
    }

    bool Contains(int id) const
    {
        return objects.count(id) > 0;
    }

    bool IsEmpty() const
    {
        return objects.empty();
    }

    const std::map<int, T>& GetAll() const
    {
        return objects;
    }

    template <typename Predicate>
    std::vector<int> FindIds(Predicate matches) const
    {
        std::vector<int> ids;
        for (const auto& [id, object] : objects)
            if (matches(object))
                ids.push_back(id);
        return ids;
    }

private:
    std::map<int, T> objects;
    int nextId = 1;
};
