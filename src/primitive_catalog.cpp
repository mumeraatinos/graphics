#include <GL/glut.h>

// Great Zimbabwe University
// Computer Graphics Practical Assignment
// This program renders a lecturer-facing reference sheet of OpenGL primitive types.

struct Point2D {
    float x;
    float y;
};

struct LabelOffset {
    float dx;
    float dy;
};

// The page extends slightly below the initial viewport, so scrolling keeps the
// bottom row visible without changing the primitive sizes.
float gScrollOffset = 0.0f;
const float kScrollStep = 4.0f;
const float kViewBottom = -12.0f;
const float kViewTop = 100.0f;
const float kMaxScrollOffset = 12.0f;

void updateProjection(int width, int height) {
    glViewport(0, 0, width, height);
    glMatrixMode(GL_PROJECTION);
    glLoadIdentity();
    gluOrtho2D(0.0, 100.0, kViewBottom - gScrollOffset, kViewTop - gScrollOffset);
    glMatrixMode(GL_MODELVIEW);
    glLoadIdentity();
}

void scrollView(float delta) {
    gScrollOffset += delta;

    if (gScrollOffset < 0.0f) {
        gScrollOffset = 0.0f;
    }
    if (gScrollOffset > kMaxScrollOffset) {
        gScrollOffset = kMaxScrollOffset;
    }

    updateProjection(glutGet(GLUT_WINDOW_WIDTH), glutGet(GLUT_WINDOW_HEIGHT));
    glutPostRedisplay();
}

void handleSpecialKeys(int key, int, int) {
    if (key == GLUT_KEY_DOWN || key == GLUT_KEY_PAGE_DOWN) {
        scrollView(kScrollStep);
    } else if (key == GLUT_KEY_UP || key == GLUT_KEY_PAGE_UP) {
        scrollView(-kScrollStep);
    }
}

void handleMouse(int button, int state, int, int) {
    if (state != GLUT_DOWN) {
        return;
    }

    if (button == 4) {
        scrollView(kScrollStep);
    } else if (button == 3) {
        scrollView(-kScrollStep);
    }
}

void drawText(float x, float y, const char* text) {
    glRasterPos2f(x, y);
    while (*text) {
        glutBitmapCharacter(GLUT_BITMAP_HELVETICA_12, *text);
        ++text;
    }
}

void drawBoldText(float x, float y, const char* text) {
    glRasterPos2f(x, y);
    while (*text) {
        glutBitmapCharacter(GLUT_BITMAP_HELVETICA_18, *text);
        ++text;
    }
}

void drawVertexMarkers(const Point2D points[], const LabelOffset offsets[], int count) {
    glPointSize(5.0f);
    glColor3f(0.0f, 0.0f, 0.0f);
    glBegin(GL_POINTS);
    for (int i = 0; i < count; ++i) {
        glVertex2f(points[i].x, points[i].y);
    }
    glEnd();

    for (int i = 0; i < count; ++i) {
        char label[8];
        label[0] = 'v';
        label[1] = static_cast<char>('0' + i);
        label[2] = '\0';
        float dx = offsets ? offsets[i].dx : 0.5f;
        float dy = offsets ? offsets[i].dy : 0.5f;
        drawText(points[i].x + dx, points[i].y + dy, label);
    }
}

void drawPrimitive(const Point2D points[], int count, GLenum mode) {
    glBegin(mode);
    for (int i = 0; i < count; ++i) {
        glVertex2f(points[i].x, points[i].y);
    }
    glEnd();
}

void drawBoldLine(const Point2D points[], int count, GLenum mode, float width) {
    glLineWidth(width);
    drawPrimitive(points, count, mode);
    glLineWidth(2.0f);
}

void drawFilledPrimitive(const Point2D points[], int count, GLenum mode) {
    glColor3f(0.0f, 0.0f, 0.0f);
    glPolygonMode(GL_FRONT_AND_BACK, GL_LINE);
    drawPrimitive(points, count, mode);
    glPolygonMode(GL_FRONT_AND_BACK, GL_FILL);
}

