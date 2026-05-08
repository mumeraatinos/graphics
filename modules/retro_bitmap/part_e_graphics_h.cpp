#include <graphics.h>
#include <conio.h>
#include <cstring>

// Great Zimbabwe University
// Computer Graphics Practical Assignment - Part E
// Image creation using the graphics.h library in C/C++.

const int WINDOW_WIDTH = 1180;
const int WINDOW_HEIGHT = 820;
const char* REFERENCE_OUTPUT_IMAGE = "part_e_all_primitives.bmp";
const int FOOTER_TOP = WINDOW_HEIGHT - 38;

void drawText(int x, int y, const char* text) {
    outtextxy(x, y, const_cast<char*>(text));
}

void drawCenteredText(int left, int right, int y, const char* text) {
    int width = textwidth(const_cast<char*>(text));
    drawText(left + (right - left - width) / 2, y, text);
}

void drawClosedPolygon(const int points[], int count) {
    for (int i = 0; i < count; ++i) {
        int next = (i + 1) % count;
        line(points[i * 2], points[i * 2 + 1],
             points[next * 2], points[next * 2 + 1]);
    }
}

void fillClosedPolygon(const int points[], int count, int fillColor, int borderColor) {
    int localPoints[64];
    for (int i = 0; i < count * 2; ++i) {
        localPoints[i] = points[i];
    }

    setfillstyle(SOLID_FILL, fillColor);
    fillpoly(count, localPoints);
    setcolor(borderColor);
    drawClosedPolygon(points, count);
}

void drawReferenceLabel(int x, int y, const char* text) {
    settextstyle(DEFAULT_FONT, HORIZ_DIR, 2);
    drawText(x, y, text);
    settextstyle(DEFAULT_FONT, HORIZ_DIR, 1);
}

void drawReferenceTitle(int left, int right, int y, const char* text) {
    settextstyle(DEFAULT_FONT, HORIZ_DIR, 2);
    drawCenteredText(left, right, y, text);
    settextstyle(DEFAULT_FONT, HORIZ_DIR, 1);
}

struct ReferencePoint {
    float x;
    float y;
};

struct ReferenceLabelOffset {
    float dx;
    float dy;
};

const float REFERENCE_LEFT = 60.0f;
const float REFERENCE_TOP = 76.0f;
const float REFERENCE_X_SCALE = 10.5f;
const float REFERENCE_Y_SCALE = 6.0f;

int refX(float x) {
    return static_cast<int>(REFERENCE_LEFT + x * REFERENCE_X_SCALE + 0.5f);
}

int refY(float y) {
    return static_cast<int>(REFERENCE_TOP + (100.0f - y) * REFERENCE_Y_SCALE + 0.5f);
}

void drawReferenceWorldText(float x, float y, const char* text, bool bold = false) {
    settextstyle(DEFAULT_FONT, HORIZ_DIR, bold ? 2 : 1);
    drawText(refX(x), refY(y), text);
    settextstyle(DEFAULT_FONT, HORIZ_DIR, 1);
}

void drawReferenceWorldLine(ReferencePoint a, ReferencePoint b) {
    line(refX(a.x), refY(a.y), refX(b.x), refY(b.y));
}

void drawReferenceWorldPolygon(const ReferencePoint points[], int count) {
    for (int i = 0; i < count; ++i) {
        drawReferenceWorldLine(points[i], points[(i + 1) % count]);
    }
}

void drawReferenceWorldTriangle(ReferencePoint a, ReferencePoint b, ReferencePoint c) {
    drawReferenceWorldLine(a, b);
    drawReferenceWorldLine(b, c);
    drawReferenceWorldLine(c, a);
}

void drawReferenceWorldPoint(ReferencePoint point, int radius) {
    setfillstyle(SOLID_FILL, BLACK);
    setcolor(BLACK);
    fillellipse(refX(point.x), refY(point.y), radius, radius);
}

void drawReferenceMarkers(const ReferencePoint points[], const ReferenceLabelOffset offsets[], int count) {
    for (int i = 0; i < count; ++i) {
        drawReferenceWorldPoint(points[i], 3);

        char label[8];
        label[0] = 'v';
        label[1] = static_cast<char>('0' + i);
        label[2] = '\0';
        drawReferenceWorldText(points[i].x + offsets[i].dx, points[i].y + offsets[i].dy, label);
    }
}

