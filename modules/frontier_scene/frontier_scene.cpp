#include <GL/glut.h>

#include <algorithm>
#include <cmath>
#include <cstdio>
#include <cstdlib>
#include <vector>

// Great Zimbabwe University
// Computer Graphics Practical Assignment - Part B
// 3D-style cartoon character scene demonstrating near and far plane depth.

const float PI = 3.1415926535f;
const float CHARACTER_HOME_Z = 1.20f;
const float DEFAULT_CAMERA_ORBIT = 24.0f;
const float DEFAULT_CAMERA_DISTANCE = 8.9f;
const float DEFAULT_CAMERA_HEIGHT = 3.05f;

struct Color {
    float r;
    float g;
    float b;
};

struct Vec3 {
    float x;
    float y;
    float z;
};

GLUquadric* gQuadric = NULL;
int gWindowWidth = 1000;
int gWindowHeight = 820;
float gCharacterX = 0.0f;
float gCharacterZ = CHARACTER_HOME_Z;
float gCharacterYaw = 0.0f;
float gRunPhase = 0.0f;
bool gIsRunning = false;
float gWorldTravel = 0.0f;
float gCameraOrbit = DEFAULT_CAMERA_ORBIT;
float gCameraDistance = DEFAULT_CAMERA_DISTANCE;
float gCameraHeight = DEFAULT_CAMERA_HEIGHT;
float gFaceTime = 0.0f;
float gBlinkAmount = 0.0f;
bool gShowInstructions = true;
bool gKeyDown[256] = {};
bool gSpecialKeyDown[512] = {};

const Color SKIN = {0.78f, 0.40f, 0.16f};
const Color SKIN_LIGHT = {0.96f, 0.58f, 0.28f};
const Color CHEEK = {0.70f, 0.24f, 0.16f};
const Color HAIR = {0.015f, 0.018f, 0.030f};
const Color HAIR_HIGHLIGHT = {0.04f, 0.20f, 0.32f};
const Color SUIT = {0.022f, 0.028f, 0.052f};
const Color SUIT_LIGHT = {0.070f, 0.090f, 0.150f};
const Color SUIT_EDGE = {0.34f, 0.42f, 0.58f};
const Color SHIRT = {0.94f, 0.96f, 0.98f};
const Color TIE = {0.55f, 0.035f, 0.085f};
const Color TIE_DARK = {0.25f, 0.015f, 0.035f};
const Color TROUSERS = {0.045f, 0.050f, 0.078f};
const Color SHOES = {0.010f, 0.008f, 0.006f};
const Color FOLDER_COVER = {0.36f, 0.12f, 0.035f};
const Color PAPER = {0.96f, 0.93f, 0.84f};
const Color WATCH_METAL = {0.78f, 0.70f, 0.50f};
const Color WATCH_FACE = {0.03f, 0.04f, 0.06f};
const Color BLACK = {0.015f, 0.015f, 0.018f};

Vec3 makeVec(float x, float y, float z) {
    Vec3 v = {x, y, z};
    return v;
}

Vec3 subtract(Vec3 a, Vec3 b) {
    return makeVec(a.x - b.x, a.y - b.y, a.z - b.z);
}

float length(Vec3 v) {
    return std::sqrt(v.x * v.x + v.y * v.y + v.z * v.z);
}

float clampValue(float value, float minimum, float maximum) {
    if (value < minimum) {
        return minimum;
    }
    if (value > maximum) {
        return maximum;
    }
    return value;
}

float loopDepth(float baseZ, float travel, float period, float frontZ) {
    float z = baseZ + std::fmod(travel, period);
    while (z > frontZ) {
        z -= period;
    }
    while (z < frontZ - period) {
        z += period;
    }
    return z;
}

float approach01(float z, float farZ, float frontZ) {
    return clampValue((z - farZ) / (frontZ - farZ), 0.0f, 1.0f);
}

float sideLaneX(float baseX, float z, float farZ, float frontZ, float extraSpread) {
    float side = baseX < 0.0f ? -1.0f : 1.0f;
    float t = approach01(z, farZ, frontZ);
    return baseX + side * extraSpread * t * t;
}

float approachScale(float baseScale, float z, float farZ, float frontZ, float extraScale) {
    float t = approach01(z, farZ, frontZ);
    return baseScale * (1.0f + extraScale * t);
}

void setMaterial(const Color& color, float shininess = 28.0f, float specularStrength = 0.25f) {
    GLfloat ambient[] = {
        color.r * 0.42f,
        color.g * 0.42f,
        color.b * 0.42f,
        1.0f
    };
    GLfloat diffuse[] = {color.r, color.g, color.b, 1.0f};
    GLfloat specular[] = {
        specularStrength,
        specularStrength,
        specularStrength,
        1.0f
    };
    GLfloat shine[] = {shininess};
    GLfloat noEmission[] = {0.0f, 0.0f, 0.0f, 1.0f};

    glMaterialfv(GL_FRONT_AND_BACK, GL_AMBIENT, ambient);
    glMaterialfv(GL_FRONT_AND_BACK, GL_DIFFUSE, diffuse);
    glMaterialfv(GL_FRONT_AND_BACK, GL_SPECULAR, specular);
    glMaterialfv(GL_FRONT_AND_BACK, GL_SHININESS, shine);
    glMaterialfv(GL_FRONT_AND_BACK, GL_EMISSION, noEmission);
    glColor3f(color.r, color.g, color.b);
}

void setEmissionMaterial(const Color& color) {
    GLfloat ambient[] = {color.r * 0.35f, color.g * 0.35f, color.b * 0.35f, 1.0f};
    GLfloat diffuse[] = {color.r, color.g, color.b, 1.0f};
    GLfloat specular[] = {0.25f, 0.22f, 0.08f, 1.0f};
    GLfloat shine[] = {18.0f};
    GLfloat emission[] = {color.r * 0.65f, color.g * 0.55f, color.b * 0.12f, 1.0f};

    glMaterialfv(GL_FRONT_AND_BACK, GL_AMBIENT, ambient);
    glMaterialfv(GL_FRONT_AND_BACK, GL_DIFFUSE, diffuse);
    glMaterialfv(GL_FRONT_AND_BACK, GL_SPECULAR, specular);
    glMaterialfv(GL_FRONT_AND_BACK, GL_SHININESS, shine);
    glMaterialfv(GL_FRONT_AND_BACK, GL_EMISSION, emission);
    glColor3f(color.r, color.g, color.b);
}

void drawRawSphere(float x, float y, float z, float sx, float sy, float sz, int slices = 48, int stacks = 32) {
    glPushMatrix();
    glTranslatef(x, y, z);
    glScalef(sx, sy, sz);
    glutSolidSphere(1.0, slices, stacks);
    glPopMatrix();
}

void drawSphere(float x, float y, float z, float sx, float sy, float sz,
                const Color& color, float shininess = 28.0f, float specularStrength = 0.25f) {
    setMaterial(color, shininess, specularStrength);
    drawRawSphere(x, y, z, sx, sy, sz);
}

void drawCube(float x, float y, float z, float sx, float sy, float sz,
              const Color& color, float shininess = 18.0f, float specularStrength = 0.16f) {
    setMaterial(color, shininess, specularStrength);
    glPushMatrix();
    glTranslatef(x, y, z);
    glScalef(sx, sy, sz);
    glutSolidCube(1.0);
    glPopMatrix();
}

void drawRotatedCube(float x, float y, float z, float sx, float sy, float sz,
                     const Color& color, float rotateZ, float rotateY = 0.0f,
                     float shininess = 18.0f, float specularStrength = 0.16f) {
    setMaterial(color, shininess, specularStrength);
    glPushMatrix();
    glTranslatef(x, y, z);
    glRotatef(rotateY, 0.0f, 1.0f, 0.0f);
    glRotatef(rotateZ, 0.0f, 0.0f, 1.0f);
    glScalef(sx, sy, sz);
    glutSolidCube(1.0);
    glPopMatrix();
}

