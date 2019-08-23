#include "CollisionReducer.h"
#include <iostream>
#include <algorithm>

using namespace std;

CollisionReducer::CollisionReducer(double partRadius) {
    partRadius *= multiplier;
    double tmpVoxLen = partRadius * 2;
    voxLen = (int64_t) round(tmpVoxLen);
}


void CollisionReducer::addPart(int64_t objIndex, int64_t partIndex, float x, float y, float z) {

    x += 100.;
    y += 100.;
    z += 100.;

    if (x < 0 || y < 0 || z < 0 || x > 100000000 || y > 100000000 || z > 100000000)
        return;

    x *= multiplier;
    y *= multiplier;
    z *= multiplier;

    int64_t ix = ((int64_t)x / voxLen);
    int64_t iy = ((int64_t)y / voxLen);
    int64_t iz = ((int64_t)z / voxLen);

    KeyXYZ key(ix, iy, iz);

    if (!mapHas(key)) {
        std::vector <KeyData> datas;
        KeyData kdata(objIndex,partIndex);
        datas.push_back(kdata);
        voxels[key] = datas;
    }
    else {
        if (voxels[key].size() < 4) {
            KeyData kdata(objIndex, partIndex);
            voxels[key].push_back(kdata);
        }
    }

}


bool CollisionReducer::mapHas(KeyXYZ& k) {
    auto it = voxels.find(k);
    if (it != voxels.end())
    {
        return true;
    }
    return false;
}