void drawReferencePoint(int x, int y, const char* label, int labelX, int labelY) {
    setcolor(BLACK);
    setfillstyle(SOLID_FILL, BLACK);
    fillellipse(x, y, 5, 5);
    drawReferenceLabel(labelX, labelY, label);
}

void drawCloud(int x, int y, int scale) {
    setfillstyle(SOLID_FILL, WHITE);
    setcolor(LIGHTGRAY);
    fillellipse(x - 22 * scale, y + 7 * scale, 24 * scale, 13 * scale);
    fillellipse(x, y, 30 * scale, 17 * scale);
    fillellipse(x + 28 * scale, y + 8 * scale, 25 * scale, 13 * scale);
}

void drawTree(int x, int y, int scale) {
    setfillstyle(SOLID_FILL, BROWN);
    bar(x - 4 * scale, y - 25 * scale, x + 4 * scale, y);

    setfillstyle(SOLID_FILL, GREEN);
    setcolor(GREEN);
    fillellipse(x, y - 38 * scale, 18 * scale, 22 * scale);
    fillellipse(x - 14 * scale, y - 25 * scale, 15 * scale, 17 * scale);
    fillellipse(x + 14 * scale, y - 25 * scale, 15 * scale, 17 * scale);
}

void drawPanelFrame(int x, int y, int width, int height, const char* title) {
    setfillstyle(SOLID_FILL, WHITE);
    bar(x, y, x + width, y + height);

    setcolor(BLACK);
    rectangle(x, y, x + width, y + height);

    setfillstyle(SOLID_FILL, LIGHTGRAY);
    bar(x + 1, y + 1, x + width - 1, y + 24);
    setcolor(BLACK);
    rectangle(x + 1, y + 1, x + width - 1, y + 24);
    drawCenteredText(x, x + width, y + 8, title);
}

void drawLandscapeImage(int x, int y, int width, int height) {
    drawPanelFrame(x, y, width, height, "Image 1: Landscape");

    int top = y + 25;
    int bottom = y + height - 1;
    int ground = y + height - 86;

    setfillstyle(SOLID_FILL, LIGHTCYAN);
    bar(x + 1, top, x + width - 1, ground);
    setfillstyle(SOLID_FILL, LIGHTGREEN);
    bar(x + 1, ground, x + width - 1, bottom);

    setfillstyle(SOLID_FILL, YELLOW);
    setcolor(YELLOW);
    fillellipse(x + width - 85, top + 44, 31, 31);
    setcolor(YELLOW);
    for (int i = 0; i < 12; ++i) {
        int dx = (i % 4 - 1) * 20;
        int dy = (i / 4 - 1) * 18;
        line(x + width - 85, top + 44, x + width - 85 + dx, top + 44 + dy);
    }

    drawCloud(x + 112, top + 56, 1);
    drawCloud(x + width - 205, top + 78, 1);

    int mountain1[] = {
        x + 40, ground,
        x + 165, top + 55,
        x + 292, ground
    };
    int mountain2[] = {
        x + 225, ground,
        x + 364, top + 68,
        x + width - 40, ground
    };
    fillClosedPolygon(mountain1, 3, DARKGRAY, BLACK);
    fillClosedPolygon(mountain2, 3, LIGHTGRAY, BLACK);

    int snow1[] = {
        x + 135, top + 91,
        x + 165, top + 55,
        x + 194, top + 92
    };
    int snow2[] = {
        x + 331, top + 102,
        x + 364, top + 68,
        x + 397, top + 102
    };
    fillClosedPolygon(snow1, 3, WHITE, WHITE);
    fillClosedPolygon(snow2, 3, WHITE, WHITE);

    int river[] = {
        x + 240, ground,
        x + 312, ground,
        x + 365, bottom,
        x + 159, bottom
    };
    fillClosedPolygon(river, 4, CYAN, BLUE);

    drawTree(x + 70, bottom - 14, 2);
    drawTree(x + width - 86, bottom - 10, 2);
    drawTree(x + width - 150, bottom - 20, 1);

    setcolor(BLACK);
    arc(x + width - 160, top + 64, 20, 160, 14);
    arc(x + width - 130, top + 60, 20, 160, 14);
}

