#include <graphics.h>
#include <conio.h>
#include <windows.h>

#include <cmath>
#include <cstring>

// Great Zimbabwe University
// Computer Graphics Practical Assignment - Part F
// Fresh graphics.h version: a stylized jet flies across the screen.

const int WINDOW_WIDTH = 1100;
const int WINDOW_HEIGHT = 650;
const int FRAME_MS = 16;
const int JET_SPEED = 7;
const char* OUTPUT_IMAGE = "part_f_jet_output.bmp";

const double PI = 3.14159265358979323846;

int rgb(int r, int g, int b) {
    return COLOR(r, g, b);
}

int mix(int a, int b, float t) {
    return static_cast<int>(a + (b - a) * t);
}

int mixColor(int r1, int g1, int b1, int r2, int g2, int b2, float t) {
    return rgb(mix(r1, r2, t), mix(g1, g2, t), mix(b1, b2, t));
}

void drawText(int x, int y, const char* text) {
    outtextxy(x, y, const_cast<char*>(text));
}

void drawCenteredText(int left, int right, int y, const char* text) {
    int width = textwidth(const_cast<char*>(text));
    drawText(left + (right - left - width) / 2, y, text);
}

void fillClosedPolygon(const int points[], int count, int fillColor, int borderColor) {
    int localPoints[80];
    for (int i = 0; i < count * 2; ++i) {
        localPoints[i] = points[i];
    }

    setfillstyle(SOLID_FILL, fillColor);
    fillpoly(count, localPoints);
    setcolor(borderColor);
    for (int i = 0; i < count; ++i) {
        int next = (i + 1) % count;
        line(points[i * 2], points[i * 2 + 1],
             points[next * 2], points[next * 2 + 1]);
    }
}

void drawSoftEllipse(int x, int y, int rx, int ry, int fillColor, int edgeColor) {
    setfillstyle(SOLID_FILL, fillColor);
    setcolor(edgeColor);
    fillellipse(x, y, rx, ry);
    ellipse(x, y, 0, 360, rx, ry);
}

void drawSoftBar(int left, int top, int right, int bottom, int fillColor, int edgeColor) {
    setfillstyle(SOLID_FILL, fillColor);
    bar(left, top, right, bottom);
    setcolor(edgeColor);
    line(left, top, right, top);
    line(left, bottom, right, bottom);
}

void drawRoundedPolygon(const int points[], int count, int fillColor, int edgeColor, int radius) {
    int localPoints[80];
    for (int i = 0; i < count * 2; ++i) {
        localPoints[i] = points[i];
    }

    setfillstyle(SOLID_FILL, fillColor);
    fillpoly(count, localPoints);

    setcolor(edgeColor);
    for (int i = 0; i < count; ++i) {
        int next = (i + 1) % count;
        line(points[i * 2], points[i * 2 + 1],
             points[next * 2], points[next * 2 + 1]);
    }

    setfillstyle(SOLID_FILL, fillColor);
    for (int i = 0; i < count; ++i) {
        fillellipse(points[i * 2], points[i * 2 + 1], radius, radius);
    }
}

void drawRoundedWindow(int x, int y, int rx, int ry, int fillColor, int edgeColor) {
    drawSoftEllipse(x, y, rx, ry, fillColor, edgeColor);
    setcolor(rgb(220, 247, 255));
    arc(x - rx / 4, y - ry / 4, 20, 150, rx / 2);
}

void drawGradientSky() {
    for (int y = 0; y < WINDOW_HEIGHT; ++y) {
        float t = static_cast<float>(y) / static_cast<float>(WINDOW_HEIGHT - 1);
        int color;
        if (t < 0.48f) {
            float local = t / 0.48f;
            color = mixColor(16, 38, 82, 88, 154, 207, local);
        } else {
            float local = (t - 0.48f) / 0.52f;
            color = mixColor(88, 154, 207, 250, 174, 105, local);
        }
        setcolor(color);
        line(0, y, WINDOW_WIDTH, y);
    }
}

void drawSunAndGlow() {
    int sunX = WINDOW_WIDTH - 145;
    int sunY = 118;

    for (int r = 78; r >= 46; r -= 8) {
        setcolor(rgb(255, 215 - r, 95));
        circle(sunX, sunY, r);
    }

    setfillstyle(SOLID_FILL, rgb(255, 222, 106));
    setcolor(rgb(255, 246, 172));
    fillellipse(sunX, sunY, 42, 42);
}