void display() {
    glClear(GL_COLOR_BUFFER_BIT);
    glColor3f(0.0f, 0.0f, 0.0f);

    // Top row: point and line-based primitives.
    const Point2D pointsExample[] = {
        {12.0f, 92.0f}, {12.0f, 87.0f}, {18.0f, 88.0f}, {18.0f, 91.0f}, {24.0f, 95.0f}
    };
    const LabelOffset pointsLabels[] = {
        {-1.8f, 0.3f},
        {-1.8f, -0.3f},
        {0.7f, -0.5f},
        {0.7f, 0.1f},
        {0.7f, 0.4f}
    };
    glPointSize(12.0f);
    glColor3f(0.0f, 0.0f, 0.0f);
    drawPrimitive(pointsExample, 5, GL_POINTS);
    glColor3f(0.0f, 0.0f, 0.0f);
    drawVertexMarkers(pointsExample, pointsLabels, 5);
    drawBoldText(9.0f, 77.0f, "GL_POINTS");

    const Point2D linesExample[] = {
        {6.0f, 59.5f}, {12.0f, 74.5f},
        {12.5f, 65.0f}, {25.5f, 62.0f},
        {8.0f, 60.0f}, {20.0f, 56.0f},
        {9.0f, 57.0f}, {19.0f, 61.0f}
    };
    const LabelOffset linesLabels[] = {
        {-1.7f, -0.7f},
        {0.7f, -0.2f},
        {-1.4f, 0.3f},
        {0.7f, -0.5f},
        {-1.2f, 0.3f},
        {0.6f, -0.9f},
        {-1.3f, -0.4f},
        {0.7f, 0.2f}
    };
    glColor3f(0.0f, 0.0f, 0.0f);
    drawBoldLine(linesExample, 8, GL_LINES, 4.0f);
    glColor3f(0.0f, 0.0f, 0.0f);
    drawVertexMarkers(linesExample, linesLabels, 8);
    drawBoldText(10.0f, 49.0f, "GL_LINES");

    const Point2D lineStripExample[] = {
        {43.0f, 68.0f}, {62.0f, 56.0f}, {56.5f, 67.0f}, {47.0f, 59.0f}, {44.5f, 54.0f}
    };
    const LabelOffset lineStripLabels[] = {
        {-1.6f, 0.4f},
        {0.8f, -0.8f},
        {0.8f, 0.3f},
        {-1.5f, 0.4f},
        {-1.5f, -0.7f}
    };
    glColor3f(0.0f, 0.0f, 0.0f);
    drawBoldLine(lineStripExample, 5, GL_LINE_STRIP, 5.0f);
    glColor3f(0.0f, 0.0f, 0.0f);
    drawVertexMarkers(lineStripExample, lineStripLabels, 5);
    drawBoldText(44.0f, 49.0f, "GL_LINE_STRIP");

    const Point2D lineLoopExample[] = {
        {77.0f, 70.0f}, {81.0f, 58.0f}, {95.0f, 58.0f}, {90.0f, 70.0f}, {86.0f, 62.0f}, {82.0f, 73.5f}
    };
    const LabelOffset lineLoopLabels[] = {
        {-1.8f, 0.2f},
        {-1.3f, -0.6f},
        {0.7f, -0.6f},
        {0.8f, 0.2f},
        {-0.5f, 0.3f},
        {0.8f, 0.3f}
    };
    glColor3f(0.0f, 0.0f, 0.0f);
    drawBoldLine(lineLoopExample, 6, GL_LINE_LOOP, 4.0f);
    glColor3f(0.0f, 0.0f, 0.0f);
    drawVertexMarkers(lineLoopExample, lineLoopLabels, 6);
    drawBoldText(79.0f, 49.0f, "GL_LINE_LOOP");

    // Middle row: triangle-based primitives.
    const Point2D trianglesExample[] = {
        {9.5f, 25.5f}, {9.5f, 36.0f}, {18.0f, 25.5f},
        {21.0f, 28.5f}, {15.0f, 41.5f}, {31.5f, 41.5f}
    };
    const LabelOffset trianglesLabels[] = {
        {-1.5f, -0.5f},
        {-1.6f, 0.3f},
        {0.7f, -0.5f},
        {-0.2f, -1.1f},
        {-1.7f, 0.3f},
        {0.7f, 0.3f}
    };
    drawFilledPrimitive(trianglesExample, 6, GL_TRIANGLES);
    drawVertexMarkers(trianglesExample, trianglesLabels, 6);
    drawBoldText(9.0f, 15.5f, "GL_TRIANGLES");

    const Point2D triangleStripExample[] = {
        {45.0f, 40.0f}, {45.0f, 30.0f}, {52.0f, 40.0f},
        {57.5f, 30.0f}, {63.0f, 40.0f}, {64.5f, 25.0f}
    };
    const LabelOffset triangleStripLabels[] = {
        {-1.7f, 0.4f},
        {-1.4f, -0.8f},
        {-0.3f, 0.7f},
        {-0.3f, -1.0f},
        {0.7f, 0.4f},
        {0.8f, -0.8f}
    };
    drawFilledPrimitive(triangleStripExample, 6, GL_TRIANGLE_STRIP);
    drawVertexMarkers(triangleStripExample, triangleStripLabels, 6);
    drawBoldText(39.0f, 15.5f, "GL_TRIANGLE_STRIP");

    const Point2D triangleFanExample[] = {
        {78.0f, 29.0f}, {78.0f, 43.0f}, {87.0f, 39.0f}, {88.5f, 31.5f}, {87.5f, 21.5f}
    };
    const LabelOffset triangleFanLabels[] = {
        {-1.3f, -0.5f},
        {-1.3f, 0.4f},
        {0.7f, 0.4f},
        {0.8f, -0.1f},
        {0.7f, -0.8f}
    };
    drawFilledPrimitive(triangleFanExample, 5, GL_TRIANGLE_FAN);
    glColor3f(0.0f, 0.0f, 0.0f);
    const Point2D triangleFanOuter[] = {
        triangleFanExample[1], triangleFanExample[2], triangleFanExample[3], triangleFanExample[4], triangleFanExample[0]
    };
    drawPrimitive(triangleFanOuter, 5, GL_LINE_LOOP);
    const Point2D triangleFanSpokes[] = {
        triangleFanExample[0], triangleFanExample[2],
        triangleFanExample[0], triangleFanExample[3]
    };
    drawPrimitive(triangleFanSpokes, 4, GL_LINES);
    drawVertexMarkers(triangleFanExample, triangleFanLabels, 5);
    drawBoldText(75.0f, 15.5f, "GL_TRIANGLE_FAN");

    // Bottom row: quad and polygon primitives.
    const Point2D quadsExample[] = {
        {6.8f, 0.6f}, {9.7f, 6.8f}, {18.2f, 7.6f}, {18.2f, 1.4f},
        {27.5f, -0.1f}, {28.3f, 6.9f}, {38.3f, 11.8f}, {34.2f, -0.1f}
    };
    const LabelOffset quadsLabels[] = {
        {-1.6f, -0.2f},
        {-1.6f, 0.2f},
        {0.7f, 0.2f},
        {0.7f, -0.4f},
        {-1.5f, -0.6f},
        {-1.6f, 0.2f},
        {0.7f, 0.2f},
        {0.7f, -0.4f}
    };
    drawFilledPrimitive(quadsExample, 8, GL_QUADS);
    drawVertexMarkers(quadsExample, quadsLabels, 8);
    drawBoldText(12.0f, -5.6f, "GL_QUADS");

    const Point2D quadStripExample[] = {
        {48.5f, 5.2f}, {48.0f, 10.0f}, {53.0f, 5.2f}, {53.0f, 10.5f},
        {57.1f, 6.0f}, {55.6f, 11.2f}, {61.6f, 6.6f}, {60.9f, 11.9f}
    };
    const LabelOffset quadStripLabels[] = {
        {-1.3f, -0.5f},
        {-1.3f, 0.2f},
        {-0.2f, -0.8f},
        {-0.2f, 0.3f},
        {-0.3f, -0.9f},
        {-0.2f, 0.4f},
        {0.8f, -0.5f},
        {0.8f, 0.3f}
    };
    drawFilledPrimitive(quadStripExample, 8, GL_QUAD_STRIP);
    glColor3f(0.0f, 0.0f, 0.0f);
    const Point2D quadStripOuter[] = {
        quadStripExample[1], quadStripExample[0], quadStripExample[2], quadStripExample[4],
        quadStripExample[6], quadStripExample[7], quadStripExample[5], quadStripExample[3]
    };
    drawPrimitive(quadStripOuter, 8, GL_LINE_LOOP);
    const Point2D quadStripInnerLeft[] = {quadStripExample[3], quadStripExample[2]};
    const Point2D quadStripInnerRight[] = {quadStripExample[5], quadStripExample[4]};
    drawPrimitive(quadStripInnerLeft, 2, GL_LINES);
    drawPrimitive(quadStripInnerRight, 2, GL_LINES);
    drawVertexMarkers(quadStripExample, quadStripLabels, 8);
    drawBoldText(43.5f, -5.6f, "GL_QUAD_STRIP");

    const Point2D polygonExample[] = {
        {80.8f, 6.8f}, {89.2f, 8.8f}, {86.4f, 2.6f}, {79.1f, -0.7f}, {77.1f, 4.0f}
    };
    const LabelOffset polygonLabels[] = {
        {-1.5f, 0.3f},
        {0.7f, 0.3f},
        {0.7f, -0.2f},
        {-0.2f, -0.8f},
        {-1.6f, -0.1f}
    };
    drawFilledPrimitive(polygonExample, 5, GL_POLYGON);
    drawVertexMarkers(polygonExample, polygonLabels, 5);
    drawBoldText(78.5f, -5.6f, "GL_POLYGON");

    glFlush();
}

void reshape(int width, int height) {
    updateProjection(width, height);
}

void init() {
    glClearColor(1.0f, 1.0f, 1.0f, 1.0f);
    glLineWidth(2.0f);
    glEnable(GL_POINT_SMOOTH);
    glHint(GL_POINT_SMOOTH_HINT, GL_NICEST);
    glEnable(GL_BLEND);
    glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);
}

int main(int argc, char** argv) {
    glutInit(&argc, argv);
    glutInitDisplayMode(GLUT_SINGLE | GLUT_RGB);
    glutInitWindowSize(1000, 750);
    glutCreateWindow("OpenGL Primitive Examples");

    init();
    glutDisplayFunc(display);
    glutReshapeFunc(reshape);
    // Scroll support makes it possible to view the full submission page.
    glutSpecialFunc(handleSpecialKeys);
    glutMouseFunc(handleMouse);
    glutMainLoop();
    return 0;
}
