#include <iostream>
#include <vector>
#include <string>

#include "raylib.h"
#include "algs.h"

using namespace std;

vector<int> getStart(bool debug) {

    cout << "Enter the start node (x, y): ";
    vector<int> start(2);
    cin >> start[0] >> start[1];
    cout << endl;

    if (debug) {
        cout << "Start node: (" << start[0] << ", " << start[1] << ")" << endl;
        cout << endl;
    }

    return start;
}

vector<int> getTarget(bool debug) {
    
    cout << "Enter the target node (x, y): ";
    vector<int> target(2);
    cin >> target[0] >> target[1];
    cout << endl;

    if (debug) {
        cout << "Target node: (" << target[0] << ", " << target[1] << ")" << endl;
        cout << endl;
    }

    return target;
}

vector<vector<int>> getGraph(bool debug) {
    cout << "Enter graph (0 - empty, X - obstacle, 'done' on new line when done):" << endl;
    // Example graph input:
    // 000000
    // 000X00
    // 000X00
    // 000000

    // Grid from question:
    // 0000000
	// 000X000
	// 000X000
    // 000X000
	// 0000000
    vector<vector<int>> grid;
    int r = 0;
    string line;
    int linesize = 0;
    while (cin >> line) {
        if (line == "done") break;

        if (r == 0) {
            linesize = line.size();
        }
        else {
            if (line.size() != linesize) {
                cout << "Invalid graph input: inconsistent line sizes." << endl;
                return vector<vector<int>>();
            }
        }

        grid.push_back(vector<int>());
        for (char c : line) {
            if (c == '0') {
                grid[r].push_back(0);
            }
            else if (c == 'X') {
                grid[r].push_back(1);
            }
            else {
                cout << "Invalid character in graph input: " << c << endl;
                return vector<vector<int>>();
            }
        }
        r++;
    }

    cout << "Graph is ready." << endl;
    cout << endl;

    if (debug) {
        // Sample output:
        // 000000
        // 000100
        // 000100
        // 000000
        for (int i = 0; i < grid.size(); i++) {
            for (int j = 0; j < grid[i].size(); j++) {
                cout << grid[i][j];
            }
            cout << endl;
        }
        cout << endl;
    }

    return grid;
}

void makeGUIGrid(vector<vector<int>>& g, Vector2 size, vector<int> start, vector<int> target) {
    // void DrawRectangleV(Vector2 position, Vector2 size, Color color);  

    for (int i = 0; i < g.size(); i++) {
        for (int j = 0; j < g[0].size(); j++) {
            Vector2 pos = { j * size.x, i * size.y};
            Rectangle box = { pos.x, pos.y, size.x, size.y };
            if (vector<int>{ j,i } == start) {
                DrawRectangleRec(box, SKYBLUE);
            }
            else if (vector<int>{j, i} == target) {
                DrawRectangleRec(box, RED);
            }
            else {
                DrawRectangleRec(box, g[i][j] ? DARKGRAY : RAYWHITE);
            }

            DrawRectangleLinesEx(box, 0.5f, DARKGRAY);
        }
    }
    
}

void drawPath(const vector<vector<int>>& path, Vector2 size, int dsize, int ssize, Color c) {
    //void DrawLineDashed(Vector2 startPos, Vector2 endPos, int dashSize, int spaceSize, Color color);
    for (int i = 0; i < path.size() - 1; i++) {
        Vector2 start = { path[i][0] * size.x + 0.5 * size.x, path[i][1] * size.y + 0.5 * size.y};

        Vector2 end = { path[i+1][0] * size.x + 0.5 * size.x, path[i+1][1] * size.y + 0.5 * size.y };

        DrawLineDashed(start, end, dsize, ssize, c);
    }
}

struct LegendEntry {
    string label;
    Color color;
    int dashSize;
    int spaceSize;
};

