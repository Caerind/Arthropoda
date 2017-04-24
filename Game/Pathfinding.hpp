#ifndef PATHFINDING_HPP
#define PATHFINDING_HPP

#include "../Sources/System/MapUtility.hpp"

#include <map>
#include <list>

class Node
{
	public:
		Node() : gScore(0), fScore(0), parent(nullptr), visited(false) {}
		Node(I32 _gScore, I32 _fScore) : score(_gScore), gScore(_gScore), fScore(_fScore), parent(nullptr), visited(false) {}
		I32 score;
		I32 gScore;
		I32 fScore;
		oe::Vector2i position;
		Node* parent;
		bool visited;
};

class CollisionMatrix
{
    public:
		CollisionMatrix() {}
		CollisionMatrix(I32 x, I32 y) { create(x, y); }
		~CollisionMatrix() { clear(); }

        void create(I32 x, I32 y)
		{
			clear();
			mSize.set(x, y);
			for (I32 i = 0; i < x; i++)
			{
				for (I32 j = 0; j < y; j++)
				{
					mMap[std::pair<I32,I32>(i, j)] = false;
				}
			}
		}

		void clear() { mMap.clear(); }

        bool get(I32 x, I32 y) 
		{ 
			auto itr = mMap.find(std::pair<I32, I32>(x, y));
			if (itr != mMap.end())
			{
				return itr->second;
			}
			return true;
		}
		bool get(const oe::Vector2i& coords) { return get(coords.x, coords.y); }

        void set(I32 x, I32 y, const bool& val) { mMap[std::pair<I32,I32>(x, y)] = val; }
		void set(const oe::Vector2i& coords, const bool& val) { set(coords.x, coords.y, val); }

		const oe::Vector2i& getSize() const { return mSize; }
		void setSize(const oe::Vector2i& size)
		{
			clear();
			mSize.set(size);
		}

    private:
        std::map<std::pair<I32,I32>,bool> mMap;
		oe::Vector2i mSize;
};

class NodeMatrix
{
	public:
		NodeMatrix() {}
		NodeMatrix(const oe::Vector2i& size) { mSize.set(size); }
		~NodeMatrix() { clear(); }

		void clear()
		{
			for (auto itr = mMap.begin(); itr != mMap.end(); ++itr)
			{
				delete itr->second;
			}
			mMap.clear();
		}

		Node* get(I32 x, I32 y) { return mMap.find(std::pair<I32, I32>(x, y))->second; }
		Node* get(const oe::Vector2i& coords) { return get(coords.x, coords.y); }

		void set(I32 x, I32 y, Node* node) { mMap[std::pair<I32, I32>(x, y)] = node; }
		void set(const oe::Vector2i& coords, Node* node) { set(coords.x, coords.y, node); }

		const oe::Vector2i& getSize() const { return mSize; }

	private:
		std::map<std::pair<I32, I32>, Node*> mMap;
		oe::Vector2i mSize;
};

class AStar
{
	public:
		static bool run(std::list<oe::Vector2i>& path, const oe::Vector2i& start, const oe::Vector2i& end, CollisionMatrix& map)
		{
			path.clear();

			if (start == end || start.x < 0 || start.y < 0)
			{
				return false;
			}

			oe::Vector2i size(map.getSize());

			NodeMatrix closeList(size);
			std::list<Node*> openList;

			// RESET
			for (I32 i = 0; i < size.x; i++)
			{
				for (I32 j = 0; j < size.y; j++)
				{
					closeList.set(i, j, new Node((map.get(i, j)) ? 1 : 0, 0));
				}
			}

			//VERIF
			if (isWall(end, closeList))
			{
				return false;
			}

			// FIRST NODE
			Node* n = closeList.get(start);
			n->position.set(start);
			n->gScore = 0;
			n->fScore = n->gScore + heuristic(start, end);

			openList.push_front(n);

			while (!openList.empty())
			{
				openList.sort(compareNode);
				Node* current = openList.front(); // Take the most interesting node

				if (current->position == end) // Check if we are the end
				{
                    Node* nend = closeList.get(end);
                    while (nend != nullptr)
                    {
						if (nend->parent != nullptr)
						{
							path.emplace_front(nend->position);
						}
                        nend = nend->parent;
                    }
					return true;
				}

				// If the end wasn't reached

				std::list<Node*> neighbors;
				getNeighbors(neighbors, current, closeList);

				openList.pop_front();
				current->visited = true; // add to closed list

				while (!neighbors.empty())
				{
					Node* neighbor = neighbors.front();
					neighbors.pop_front();
					if (!neighbor->visited) // If not in the closed list
					{
						I32 gscore = current->gScore + current->score;
						if (!isInOpenList(neighbor, openList) || gscore < neighbor->gScore)
						{
							neighbor->parent = current;
							neighbor->gScore = gscore;
							neighbor->fScore = neighbor->gScore + heuristic(neighbor->position, end);
							if (!isInOpenList(neighbor, openList))
							{
								openList.push_front(neighbor);
							}
						}
					}
				}
			}
			return false;
		}