void orientLocalZToVector(Vec3 direction) {
    float len = length(direction);
    if (len <= 0.0001f) {
        return;
    }

    float cosAngle = direction.z / len;
    if (cosAngle > 1.0f) {
        cosAngle = 1.0f;
    }
    if (cosAngle < -1.0f) {
        cosAngle = -1.0f;
    }

    float angle = std::acos(cosAngle) * 180.0f / PI;
    float axisX = -direction.y;
    float axisY = direction.x;
    float axisLength = std::sqrt(axisX * axisX + axisY * axisY);

    if (axisLength < 0.0001f) {
        if (direction.z < 0.0f) {
            glRotatef(180.0f, 1.0f, 0.0f, 0.0f);
        }
        return;
    }

    glRotatef(angle, axisX, axisY, 0.0f);
}

void drawCylinderBetween(Vec3 start, Vec3 end, float radius, const Color& color,
                         float shininess = 24.0f, float specularStrength = 0.20f, bool capEnds = true) {
    Vec3 direction = subtract(end, start);
    float len = length(direction);
    if (len <= 0.0001f) {
        return;
    }

    setMaterial(color, shininess, specularStrength);
    glPushMatrix();
    glTranslatef(start.x, start.y, start.z);
    orientLocalZToVector(direction);

    if (capEnds) {
        gluDisk(gQuadric, 0.0, radius, 36, 1);
    }
    gluCylinder(gQuadric, radius, radius, len, 36, 10);
    if (capEnds) {
        glTranslatef(0.0f, 0.0f, len);
        gluDisk(gQuadric, 0.0, radius, 36, 1);
    }

    glPopMatrix();
}

void drawConeBetween(Vec3 base, Vec3 tip, float radius, const Color& color,
                     float shininess = 20.0f, float specularStrength = 0.16f) {
    Vec3 direction = subtract(tip, base);
    float len = length(direction);
    if (len <= 0.0001f) {
        return;
    }

    setMaterial(color, shininess, specularStrength);
    glPushMatrix();
    glTranslatef(base.x, base.y, base.z);
    orientLocalZToVector(direction);
    gluDisk(gQuadric, 0.0, radius, 36, 1);
    gluCylinder(gQuadric, radius, 0.02f, len, 36, 10);
    glPopMatrix();
}

void drawTorus(float x, float y, float z, float tubeRadius, float ringRadius,
               float sx, float sy, float sz, const Color& color) {
    setMaterial(color, 32.0f, 0.22f);
    glPushMatrix();
    glTranslatef(x, y, z);
    glScalef(sx, sy, sz);
    glutSolidTorus(tubeRadius, ringRadius, 18, 48);
    glPopMatrix();
}

void drawAngularJoint(float x, float y, float z, float sx, float sy, float sz,
                      const Color& color, float rotateZ = 0.0f, float rotateY = 0.0f) {
    float rz = rotateZ * PI / 180.0f;
    float ry = rotateY * PI / 180.0f;
    Vec3 axis = makeVec(std::cos(rz) * std::cos(ry),
                        std::sin(rz),
                        -std::cos(rz) * std::sin(ry));
    float halfLength = sx * 0.56f;
    Vec3 start = makeVec(x - axis.x * halfLength, y - axis.y * halfLength, z - axis.z * halfLength);
    Vec3 end = makeVec(x + axis.x * halfLength, y + axis.y * halfLength, z + axis.z * halfLength);

    drawCylinderBetween(start, end, sy * 0.50f, color, 34.0f, 0.24f);
    drawCylinderBetween(makeVec(x - axis.x * sx * 0.10f, y - axis.y * sx * 0.10f, z - axis.z * sx * 0.10f),
                        makeVec(x + axis.x * sx * 0.10f, y + axis.y * sx * 0.10f, z + axis.z * sx * 0.10f),
                        sy * 0.64f, {0.015f, 0.018f, 0.030f}, 30.0f, 0.16f);
}

void drawLine3D(Vec3 a, Vec3 b, const Color& color, float width) {
    glDisable(GL_LIGHTING);
    glColor3f(color.r, color.g, color.b);
    glLineWidth(width);
    glBegin(GL_LINES);
    glVertex3f(a.x, a.y, a.z);
    glVertex3f(b.x, b.y, b.z);
    glEnd();
    glEnable(GL_LIGHTING);
}

void drawSky() {
    glDisable(GL_LIGHTING);
    glDepthMask(GL_FALSE);

    glBegin(GL_QUADS);
    glColor3f(0.045f, 0.060f, 0.140f);
    glVertex3f(-65.0f, 32.0f, -55.0f);
    glVertex3f(65.0f, 32.0f, -55.0f);
    glColor3f(0.115f, 0.130f, 0.230f);
    glVertex3f(65.0f, -5.0f, -55.0f);
    glVertex3f(-65.0f, -5.0f, -55.0f);
    glEnd();

    glPointSize(2.4f);
    glBegin(GL_POINTS);
    glColor3f(0.58f, 0.88f, 1.0f);
    for (int i = 0; i < 52; ++i) {
        float x = -46.0f + static_cast<float>((i * 19) % 93);
        float y = 5.8f + static_cast<float>((i * 23) % 210) * 0.075f;
        float z = -53.0f;
        glVertex3f(x, y, z);
    }
    glEnd();

    glDepthMask(GL_TRUE);
    glEnable(GL_LIGHTING);
}

void drawGround() {
    setMaterial({0.055f, 0.065f, 0.092f}, 42.0f, 0.30f);
    glBegin(GL_QUADS);
    glNormal3f(0.0f, 1.0f, 0.0f);
    glVertex3f(-24.0f, 0.0f, 7.5f);
    glVertex3f(24.0f, 0.0f, 7.5f);
    glVertex3f(38.0f, 0.0f, -48.0f);
    glVertex3f(-38.0f, 0.0f, -48.0f);
    glEnd();

    setMaterial({0.080f, 0.090f, 0.135f}, 48.0f, 0.36f);
    glBegin(GL_QUADS);
    glNormal3f(0.0f, 1.0f, 0.0f);
    glVertex3f(-38.0f, 0.025f, -14.0f);
    glVertex3f(38.0f, 0.025f, -14.0f);
    glVertex3f(44.0f, 0.025f, -48.0f);
    glVertex3f(-44.0f, 0.025f, -48.0f);
    glEnd();
}