void drawLegend(const vector<LegendEntry>& entries) {
    if (entries.empty()) return;

    const int screenW = GetScreenWidth();
    const int screenH = GetScreenHeight();

    // Scale with window size (clamped so it stays readable)
    const int fontSize = max(12, min(24, screenH / 30));
    const int margin = max(6, screenH / 60);
    const int padding = fontSize / 2;
    const int sampleLen = fontSize * 3;   // length of the dashed sample line
    const int rowHeight = fontSize + padding;

    // Box width depends on the longest label
    int maxTextW = 0;
    for (const LegendEntry& e : entries) {
        maxTextW = max(maxTextW, MeasureText(e.label.c_str(), fontSize));
    }

    const int boxW = padding + sampleLen + padding + maxTextW + padding;
    const int boxH = padding + (int)entries.size() * rowHeight;
    const int boxX = screenW - boxW - margin;
    const int boxY = margin;

    // Opaque white background + border
    Rectangle box = { (float)boxX, (float)boxY, (float)boxW, (float)boxH };
    DrawRectangleRec(box, WHITE);
    DrawRectangleLinesEx(box, 1.5f, DARKGRAY);

    // Rows
    for (size_t i = 0; i < entries.size(); i++) {
        const LegendEntry& e = entries[i];
        const int rowY = boxY + padding + (int)i * rowHeight;
        const float lineY = rowY + fontSize / 2.0f;

        Vector2 a = { (float)(boxX + padding), lineY };
        Vector2 b = { (float)(boxX + padding + sampleLen), lineY };
        DrawLineDashed(a, b, e.dashSize, e.spaceSize, e.color);

        DrawText(e.label.c_str(), boxX + padding + sampleLen + padding, rowY, fontSize, BLACK);
    }
}

void makeGUI(vector<vector<int>> grid, vector<int> start, vector<int> target, vector<vector<int>> dfsRes, vector<vector<int>> bfsRes, vector<vector<int>> astarRes, vector<int> costs) {
	const int screenWidth = 1000;
	const int screenHeight = 800;
	SetTraceLogLevel(LOG_NONE);
	InitWindow(screenWidth, screenHeight, "Graph Search");
	SetTargetFPS(60);

	Vector2 size = { screenWidth / grid[0].size(), screenHeight / grid.size() };

    while (!WindowShouldClose())    // Detect window close button or ESC key
    {
        // Update
        //----------------------------------------------------------------------------------
        // TODO: Update your variables here
        //----------------------------------------------------------------------------------

        // Draw
        //----------------------------------------------------------------------------------
        BeginDrawing();
        ClearBackground(RAYWHITE);
        
		makeGUIGrid(grid, size, start, target);
        if (dfsRes.size() == 1) {
			DrawCircle(dfsRes[0][0] * size.x + 0.5 * size.x, dfsRes[0][1] * size.y + 0.5 * size.y, 10, RED);
        }
        else {
            drawPath(dfsRes, size, 10, 6, RED);
        }

        if (bfsRes.size() == 1) {
            DrawCircle(bfsRes[0][0] * size.x + 0.5 * size.x, bfsRes[0][1] * size.y + 0.5 * size.y, 10, BLUE);
        }
        else {
            drawPath(bfsRes, size, 5, 5, BLUE);
        }

        if (astarRes.size() == 1) {
            DrawCircle(astarRes[0][0] * size.x + 0.5 * size.x, astarRes[0][1] * size.y + 0.5 * size.y, 10, BLACK);
        }
        else {
            drawPath(astarRes, size, 2, 4, BLACK);
        }

        drawLegend({{ "DFS (cost " + to_string(costs[0]) + ")", RED, 10, 6},
                    { "BFS (cost " + to_string(costs[1]) + ")", BLUE, 5, 5},
                    { "A* (cost " + to_string(costs[2]) + ")", BLACK, 2, 4},
                   });
        

        EndDrawing();
        //----------------------------------------------------------------------------------
    }

    // De-Initialization
    //--------------------------------------------------------------------------------------
    CloseWindow();
}

vector<vector<int>> createGrid();

int main() {

    bool debug = true;

    cout << "Please enter the your graph details." << endl;
    
    vector<int> start = getStart(debug);
	vector<int> target = getTarget(debug);
	vector<vector<int>> grid = getGraph(debug);
    if (grid[0].empty()) return 1;

	cout << "Starting GUI..." << endl;

	pair<vector<vector<int>>, int> dfsResults = dfs(start, target, grid);

    //cout << "DFS cost: " << dfsResults.second << endl;

    pair<vector<vector<int>>, int> bfsResults = bfs(start, target, grid);
    //cout << "BFS cost: " << bfsResults.second << endl;

	pair<vector<vector<int>>, int> astarResults = astar(start, target, grid);

    makeGUI(grid, start, target, dfsResults.first, bfsResults.first, astarResults.first, { dfsResults.second, bfsResults.second, astarResults.second  });


    cout << "GUI closed." << endl;

    return 0;
}