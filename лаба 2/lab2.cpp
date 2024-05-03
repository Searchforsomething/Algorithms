#include <iostream>
#include <vector>
#include <unordered_set>
#include <string>

using namespace std;

// Структура для представления перехода
struct Transition {
    char symbol;
    int nextState;
};

class NFA {
private:
    int numStates;
    vector<vector<Transition>> transitions;

public:
    // Конструктор с указанием количества состояний
    NFA(int states) : numStates(states), transitions(states) {}

    // Добавление перехода из одного состояния в другое по символу
    void addTransition(int fromState, int toState, char symbol) {
        transitions[fromState].push_back({symbol, toState});
    }

    // Метод для выполнения перехода по символу
    unordered_set<int> move(const unordered_set<int>& states, char symbol) {
        unordered_set<int> result;
        for (int state : states) {
            for (const Transition& transition : transitions[state]) {
                if (transition.symbol == symbol) {
                    result.insert(transition.nextState);
                }
            }
        }
        return result;
    }

    // Метод для выполнения epsilon-перехода
    unordered_set<int> epsilonClosure(const unordered_set<int>& states) {
        unordered_set<int> result = states;
        bool changed = true;
        while (changed) {
            changed = false;
            for (int state : result) {
                for (const Transition& transition : transitions[state]) {
                    if (transition.symbol == '\0' && result.find(transition.nextState) == result.end()) {
                        result.insert(transition.nextState);
                        changed = true;
                    }
                }
            }
        }
        return result;
    }

    // Метод для проверки, принимает ли автомат данную строку
    bool accepts(const string& input) {
        unordered_set<int> currentStates = epsilonClosure({0});
        for (char symbol : input) {
            currentStates = move(currentStates, symbol);
            currentStates = epsilonClosure(currentStates);
        }
        return currentStates.count(numStates - 1) > 0;
    }
};

int main() {
    // Создание автомата
    NFA nfa(3);
    nfa.addTransition(0, 0, 'a');
    nfa.addTransition(0, 1, '\0');
   // nfa.addTransition(0, 2, '\0');
    nfa.addTransition(1, 1, 'c');
    nfa.addTransition(1, 2, '\0');
    nfa.addTransition(2, 2, 'd');
    nfa.addTransition(2, 1, '\0');

    // Проверка принятия строк
    cout << "Accepts \"\": " << nfa.accepts("") << endl;
    cout << "Accepts \"a\": " << nfa.accepts("a") << endl;
    cout << "Accepts \"ac\": " << nfa.accepts("ac") << endl;
    cout << "Accepts \"ad\": " << nfa.accepts("ad") << endl;
    cout << "Accepts \"acc\": " << nfa.accepts("acc") << endl;
    cout << "Accepts \"acd\": " << nfa.accepts("acd") << endl;
    cout << "Accepts \"aaaccc\": " << nfa.accepts("aaaccc") << endl;

    return 0;
}