void drawPath() {
    setMaterial({0.045f, 0.055f, 0.095f}, 52.0f, 0.38f);
    glBegin(GL_QUADS);
    glNormal3f(0.0f, 1.0f, 0.0f);
    glVertex3f(-3.3f, 0.055f, 7.0f);
    glVertex3f(3.3f, 0.055f, 7.0f);
    glVertex3f(0.78f, 0.055f, -38.0f);
    glVertex3f(-0.78f, 0.055f, -38.0f);
    glEnd();

    glDisable(GL_LIGHTING);
    glDepthMask(GL_FALSE);
    glLineWidth(2.0f);
    glColor4f(0.05f, 0.86f, 1.0f, 0.70f);
    glBegin(GL_LINES);
    glVertex3f(-3.10f, 0.075f, 7.0f);
    glVertex3f(-0.70f, 0.075f, -38.0f);
    glVertex3f(3.10f, 0.075f, 7.0f);
    glVertex3f(0.70f, 0.075f, -38.0f);
    for (int i = 0; i < 12; ++i) {
        float z = 5.8f - static_cast<float>(i) * 3.55f;
        float depthT = clampValue((7.0f - z) / 45.0f, 0.0f, 1.0f);
        float halfWidth = 3.0f - depthT * 2.18f;
        glVertex3f(-halfWidth, 0.080f, z);
        glVertex3f(halfWidth, 0.080f, z);
    }
    glEnd();
    glLineWidth(1.0f);
    glDepthMask(GL_TRUE);
    glEnable(GL_LIGHTING);

    const float markerSpacing = 4.8f;
    for (int i = 0; i < 10; ++i) {
        float z = loopDepth(-2.0f - markerSpacing * static_cast<float>(i), gWorldTravel, markerSpacing * 10.0f, 7.2f);
        float depthT = clampValue((7.2f - z) / 44.0f, 0.0f, 1.0f);
        float xOffset = (i % 2 == 0 ? -1.25f : 1.25f) * (1.0f - depthT * 0.60f);
        float scale = 0.14f - depthT * 0.05f;
        setEmissionMaterial(i % 2 == 0 ? Color{0.05f, 0.90f, 1.0f} : Color{0.98f, 0.12f, 0.75f});
        drawRawSphere(xOffset, 0.16f, z, scale * 1.1f, scale * 0.30f, scale * 1.1f, 18, 10);
    }
}

void drawSun() {
    setEmissionMaterial({0.70f, 0.90f, 1.0f});
    drawRawSphere(8.6f, 9.2f, -31.0f, 0.90f, 0.90f, 0.90f, 48, 28);
    glDisable(GL_LIGHTING);
    glColor4f(0.45f, 0.78f, 1.0f, 0.65f);
    glLineWidth(2.0f);
    glBegin(GL_LINE_LOOP);
    for (int i = 0; i < 72; ++i) {
        float a = 2.0f * PI * static_cast<float>(i) / 72.0f;
        glVertex3f(8.6f + std::cos(a) * 1.45f, 9.2f + std::sin(a) * 1.45f, -31.0f);
    }
    glEnd();
    glLineWidth(1.0f);
    glEnable(GL_LIGHTING);
}

void drawCloud(float x, float y, float z, float scale) {
    setEmissionMaterial({0.11f, 0.82f, 1.0f});
    drawRawSphere(x, y, z, 0.62f * scale, 0.16f * scale, 0.24f * scale, 36, 14);
    drawRawSphere(x - 0.48f * scale, y + 0.06f * scale, z + 0.02f * scale, 0.32f * scale, 0.11f * scale, 0.18f * scale, 24, 10);
    drawRawSphere(x + 0.54f * scale, y - 0.03f * scale, z - 0.02f * scale, 0.26f * scale, 0.09f * scale, 0.15f * scale, 22, 8);
    drawRawSphere(x + 0.12f * scale, y + 0.15f * scale, z, 0.28f * scale, 0.08f * scale, 0.13f * scale, 22, 8);

    glDisable(GL_LIGHTING);
    glColor4f(0.08f, 0.95f, 1.0f, 0.58f);
    glLineWidth(2.1f);
    glBegin(GL_LINE_LOOP);
    for (int i = 0; i < 48; ++i) {
        float a = 2.0f * PI * static_cast<float>(i) / 48.0f;
        glVertex3f(x + std::cos(a) * 1.16f * scale, y, z + std::sin(a) * 0.30f * scale);
    }
    glEnd();
    glLineWidth(1.0f);
    glEnable(GL_LIGHTING);
}

void drawClouds() {
    float drift = std::sin(gFaceTime * 0.20f) * 0.32f;
    float z1 = loopDepth(-16.5f, gWorldTravel * 0.22f, 32.0f, 5.8f);
    float z2 = loopDepth(-22.0f, gWorldTravel * 0.22f, 32.0f, 5.8f);
    float z3 = loopDepth(-31.0f, gWorldTravel * 0.22f, 32.0f, 5.8f);
    float cloud1X = sideLaneX(-5.8f, z1, -26.0f, 5.8f, 3.8f) + drift;
    float cloud1Scale = approachScale(1.35f, z1, -26.0f, 5.8f, 0.62f);
    drawCloud(cloud1X, 8.2f, z1, cloud1Scale);
    float cloud2X = sideLaneX(4.7f, z2, -26.0f, 5.8f, 3.5f) - drift * 0.7f;
    float cloud2Scale = approachScale(1.05f, z2, -26.0f, 5.8f, 0.58f);
    drawCloud(cloud2X, 7.3f, z2, cloud2Scale);
    float cloud3X = sideLaneX(9.4f, z3, -31.0f, 5.8f, 2.9f) + drift * 0.5f;
    float cloud3Scale = approachScale(0.86f, z3, -31.0f, 5.8f, 0.52f);
    drawCloud(cloud3X, 8.8f, z3, cloud3Scale);
}

void drawBackgroundTower(float x, float z, float scale, float heightMultiplier,
                         const Color& body, const Color& trim) {
    float towerHeight = 3.8f * scale * heightMultiplier;
    float towerWidth = 0.86f * scale;
    drawCube(x, towerHeight * 0.50f, z, towerWidth, towerHeight, 0.78f * scale,
             body, 54.0f, 0.32f);
    drawCube(x + 0.26f * scale, towerHeight + 0.10f * scale, z + 0.02f * scale,
             0.34f * scale, 0.20f * scale, 0.72f * scale, trim, 42.0f, 0.22f);

    int rows = std::max(4, static_cast<int>(heightMultiplier * 6.0f));
    for (int row = 0; row < rows; ++row) {
        float y = (0.48f + static_cast<float>(row) * 0.46f) * scale;
        if (y > towerHeight - 0.24f * scale) {
            break;
        }
        Color light = row % 3 == 0 ? Color{0.95f, 0.78f, 0.20f} : Color{0.10f, 0.76f, 1.0f};
        setEmissionMaterial(light);
        drawRawSphere(x - 0.22f * scale, y, z + 0.42f * scale,
                      0.050f * scale, 0.085f * scale, 0.018f * scale, 12, 6);
        drawRawSphere(x + 0.02f * scale, y, z + 0.42f * scale,
                      0.050f * scale, 0.085f * scale, 0.018f * scale, 12, 6);
        drawRawSphere(x + 0.26f * scale, y, z + 0.42f * scale,
                      0.050f * scale, 0.085f * scale, 0.018f * scale, 12, 6);
    }

    drawCylinderBetween(makeVec(x, towerHeight + 0.18f * scale, z),
                        makeVec(x, towerHeight + 0.92f * scale, z),
                        0.030f * scale, trim, 22.0f, 0.18f);
    setEmissionMaterial({0.95f, 0.78f, 0.20f});
    drawRawSphere(x, towerHeight + 1.02f * scale, z, 0.085f * scale, 0.085f * scale, 0.085f * scale, 16, 8);
}

