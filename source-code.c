#include "raylib.h"
#include <string.h>
#include <stdio.h>
#include <stdlib.h>

int BufferCapacity = 10000;
char *buffer;
char pathBuffer[256] = "path to save or open";
int pathLetterCount = 0;
bool pathActivated = false;
int letterCount = 0;
int scrollY = 0;
int scrollX = 0;
int textPos = 0;
char cursorChar[2] = "|";
Texture2D bgTexture;

int main(void) {
    buffer = malloc(BufferCapacity);
    strcpy(buffer, "Click on the path thing to write the path\n (with the file) then click open to open it \n save to save it");
    // i am terry davis on a opposite day
    SetConfigFlags(FLAG_WINDOW_RESIZABLE);
    InitWindow(800, 800, "SealPad44");
    bgTexture = LoadTexture("wallpaper.png");
    printf("Texture width: %d, height: %d\n", bgTexture.width, bgTexture.height);
    InitAudioDevice();
    Sound sealNoise = LoadSound("fok.mp3");
    SetTargetFPS(60);

    Font ProfatrialTTF = LoadFont("ProfaTrial-LightExpanded.ttf");

    while (!WindowShouldClose()) {
        BeginDrawing();

        // mouse and window info
        Vector2 mousePos = GetMousePosition();
        int currentX = GetScreenWidth();
        int currentY = GetScreenHeight();

        // background
        DrawTexturePro(bgTexture,
                       (Rectangle){0, 0, bgTexture.width, bgTexture.height},
                       (Rectangle){0, 0, currentX, currentY},
                       (Vector2){0, 0},
                       0.0f,
                       WHITE);

        // rectangle definitions for buttons (used for click detection below)
        Rectangle saveButton = {currentX - 100, 0, 100, 50};
        float wheel = GetMouseWheelMove();
        Rectangle openRectangle = {currentX - 100, 50, 100, 50};

        // main text
        Vector2 SizeForOpen = MeasureTextEx(ProfatrialTTF, "open", 32, 2);
        DrawTextEx(ProfatrialTTF, buffer, (Vector2){scrollX + 20, 55 + scrollY}, 32, 2, WHITE);

        // cursor position calculation
        char *beforeCursor = malloc(BufferCapacity);
        memcpy(beforeCursor, buffer, textPos);
        beforeCursor[textPos] = 0;

        Vector2 cursorSize = MeasureTextEx(ProfatrialTTF, beforeCursor, 32, 2);
        free(beforeCursor);
        DrawTextEx(ProfatrialTTF, "|", (Vector2){scrollX + 20 + cursorSize.x, 55 + scrollY + cursorSize.y - 32}, 32, 2, RED);

        Vector2 size2 = MeasureTextEx(ProfatrialTTF, "yeni yazi", 32, 2);
        Vector2 size = MeasureTextEx(ProfatrialTTF, "Save", 32, 2);

        bool IsInside = CheckCollisionPointRec(mousePos, saveButton);

        Rectangle pathbutton = {0, 0, currentX - 100, 50};
        bool InPath = CheckCollisionPointRec(mousePos, pathbutton);
        bool InOpen = CheckCollisionPointRec(mousePos, openRectangle);

        // top bar UI, drawn last so it always stays on top of the text
        DrawRectangle(currentX - 100, 0, 100, 50, BLUE);
        DrawRectangle(0, 0, currentX, 50, BLACK);
        DrawRectangleRec(openRectangle, BLUE);
        DrawTextEx(ProfatrialTTF, "open", (Vector2){(currentX - 100) + 50 - (SizeForOpen.x / 2), 50 + 25 - (SizeForOpen.y / 2)}, 32, 2, WHITE);
        DrawTextEx(ProfatrialTTF, "save", (Vector2){(currentX - 100) + 50 - (size.x / 2), 0 + 25 - (size.y / 2)}, 32, 2, RED);
        DrawTextEx(ProfatrialTTF, pathBuffer, (Vector2){10, 5}, 32, 2, RED);

        // INPUT
        if (IsKeyPressed(KEY_LEFT) && textPos > 0) {
            textPos--;
        }
        if (IsKeyPressed(KEY_RIGHT) && textPos < letterCount) {
            textPos++;
        }
        if (!IsKeyDown(KEY_LEFT_SHIFT)) {
            scrollY += wheel * 50;
        } else {
            scrollX += wheel * 20;
        }

        // open file
        if (InOpen && IsMouseButtonPressed(MOUSE_BUTTON_LEFT)) {
            FILE *openfile = fopen(pathBuffer, "r");
            if (openfile != NULL) {
                fseek(openfile, 0, SEEK_END);
                long fileSize = ftell(openfile);
                fseek(openfile, 0, SEEK_SET);

                if (fileSize + 1 > BufferCapacity) {
                    BufferCapacity = fileSize + 1;
                    buffer = realloc(buffer, BufferCapacity);
                }

                letterCount = fread(buffer, 1, BufferCapacity - 1, openfile);
                buffer[letterCount] = 0;
                fclose(openfile);
                PlaySound(sealNoise);
                textPos = letterCount;
            }
        }

        // activate path editing
        if (InPath && IsMouseButtonPressed(MOUSE_BUTTON_LEFT)) {
            pathActivated = true;
        }

        // save file
        if (IsInside && IsMouseButtonPressed(MOUSE_BUTTON_LEFT)) {
            FILE *f = fopen(pathBuffer, "w");
            fprintf(f, "%s", buffer);
            fclose(f);
            PlaySound(sealNoise);
        }

        int currentKey = GetCharPressed();

        // editing the path text
        if (pathActivated) {
            if (currentKey > 0) {
                pathBuffer[pathLetterCount] = currentKey;
                pathLetterCount++;
                pathBuffer[pathLetterCount] = 0;
            }
            if (IsKeyPressed(KEY_BACKSPACE) && (pathLetterCount > 0)) {
                pathLetterCount--;
                pathBuffer[pathLetterCount] = 0;
            }
            if (IsKeyPressed(KEY_ENTER)) {
                pathActivated = false;
            }
        }

        // editing the main text
        if (!pathActivated) {
            // add a typed character at the cursor position
            if (currentKey > 0) {
                if (letterCount + 1 >= BufferCapacity) {
                    BufferCapacity *= 2;
                    buffer = realloc(buffer, BufferCapacity);
                }
                memmove(&buffer[textPos + 1], &buffer[textPos], letterCount - textPos + 1);
                buffer[textPos] = currentKey;
                textPos++;
                letterCount++;
                buffer[letterCount] = 0;
            }
            // remove the character before the cursor
            if (IsKeyPressed(KEY_BACKSPACE) && (textPos > 0)) {
                memmove(&buffer[textPos - 1], &buffer[textPos], letterCount - textPos + 1);
                textPos--;
                letterCount--;
                buffer[letterCount] = 0;
            }
            // remove the character after the cursor
            if (IsKeyPressed(KEY_DELETE) && (textPos < letterCount)) {
                memmove(&buffer[textPos], &buffer[textPos + 1], letterCount - textPos);
                letterCount--;
            }
            // insert a newline at the cursor position
            if (IsKeyPressed(KEY_ENTER)) {
                textPos++;
                buffer[letterCount] = '\n';
                letterCount++;
                buffer[letterCount] = 0;
            }
        }

        EndDrawing();
    }
    CloseWindow();
    UnloadTexture(bgTexture);
    UnloadSound(sealNoise);
    UnloadFont(ProfatrialTTF);
    free(buffer);
    return 0;
}