void drawCityImage(int x, int y, int width, int height) {
    drawPanelFrame(x, y, width, height, "Image 2: City Road");

    int top = y + 25;
    int bottom = y + height - 1;
    int horizon = y + height - 92;

    setfillstyle(SOLID_FILL, LIGHTBLUE);
    bar(x + 1, top, x + width - 1, horizon);
    setfillstyle(SOLID_FILL, LIGHTGREEN);
    bar(x + 1, horizon, x + width - 1, horizon + 26);

    setcolor(BLACK);
    setfillstyle(SOLID_FILL, LIGHTGRAY);
    bar(x + 48, top + 72, x + 124, horizon);
    bar(x + 140, top + 42, x + 224, horizon);
    bar(x + width - 214, top + 55, x + width - 130, horizon);
    bar(x + width - 112, top + 82, x + width - 42, horizon);

    setfillstyle(SOLID_FILL, YELLOW);
    for (int bx = x + 60; bx < x + 220; bx += 32) {
        for (int by = top + 84; by < horizon - 12; by += 28) {
            bar(bx, by, bx + 12, by + 13);
        }
    }
    for (int bx = x + width - 196; bx < x + width - 55; bx += 32) {
        for (int by = top + 86; by < horizon - 12; by += 28) {
            bar(bx, by, bx + 12, by + 13);
        }
    }

    int road[] = {
        x + 180, horizon,
        x + width - 180, horizon,
        x + width - 42, bottom,
        x + 42, bottom
    };
    fillClosedPolygon(road, 4, DARKGRAY, BLACK);

    setcolor(WHITE);
    for (int yy = horizon + 12; yy < bottom - 20; yy += 36) {
        line(x + width / 2, yy, x + width / 2, yy + 20);
    }

    setfillstyle(SOLID_FILL, RED);
    bar(x + 170, bottom - 74, x + 304, bottom - 38);
    fillellipse(x + 235, bottom - 76, 45, 22);
    setfillstyle(SOLID_FILL, LIGHTCYAN);
    bar(x + 212, bottom - 94, x + 266, bottom - 72);
    setfillstyle(SOLID_FILL, BLACK);
    fillellipse(x + 195, bottom - 35, 15, 15);
    fillellipse(x + 280, bottom - 35, 15, 15);

    setfillstyle(SOLID_FILL, BLACK);
    bar(x + width - 92, horizon - 82, x + width - 72, horizon - 20);
    setfillstyle(SOLID_FILL, RED);
    fillellipse(x + width - 82, horizon - 72, 6, 6);
    setfillstyle(SOLID_FILL, YELLOW);
    fillellipse(x + width - 82, horizon - 52, 6, 6);
    setfillstyle(SOLID_FILL, GREEN);
    fillellipse(x + width - 82, horizon - 32, 6, 6);
    setcolor(BLACK);
    line(x + width - 82, horizon - 20, x + width - 82, horizon + 30);
}