void drawCloudLayer(int frame) {
    const int cloudColor = rgb(236, 246, 255);
    const int cloudShadow = rgb(172, 205, 229);
    int offset = (frame * 2) % (WINDOW_WIDTH + 240);

    for (int i = 0; i < 5; ++i) {
        int baseX = i * 275 - offset + 160;
        while (baseX < -190) {
            baseX += WINDOW_WIDTH + 240;
        }
        int baseY = 92 + (i % 3) * 36;
        int scale = 1 + (i % 2);

        drawSoftEllipse(baseX - 54 * scale, baseY + 12 * scale, 42 * scale, 18 * scale, cloudShadow, cloudShadow);
        drawSoftEllipse(baseX, baseY, 58 * scale, 26 * scale, cloudColor, cloudShadow);
        drawSoftEllipse(baseX + 56 * scale, baseY + 14 * scale, 48 * scale, 20 * scale, cloudColor, cloudShadow);
        drawSoftEllipse(baseX - 28 * scale, baseY - 18 * scale, 34 * scale, 22 * scale, cloudColor, cloudShadow);
        drawSoftEllipse(baseX + 28 * scale, baseY - 12 * scale, 38 * scale, 20 * scale, cloudColor, cloudShadow);
    }
}

void drawDistantCity(int frame) {
    int horizon = WINDOW_HEIGHT - 135;
    int drift = (frame / 4) % 80;

    setfillstyle(SOLID_FILL, rgb(38, 67, 99));
    setcolor(rgb(19, 44, 73));
    for (int x = -80 - drift; x < WINDOW_WIDTH + 80; x += 80) {
        int h = 58 + ((x + 240) % 130);
        bar(x, horizon - h, x + 48, horizon);
        rectangle(x, horizon - h, x + 48, horizon);
        setfillstyle(SOLID_FILL, rgb(255, 220, 112));
        for (int wy = horizon - h + 12; wy < horizon - 10; wy += 22) {
            bar(x + 12, wy, x + 20, wy + 8);
            bar(x + 30, wy, x + 38, wy + 8);
        }
        setfillstyle(SOLID_FILL, rgb(38, 67, 99));
    }

    setfillstyle(SOLID_FILL, rgb(24, 55, 86));
    bar(0, horizon, WINDOW_WIDTH, WINDOW_HEIGHT);
}

void drawRunwayPerspective(int frame) {
    int horizon = WINDOW_HEIGHT - 134;

    int runway[] = {
        WINDOW_WIDTH / 2 - 80, horizon,
        WINDOW_WIDTH / 2 + 80, horizon,
        WINDOW_WIDTH + 90, WINDOW_HEIGHT,
        -90, WINDOW_HEIGHT
    };
    fillClosedPolygon(runway, 4, rgb(36, 42, 50), rgb(12, 18, 26));

    setcolor(rgb(255, 225, 92));
    int stripeOffset = (frame * 6) % 58;
    for (int y = horizon + 18 + stripeOffset; y < WINDOW_HEIGHT; y += 58) {
        float t = static_cast<float>(y - horizon) / static_cast<float>(WINDOW_HEIGHT - horizon);
        int half = 7 + static_cast<int>(t * 30);
        int length = 18 + static_cast<int>(t * 42);
        line(WINDOW_WIDTH / 2 - half, y, WINDOW_WIDTH / 2 - half, y + length);
        line(WINDOW_WIDTH / 2 + half, y, WINDOW_WIDTH / 2 + half, y + length);
    }

    setcolor(rgb(168, 218, 255));
    line(WINDOW_WIDTH / 2 - 80, horizon, -90, WINDOW_HEIGHT);
    line(WINDOW_WIDTH / 2 + 80, horizon, WINDOW_WIDTH + 90, WINDOW_HEIGHT);

    setfillstyle(SOLID_FILL, rgb(13, 36, 58));
    bar(0, WINDOW_HEIGHT - 66, WINDOW_WIDTH, WINDOW_HEIGHT);
    setcolor(rgb(79, 133, 178));
    line(0, WINDOW_HEIGHT - 66, WINDOW_WIDTH, WINDOW_HEIGHT - 66);
}

void drawContrail(int jetX, int jetY, int frame) {
    for (int i = 0; i < 9; ++i) {
        int x = jetX - 188 - i * 48;
        int y = jetY + 19 + static_cast<int>(std::sin((frame + i * 7) * 0.18) * 8.0);
        int radius = 10 + i * 2;
        int color = i < 4 ? rgb(224, 242, 255) : rgb(160, 196, 220);
        drawSoftEllipse(x, y, radius * 2, radius, color, color);
    }
}