void drawBackgroundSkyline() {
    const float frontZ = 4.0f;
    const float farZ = -38.0f;
    float z1 = loopDepth(-24.0f, gWorldTravel * 0.15f, 44.0f, frontZ);
    float z2 = loopDepth(-33.0f, gWorldTravel * 0.15f, 44.0f, frontZ);
    float z3 = loopDepth(-29.0f, gWorldTravel * 0.15f, 44.0f, frontZ);
    float z4 = loopDepth(-20.5f, gWorldTravel * 0.15f, 44.0f, frontZ);
    float s1 = approachScale(1.0f, z1, farZ, frontZ, 0.58f);
    float s2 = approachScale(1.0f, z2, farZ, frontZ, 0.48f);
    float s3 = approachScale(1.0f, z3, farZ, frontZ, 0.52f);
    float s4 = approachScale(1.0f, z4, farZ, frontZ, 0.54f);
    drawBackgroundTower(sideLaneX(-8.2f, z1, farZ, frontZ, 7.2f), z1, 1.0f * s1, 1.40f,
                        {0.030f, 0.040f, 0.088f}, {0.12f, 0.18f, 0.31f});
    drawBackgroundTower(sideLaneX(-2.8f, z2, farZ, frontZ, 6.0f), z2, 0.92f * s2, 1.08f,
                        {0.045f, 0.055f, 0.118f}, {0.16f, 0.22f, 0.34f});
    drawBackgroundTower(sideLaneX(7.3f, z3, farZ, frontZ, 7.0f), z3, 0.96f * s3, 1.30f,
                        {0.026f, 0.048f, 0.096f}, {0.10f, 0.20f, 0.36f});
    drawBackgroundTower(sideLaneX(2.6f, z4, farZ, frontZ, 5.8f), z4, 0.78f * s4, 0.96f,
                        {0.040f, 0.032f, 0.082f}, {0.22f, 0.16f, 0.30f});
}

void drawTree(float x, float z, float scale, bool roundTop) {
    float trunkHeight = 1.72f * scale;
    drawCylinderBetween(makeVec(x, 0.0f, z), makeVec(x, trunkHeight, z), 0.13f * scale,
                        {0.025f, 0.035f, 0.070f}, 26.0f, 0.24f);

    if (roundTop) {
        setEmissionMaterial({0.95f, 0.15f, 0.78f});
        drawRawSphere(x, trunkHeight + 0.32f * scale, z, 0.34f * scale, 0.34f * scale, 0.34f * scale, 24, 12);
        drawTorus(x, trunkHeight + 0.32f * scale, z, 0.018f * scale, 0.54f * scale, 1.0f, 0.10f, 1.0f, {0.95f, 0.15f, 0.78f});
    } else {
        setEmissionMaterial({0.10f, 0.86f, 1.0f});
        drawRawSphere(x, trunkHeight + 0.38f * scale, z, 0.28f * scale, 0.28f * scale, 0.28f * scale, 24, 12);
        drawCylinderBetween(makeVec(x - 0.46f * scale, trunkHeight + 0.22f * scale, z),
                            makeVec(x + 0.46f * scale, trunkHeight + 0.22f * scale, z),
                            0.030f * scale, {0.10f, 0.86f, 1.0f}, 20.0f, 0.16f);
    }
}

void drawFarTrees() {
    const float frontZ = 6.6f;
    const float farZ = -24.0f;
    float z1 = loopDepth(-12.5f, gWorldTravel * 0.72f, 32.0f, frontZ);
    float z2 = loopDepth(-17.0f, gWorldTravel * 0.72f, 32.0f, frontZ);
    float z3 = loopDepth(-14.0f, gWorldTravel * 0.72f, 32.0f, frontZ);
    float z4 = loopDepth(-20.0f, gWorldTravel * 0.72f, 32.0f, frontZ);
    float z5 = loopDepth(-10.8f, gWorldTravel * 0.72f, 32.0f, frontZ);
    drawTree(sideLaneX(-8.7f, z1, farZ, frontZ, 4.6f), z1, approachScale(0.72f, z1, farZ, frontZ, 0.95f), false);
    drawTree(sideLaneX(-5.7f, z2, farZ, frontZ, 4.0f), z2, approachScale(0.48f, z2, farZ, frontZ, 1.05f), false);
    drawTree(sideLaneX(5.9f, z3, farZ, frontZ, 4.0f), z3, approachScale(0.58f, z3, farZ, frontZ, 1.00f), false);
    drawTree(sideLaneX(8.1f, z4, farZ, frontZ, 4.6f), z4, approachScale(0.43f, z4, farZ, frontZ, 1.10f), true);
    drawTree(sideLaneX(10.8f, z5, farZ, frontZ, 5.0f), z5, approachScale(0.88f, z5, farZ, frontZ, 0.85f), true);
}

void drawBuilding(float x, float z, float scale) {
    float towerHeight = 2.6f * scale;
    drawCube(x, towerHeight * 0.50f, z, 0.82f * scale, towerHeight, 0.70f * scale,
             {0.025f, 0.035f, 0.075f}, 52.0f, 0.34f);
    drawCube(x + 0.54f * scale, towerHeight * 0.38f, z - 0.16f * scale, 0.42f * scale, towerHeight * 0.76f, 0.54f * scale,
             {0.035f, 0.048f, 0.100f}, 48.0f, 0.30f);

    for (int row = 0; row < 5; ++row) {
        float y = (0.42f + static_cast<float>(row) * 0.38f) * scale;
        Color light = row % 2 == 0 ? Color{0.08f, 0.92f, 1.0f} : Color{0.95f, 0.15f, 0.78f};
        setEmissionMaterial(light);
        drawRawSphere(x - 0.23f * scale, y, z + 0.38f * scale, 0.045f * scale, 0.080f * scale, 0.018f * scale, 12, 6);
        drawRawSphere(x + 0.23f * scale, y, z + 0.38f * scale, 0.045f * scale, 0.080f * scale, 0.018f * scale, 12, 6);
    }

    drawCylinderBetween(makeVec(x, towerHeight + 0.02f * scale, z),
                        makeVec(x, towerHeight + 0.72f * scale, z),
                        0.035f * scale, {0.10f, 0.86f, 1.0f}, 20.0f, 0.20f);
    setEmissionMaterial({0.10f, 0.86f, 1.0f});
    drawRawSphere(x, towerHeight + 0.86f * scale, z, 0.10f * scale, 0.10f * scale, 0.10f * scale, 16, 8);
}

void drawFarBuildings() {
    const float frontZ = 6.4f;
    const float farZ = -27.0f;
    float z1 = loopDepth(-18.5f, gWorldTravel * 0.58f, 36.0f, frontZ);
    float z2 = loopDepth(-24.0f, gWorldTravel * 0.58f, 36.0f, frontZ);
    float z3 = loopDepth(-30.0f, gWorldTravel * 0.58f, 36.0f, frontZ);
    drawBuilding(sideLaneX(-5.8f, z1, farZ, frontZ, 5.2f), z1,
                 approachScale(0.78f, z1, farZ, frontZ, 1.35f));
    drawBuilding(sideLaneX(5.2f, z2, farZ, frontZ, 5.0f), z2,
                 approachScale(0.70f, z2, farZ, frontZ, 1.42f));
    drawBuilding(sideLaneX(9.0f, z3, farZ, frontZ, 5.3f), z3,
                 approachScale(0.62f, z3, farZ, frontZ, 1.50f));
}

void drawCharacterShadow() {
    glDisable(GL_LIGHTING);
    glDepthMask(GL_FALSE);
    float hoverScale = 1.0f;
    glColor4f(0.03f, 0.04f, 0.03f, 0.24f);
    glBegin(GL_TRIANGLE_FAN);
    glVertex3f(gCharacterX, 0.07f, gCharacterZ);
    for (int i = 0; i <= 80; ++i) {
        float angle = 2.0f * PI * static_cast<float>(i) / 80.0f;
        glVertex3f(gCharacterX + std::cos(angle) * 1.35f * hoverScale,
                   0.07f,
                   gCharacterZ + std::sin(angle) * 0.58f * hoverScale);
    }
    glEnd();
    glDepthMask(GL_TRUE);
    glEnable(GL_LIGHTING);
}

void drawPortfolioPanel(float x, float y, float z, float angleY, const Color& color,
                        float width, float height, float thickness) {
    setMaterial(color, 18.0f, 0.14f);
    glPushMatrix();
    glTranslatef(x, y, z);
    glRotatef(angleY, 0.0f, 1.0f, 0.0f);
    glScalef(width, height, thickness);
    glutSolidCube(1.0);
    glPopMatrix();
}