void drawStudentImage(int x, int y, int width, int height) {
    drawPanelFrame(x, y, width, height, "Image 3: Cartoon Student");

    int top = y + 25;
    int bottom = y + height - 1;
    int ground = y + height - 68;
    int cx = x + width / 2;

    setfillstyle(SOLID_FILL, LIGHTCYAN);
    bar(x + 1, top, x + width - 1, ground);
    setfillstyle(SOLID_FILL, LIGHTGREEN);
    bar(x + 1, ground, x + width - 1, bottom);

    setfillstyle(SOLID_FILL, LIGHTGRAY);
    bar(x + 48, top + 102, x + 178, ground);
    setcolor(BLACK);
    rectangle(x + 48, top + 102, x + 178, ground);
    drawText(x + 70, top + 118, "GZU LAB");

    setfillstyle(SOLID_FILL, BROWN);
    bar(cx - 58, ground - 96, cx + 58, ground - 16);
    setfillstyle(SOLID_FILL, BLUE);
    bar(cx - 70, ground - 118, cx + 70, ground - 64);
    setcolor(BLACK);
    rectangle(cx - 70, ground - 118, cx + 70, ground - 64);

    setfillstyle(SOLID_FILL, BROWN);
    fillellipse(cx, ground - 165, 46, 55);
    setfillstyle(SOLID_FILL, BLACK);
    fillellipse(cx, ground - 205, 48, 20);
    fillellipse(cx - 38, ground - 177, 10, 23);
    fillellipse(cx + 38, ground - 177, 10, 23);

    setfillstyle(SOLID_FILL, WHITE);
    fillellipse(cx - 18, ground - 168, 10, 7);
    fillellipse(cx + 18, ground - 168, 10, 7);
    setfillstyle(SOLID_FILL, BLACK);
    fillellipse(cx - 18, ground - 168, 3, 3);
    fillellipse(cx + 18, ground - 168, 3, 3);
    setcolor(BLACK);
    arc(cx, ground - 151, 200, 340, 18);

    line(cx - 72, ground - 105, cx - 116, ground - 56);
    line(cx + 72, ground - 105, cx + 116, ground - 56);
    setfillstyle(SOLID_FILL, BROWN);
    fillellipse(cx - 118, ground - 54, 9, 9);
    fillellipse(cx + 118, ground - 54, 9, 9);

    setfillstyle(SOLID_FILL, LIGHTGRAY);
    bar(cx - 52, ground - 48, cx + 52, ground - 8);
    setcolor(BLACK);
    rectangle(cx - 52, ground - 48, cx + 52, ground - 8);
    line(cx, ground - 48, cx, ground - 8);
    drawText(cx - 38, ground - 34, "M230913");

    setcolor(BLACK);
    line(cx - 28, ground - 16, cx - 42, bottom - 8);
    line(cx + 28, ground - 16, cx + 42, bottom - 8);
    fillellipse(cx - 45, bottom - 8, 19, 7);
    fillellipse(cx + 45, bottom - 8, 19, 7);
}

void drawPrimitiveImage(int x, int y, int width, int height) {
    drawPanelFrame(x, y, width, height, "Image 4: graphics.h Primitives");

    int top = y + 25;
    int left = x + 1;
    int right = x + width - 1;
    int bottom = y + height - 1;

    setfillstyle(SOLID_FILL, WHITE);
    bar(left, top, right, bottom);

    setcolor(BLACK);
    line(x + 42, top + 48, x + 180, top + 86);
    drawText(x + 62, top + 94, "line()");

    rectangle(x + 230, top + 35, x + 355, top + 112);
    drawText(x + 246, top + 123, "rectangle()");

    setfillstyle(SOLID_FILL, LIGHTCYAN);
    bar(x + 405, top + 42, x + 522, top + 108);
    setcolor(BLACK);
    rectangle(x + 405, top + 42, x + 522, top + 108);
    drawText(x + 438, top + 123, "bar()");

    setfillstyle(SOLID_FILL, YELLOW);
    setcolor(BLACK);
    fillellipse(x + 92, top + 196, 44, 44);
    circle(x + 92, top + 196, 44);
    drawText(x + 65, top + 246, "circle()");

    setfillstyle(SOLID_FILL, LIGHTMAGENTA);
    fillellipse(x + 272, top + 194, 70, 36);
    ellipse(x + 272, top + 194, 0, 360, 70, 36);
    drawText(x + 242, top + 246, "ellipse()");

    int poly[] = {
        x + 425, top + 170,
        x + 516, top + 170,
        x + 538, top + 226,
        x + 472, top + 260,
        x + 404, top + 224
    };
    fillClosedPolygon(poly, 5, LIGHTGREEN, BLACK);
    drawText(x + 438, top + 246, "fillpoly()");

    setfillstyle(SOLID_FILL, LIGHTRED);
    pieslice(x + 180, top + 192, 30, 310, 36);
    drawText(x + 157, top + 246, "pieslice()");

    setcolor(BLUE);
    arc(x + 360, top + 203, 20, 170, 42);
    setcolor(BLACK);
    drawText(x + 350, top + 246, "arc()");
}

