#include "ParticleCreator.h"
#include <limits>

using namespace std;

ParticleCreator::ParticleCreator(std::vector<glm::vec3>& _vertices, float voxLen_) : vertices(_vertices), voxLen(voxLen_) {
    zmin = ymin = xmin = std::numeric_limits<float>::max();
    zmax = ymax = xmax = std::numeric_limits<float>::min();
    midVoxVal = voxLen / 2.0f;

    //for (int i = 0; i < vertices.size(); i++) { //can be used to scale vertices if needed.
        //vertices[i] *= 20;
    //}

    for (auto v : vertices) {
        if (v.x < xmin)
            xmin = v.x;
        if (v.y < ymin)
            ymin = v.y;
        if (v.z < zmin)
            zmin = v.z;
        if (v.x > xmax)
            xmax = v.x;
        if (v.y > ymax)
            ymax = v.y;
        if (v.z > zmax)
            zmax = v.z;
    }

    glm::vec3 maxP(xmax, ymax, zmax);
    glm::vec3 minP(xmin, ymin, zmin);
    cubeDimensions = maxP - minP;

    numBoxIndices = cubeDimensions / voxLen;
    numBoxIndices.x = roundf(numBoxIndices.x);
    numBoxIndices.y = roundf(numBoxIndices.y);
    numBoxIndices.z = roundf(numBoxIndices.z);

    zvalue_hits = std::vector<std::vector<std::vector<float>>>(numBoxIndices.x, std::vector<std::vector<float>>(numBoxIndices.y));

}



//returns z-value for line-triangle hits, didHit_ tells if any hit
float ParticleCreator::checkLineTriangle(glm::vec3 p1, glm::vec3 p2, glm::vec3 p3, glm::vec3 q1, glm::vec3 q2, bool& didHit_) {
    float result = std::numeric_limits<float>::min();;
    //p1.x += (xmin*-1.f);
    didHit_ = false;
    int s1 = signedVolume(q1, p1, p2, p3);
    int s2 = signedVolume(q2, p1, p2, p3);

    if (s1 != s2) {
        int s3 = signedVolume(q1, q2, p1, p2);
        int s4 = signedVolume(q1, q2, p2, p3);
        int s5 = signedVolume(q1, q2, p3, p1);

        if (s3 == s4 && s4 == s5) {
            didHit_ = true;
            glm::vec3 n = glm::cross(p2 - p1, p3 - p1);
            float t = glm::dot(p1 - q1, n) / glm::dot(q2 - q1, n);
            result = (q1 + t * (q2 - q1)).z;
        }
    }
    return result;
}


bool ParticleCreator::sameFloat(float f1, float f2) {
    if (f2 > f1) {
        float temp = f1;
        f1 = f2;
        f2 = temp;
    }
    float res = f1 - f2;
    return res < 0.001f;
}


int ParticleCreator::signedVolume(glm::vec3 a, glm::vec3 b, glm::vec3 c, glm::vec3 d) {
    float temp = glm::dot(glm::cross(b - a, c - a), d - a);
    if (temp == 0)
        return 0;
    if (temp < 0)
        return -1;
    return 1;
}


std::vector<glm::vec3> ParticleCreator::createParticles() {
    std::vector<glm::vec3> result;

    for (int i = 0; i < vertices.size(); i+=3) {
        glm::vec3 p1 = vertices[i];
        glm::vec3 p2 = vertices[i+1];
        glm::vec3 p3 = vertices[i+2];

        p1.x += (xmin*-1.f);
        p2.x += (xmin*-1.f);
        p3.x += (xmin*-1.f);
        p1.y += (ymin*-1.f);
        p2.y += (ymin*-1.f);
        p3.y += (ymin*-1.f);
        p1.z += (zmin*-1.f);
        p2.z += (zmin*-1.f);
        p3.z += (zmin*-1.f);

        int fromX = (int)(std::min(p1.x, std::min(p2.x,p3.x)) / voxLen);
        int fromY = (int)(std::min(p1.y, std::min(p2.y, p3.y)) / voxLen);

        int toX = (int)(std::max(p1.x, std::max(p2.x, p3.x)) / voxLen);
        int toY = (int)(std::max(p1.y, std::max(p2.y, p3.y)) / voxLen);

        for (int k = fromX; k <= toX; k++) { //lines to check for triangle intersection
            for (int l = fromY; l <= toY; l++) {
                glm::vec3 q1(k*voxLen + midVoxVal, l*voxLen + midVoxVal, -9999999.0f);
                glm::vec3 q2(k*voxLen + midVoxVal, l*voxLen + midVoxVal, 9999999.0f);

                bool didHit = false;
                float zHit = checkLineTriangle(p1, p2, p3, q1, q2, didHit);
                if (didHit) {
                    zvalue_hits[k][l].push_back(zHit);
                }
            }
        }
    }

    for (int i = 0; i < numBoxIndices.x; i++) {
        for (int j = 0; j < numBoxIndices.y; j++) {
            if (zvalue_hits[i][j].size() % 2 != 0) {
                cout << "Wrong z-values: " << i << " , " << j << " ENDING PROGRAM." << endl;
               // exit(EXIT_FAILURE);
                continue;
            }
            if (zvalue_hits[i][j].empty())
                continue;

            sort(zvalue_hits[i][j].begin(), zvalue_hits[i][j].end());

            bool isOdd = false;
            int hitIndex = 0;
            for (int z_box = 0; z_box < numBoxIndices.z && hitIndex < numBoxIndices.z; z_box+=1) {
                float nowZ = z_box * voxLen + midVoxVal;
                while (hitIndex < zvalue_hits[i][j].size() && zvalue_hits[i][j][hitIndex] < nowZ) {
                    hitIndex++;
                    isOdd = !isOdd;
                }
                if (isOdd) {
                    float nowX = i * voxLen + midVoxVal + xmin;
                    float nowY = j * voxLen + midVoxVal + ymin;
                    nowZ += zmin;
                    glm::vec3 particlePoint(nowX, nowY, nowZ);
                    result.push_back(particlePoint);
                }

            }

        }
    }


    return result;
}