void drawPortfolio() {
    drawPortfolioPanel(0.0f, 1.82f, 0.62f, 0.0f, FOLDER_COVER, 0.66f, 0.72f, 0.055f);
    drawPortfolioPanel(0.0f, 1.82f, 0.655f, 0.0f, PAPER, 0.50f, 0.56f, 0.012f);
    drawPortfolioPanel(0.0f, 2.10f, 0.695f, 0.0f, FOLDER_COVER, 0.30f, 0.045f, 0.018f);

    glDisable(GL_LIGHTING);
    glColor3f(0.18f, 0.10f, 0.04f);
    glLineWidth(1.6f);
    glBegin(GL_LINES);
    glVertex3f(-0.18f, 1.98f, 0.666f);
    glVertex3f(0.18f, 1.98f, 0.666f);
    glVertex3f(-0.23f, 1.82f, 0.666f);
    glVertex3f(0.23f, 1.82f, 0.666f);
    glVertex3f(-0.15f, 1.66f, 0.666f);
    glVertex3f(0.15f, 1.66f, 0.666f);
    glEnd();
    glEnable(GL_LIGHTING);
}

void drawSmile() {
    float smilePulse = 0.5f + 0.5f * std::sin(gFaceTime * 2.0f);
    float smileRy = 0.105f + smilePulse * 0.025f;
    float smileY = 3.42f + smilePulse * 0.015f;

    glDisable(GL_LIGHTING);
    glColor3f(0.13f, 0.06f, 0.04f);
    glLineWidth(2.2f);
    glBegin(GL_LINE_STRIP);
    for (int i = 0; i <= 28; ++i) {
        float t = static_cast<float>(i) / 28.0f;
        float degrees = 205.0f + (335.0f - 205.0f) * t;
        float angle = degrees * PI / 180.0f;
        glVertex3f(std::cos(angle) * 0.18f, smileY + std::sin(angle) * smileRy, 0.64f);
    }
    glEnd();
    glEnable(GL_LIGHTING);
}

void drawFaceDetails() {
    float eyeOpen = 1.0f - gBlinkAmount * 0.92f;
    float eyeRy = 0.045f + 0.050f * eyeOpen;

    drawSphere(-0.20f, 3.66f, 0.61f, 0.115f, eyeRy, 0.022f, SHIRT, 22.0f, 0.12f);
    drawSphere(0.20f, 3.66f, 0.61f, 0.115f, eyeRy, 0.022f, SHIRT, 22.0f, 0.12f);
    drawSphere(-0.20f, 3.66f, 0.64f, 0.034f, eyeRy * 0.42f, 0.010f, BLACK, 28.0f, 0.10f);
    drawSphere(0.20f, 3.66f, 0.64f, 0.034f, eyeRy * 0.42f, 0.010f, BLACK, 28.0f, 0.10f);

    if (gBlinkAmount > 0.65f) {
        drawCylinderBetween(makeVec(-0.30f, 3.66f, 0.65f), makeVec(0.30f, 3.66f, 0.65f), 0.010f, BLACK, 16.0f, 0.06f);
    }

    drawTorus(-0.20f, 3.66f, 0.642f, 0.010f, 0.125f, 1.25f, 0.66f, 0.10f, BLACK);
    drawTorus(0.20f, 3.66f, 0.642f, 0.010f, 0.125f, 1.25f, 0.66f, 0.10f, BLACK);
    drawCylinderBetween(makeVec(-0.075f, 3.66f, 0.64f), makeVec(0.075f, 3.66f, 0.64f), 0.010f, BLACK, 20.0f, 0.08f);
    drawCylinderBetween(makeVec(-0.39f, 3.67f, 0.61f), makeVec(-0.54f, 3.66f, 0.36f), 0.010f, BLACK, 20.0f, 0.08f);
    drawCylinderBetween(makeVec(0.39f, 3.67f, 0.61f), makeVec(0.54f, 3.66f, 0.36f), 0.010f, BLACK, 20.0f, 0.08f);

    drawCylinderBetween(makeVec(-0.34f, 3.86f, 0.60f), makeVec(-0.10f, 3.90f, 0.60f), 0.014f, HAIR, 16.0f, 0.08f);
    drawCylinderBetween(makeVec(0.10f, 3.90f, 0.60f), makeVec(0.34f, 3.86f, 0.60f), 0.014f, HAIR, 16.0f, 0.08f);

    drawSphere(0.0f, 3.52f, 0.64f, 0.085f, 0.075f, 0.075f, SKIN_LIGHT, 22.0f, 0.10f);
    setMaterial(CHEEK, 18.0f, 0.08f);
    drawRawSphere(-0.38f, 3.45f, 0.58f, 0.070f, 0.028f, 0.018f, 12, 6);
    drawRawSphere(0.38f, 3.45f, 0.58f, 0.070f, 0.028f, 0.018f, 12, 6);
    drawSmile();
}

void drawHair() {
    drawSphere(0.0f, 4.05f, -0.03f, 0.60f, 0.22f, 0.56f, HAIR, 34.0f, 0.22f);
    drawSphere(-0.22f, 4.10f, 0.18f, 0.30f, 0.14f, 0.28f, HAIR_HIGHLIGHT, 32.0f, 0.20f);
    drawConeBetween(makeVec(-0.34f, 4.08f, 0.22f), makeVec(-0.58f, 4.24f, 0.62f), 0.13f, HAIR_HIGHLIGHT, 24.0f, 0.14f);
    drawConeBetween(makeVec(0.04f, 4.14f, 0.28f), makeVec(0.00f, 4.36f, 0.68f), 0.16f, HAIR, 24.0f, 0.14f);
    drawConeBetween(makeVec(0.32f, 4.08f, 0.22f), makeVec(0.58f, 4.20f, 0.60f), 0.13f, HAIR_HIGHLIGHT, 24.0f, 0.14f);
}

void drawEarsAndSideburns() {
    drawSphere(-0.62f, 3.58f, 0.02f, 0.095f, 0.180f, 0.120f, SKIN, 24.0f, 0.12f);
    drawSphere(0.62f, 3.58f, 0.02f, 0.095f, 0.180f, 0.120f, SKIN, 24.0f, 0.12f);
    drawSphere(-0.63f, 3.57f, 0.05f, 0.042f, 0.096f, 0.044f, SKIN_LIGHT, 18.0f, 0.08f);
    drawSphere(0.63f, 3.57f, 0.05f, 0.042f, 0.096f, 0.044f, SKIN_LIGHT, 18.0f, 0.08f);
    drawCylinderBetween(makeVec(-0.53f, 3.86f, 0.08f), makeVec(-0.58f, 3.48f, 0.08f), 0.035f, HAIR, 18.0f, 0.08f);
    drawCylinderBetween(makeVec(0.53f, 3.86f, 0.08f), makeVec(0.58f, 3.48f, 0.08f), 0.035f, HAIR, 18.0f, 0.08f);
}