void drawPartAPrimitiveReference() {
    setcolor(BLACK);
    setfillstyle(SOLID_FILL, BLACK);
    setlinestyle(SOLID_LINE, 0, THICK_WIDTH);

    const ReferencePoint pointsExample[] = {
        {12.0f, 92.0f}, {12.0f, 87.0f}, {18.0f, 88.0f}, {18.0f, 91.0f}, {24.0f, 95.0f}
    };
    const ReferenceLabelOffset pointsLabels[] = {
        {-1.8f, 0.3f}, {-1.8f, -0.3f}, {0.7f, -0.5f}, {0.7f, 0.1f}, {0.7f, 0.4f}
    };
    for (int i = 0; i < 5; ++i) {
        drawReferenceWorldPoint(pointsExample[i], 7);
    }
    drawReferenceMarkers(pointsExample, pointsLabels, 5);
    drawReferenceWorldText(9.0f, 77.0f, "GL_POINTS", true);

    const ReferencePoint linesExample[] = {
        {6.0f, 59.5f}, {12.0f, 74.5f}, {12.5f, 65.0f}, {25.5f, 62.0f},
        {8.0f, 60.0f}, {20.0f, 56.0f}, {9.0f, 57.0f}, {19.0f, 61.0f}
    };
    const ReferenceLabelOffset linesLabels[] = {
        {-1.7f, -0.7f}, {0.7f, -0.2f}, {-1.4f, 0.3f}, {0.7f, -0.5f},
        {-1.2f, 0.3f}, {0.6f, -0.9f}, {-1.3f, -0.4f}, {0.7f, 0.2f}
    };
    for (int i = 0; i < 8; i += 2) {
        drawReferenceWorldLine(linesExample[i], linesExample[i + 1]);
    }
    drawReferenceMarkers(linesExample, linesLabels, 8);
    drawReferenceWorldText(10.0f, 49.0f, "GL_LINES", true);

    const ReferencePoint lineStripExample[] = {
        {43.0f, 68.0f}, {62.0f, 56.0f}, {56.5f, 67.0f}, {47.0f, 59.0f}, {44.5f, 54.0f}
    };
    const ReferenceLabelOffset lineStripLabels[] = {
        {-1.6f, 0.4f}, {0.8f, -0.8f}, {0.8f, 0.3f}, {-1.5f, 0.4f}, {-1.5f, -0.7f}
    };
    for (int i = 0; i < 4; ++i) {
        drawReferenceWorldLine(lineStripExample[i], lineStripExample[i + 1]);
    }
    drawReferenceMarkers(lineStripExample, lineStripLabels, 5);
    drawReferenceWorldText(44.0f, 49.0f, "GL_LINE_STRIP", true);

    const ReferencePoint lineLoopExample[] = {
        {77.0f, 70.0f}, {81.0f, 58.0f}, {95.0f, 58.0f}, {90.0f, 70.0f}, {86.0f, 62.0f}, {82.0f, 73.5f}
    };
    const ReferenceLabelOffset lineLoopLabels[] = {
        {-1.8f, 0.2f}, {-1.3f, -0.6f}, {0.7f, -0.6f}, {0.8f, 0.2f}, {-0.5f, 0.3f}, {0.8f, 0.3f}
    };
    drawReferenceWorldPolygon(lineLoopExample, 6);
    drawReferenceMarkers(lineLoopExample, lineLoopLabels, 6);
    drawReferenceWorldText(79.0f, 49.0f, "GL_LINE_LOOP", true);

    const ReferencePoint trianglesExample[] = {
        {9.5f, 25.5f}, {9.5f, 36.0f}, {18.0f, 25.5f},
        {21.0f, 28.5f}, {15.0f, 41.5f}, {31.5f, 41.5f}
    };
    const ReferenceLabelOffset trianglesLabels[] = {
        {-1.5f, -0.5f}, {-1.6f, 0.3f}, {0.7f, -0.5f}, {-0.2f, -1.1f}, {-1.7f, 0.3f}, {0.7f, 0.3f}
    };
    for (int i = 0; i < 6; i += 3) {
        drawReferenceWorldTriangle(trianglesExample[i], trianglesExample[i + 1], trianglesExample[i + 2]);
    }
    drawReferenceMarkers(trianglesExample, trianglesLabels, 6);
    drawReferenceWorldText(9.0f, 15.5f, "GL_TRIANGLES", true);

    const ReferencePoint triangleStripExample[] = {
        {45.0f, 40.0f}, {45.0f, 30.0f}, {52.0f, 40.0f},
        {57.5f, 30.0f}, {63.0f, 40.0f}, {64.5f, 25.0f}
    };
    const ReferenceLabelOffset triangleStripLabels[] = {
        {-1.7f, 0.4f}, {-1.4f, -0.8f}, {-0.3f, 0.7f}, {-0.3f, -1.0f}, {0.7f, 0.4f}, {0.8f, -0.8f}
    };
    for (int i = 0; i < 4; ++i) {
        drawReferenceWorldTriangle(triangleStripExample[i], triangleStripExample[i + 1], triangleStripExample[i + 2]);
    }
    drawReferenceMarkers(triangleStripExample, triangleStripLabels, 6);
    drawReferenceWorldText(39.0f, 15.5f, "GL_TRIANGLE_STRIP", true);

    const ReferencePoint triangleFanExample[] = {
        {78.0f, 29.0f}, {78.0f, 43.0f}, {87.0f, 39.0f}, {88.5f, 31.5f}, {87.5f, 21.5f}
    };
    const ReferenceLabelOffset triangleFanLabels[] = {
        {-1.3f, -0.5f}, {-1.3f, 0.4f}, {0.7f, 0.4f}, {0.8f, -0.1f}, {0.7f, -0.8f}
    };
    for (int i = 1; i < 4; ++i) {
        drawReferenceWorldTriangle(triangleFanExample[0], triangleFanExample[i], triangleFanExample[i + 1]);
    }
    drawReferenceMarkers(triangleFanExample, triangleFanLabels, 5);
    drawReferenceWorldText(75.0f, 15.5f, "GL_TRIANGLE_FAN", true);

    const ReferencePoint quadsExample[] = {
        {6.8f, 0.6f}, {9.7f, 6.8f}, {18.2f, 7.6f}, {18.2f, 1.4f},
        {27.5f, -0.1f}, {28.3f, 6.9f}, {38.3f, 11.8f}, {34.2f, -0.1f}
    };
    const ReferenceLabelOffset quadsLabels[] = {
        {-1.6f, -0.2f}, {-1.6f, 0.2f}, {0.7f, 0.2f}, {0.7f, -0.4f},
        {-1.5f, -0.6f}, {-1.6f, 0.2f}, {0.7f, 0.2f}, {0.7f, -0.4f}
    };
    for (int i = 0; i < 8; i += 4) {
        drawReferenceWorldPolygon(quadsExample + i, 4);
    }
    drawReferenceMarkers(quadsExample, quadsLabels, 8);
    drawReferenceWorldText(12.0f, -5.6f, "GL_QUADS", true);

    const ReferencePoint quadStripExample[] = {
        {47.5f, 1.5f}, {46.8f, 8.5f}, {53.4f, 1.5f}, {53.4f, 9.1f},
        {58.2f, 2.6f}, {56.5f, 10.0f}, {63.4f, 3.3f}, {62.5f, 10.9f}
    };
    const ReferenceLabelOffset quadStripLabels[] = {
        {-1.3f, -0.5f}, {-1.3f, 0.2f}, {-0.2f, -0.8f}, {-0.2f, 0.3f},
        {-0.3f, -0.9f}, {-0.2f, 0.4f}, {0.8f, -0.5f}, {0.8f, 0.3f}
    };
    const ReferencePoint quadStripOuter[] = {
        quadStripExample[1], quadStripExample[0], quadStripExample[2], quadStripExample[4],
        quadStripExample[6], quadStripExample[7], quadStripExample[5], quadStripExample[3]
    };
    drawReferenceWorldPolygon(quadStripOuter, 8);
    drawReferenceWorldLine(quadStripExample[3], quadStripExample[2]);
    drawReferenceWorldLine(quadStripExample[5], quadStripExample[4]);
    drawReferenceMarkers(quadStripExample, quadStripLabels, 8);
    drawReferenceWorldText(43.5f, -5.6f, "GL_QUAD_STRIP", true);

    const ReferencePoint polygonExample[] = {
        {80.8f, 6.8f}, {89.2f, 8.8f}, {86.4f, 2.6f}, {79.1f, -0.7f}, {77.1f, 4.0f}
    };
    const ReferenceLabelOffset polygonLabels[] = {
        {-1.5f, 0.3f}, {0.7f, 0.3f}, {0.7f, -0.2f}, {-0.2f, -0.8f}, {-1.6f, -0.1f}
    };
    drawReferenceWorldPolygon(polygonExample, 5);
    drawReferenceMarkers(polygonExample, polygonLabels, 5);
    drawReferenceWorldText(78.5f, -5.6f, "GL_POLYGON", true);

    setlinestyle(SOLID_LINE, 0, NORM_WIDTH);
}

