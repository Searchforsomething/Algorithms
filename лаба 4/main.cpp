#include <iostream>
#include <vector>
#include <cmath>
#include <ctime>
#include <algorithm>
#include <limits>
#include <random>

#define GRAPH_SIZE 5
#define ANT_ROUND_COUNT 100

std::vector<std::vector<int>> graph(GRAPH_SIZE, std::vector<int>(GRAPH_SIZE));
std::vector<std::vector<double>> distance(GRAPH_SIZE, std::vector<double>(GRAPH_SIZE));
std::vector<std::vector<double>> pheromone(GRAPH_SIZE, std::vector<double>(GRAPH_SIZE, 0.5));
std::vector<int> alphaArr = {1};
std::vector<int> betaArr = {2};
int minCost = std::numeric_limits<int>::max();
std::vector<int> minPath(GRAPH_SIZE);
std::vector<int> visited(GRAPH_SIZE, 0);

void swap(int& x, int& y) {
    int temp = x;
    x = y;
    y = temp;
}

void permute(std::vector<int>& a, int l, int r, std::vector<int>& query, int& count) {
    if (l == r) {
        int b = 0;
        int k = std::pow(10, GRAPH_SIZE - 1);
        for (int i = 0; i <= r; i++) {
            b += k * a[i];
            k /= 10;
        }
        query[count++] = b;
    } else {
        for (int i = l; i <= r; i++) {
            swap(a[l], a[i]);
            permute(a, l + 1, r, query, count);
            swap(a[l], a[i]);
        }
    }
}

int summ(int querry) {
    int sum = 0;
    std::vector<int> stations(GRAPH_SIZE);
    stations[0] = (querry / static_cast<int>(std::pow(10, GRAPH_SIZE - 1))) - 1;
    for (int i = 1; i < GRAPH_SIZE; i++) {
        stations[i] = ((querry / static_cast<int>(std::pow(10, GRAPH_SIZE - i - 1))) % 10) - 1;
    }
    for (int i = 0; i < GRAPH_SIZE - 1; i++) {
        sum += graph[stations[i]][stations[i + 1]];
    }
    sum += graph[stations[GRAPH_SIZE - 1]][stations[0]];
    return sum;
}

void bruteForce() {
    std::vector<int> arr(GRAPH_SIZE);
    for (int i = 1; i <= GRAPH_SIZE; i++) {
        arr[i - 1] = i;
    }

    std::vector<int> query(std::pow(GRAPH_SIZE, GRAPH_SIZE));
    int count = 0;
    permute(arr, 0, GRAPH_SIZE - 1, query, count);

    int min = std::numeric_limits<int>::max();
    int min_way = std::numeric_limits<int>::max();
    for (int i = 0; i < count; i++) {
        int current_sum = summ(query[i]);
        if (current_sum < min) {
            min = current_sum;
            min_way = query[i];
        }
    }
    std::cout << "Minimal path: " << min_way << "  Minimal distance: " << min << "\n";
}

void branchAndBound(std::vector<int>& currentPath, int level, int currentCost) {
    if (level == GRAPH_SIZE) {
        if (graph[currentPath[level - 1]][currentPath[0]] != 0) {
            int finalCost = currentCost + graph[currentPath[level - 1]][currentPath[0]];
            if (finalCost < minCost) {
                minCost = finalCost;
                std::copy(currentPath.begin(), currentPath.end(), minPath.begin());
            }
        }
        return;
    }

    for (int i = 0; i < GRAPH_SIZE; i++) {
        if (!visited[i] && graph[currentPath[level - 1]][i] != 0) {
            visited[i] = 1;
            currentPath[level] = i;
            if (currentCost < minCost)
                branchAndBound(currentPath, level + 1, currentCost + graph[currentPath[level - 1]][i]);
            visited[i] = 0;
        }
    }
}

void startBranchAndBound() {
    std::vector<int> currentPath(GRAPH_SIZE);
    currentPath[0] = 0;
    visited[0] = 1;

    branchAndBound(currentPath, 1, 0);

    std::cout << "Minimal path: ";
    for (int i = 0; i < GRAPH_SIZE; i++) {
        std::cout << minPath[i] + 1 << " ";
    }
    std::cout << "\tMinimal distance: " << minCost << "\n";
}

void initialize() {
    for (auto& row : pheromone) {
        std::fill(row.begin(), row.end(), 0.5);
    }
}