void drawTorso() {
    drawCylinderBetween(makeVec(0.0f, 1.24f, 0.0f), makeVec(0.0f, 2.58f, 0.0f), 0.44f, SUIT, 48.0f, 0.28f);
    drawSphere(0.0f, 2.62f, 0.0f, 0.80f, 0.16f, 0.38f, SUIT_LIGHT, 42.0f, 0.24f);
    drawSphere(0.0f, 1.24f, 0.0f, 0.40f, 0.13f, 0.33f, SUIT, 38.0f, 0.22f);

    drawCube(0.0f, 1.95f, 0.52f, 0.34f, 1.02f, 0.045f, SHIRT, 24.0f, 0.10f);
    drawRotatedCube(-0.26f, 2.12f, 0.55f, 0.105f, 0.76f, 0.050f, SUIT_EDGE, -20.0f, 0.0f, 30.0f, 0.16f);
    drawRotatedCube(0.26f, 2.12f, 0.55f, 0.105f, 0.76f, 0.050f, SUIT_EDGE, 20.0f, 0.0f, 30.0f, 0.16f);
    drawRotatedCube(-0.13f, 2.48f, 0.58f, 0.20f, 0.080f, 0.045f, SHIRT, 24.0f, 0.0f, 20.0f, 0.10f);
    drawRotatedCube(0.13f, 2.48f, 0.58f, 0.20f, 0.080f, 0.045f, SHIRT, -24.0f, 0.0f, 20.0f, 0.10f);

    drawSphere(0.0f, 2.35f, 0.62f, 0.105f, 0.080f, 0.040f, TIE, 22.0f, 0.10f);
    drawConeBetween(makeVec(0.0f, 2.25f, 0.61f), makeVec(0.0f, 1.42f, 0.62f), 0.115f, TIE, 24.0f, 0.12f);
    drawCylinderBetween(makeVec(0.0f, 2.16f, 0.66f), makeVec(0.0f, 1.52f, 0.66f), 0.020f, TIE_DARK, 18.0f, 0.08f);

    for (int i = 0; i < 3; ++i) {
        drawSphere(0.0f, 1.72f + static_cast<float>(i) * 0.22f, 0.67f, 0.028f, 0.028f, 0.012f, BLACK, 18.0f, 0.06f);
    }
}

void drawArmsAndHands() {
    float walkMotion = gIsRunning ? 1.0f : 0.0f;
    float swing = std::sin(gRunPhase) * walkMotion;
    float lift = std::fabs(std::sin(gRunPhase)) * walkMotion;
    float leftHandY = 2.12f + lift * 0.06f;
    float rightHandY = 1.50f + (1.0f - lift) * 0.04f * walkMotion;
    float leftHandZ = 0.64f - swing * 0.08f;
    float rightHandZ = 0.76f + swing * 0.08f;

    Vec3 leftShoulder = makeVec(-0.78f, 2.52f, 0.02f);
    Vec3 leftElbow = makeVec(-1.10f, 2.32f, 0.22f);
    Vec3 leftHand = makeVec(-0.92f, leftHandY, leftHandZ);
    Vec3 rightShoulder = makeVec(0.78f, 2.52f, 0.02f);
    Vec3 rightElbow = makeVec(1.08f, 2.02f, 0.26f);
    Vec3 rightHand = makeVec(0.64f, rightHandY, rightHandZ);

    drawAngularJoint(leftShoulder.x, leftShoulder.y, leftShoulder.z, 0.28f, 0.20f, 0.22f, SUIT_LIGHT, -18.0f, 10.0f);
    drawAngularJoint(rightShoulder.x, rightShoulder.y, rightShoulder.z, 0.28f, 0.20f, 0.22f, SUIT_LIGHT, 18.0f, -10.0f);

    drawCylinderBetween(leftShoulder, leftElbow, 0.090f, SUIT, 38.0f, 0.24f);
    drawCylinderBetween(leftElbow, leftHand, 0.078f, SUIT, 38.0f, 0.24f);
    drawCylinderBetween(rightShoulder, rightElbow, 0.090f, SUIT, 38.0f, 0.24f);
    drawCylinderBetween(rightElbow, rightHand, 0.078f, SUIT, 38.0f, 0.24f);

    drawAngularJoint(leftElbow.x, leftElbow.y, leftElbow.z, 0.24f, 0.16f, 0.18f, SUIT_EDGE, 36.0f, -14.0f);
    drawAngularJoint(rightElbow.x, rightElbow.y, rightElbow.z, 0.24f, 0.16f, 0.18f, SUIT_EDGE, -36.0f, 14.0f);
    drawCylinderBetween(makeVec(leftHand.x - 0.11f, leftHand.y + 0.06f, leftHand.z + 0.03f),
                        makeVec(leftHand.x + 0.11f, leftHand.y + 0.06f, leftHand.z + 0.03f),
                        0.030f, SHIRT, 18.0f, 0.08f);
    drawCylinderBetween(makeVec(rightHand.x - 0.11f, rightHand.y + 0.06f, rightHand.z + 0.03f),
                        makeVec(rightHand.x + 0.11f, rightHand.y + 0.06f, rightHand.z + 0.03f),
                        0.030f, SHIRT, 18.0f, 0.08f);
    drawTorus(rightHand.x - 0.02f, rightHand.y + 0.08f, rightHand.z + 0.07f, 0.010f, 0.075f, 1.0f, 0.62f, 0.16f, WATCH_METAL);
    drawSphere(rightHand.x - 0.02f, rightHand.y + 0.08f, rightHand.z + 0.13f, 0.035f, 0.024f, 0.010f, WATCH_FACE, 16.0f, 0.08f);
    drawSphere(leftHand.x, leftHand.y, leftHand.z + 0.05f, 0.125f, 0.100f, 0.100f, SKIN, 26.0f, 0.16f);
    drawSphere(rightHand.x, rightHand.y, rightHand.z + 0.05f, 0.125f, 0.100f, 0.100f, SKIN, 26.0f, 0.16f);
}

void drawLegsAndShoes() {
    float walkMotion = gIsRunning ? 1.0f : 0.0f;
    float swing = std::sin(gRunPhase) * walkMotion;
    float oppositeSwing = std::sin(gRunPhase + PI) * walkMotion;
    float leftLift = std::max(0.0f, swing) * 0.12f;
    float rightLift = std::max(0.0f, oppositeSwing) * 0.12f;
    float boardStance = 0.16f;
    Vec3 leftHip = makeVec(-0.28f - boardStance * 0.35f, 1.36f, 0.0f);
    Vec3 rightHip = makeVec(0.28f + boardStance * 0.35f, 1.36f, 0.0f);
    Vec3 leftKnee = makeVec(-0.58f - boardStance, 0.86f + leftLift, 0.18f + swing * 0.06f);
    Vec3 rightKnee = makeVec(0.58f + boardStance, 0.82f + rightLift, -0.02f + oppositeSwing * 0.06f);
    Vec3 leftAnkle = makeVec(-0.52f - boardStance, 0.30f + leftLift, 0.50f + swing * 0.15f);
    Vec3 rightAnkle = makeVec(0.54f + boardStance, 0.30f + rightLift, 0.18f + oppositeSwing * 0.15f);

    drawCylinderBetween(leftHip, leftKnee, 0.120f, TROUSERS, 32.0f, 0.22f);
    drawCylinderBetween(leftKnee, leftAnkle, 0.110f, TROUSERS, 32.0f, 0.22f);
    drawCylinderBetween(rightHip, rightKnee, 0.120f, TROUSERS, 32.0f, 0.22f);
    drawCylinderBetween(rightKnee, rightAnkle, 0.110f, TROUSERS, 32.0f, 0.22f);
    drawAngularJoint(leftHip.x, leftHip.y, leftHip.z, 0.24f, 0.16f, 0.18f, TROUSERS, -12.0f, 8.0f);
    drawAngularJoint(rightHip.x, rightHip.y, rightHip.z, 0.24f, 0.16f, 0.18f, TROUSERS, 12.0f, -8.0f);
    drawAngularJoint(leftKnee.x, leftKnee.y, leftKnee.z, 0.23f, 0.15f, 0.18f, SUIT_EDGE, -32.0f, 16.0f);
    drawAngularJoint(rightKnee.x, rightKnee.y, rightKnee.z, 0.23f, 0.15f, 0.18f, SUIT_EDGE, 32.0f, -16.0f);
    setMaterial(SHIRT, 16.0f, 0.08f);
    drawRawSphere(leftAnkle.x, leftAnkle.y, leftAnkle.z + 0.12f, 0.065f, 0.035f, 0.024f, 16, 8);
    drawRawSphere(rightAnkle.x, rightAnkle.y, rightAnkle.z + 0.12f, 0.065f, 0.035f, 0.024f, 16, 8);

    drawSphere(leftAnkle.x, 0.18f + leftLift, leftAnkle.z, 0.39f, 0.085f, 0.32f, SHOES, 42.0f, 0.30f);
    drawSphere(rightAnkle.x, 0.18f + rightLift, rightAnkle.z, 0.39f, 0.085f, 0.32f, SHOES, 42.0f, 0.30f);
    drawCube(leftAnkle.x, 0.11f + leftLift, leftAnkle.z + 0.24f, 0.45f, 0.035f, 0.065f, {0.18f, 0.12f, 0.06f}, 24.0f, 0.14f);
    drawCube(rightAnkle.x, 0.11f + rightLift, rightAnkle.z + 0.24f, 0.45f, 0.035f, 0.065f, {0.18f, 0.12f, 0.06f}, 24.0f, 0.14f);
}