void drawReferenceHeader() {
    setfillstyle(SOLID_FILL, WHITE);
    bar(0, 0, WINDOW_WIDTH, 56);
    setcolor(BLACK);

    settextstyle(DEFAULT_FONT, HORIZ_DIR, 2);
    drawCenteredText(0, WINDOW_WIDTH, 10, "PART E - graphics.h RECREATION OF PART A LINES AND SHAPES");

    settextstyle(DEFAULT_FONT, HORIZ_DIR, 1);
    drawCenteredText(0, WINDOW_WIDTH, 38, "Same GL_POINTS, GL_LINES, strips, loops, triangles, quads, and polygon layout redrawn with BGI graphics.h calls");
}

void drawReferenceFooter(bool saved) {
    setfillstyle(SOLID_FILL, LIGHTGRAY);
    bar(0, FOOTER_TOP, WINDOW_WIDTH, WINDOW_HEIGHT);
    setcolor(BLACK);
    rectangle(0, FOOTER_TOP, WINDOW_WIDTH - 1, WINDOW_HEIGHT - 1);

    if (saved) {
        drawCenteredText(0, WINDOW_WIDTH, WINDOW_HEIGHT - 25, "Saved part_e_all_primitives.bmp. S saves again, Q/Esc closes.");
    } else {
        drawCenteredText(0, WINDOW_WIDTH, WINDOW_HEIGHT - 25, "Part A primitive sheet recreated using graphics.h. S saves, Q/Esc closes.");
    }
}

