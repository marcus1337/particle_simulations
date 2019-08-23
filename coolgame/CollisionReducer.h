#ifndef COLLISIONREDUCER_H
#define COLLISIONREDUCER_H

#include <unordered_map>
#include <vector>
#include <string>
#include <set>
#include <map>

struct KeyXYZ {
    int64_t x, y, z;
    KeyXYZ(int64_t x_, int64_t y_, int64_t z_): x(x_),y(y_), z(z_){}
    bool operator==(const KeyXYZ &other) const
    {
        return (x == other.x && y == other.y && z == other.z);
    }

    bool operator< (const KeyXYZ &right) const
    {
        if (x == right.x) {
            if (y != right.y)
                return y < right.y;
            return z < right.z;
        }
        return x < right.x;
    }

};

struct KeyData {
    KeyData() : objIndex(0), partIndex(0){};
    KeyData(int64_t objIndex_, int64_t partIndex_):objIndex(objIndex_), partIndex(partIndex_){}

    int64_t objIndex, partIndex;
    bool operator< (const KeyData &right) const
    {
        if (objIndex != right.objIndex)
            return objIndex < right.objIndex;
        return partIndex < right.partIndex;
    }
    bool operator==(const KeyData& rhs) const
    {
        return (objIndex == rhs.objIndex) && (partIndex == rhs.partIndex);
    }
    bool operator!=(const KeyData& rhs) const
    {
        return !operator==(rhs);
    }

};

class CollisionReducer {

public:
    float multiplier = 10000.f;
    int64_t voxLen;

    std::map<KeyXYZ, std::vector<KeyData>> voxels;
    CollisionReducer(double partRadius); //initialize a new, unused hash
    void addPart(int64_t objIndex, int64_t partIndex, float x, float y, float z); //stores in hasher
    std::set<KeyXYZ> potentialCollisions;
    bool mapHas(KeyXYZ& k);
};


#endif