void drawHeadAndNeck() {
    drawCylinderBetween(makeVec(0.0f, 2.58f, 0.0f), makeVec(0.0f, 3.02f, 0.0f), 0.14f, SKIN, 30.0f, 0.18f);
    drawSphere(0.0f, 3.55f, 0.0f, 0.60f, 0.62f, 0.54f, SKIN, 40.0f, 0.20f);
    drawHair();
    drawFaceDetails();
    drawEarsAndSideburns();
}

void drawCharacter() {
    // The main cartoon character is drawn large and close to the camera to represent the near plane.
    // The character is built from 3D OpenGL primitives: spheres, cylinders, cones, scaled cubes, and line strips.
    float walkBounce = gIsRunning ? std::fabs(std::sin(gRunPhase * 2.0f)) * 0.035f : 0.0f;

    glPushMatrix();
    glTranslatef(gCharacterX, walkBounce, gCharacterZ);
    glRotatef(gCharacterYaw, 0.0f, 1.0f, 0.0f);

    drawLegsAndShoes();
    drawTorso();
    drawArmsAndHands();
    drawPortfolio();
    drawHeadAndNeck();

    glPopMatrix();
}

void setupLights() {
    GLfloat globalAmbient[] = {0.18f, 0.18f, 0.22f, 1.0f};
    GLfloat lightPosition[] = {-3.2f, 7.6f, 5.8f, 1.0f};
    GLfloat lightDiffuse[] = {0.64f, 0.82f, 1.0f, 1.0f};
    GLfloat lightSpecular[] = {0.90f, 0.95f, 1.0f, 1.0f};
    GLfloat magentaPosition[] = {4.6f, 3.4f, 2.2f, 1.0f};
    GLfloat magentaDiffuse[] = {0.52f, 0.10f, 0.34f, 1.0f};
    GLfloat magentaSpecular[] = {0.45f, 0.18f, 0.38f, 1.0f};
    GLfloat warmPosition[] = {0.0f, 4.2f, 7.2f, 1.0f};
    GLfloat warmDiffuse[] = {0.66f, 0.48f, 0.34f, 1.0f};
    GLfloat warmSpecular[] = {0.40f, 0.30f, 0.22f, 1.0f};

    glLightModelfv(GL_LIGHT_MODEL_AMBIENT, globalAmbient);
    glLightfv(GL_LIGHT0, GL_POSITION, lightPosition);
    glLightfv(GL_LIGHT0, GL_DIFFUSE, lightDiffuse);
    glLightfv(GL_LIGHT0, GL_SPECULAR, lightSpecular);
    glLightfv(GL_LIGHT1, GL_POSITION, magentaPosition);
    glLightfv(GL_LIGHT1, GL_DIFFUSE, magentaDiffuse);
    glLightfv(GL_LIGHT1, GL_SPECULAR, magentaSpecular);
    glLightfv(GL_LIGHT2, GL_POSITION, warmPosition);
    glLightfv(GL_LIGHT2, GL_DIFFUSE, warmDiffuse);
    glLightfv(GL_LIGHT2, GL_SPECULAR, warmSpecular);
}

void drawBitmapText(float x, float y, const char* text) {
    glRasterPos2f(x, y);
    for (const char* ch = text; *ch != '\0'; ++ch) {
        glutBitmapCharacter(GLUT_BITMAP_HELVETICA_18, *ch);
    }
}

void drawInstructionOverlay() {
    if (!gShowInstructions) {
        return;
    }

    glDisable(GL_LIGHTING);
    glDisable(GL_DEPTH_TEST);
    glMatrixMode(GL_PROJECTION);
    glPushMatrix();
    glLoadIdentity();
    gluOrtho2D(0.0, static_cast<double>(gWindowWidth), 0.0, static_cast<double>(gWindowHeight));
    glMatrixMode(GL_MODELVIEW);
    glPushMatrix();
    glLoadIdentity();

    glColor4f(0.0f, 0.0f, 0.0f, 0.54f);
    glBegin(GL_QUADS);
    glVertex2f(24.0f, static_cast<float>(gWindowHeight) - 94.0f);
    glVertex2f(470.0f, static_cast<float>(gWindowHeight) - 94.0f);
    glVertex2f(470.0f, static_cast<float>(gWindowHeight) - 24.0f);
    glVertex2f(24.0f, static_cast<float>(gWindowHeight) - 24.0f);
    glEnd();

    glColor3f(0.96f, 0.98f, 1.0f);
    drawBitmapText(42.0f, static_cast<float>(gWindowHeight) - 50.0f, "Press arrow keys to orbit the camera");
    glColor3f(0.74f, 0.90f, 1.0f);
    drawBitmapText(42.0f, static_cast<float>(gWindowHeight) - 76.0f, "Up/Down move closer or farther. A/D also orbit.");

    glPopMatrix();
    glMatrixMode(GL_PROJECTION);
    glPopMatrix();
    glMatrixMode(GL_MODELVIEW);
    glEnable(GL_DEPTH_TEST);
    glEnable(GL_LIGHTING);
}

void display() {
    glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
    glMatrixMode(GL_MODELVIEW);
    glLoadIdentity();

    // Perspective camera: keyboard controls move the viewer around the fixed near-plane character.
    float orbitRadians = gCameraOrbit * PI / 180.0f;
    float targetX = gCharacterX;
    float targetY = 2.38f;
    float targetZ = gCharacterZ - 0.80f;
    float cameraX = targetX + std::sin(orbitRadians) * gCameraDistance;
    float cameraZ = targetZ + std::cos(orbitRadians) * gCameraDistance;

    gluLookAt(cameraX, gCameraHeight, cameraZ,
              targetX, targetY, targetZ,
              0.0, 1.0, 0.0);
    setupLights();

    // Far plane: drawn smaller, lighter, higher, and behind the foreground character.
    drawSky();
    drawSun();
    drawClouds();
    drawBackgroundSkyline();
    drawFarBuildings();
    drawFarTrees();

    // Mid plane: ground and path support the depth illusion.
    drawGround();
    drawPath();

    // Near plane: the large connected 3D character is drawn last in the foreground.
    drawCharacterShadow();
    drawCharacter();
    drawInstructionOverlay();

    glutSwapBuffers();
}

void reshape(int width, int height) {
    if (height == 0) {
        height = 1;
    }

    gWindowWidth = width;
    gWindowHeight = height;
    float aspect = static_cast<float>(width) / static_cast<float>(height);

    glViewport(0, 0, width, height);
    glMatrixMode(GL_PROJECTION);
    glLoadIdentity();

    // The near and far clipping planes are explicit here.
    // The character sits close to the near plane; tall buildings, clouds, trees, and sun sit toward the far plane.
    gluPerspective(48.0, aspect, 1.0, 80.0);

    glMatrixMode(GL_MODELVIEW);
    glLoadIdentity();
}

