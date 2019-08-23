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

struct KeyHasher
{
    std::size_t operator()(const KeyXYZ& k) const
    {
        using std::hash;
        size_t res = 17;
        res = res * 31 + hash<int64_t>()(k.x);
        res = res * 31 + hash<int64_t>()(k.y);
        res = res * 31 + hash<int64_t>()(k.z);
        return res;
    }
};

struct KeyDataObj {
    int64_t kx, ky, kz;
    int64_t objIndex, partIndex;
    KeyDataObj(int64_t objIndex_, int64_t partIndex_) : objIndex(objIndex_), partIndex(partIndex_) {}

    bool operator==(const KeyDataObj& rhs) const
    {
        return (objIndex == rhs.objIndex) && (partIndex == rhs.partIndex);
    }
    bool operator!=(const KeyDataObj& rhs) const
    {
        return !operator==(rhs);
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

   // std::vector<KeyDataObj> storedParticles;
  //  int64_t potentials = 0; //amount of different objects in this + surrounding voxels, if >= 2 then potential collision

};

class CollisionReducer {

public:
    int64_t minVal = 10000;
    int64_t gridSize = 10000000;
    float multiplier = 10000.f;
    int64_t voxLen; //int y = (int)(x + 0.5);

    std::map<KeyXYZ, std::vector<KeyData>> voxels;
   // std::unordered_map<KeyXYZ, KeyData, KeyHasher> voxels;
    CollisionReducer(double partRadius); //initialize a new, unused hash
    void addPart(int64_t objIndex, int64_t partIndex, float x, float y, float z); //stores in hasher
    std::set<KeyXYZ> potentialCollisions;

    bool mapHas(KeyXYZ& k);

};


#endif