void drawAfterburner(int x, int y, int frame) {
    int flame = 18 + (frame % 4) * 8;
    int outer[] = {
        x - 150, y + 14,
        x - 226 - flame, y + 36,
        x - 150, y + 58
    };
    int middle[] = {
        x - 154, y + 22,
        x - 208 - flame, y + 36,
        x - 154, y + 50
    };
    int core[] = {
        x - 157, y + 29,
        x - 188 - flame, y + 36,
        x - 157, y + 43
    };

    drawRoundedPolygon(outer, 3, rgb(255, 83, 49), rgb(156, 35, 42), 2);
    drawRoundedPolygon(middle, 3, rgb(255, 174, 54), rgb(255, 174, 54), 2);
    drawRoundedPolygon(core, 3, rgb(255, 246, 128), rgb(255, 246, 128), 1);
}

void drawJet(int x, int y, int frame) {
    int bob = static_cast<int>(std::sin(frame * 0.08) * 18.0);
    int pitch = static_cast<int>(std::sin(frame * 0.05) * 5.0);
    y += bob;

    drawContrail(x, y, frame);
    drawAfterburner(x, y, frame);

    int shadowColor = rgb(21, 45, 66);
    drawSoftEllipse(x + 4, WINDOW_HEIGHT - 92, 132, 14, shadowColor, shadowColor);

    drawSoftBar(x - 154, y + 21 + pitch, x - 116, y + 54 + pitch,
                rgb(41, 48, 58), rgb(15, 19, 24));
    drawSoftEllipse(x - 154, y + 38 + pitch, 14, 17, rgb(41, 48, 58), rgb(15, 19, 24));
    drawSoftEllipse(x - 116, y + 38 + pitch, 12, 17, rgb(61, 69, 82), rgb(15, 19, 24));

    drawSoftBar(x - 132, y + 12 + pitch, x + 112, y + 57 + pitch,
                rgb(221, 228, 235), rgb(28, 39, 52));
    drawSoftEllipse(x - 132, y + 34 + pitch, 33, 23, rgb(221, 228, 235), rgb(28, 39, 52));
    drawSoftEllipse(x + 112, y + 34 + pitch, 63, 25, rgb(238, 244, 249), rgb(28, 39, 52));
    setfillstyle(SOLID_FILL, rgb(221, 228, 235));
    bar(x - 132, y + 13 + pitch, x + 112, y + 57 + pitch);

    drawSoftBar(x - 116, y + 16, x + 94, y + 29, rgb(248, 252, 255), rgb(248, 252, 255));
    drawSoftEllipse(x + 106, y + 28, 32, 13, rgb(248, 252, 255), rgb(248, 252, 255));

    drawSoftBar(x - 130, y + 44, x + 98, y + 58, rgb(146, 166, 184), rgb(93, 111, 131));
    drawSoftEllipse(x - 130, y + 51, 28, 8, rgb(146, 166, 184), rgb(93, 111, 131));
    drawSoftEllipse(x + 102, y + 49, 38, 11, rgb(146, 166, 184), rgb(93, 111, 131));

    int topWing[] = {
        x - 22, y + 14,
        x + 42, y - 84,
        x + 96, y + 12
    };
    int lowerWing[] = {
        x - 14, y + 56,
        x + 58, y + 136,
        x + 104, y + 58
    };
    drawRoundedPolygon(topWing, 3, rgb(38, 89, 151), rgb(18, 38, 70), 2);
    drawRoundedPolygon(lowerWing, 3, rgb(28, 68, 126), rgb(18, 38, 70), 2);

    int topTail[] = {
        x - 130, y + 14,
        x - 84, y - 62,
        x - 58, y + 18
    };
    int lowerTail[] = {
        x - 130, y + 57,
        x - 86, y + 104,
        x - 58, y + 54
    };
    drawRoundedPolygon(topTail, 3, rgb(223, 46, 71), rgb(102, 18, 35), 2);
    drawRoundedPolygon(lowerTail, 3, rgb(180, 32, 55), rgb(102, 18, 35), 2);

    drawSoftEllipse(x + 151, y + 34, 50, 18, rgb(247, 250, 252), rgb(28, 39, 52));
    setfillstyle(SOLID_FILL, rgb(247, 250, 252));
    bar(x + 108, y + 17, x + 151, y + 51);

    int canopy[] = {
        x + 36, y + 12,
        x + 90, y + 16,
        x + 112, y + 30,
        x + 26, y + 29
    };
    drawRoundedPolygon(canopy, 4, rgb(92, 184, 232), rgb(16, 54, 86), 2);

    setcolor(rgb(255, 255, 255));
    line(x + 45, y + 16, x + 88, y + 19);

    setfillstyle(SOLID_FILL, rgb(43, 78, 111));
    setcolor(rgb(16, 37, 57));
    for (int i = 0; i < 4; ++i) {
        int wx = x - 70 + i * 34;
        drawRoundedWindow(wx, y + 33, 9, 7, rgb(43, 78, 111), rgb(16, 37, 57));
    }

    setcolor(rgb(35, 56, 78));
    line(x - 92, y + 17, x - 102, y + 56);
    line(x - 34, y + 15, x - 42, y + 58);
    line(x + 20, y + 14, x + 18, y + 58);

    settextstyle(DEFAULT_FONT, HORIZ_DIR, 1);
    setcolor(rgb(17, 47, 85));
    drawText(x - 95, y + 42, "JET-100");
}