		static bool canGo(const oe::Vector2i& start, const oe::Vector2i& end, CollisionMatrix& map)
		{
			oe::Vector2i size(map.getSize());
			NodeMatrix closeList(size);
			std::list<Node*> openList;
			for (I32 i = 0; i < size.x; i++)
			{
				for (I32 j = 0; j < size.y; j++)
				{
					closeList.set(i, j, new Node((map.get(i, j)) ? 1 : 0, 0));
				}
			}
			Node* n = closeList.get(start);
			n->position.set(start);
			n->gScore = 0;
			n->fScore = n->gScore + heuristic(start, end);
			openList.push_front(n);
			while (!openList.empty())
			{
				openList.sort(compareNode);
				Node* current = openList.front(); // Take the most interesting node

				if (current->position == end) // Check if we are the end
				{
					return true;
				}

				// If the end wasn't reached

				std::list<Node*> neighbors;
				getNeighbors(neighbors, current, closeList);

				openList.pop_front();
				current->visited = true; // add to closed list

				while (!neighbors.empty())
				{
					Node* neighbor = neighbors.front();
					neighbors.pop_front();
					if (!neighbor->visited) // If not in the closed list
					{
						I32 gscore = current->gScore + current->score;
						if (!isInOpenList(neighbor, openList) || gscore < neighbor->gScore)
						{
							neighbor->parent = current;
							neighbor->gScore = gscore;
							neighbor->fScore = neighbor->gScore + heuristic(neighbor->position, end);
							if (!isInOpenList(neighbor, openList))
							{
								openList.push_front(neighbor);
							}
						}
					}
				}
			}
			return false;
		}

		static I32 heuristic(const oe::Vector2i& p1, const oe::Vector2i& p2)
		{
			I32 dx = p2.x - p1.x;
			I32 dy = p2.y - p1.y;
			return (I32)std::sqrt(dx * dx + dy * dy);
		}

	private:
		static bool isInOpenList(const Node* n, std::list<Node*>& openList) // Check if the node is in the openlist
		{
			std::list<Node*>::iterator end(openList.end());
			return std::find(openList.begin(), end, n) != end;
		}

		static bool isVisited(const oe::Vector2i& position, NodeMatrix& closeList) // Return if the position in the map has already been visited
		{
			return closeList.get(position)->visited;
		}

		static bool isWall(const oe::Vector2i& position, NodeMatrix& closeList) // Return if the point at this position is a wall
		{
			return closeList.get(position)->gScore == 1;// 1 = collision
		}

		static void getNeighbors(std::list<Node*>& neighbors, const Node* node, NodeMatrix& closeList) // Get neighbors nodes of a node (point)
		{
			std::vector<oe::Vector2i> positionsNeighbors;
			oe::MapUtility::getNeighboors(positionsNeighbors, node->position, oe::MapUtility::Orientation::Hexagonal);

			for (oe::Vector2i& p : positionsNeighbors)
			{
				if (p.x >= 0 && p.y >= 0 && p.x < closeList.getSize().x && p.y < closeList.getSize().y)
				{
					Node* newnode = closeList.get(p);
					newnode->position.set(p);
					if (!isWall(newnode->position, closeList))
					{
						neighbors.push_back(newnode);
					}
				}
			}
		}

		static bool compareNode(const Node* first, const Node* second)
		{
			return first->fScore < second->fScore;
		}
};

class Distance
{
    public:
		static void run(std::vector<oe::Vector2i>& reachables, const oe::Vector2i& start, U32 length, CollisionMatrix& map)
		{
			reachables.clear();

			if (length == 0)
			{
				return;
			}

			std::vector<oe::Vector2i> neighbors;
			std::vector<oe::Vector2i> checks;
			std::vector<oe::Vector2i> futurChecks;

			reachables.push_back(start);
			checks.push_back(start);

			for (U32 i = 0; i < length; i++)
			{
				for (const oe::Vector2i& check : checks)
				{
					neighbors.clear();
					oe::MapUtility::getNeighboors(neighbors, check, oe::MapUtility::Orientation::Hexagonal);
					for (const oe::Vector2i& neighbor : neighbors)
					{
						if (!has(neighbor, reachables) && map.get(neighbor) != 1) // isnt a wall
						{
							reachables.emplace_back(neighbor);
							futurChecks.emplace_back(neighbor);
						}
					}
				}
				checks.clear();
				for (const oe::Vector2i& check : futurChecks)
				{
					checks.emplace_back(check);
				}
				futurChecks.clear();
			}
		}

		static void run(std::vector<oe::Vector2i>& reachables, const oe::Vector2i& start, U32 length, I32 mapSizeX, I32 mapSizeY)
		{
			reachables.clear();

			if (length == 0)
			{
				return;
			}

			std::vector<oe::Vector2i> neighbors;
			std::vector<oe::Vector2i> checks;
			std::vector<oe::Vector2i> futurChecks;

			reachables.push_back(start);
			checks.push_back(start);

			for (U32 i = 0; i < length; i++)
			{
				for (const oe::Vector2i& check : checks)
				{
					neighbors.clear();
					oe::MapUtility::getNeighboors(neighbors, check, oe::MapUtility::Orientation::Hexagonal);
					for (const oe::Vector2i& neighbor : neighbors)
					{
						if (neighbor.x < mapSizeX && neighbor.y < mapSizeY && !has(neighbor, reachables))
						{
							reachables.emplace_back(neighbor);
							futurChecks.emplace_back(neighbor);
						}
					}
				}
				checks.clear();
				for (const oe::Vector2i& check : futurChecks)
				{
					checks.emplace_back(check);
				}
				futurChecks.clear();
			}
		}

	private:
		static bool has(const oe::Vector2i& value, std::vector<oe::Vector2i>& in)
		{
			for (const oe::Vector2i& v : in)
			{
				if (value == v)
				{
					return true;
				}
			}
			return false;
		}
};

#endif // PATHFINDING_HPP