void drawAllPrimitiveReference(bool saved = false) {
    setbkcolor(WHITE);
    setfillstyle(SOLID_FILL, WHITE);
    cleardevice();
    setlinestyle(SOLID_LINE, 0, THICK_WIDTH);

    drawReferenceHeader();

    drawPartAPrimitiveReference();

    setlinestyle(SOLID_LINE, 0, NORM_WIDTH);
    drawReferenceFooter(saved);
}

void saveReferenceImage() {
    drawAllPrimitiveReference(false);
    writeimagefile(const_cast<char*>(REFERENCE_OUTPUT_IMAGE), 0, 0, WINDOW_WIDTH - 1, WINDOW_HEIGHT - 1);
    drawAllPrimitiveReference(true);
}

bool hasOption(int argc, char* argv[], const char* option) {
    for (int i = 1; i < argc; ++i) {
        if (std::strcmp(argv[i], option) == 0) {
            return true;
        }
    }
    return false;
}

int main(int argc, char* argv[]) {
    initwindow(WINDOW_WIDTH, WINDOW_HEIGHT, "Part E - graphics.h Image Creation");

    if (hasOption(argc, argv, "--save-and-exit")) {
        saveReferenceImage();
        delay(250);
        closegraph();
        return 0;
    }

    if (hasOption(argc, argv, "--save-reference-and-exit")) {
        saveReferenceImage();
        delay(250);
        closegraph();
        return 0;
    }

    drawAllPrimitiveReference(false);

    bool running = true;
    while (running) {
        if (kbhit()) {
            int key = getch();
            if (key == 27 || key == 'q' || key == 'Q') {
                running = false;
            } else if (key == 's' || key == 'S') {
                saveReferenceImage();
            }
        }
        delay(20);
    }

    closegraph();
    return 0;
}