void antTour(std::vector<std::vector<int>>& ants, int alpha, int beta) {
    std::random_device rd;
    std::mt19937 gen(rd());

    for (int k = 0; k < GRAPH_SIZE; k++) {
        int start = gen() % GRAPH_SIZE;
        int current = start;
        std::vector<int> visited(GRAPH_SIZE, 0);
        ants[k][0] = start;
        visited[start] = 1;
        for (int i = 1; i < GRAPH_SIZE; i++) {
            std::vector<double> prob(GRAPH_SIZE, 0.0);
            double prob_sum = 0.0;
            for (int j = 0; j < GRAPH_SIZE; j++) {
                if (visited[j] == 0) {
                    prob[j] = std::pow(pheromone[current][j], alpha) * std::pow(1.0 / distance[current][j], beta);
                    prob_sum += prob[j];
                }
            }
            std::uniform_real_distribution<> dis(0.0, 1.0);
            double rnd = dis(gen);
            double prob_threshold = 0.0;
            int next = -1;
            for (int j = 0; j < GRAPH_SIZE; j++) {
                if (visited[j] == 0) {
                    prob[j] /= prob_sum;
                    prob_threshold += prob[j];
                    if (rnd < prob_threshold) {
                        next = j;
                        break;
                    }
                }
            }
            ants[k][i] = next;
            visited[next] = 1;
            current = next;
        }
    }
}

void updatePheromone(std::vector<std::vector<int>>& ants) {
    double decay = 0.8;
    double pheromone_increment = 0.5;
    for (auto& row : pheromone) {
        for (auto& p : row) {
            p *= decay;
        }
    }
    for (int k = 0; k < GRAPH_SIZE; k++) {
        double tour_length = 0.0;
        for (int i = 0; i < GRAPH_SIZE - 1; i++) {
            int city1 = ants[k][i];
            int city2 = ants[k][i + 1];
            tour_length += distance[city1][city2];
        }
        tour_length += distance[ants[k][GRAPH_SIZE - 1]][ants[k][0]];
        for (int i = 0; i < GRAPH_SIZE - 1; i++) {
            int city1 = ants[k][i];
            int city2 = ants[k][i + 1];
            pheromone[city1][city2] += pheromone_increment / tour_length;
            pheromone[city2][city1] += pheromone_increment / tour_length;
        }
        pheromone[ants[k][GRAPH_SIZE - 1]][ants[k][0]] += pheromone_increment / tour_length;
        pheromone[ants[k][0]][ants[k][GRAPH_SIZE - 1]] += pheromone_increment / tour_length;
    }
}

void antMethod(int alpha, int beta) {
    for (int i = 0; i < GRAPH_SIZE; i++) {
        for (int j = 0; j < GRAPH_SIZE; j++) {
            distance[i][j] = graph[i][j];
        }
    }

    std::vector<std::vector<int>> ants(GRAPH_SIZE, std::vector<int>(GRAPH_SIZE));
    initialize();
    for (int i = 0; i < ANT_ROUND_COUNT; i++) {
        antTour(ants, alpha, beta);
        updatePheromone(ants);
    }

    double shortest_tour_distance = -1.0;
    std::vector<int> shortest_tour(GRAPH_SIZE);
    for (int k = 0; k < GRAPH_SIZE; k++) {
        double tour_length = 0.0;
        for (int i = 0; i < GRAPH_SIZE - 1; i++) {
            int city1 = ants[k][i];
            int city2 = ants[k][i + 1];
            tour_length += distance[city1][city2];
        }
        tour_length += distance[ants[k][GRAPH_SIZE - 1]][ants[k][0]];

        if (shortest_tour_distance == -1.0 || tour_length < shortest_tour_distance) {
            shortest_tour_distance = tour_length;
            std::copy(ants[k].begin(), ants[k].end(), shortest_tour.begin());
        }
    }

    std::cout << "Minimal path: ";
    for (int i = 0; i < GRAPH_SIZE; i++) {
        std::cout << shortest_tour[i] + 1 << " ";
    }
    std::cout << "\tMinimal distance: " << static_cast<int>(shortest_tour_distance) << "\n";
}

int main() {
    std::cout << "Random graph: \n";
    std::mt19937 gen(static_cast<unsigned>(time(nullptr)));
    std::uniform_int_distribution<> dist(1, 100);

    for (int i = 1; i < GRAPH_SIZE; i++) {
        for (int j = 0; j < i; j++) {
            graph[i][j] = dist(gen);
            graph[j][i] = graph[i][j];
        }
    }

    for (const auto& row : graph) {
        for (const auto& val : row) {
            std::cout << val << " ";
        }
        std::cout << "\n";
    }

    std::cout << "\n\nBrute force: \n";
    auto start_time = std::clock();
    bruteForce();
    auto end_time = std::clock();
    std::cout << "Time: " << end_time - start_time << "\n\n\n";

    std::cout << "Branch and Bound Method: \n";
    start_time = std::clock();
    startBranchAndBound();
    end_time = std::clock();
    std::cout << "Time: " << end_time - start_time << "\n\n\n";

    std::cout << "Ant Method: \n";
    for (int alpha : alphaArr) {
        for (int beta : betaArr) {
            std::cout << "alpha = " << alpha << ", beta = " << beta << "\n";
            start_time = std::clock();
            antMethod(alpha, beta);
            end_time = std::clock();
            std::cout << "Time: " << end_time - start_time << "\n\n";
        }
    }

    return 0;
}
