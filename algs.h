#pragma once
#include <vector>
#include <string>
#include <set>
#include <utility>
#include <queue>
#include <algorithm>

using namespace std;

// DFS algorithm
// Start up
//		Set all nodes to not visited
//		Set start node to square(2, 1) (see diagram below)
//		Set start node’s parent to null
//		Set goal node to square(2, 5) (see diagram below)
//		Call Traverse(start node)
//		
//		Traverse(Node n)
//			Mark node n as visited
//			If node n is the goal node
//				Print all nodes starting from n and traversing
//					through all ancestors(parents)
//				Halt entire process
//			For each node t that is an 8 - connected neighbor node of n
//				If node t has not been visited and node t is not a wall
//					Set parent of node t to node n
//					Call Traverse(node t)

static bool checkBounds(int x, int y, vector<vector<int>>& grid) {
	return (x >= 0 && x < grid[0].size() && 
			y >= 0 && y < grid.size() && 
			grid[y][x] == 0);
}

static void dfsTraverse(vector<int> node, vector<vector<int>>& grid, vector<bool>& visited, vector<vector<int>>& path, vector<int> target, vector<vector<int>>& neighbors, int& cost) {

	// Mark node as visited
	visited[node[1] * grid[0].size() + node[0]] = true;

	// Check if node is the target
	if (node == target) {
		return;
	}

	// For each neighbor of node
	for (vector<int> n : neighbors) {

		// Get neighbor coordinates
		vector<int> neighbor = { node[0] + n[0], node[1] + n[1] };
		
		// Check if neighbor is within bounds/isn't a wall and not visited
		if (checkBounds(neighbor[0], neighbor[1], grid)) {
			if (!visited[neighbor[1] * grid[0].size() + neighbor[0]]) {

				path.push_back(neighbor);
				cost += n[2];
				dfsTraverse(neighbor, grid, visited, path, target, neighbors, cost);
				if (path[path.size() - 1] == target) {
					return;
				}
				path.pop_back();
				cost -= n[2];
			}
		}
	}

}


pair<vector<vector<int>>, int> dfs(vector<int> start, vector<int> target, vector<vector<int>>& grid) {

	// Initialize visited vector
	// Point (x,y) to index y * grid[0].size() + x
	vector<bool> visited(grid.size() * grid[0].size(), false);
	vector<vector<int>> path = { start };
	int cost = 0;
	vector<vector<int>> neighbors = { {-1, -1, 14}, {0, -1, 10}, {1, -1, 14}, {-1, 0, 10}, {1, 0, 10}, {-1, 1, 14}, {0, 1, 10}, {1,1, 14} };
	dfsTraverse(start, grid, visited, path, target, neighbors, cost);

	return { path, cost };
}

// BFS Algorithm
// Start up
//		Set all nodes to not visited
//		Set start node to square(2, 1) (see figure below)
//		Set start node’s parent to null
//		Set goal node to square(2, 5) (see figure below)
//		Initialize queue to empty
//		Call Traverse(start node)
//
//		Traverse(Node n)
//			Mark node n as visited
//			Add node n to the back of the queue
//			While the queue is not empty
//				Remove the front node from the queue, call it node t
//				If node t is the goal node
//					Print all nodes starting from t and traversing through
//						all ancestors(parents)
//					Halt entire process
//				For each node u that is an 8 - connected neighbor node of t
//					If node u has not been visited and node u is not a wall
//						Set parent of node u to node t
//						Mark node u as visited
//						Add node u to the back of the queue