void saveScreenshot() {
    std::vector<unsigned char> pixels(static_cast<size_t>(gWindowWidth) * static_cast<size_t>(gWindowHeight) * 3);
    glPixelStorei(GL_PACK_ALIGNMENT, 1);
    glReadBuffer(GL_FRONT);
    glReadPixels(0, 0, gWindowWidth, gWindowHeight, GL_RGB, GL_UNSIGNED_BYTE, pixels.data());

    FILE* file = std::fopen("part_b_3d_cartoon_scene.ppm", "wb");
    if (!file) {
        std::printf("Could not save screenshot.\n");
        return;
    }

    std::fprintf(file, "P6\n%d %d\n255\n", gWindowWidth, gWindowHeight);
    for (int y = gWindowHeight - 1; y >= 0; --y) {
        const unsigned char* row = pixels.data() + static_cast<size_t>(y) * static_cast<size_t>(gWindowWidth) * 3;
        std::fwrite(row, 1, static_cast<size_t>(gWindowWidth) * 3, file);
    }
    std::fclose(file);
    std::printf("Saved screenshot as part_b_3d_cartoon_scene.ppm\n");
}

void keyboard(unsigned char key, int, int) {
    gKeyDown[key] = true;

    if (key == 'w' || key == 'W' || key == 's' || key == 'S' ||
        key == 'a' || key == 'A' || key == 'd' || key == 'D' ||
        key == 'e' || key == 'E' || key == 'c' || key == 'C') {
        gShowInstructions = false;
    }
    if (key == 27 || key == 'q' || key == 'Q') {
        std::exit(0);
    }
    if (key == 'p' || key == 'P') {
        saveScreenshot();
    }
    if (key == 'r' || key == 'R') {
        gCharacterX = 0.0f;
        gCharacterZ = CHARACTER_HOME_Z;
        gCharacterYaw = 0.0f;
        gRunPhase = 0.0f;
        gIsRunning = false;
        gWorldTravel = 0.0f;
        gCameraOrbit = DEFAULT_CAMERA_ORBIT;
        gCameraDistance = DEFAULT_CAMERA_DISTANCE;
        gCameraHeight = DEFAULT_CAMERA_HEIGHT;
    }
    glutPostRedisplay();
}

void keyboardUp(unsigned char key, int, int) {
    gKeyDown[key] = false;
}

void specialKeyboard(int key, int, int) {
    if (key >= 0 && key < 512) {
        gSpecialKeyDown[key] = true;
    }
    if (key == GLUT_KEY_UP || key == GLUT_KEY_DOWN || key == GLUT_KEY_LEFT || key == GLUT_KEY_RIGHT) {
        gShowInstructions = false;
    }
}

void specialKeyboardUp(int key, int, int) {
    if (key >= 0 && key < 512) {
        gSpecialKeyDown[key] = false;
    }
}

void updateAnimation(int) {
    const float dt = 0.016f;
    const float orbitSpeed = 88.0f;
    const float dollySpeed = 4.4f;
    const float heightSpeed = 2.0f;
    const float returnEase = 0.055f;

    gFaceTime += dt;
    float blinkCycle = std::fmod(gFaceTime, 4.2f);
    float blinkPulse = blinkCycle < 0.16f ? std::sin(PI * blinkCycle / 0.16f) : 0.0f;
    float secondBlinkCycle = std::fmod(gFaceTime + 1.8f, 7.1f);
    float secondBlinkPulse = secondBlinkCycle < 0.12f ? std::sin(PI * secondBlinkCycle / 0.12f) : 0.0f;
    gBlinkAmount = std::max(blinkPulse, secondBlinkPulse);

    bool forward = gKeyDown['w'] || gKeyDown['W'] || gSpecialKeyDown[GLUT_KEY_UP];
    bool backward = gKeyDown['s'] || gKeyDown['S'] || gSpecialKeyDown[GLUT_KEY_DOWN];
    bool turnLeft = gKeyDown['a'] || gKeyDown['A'] || gSpecialKeyDown[GLUT_KEY_LEFT];
    bool turnRight = gKeyDown['d'] || gKeyDown['D'] || gSpecialKeyDown[GLUT_KEY_RIGHT];
    float sideMovement = 0.0f;
    if (turnLeft) {
        sideMovement -= 1.0f;
    }
    if (turnRight) {
        sideMovement += 1.0f;
    }

    float depthMovement = 0.0f;
    if (forward) {
        depthMovement -= 1.0f;
    }
    if (backward) {
        depthMovement += 1.0f;
    }

    if (std::fabs(sideMovement) > 0.001f) {
        gCameraOrbit += sideMovement * orbitSpeed * dt;
    }
    if (std::fabs(depthMovement) > 0.001f) {
        gCameraDistance += depthMovement * dollySpeed * dt;
        gCameraDistance = clampValue(gCameraDistance, 4.8f, 18.0f);
    }
    if (gKeyDown['e'] || gKeyDown['E']) {
        gCameraHeight += heightSpeed * dt;
    }
    if (gKeyDown['c'] || gKeyDown['C']) {
        gCameraHeight -= heightSpeed * dt;
    }
    gCameraHeight = clampValue(gCameraHeight, 1.4f, 7.2f);

    // The character remains fixed while the viewer orbits, dollies, and changes height around it.
    gIsRunning = false;
    gRunPhase *= 0.90f;
    gCharacterX += (0.0f - gCharacterX) * returnEase;
    gCharacterZ += (CHARACTER_HOME_Z - gCharacterZ) * returnEase;
    gCharacterYaw = 0.0f;
    gWorldTravel = 0.0f;

    glutPostRedisplay();
    glutTimerFunc(16, updateAnimation, 0);
}

void init() {
    glClearColor(0.045f, 0.055f, 0.120f, 1.0f);

    // 3D rendering settings required for the improved Part B scene.
    glEnable(GL_DEPTH_TEST);
    glDepthFunc(GL_LEQUAL);
    glEnable(GL_LIGHTING);
    glEnable(GL_LIGHT0);
    glEnable(GL_LIGHT1);
    glEnable(GL_LIGHT2);
    glEnable(GL_COLOR_MATERIAL);
    glColorMaterial(GL_FRONT_AND_BACK, GL_AMBIENT_AND_DIFFUSE);
    glShadeModel(GL_SMOOTH);
    glEnable(GL_NORMALIZE);
    glEnable(GL_BLEND);
    glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);
    glEnable(GL_LINE_SMOOTH);
    glHint(GL_LINE_SMOOTH_HINT, GL_NICEST);

    gQuadric = gluNewQuadric();
    gluQuadricNormals(gQuadric, GLU_SMOOTH);
}

int main(int argc, char** argv) {
    glutInit(&argc, argv);
    glutInitDisplayMode(GLUT_DOUBLE | GLUT_RGB | GLUT_DEPTH | GLUT_MULTISAMPLE);
    glutInitWindowSize(gWindowWidth, gWindowHeight);
    glutInitWindowPosition(90, 40);
    glutCreateWindow("Part B - Formal Cartoon Character Camera Orbit");
    glutIgnoreKeyRepeat(1);

    init();
    glutDisplayFunc(display);
    glutReshapeFunc(reshape);
    glutKeyboardFunc(keyboard);
    glutKeyboardUpFunc(keyboardUp);
    glutSpecialFunc(specialKeyboard);
    glutSpecialUpFunc(specialKeyboardUp);
    glutTimerFunc(16, updateAnimation, 0);
    glutMainLoop();
    return 0;
}
