#include <graphics.h>
#include <conio.h>
#include <cstring>
#include <windows.h>

// Great Zimbabwe University
// Computer Graphics Practical Assignment - Part F
// Modified graphics.h hello-world style program: a jet flies across the screen.

const int WINDOW_WIDTH = 1000;
const int WINDOW_HEIGHT = 620;
const int FRAME_MS = 16;
const int JET_SPEED = 5;
const char* OUTPUT_IMAGE = "part_f_jet_output.bmp";

void drawText(int x, int y, const char* text) {
    outtextxy(x, y, const_cast<char*>(text));
}

void drawCenteredText(int left, int right, int y, const char* text) {
    int width = textwidth(const_cast<char*>(text));
    drawText(left + (right - left - width) / 2, y, text);
}

void fillClosedPolygon(const int points[], int count, int fillColor, int borderColor) {
    int localPoints[64];
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

void drawCloud(int x, int y, int scale) {
    setfillstyle(SOLID_FILL, WHITE);
    setcolor(LIGHTGRAY);
    fillellipse(x - 32 * scale, y + 8 * scale, 32 * scale, 18 * scale);
    fillellipse(x, y, 42 * scale, 24 * scale);
    fillellipse(x + 38 * scale, y + 10 * scale, 34 * scale, 18 * scale);
    fillellipse(x + 5 * scale, y - 12 * scale, 30 * scale, 20 * scale);
}

void drawRunway() {
    int groundY = WINDOW_HEIGHT - 120;

    setfillstyle(SOLID_FILL, LIGHTGREEN);
    bar(0, groundY, WINDOW_WIDTH, WINDOW_HEIGHT);

    int runway[] = {
        80, WINDOW_HEIGHT,
        420, groundY,
        580, groundY,
        920, WINDOW_HEIGHT
    };
    fillClosedPolygon(runway, 4, DARKGRAY, BLACK);

    setcolor(WHITE);
    for (int y = groundY + 12; y < WINDOW_HEIGHT - 12; y += 36) {
        int halfWidth = 10 + (y - groundY) / 4;
        line(WINDOW_WIDTH / 2 - halfWidth, y, WINDOW_WIDTH / 2 + halfWidth, y);
    }

    setfillstyle(SOLID_FILL, LIGHTGRAY);
    bar(40, groundY - 44, 160, groundY);
    bar(178, groundY - 72, 300, groundY);
    setcolor(BLACK);
    rectangle(40, groundY - 44, 160, groundY);
    rectangle(178, groundY - 72, 300, groundY);

    setfillstyle(SOLID_FILL, YELLOW);
    for (int x = 56; x < 285; x += 34) {
        bar(x, groundY - 32, x + 14, groundY - 18);
    }

    setcolor(BLACK);
    drawText(76, groundY - 60, "GZU AIR");
}

void drawBackground() {
    setbkcolor(LIGHTCYAN);
    cleardevice();

    setfillstyle(SOLID_FILL, LIGHTCYAN);
    bar(0, 0, WINDOW_WIDTH, WINDOW_HEIGHT);

    setfillstyle(SOLID_FILL, YELLOW);
    setcolor(YELLOW);
    fillellipse(WINDOW_WIDTH - 95, 86, 42, 42);
    for (int i = 0; i < 12; ++i) {
        int x1 = WINDOW_WIDTH - 95;
        int y1 = 86;
        int x2 = x1 + (i % 4 - 1) * 28;
        int y2 = y1 + (i / 4 - 1) * 24;
        line(x1, y1, x2, y2);
    }

    drawCloud(150, 96, 1);
    drawCloud(470, 138, 1);
    drawCloud(760, 106, 1);
    drawCloud(1060, 154, 1);

    int mountain1[] = {0, 500, 160, 315, 340, 500};
    int mountain2[] = {250, 500, 470, 285, 710, 500};
    int mountain3[] = {610, 500, 805, 330, 1000, 500};
    fillClosedPolygon(mountain1, 3, LIGHTGRAY, BLACK);
    fillClosedPolygon(mountain2, 3, DARKGRAY, BLACK);
    fillClosedPolygon(mountain3, 3, LIGHTGRAY, BLACK);

    drawRunway();

    setfillstyle(SOLID_FILL, WHITE);
    bar(0, 0, WINDOW_WIDTH, 48);
    setcolor(BLACK);
    rectangle(0, 0, WINDOW_WIDTH - 1, 48);
    settextstyle(DEFAULT_FONT, HORIZ_DIR, 2);
    drawCenteredText(0, WINDOW_WIDTH, 10, "PART F - JET FLYING ACROSS THE SCREEN");
    settextstyle(DEFAULT_FONT, HORIZ_DIR, 1);
}

void drawJet(int x, int y, int frame) {
    int outerExhaust[] = {
        x - 154, y + 18,
        x - 222 - (frame % 2) * 14, y + 34,
        x - 154, y + 60
    };
    int innerExhaust[] = {
        x - 157, y + 27,
        x - 200 - (frame % 2) * 10, y + 34,
        x - 157, y + 51
    };
    fillClosedPolygon(outerExhaust, 3, LIGHTRED, RED);
    fillClosedPolygon(innerExhaust, 3, YELLOW, YELLOW);

    int rearNozzle[] = {
        x - 152, y + 16,
        x - 116, y + 20,
        x - 116, y + 56,
        x - 154, y + 60
    };
    fillClosedPolygon(rearNozzle, 4, DARKGRAY, BLACK);

    int body[] = {
        x - 130, y + 10,
        x + 110, y + 10,
        x + 165, y + 34,
        x + 110, y + 58,
        x - 130, y + 58,
        x - 160, y + 34
    };
    fillClosedPolygon(body, 6, LIGHTGRAY, BLACK);

    int topWing[] = {
        x - 24, y + 10,
        x + 38, y - 72,
        x + 86, y + 10
    };
    int bottomWing[] = {
        x - 24, y + 58,
        x + 40, y + 126,
        x + 86, y + 58
    };
    fillClosedPolygon(topWing, 3, BLUE, BLACK);
    fillClosedPolygon(bottomWing, 3, BLUE, BLACK);

    int tailTop[] = {
        x - 126, y + 14,
        x - 90, y - 52,
        x - 68, y + 18
    };
    int tailBottom[] = {
        x - 124, y + 56,
        x - 86, y + 94,
        x - 65, y + 54
    };
    fillClosedPolygon(tailTop, 3, RED, BLACK);
    fillClosedPolygon(tailBottom, 3, RED, BLACK);

    setfillstyle(SOLID_FILL, LIGHTBLUE);
    setcolor(BLACK);
    fillellipse(x + 58, y + 24, 34, 12);
    ellipse(x + 58, y + 24, 0, 360, 34, 12);

    setfillstyle(SOLID_FILL, WHITE);
    bar(x - 72, y + 24, x - 50, y + 42);
    bar(x - 36, y + 24, x - 14, y + 42);
    bar(x, y + 24, x + 22, y + 42);
    setcolor(BLACK);
    rectangle(x - 72, y + 24, x - 50, y + 42);
    rectangle(x - 36, y + 24, x - 14, y + 42);
    rectangle(x, y + 24, x + 22, y + 42);

}

void drawHud(bool paused, bool saved) {
    setfillstyle(SOLID_FILL, LIGHTGRAY);
    bar(0, WINDOW_HEIGHT - 34, WINDOW_WIDTH, WINDOW_HEIGHT);
    setcolor(BLACK);
    rectangle(0, WINDOW_HEIGHT - 34, WINDOW_WIDTH - 1, WINDOW_HEIGHT - 1);

    if (saved) {
        drawCenteredText(0, WINDOW_WIDTH, WINDOW_HEIGHT - 23, "Saved part_f_jet_output.bmp. P pauses, R restarts, S saves, Q/Esc closes.");
    } else if (paused) {
        drawCenteredText(0, WINDOW_WIDTH, WINDOW_HEIGHT - 23, "Paused. Press P to continue, R to restart, S to save, Q/Esc to close.");
    } else {
        drawCenteredText(0, WINDOW_WIDTH, WINDOW_HEIGHT - 23, "Jet animation running. P pauses, R restarts, S saves, Q/Esc closes.");
    }
}

void drawFrame(int jetX, int frame, bool paused = false, bool saved = false) {
    drawBackground();
    drawJet(jetX, 180, frame);
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
    initwindow(WINDOW_WIDTH, WINDOW_HEIGHT, "Part F - Jet Flying Across The Screen");

    int frame = 0;
    int jetX = -220;
    int drawPage = 0;
    bool paused = false;

    if (hasOption(argc, argv, "--save-and-exit")) {
        saveOutputImage(520, 18, false, drawPage);
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
                jetX = -220;
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
            if (jetX > WINDOW_WIDTH + 230) {
                jetX = -230;
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