static void bfsTraverse(vector<int> node, vector<vector<int>>& grid, vector<bool>& visited, vector<vector<int>>& parents, vector<int> target, vector<vector<int>>& neighbors, queue<vector<int>>& q) {

	// Mark node as visited
	visited[node[1] * grid[0].size() + node[0]] = true;

	parents[node[1] * grid[0].size() + node[0]][0] = -2; // Start node has no parent

	q.push(node);
	
	while (!q.empty()) {
		vector<int> t = q.front();
		q.pop();

		if (t == target) {
			return;
		}

		for (vector<int> n : neighbors) {
			vector<int> neighbor = { t[0] + n[0], t[1] + n[1] };

			// Check if neighbor is within bounds/isn't a wall and not visited
			if (checkBounds(neighbor[0], neighbor[1], grid)) {
				if (!visited[neighbor[1] * grid[0].size() + neighbor[0]]) {
					visited[neighbor[1] * grid[0].size() + neighbor[0]] = true;
					parents[neighbor[1] * grid[0].size() + neighbor[0]][0] = t[1] * grid[0].size() + t[0];
					parents[neighbor[1] * grid[0].size() + neighbor[0]][1] = n[2];
					q.push(neighbor);
				}
			}
		}


	}

}

pair<vector<vector<int>>, int> bfs(vector<int> start, vector<int> target, vector<vector<int>>& grid) {
	// Initialize visited vector
	// Point (x,y) to index y * grid[0].size() + x
	vector<bool> visited(grid.size() * grid[0].size(), false);
	vector<vector<int>> parents(grid.size() * grid[0].size(), {-1, 0});

	vector<vector<int>> neighbors = { {-1, -1, 14}, {0, -1, 10}, {1, -1, 14}, {-1, 0, 10}, {1, 0, 10}, {-1, 1, 14}, {0, 1, 10}, {1,1, 14} };

	queue<vector<int>> q;
	bfsTraverse(start, grid, visited, parents, target, neighbors, q);

	const int width = static_cast<int>(grid[0].size());
	vector<vector<int>> path;
	int idx = target[1] * width + target[0];

	// Target never reached -> no path
	if (parents[idx][0] == -1) {
		return { { start }, 0 };
	}

	int cost = 0;
	while (true) {
		path.push_back({ idx % width, idx / width });
		if (parents[idx][0] == -2) break; // reached start
		cost += parents[idx][1];
		idx = parents[idx][0];
	}

	reverse(path.begin(), path.end());
	return { path, cost };
}

// A* Algorithm
// Start up
//		Set all nodes to not visited
//		Set start node to square(2, 1) (see figure below)
//		Set start node’s parent to null
//		Set start node’s gscore and hscore to 0
//		Set goal node to square(2, 5) (see figure below)
//		Set goalnode’s gscore and hscore to 0
//		Initialize open queue to empty
//		Initialize closed queue to empty
//		Add start node to open queue
//		Call Traverse(start node)
//
//		Traverse(Node n)
//			While the open queue is not empty
//				Remove node with lowest fscore from open queue, call it
//					node c
//				If node c is the goal node
//					Print all nodes starting from c and traversing through
//						all ancestors(parents)
//					Halt entire process
//				Add node c to the closed queue
//				For each node n that is an 8 - connected neighbor of node c
//					If node n is not a wall node
//						Set parent of node n to node c
//							Add 10 to node n’s gscore if it is a horizontal or
//								verticalneighbor of node c, add 14 if it is a
//									diagonal neighbor
//							Set node n’s hscore to the “Manhattan” distance to the
//								goal node
//							If n is not on the closed queue
//								If this node n is on the open queue
//									If node n’s new gscore is less than it’s gscore
//											on the open queue
//										Replace node n on the open queue with this new
//											node n
//									Else
//										Do nothing with the new node n(throw it away)
//								Else
//									Add node n to the open queue

// Comparison function for A* priority queue (min heap on fscore)
// Queue entries are { index, gscore, hscore }
// Comparison function for A* priority queue (min heap on fscore)
// Queue entries are { index, gscore, hscore, order }
// Ties on fscore are broken by push order (FIFO) to keep raster scan order
struct AStarCmp {
	bool operator()(const vector<int>& a, const vector<int>& b) const {
		int fa = a[1] + a[2];
		int fb = b[1] + b[2];
		if (fa != fb) return fa > fb;	// lower fscore first
		return a[3] > b[3];				// tie: earlier push first
	}
};
using AStarQueue = priority_queue<vector<int>, vector<vector<int>>, AStarCmp>;

