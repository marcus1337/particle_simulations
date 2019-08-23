#include "CollisionReducer.h"
#include <iostream>
#include <algorithm>

using namespace std;

CollisionReducer::CollisionReducer(double partRadius) {

    //cout << "RAD " << partRadius << endl;
   // minVal *= multiplier;
   // gridSize *= multiplier;
    partRadius *= multiplier;
   // double voxTmpLen = double(gridSize) / (partRadius * 2.f * 4);
   // voxLen = (int64_t)(voxTmpLen + 0.5);
    double tmpVoxLen = partRadius * 2;
    voxLen = (int64_t) round(tmpVoxLen);

}


void CollisionReducer::addPart(int64_t objIndex, int64_t partIndex, float x, float y, float z) {

    x += 100.;
    y += 100.;
    z += 100.;

    x *= multiplier;
    y *= multiplier;
    z *= multiplier;

    int64_t ix = ((int64_t)x / voxLen);
    int64_t iy = ((int64_t)y / voxLen);
    int64_t iz = ((int64_t)z / voxLen);

   // cout << "TEST " << ix << "," << iy << "," << iz << endl;

    KeyXYZ key(ix, iy, iz);
    KeyDataObj obj(objIndex, partIndex);
    obj.kx = ix;
    obj.ky = iy;
    obj.kz = iz;

    if (!mapHas(key)) {
        std::vector <KeyData> datas;
        KeyData kdata(objIndex,partIndex);
        datas.push_back(kdata);
        //kdata.storedParticles.push_back(obj);
        voxels[key] = datas;
    }
    else {
        if (voxels[key].size() < 4) {
            KeyData kdata(objIndex, partIndex);
            voxels[key].push_back(kdata);
        }
      //  if (voxels[key].storedParticles.size() < 4)
       //     voxels[key].storedParticles.push_back(obj);
    }

    //cout << "WAT " << objIndex << "_ " << ix << " " << iy << " " << iz << " AA " << voxLen << endl;

  /*  x *= multiplier;
    y *= multiplier;
    z *= multiplier;

    int64_t ix = (x / voxLen);
    int64_t iy = (y / voxLen);
    int64_t iz = (z / voxLen);

    //cout << "WAT " << objIndex << "_ " << ix << " " << iy << " " << iz << " AA " << voxLen << endl;

    KeyXYZ key(ix, iy, iz);
    KeyDataObj obj(objIndex, partIndex);

    if (!mapHas(key)) {
        KeyData kdata;
        kdata.potentials = 1;
        kdata.storedParticles.push_back(obj);
        voxels[key] = kdata;
    }
    else {
        if (voxels[key].storedParticles.size() > 4) {
            return;
        }

        for (auto& x : voxels[key].storedParticles) {
            if (x.objIndex == objIndex) {
                voxels[key].storedParticles.push_back(obj);
                return;
            }
        }
        voxels[key].storedParticles.push_back(obj);
        voxels[key].potentials++;
        potentialCollisions.insert(key);
    }

    for (int64_t a = ix - 1; a <= ix + 1; a++) {
        for (int64_t b = iy - 1; b <= iy + 1; b++) {
            for (int64_t c = iz - 1; c <= iz + 1; c++) {
                if (a == ix && b == iy && c == iz)
                    continue;

                KeyXYZ otherKey(a, b, c);

                if (!mapHas(otherKey)) {
                    KeyData kdata;
                    kdata.potentials = 1;
                    voxels[otherKey] = kdata;
                }
                else {
                    bool hasSame = false;
                    for (auto& x : voxels[otherKey].storedParticles) {
                        if (x.objIndex == objIndex) {
                            hasSame = true;
                        }
                    }
                    if (hasSame)
                        continue;
                    if (!voxels[otherKey].storedParticles.empty()) {
                        voxels[otherKey].potentials++;
                        potentialCollisions.insert(otherKey);
                    }
                }

            }
        }
    }*/
}


bool CollisionReducer::mapHas(KeyXYZ& k) {
    auto it = voxels.find(k);
    if (it != voxels.end())
    {
        return true;
    }
    return false;
}