void drawHud(bool paused, bool saved) {
    setfillstyle(SOLID_FILL, rgb(7, 20, 33));
    bar(0, 0, WINDOW_WIDTH, 44);
    setcolor(rgb(84, 166, 222));
    rectangle(0, 0, WINDOW_WIDTH - 1, 44);

    settextstyle(DEFAULT_FONT, HORIZ_DIR, 2);
    setcolor(rgb(238, 248, 255));
    drawText(18, 12, "PART F: graphics.h Jet Flight");

    settextstyle(DEFAULT_FONT, HORIZ_DIR, 1);
    if (saved) {
        setcolor(rgb(255, 222, 106));
        drawText(WINDOW_WIDTH - 420, 18, "Saved part_f_jet_output.bmp");
    } else if (paused) {
        setcolor(rgb(255, 222, 106));
        drawText(WINDOW_WIDTH - 350, 18, "Paused: P continue | R restart | S save | Q quit");
    } else {
        setcolor(rgb(200, 229, 247));
        drawText(WINDOW_WIDTH - 360, 18, "P pause | R restart | S save | Q/Esc quit");
    }
}

void drawFrame(int jetX, int frame, bool paused = false, bool saved = false) {
    drawGradientSky();
    drawSunAndGlow();
    drawCloudLayer(frame);
    drawDistantCity(frame);
    drawRunwayPerspective(frame);
    drawJet(jetX, 182, frame);
    drawHud(paused, saved);
}

void presentFrame(int& drawPage, int jetX, int frame, bool paused, bool saved = false) {
    setactivepage(drawPage);
    drawFrame(jetX, frame, paused, saved);
    setvisualpage(drawPage);
    drawPage = 1 - drawPage;
}

void saveOutputImage(int jetX, int frame, bool paused, int& drawPage) {
    setactivepage(drawPage);
    drawFrame(jetX, frame, paused, false);
    writeimagefile(const_cast<char*>(OUTPUT_IMAGE), 0, 0, WINDOW_WIDTH - 1, WINDOW_HEIGHT - 1);
    drawFrame(jetX, frame, paused, true);
    setvisualpage(drawPage);
    drawPage = 1 - drawPage;
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
    initwindow(WINDOW_WIDTH, WINDOW_HEIGHT, "Part F - graphics.h Jet Flight");

    int frame = 0;
    int jetX = -280;
    int drawPage = 0;
    bool paused = false;

    if (hasOption(argc, argv, "--save-and-exit")) {
        saveOutputImage(530, 26, false, drawPage);
        delay(250);
        closegraph();
        return 0;
    }

    presentFrame(drawPage, jetX, frame, paused);
    DWORD lastTick = GetTickCount();
    bool needsRedraw = false;
    bool running = true;

    while (running) {
        if (kbhit()) {
            int key = getch();
            if (key == 27 || key == 'q' || key == 'Q') {
                running = false;
            } else if (key == 'p' || key == 'P') {
                paused = !paused;
                needsRedraw = true;
            } else if (key == 'r' || key == 'R') {
                jetX = -280;
                frame = 0;
                paused = false;
                lastTick = GetTickCount();
                needsRedraw = true;
            } else if (key == 's' || key == 'S') {
                saveOutputImage(jetX, frame, paused, drawPage);
                lastTick = GetTickCount();
                needsRedraw = false;
            }
        }

        DWORD now = GetTickCount();
        if (!paused && now - lastTick >= FRAME_MS) {
            jetX += JET_SPEED;
            ++frame;
            if (jetX > WINDOW_WIDTH + 300) {
                jetX = -300;
            }
            lastTick = now;
            needsRedraw = true;
        }

        if (needsRedraw) {
            presentFrame(drawPage, jetX, frame, paused);
            needsRedraw = false;
        }

        delay(1);
    }

    closegraph();
    return 0;
}