// "Manhattan" distance to target, scaled by 10 to match move vertical and horizontal costs
static int manhattan(int x, int y, const vector<int>& target) {
	return 10 * (abs(x - target[0]) + abs(y - target[1]));
}

static void astarTraverse(vector<vector<int>>& grid, vector<bool>& closed, vector<vector<int>>& parents, vector<int> target, vector<vector<int>>& neighbors, AStarQueue& openq, int& order) {

	const int width = static_cast<int>(grid[0].size());
	const int targetIdx = target[1] * width + target[0];

	while (!openq.empty()) {

		// Remove node with lowest fscore, call it node c
		vector<int> c = openq.top();
		openq.pop();
		int cIdx = c[0];

		// Skip stale entries (already closed, or replaced by a cheaper gscore)
		if (closed[cIdx] || c[1] != parents[cIdx][1]) {
			continue;
		}

		// Check if node is the target
		if (cIdx == targetIdx) {
			return;
		}

		// Add node c to the closed set
		closed[cIdx] = true;

		int cx = cIdx % width;
		int cy = cIdx / width;

		// For each neighbor of node c
		for (vector<int>& n : neighbors) {

			// Get neighbor coordinates
			int nx = cx + n[0];
			int ny = cy + n[1];

			// Check if neighbor is within bounds/isn't a wall
			if (!checkBounds(nx, ny, grid)) {
				continue;
			}

			int nIdx = ny * width + nx;

			// Ignore nodes already on the closed set
			if (closed[nIdx]) {
				continue;
			}

			// New scores through node c
			int g = c[1] + n[2];
			int h = manhattan(nx, ny, target);

			// Parent of -1 means node has never been added to the open queue
			bool onOpen = parents[nIdx][0] != -1;

			// Add if new, or "replace" if this path is cheaper
			if (!onOpen || g < parents[nIdx][1]) {
				parents[nIdx] = { cIdx, g, h };
				openq.push({ nIdx, g, h, order++ });
			}
		}
	}
}

pair<vector<vector<int>>, int> astar(vector<int> start, vector<int> target, vector<vector<int>>& grid) {

	const int width = static_cast<int>(grid[0].size());
	const int startIdx = start[1] * width + start[0];
	const int targetIdx = target[1] * width + target[0];

	// Initialize visited (closed) vector
	// Point (x,y) to index y * grid[0].size() + x
	vector<bool> visited(grid.size() * grid[0].size(), false);

	// Initialize parents vector
	// { parent, gscore, hscore} -> {-1 , -1, -1}
	vector<vector<int>> parents(grid.size() * grid[0].size(), { -1, -1, -1 });

	// Initialize start and target values
	parents[startIdx] = { -2, 0, 0 };
	parents[targetIdx] = { -1, 0, 0 };

	// Min heap priority queue of { index, gscore, hscore }
	// order is a push counter used to break fscore ties in raster order
	int order = 0;
	AStarQueue openq;
	openq.push({ startIdx, 0, 0, order++ });

	// Neighbor offsets and costs
	vector<vector<int>> neighbors = { {-1, -1, 14}, {0, -1, 10}, {1, -1, 14}, {-1, 0, 10}, {1, 0, 10}, {-1, 1, 14}, {0, 1, 10}, {1,1, 14} };

	astarTraverse(grid, visited, parents, target, neighbors, openq, order);

	// Target never reached -> no path
	if (parents[targetIdx][0] == -1) {
		return { { start }, 0 };
	}

	// Rebuild path from target back to start
	vector<vector<int>> path;
	int idx = targetIdx;
	while (true) {
		path.push_back({ idx % width, idx / width });
		if (parents[idx][0] == -2) break; // reached start
		idx = parents[idx][0];
	}

	reverse(path.begin(), path.end());

	// Target's gscore is the total path cost
	return { path, parents[targetIdx][1] };
}