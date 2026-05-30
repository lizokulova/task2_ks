#include <iostream>
#include <vector>
#include <cstdlib>
#include <ctime>
#include <queue>
#include <algorithm>
#include <functional>
using namespace std;

struct Node {
    int level;
    int weight;
    int value;
    vector<int> selected;
    double upper_bound;
    Node(int l, int w, int v, vector<int> s, double ub): level(l), weight(w), value(v), selected(s), upper_bound(ub){}
};

class Knapsack{
private:
    vector<int> values;
    vector<int> weights;
    int W;
    int N;
    bool compare(int a, int b){return (double)values[a]/weights[a]>(double)values[b]/weights[b];}

public:
    Knapsack(int N, int W, vector<int>& v, vector<int>& w): N(N), W(W), values(v), weights(w){}

    void print(const vector<int>& sol){
        int w=0, i;
        for (i=0; i<N; i++){
            if (sol[i] == 1) w+=weights[i];
        }
        cout << w << endl;
        for (i=0; i < N; i++){
            cout << sol[i];
            if (i != N-1) cout << " ";
        }
        cout << endl;
    }

    vector<int> get_sorted_indices(){
        vector<int> ind(N);
        int i;
        for (i=0; i<N; i++) ind[i] = i;
        sort(ind.begin(), ind.end(), [this](int a, int b){return compare(a, b);});
        return ind;
    }

    double bound(int l, int weight, int value, const vector<int>& ind){
        if (weight>=W) return 0;
        int i, curr_w, idx;
        double ans_v = value;
        curr_w = weight;
        for (i=l; i<N; i++){
            idx = ind[i];
            if (curr_w + weights[idx] <= W){
                curr_w += weights[idx];
                ans_v += values[idx];
            }else{
                int remaining = W - curr_w;
                ans_v += values[idx]*((double)remaining/weights[idx]);
                break;
            }
        }
        return ans_v;
    }

    pair<vector<int>, int> branch_and_bound(){
        vector<int> ind = get_sorted_indices();
        auto cmp = [](const Node& a, const Node& b){return a.upper_bound < b.upper_bound;};

        priority_queue<Node, vector<Node>, decltype(cmp)> pq(cmp);
        vector<int> best_solution(N, 0);
        int best_value = 0, idx;

        vector<int> init(N, 0);
        double initial_bound = bound(0, 0, 0, ind);
        pq.push(Node(0, 0, 0, init, initial_bound));

        while (!pq.empty()){
            Node current = pq.top();
            pq.pop();

            if (current.upper_bound <= best_value) continue;

            if (current.level == N){
                if (current.value > best_value){
                    best_value = current.value;
                    best_solution = current.selected;
                } continue;
            }
            idx = ind[current.level];

            if (current.weight + weights[idx] <= W){
                vector<int> sel = current.selected;
                sel[idx] = 1;
                double b = bound(current.level+1, current.weight+weights[idx], current.value+values[idx], ind);
                if (b>best_value) pq.push(Node(current.level + 1, current.weight + weights[idx], current.value + values[idx], sel, b));
            }
            vector<int> n_s = current.selected;
            n_s[idx] = 0;
            double n_b = bound(current.level+1, current.weight, current.value, ind);
            if (n_b > best_value) pq.push(Node(current.level+1, current.weight, current.value, n_s, n_b));
        }
        return {best_solution, best_value};
    }
};

class Solver{
public:
    vector<int> solve(Knapsack& task, int st=1500){
        auto result = task.branch_and_bound();
        return result.first;
    }
};

int main(){
    srand(time(0));

    int N, W, i;
    cin >> N >> W;
    vector<int> val(N);
    vector<int> weig(N);
    for (i = 0; i < N; i++) cin >> val[i] >> weig[i];
    Knapsack k(N, W, val, weig);
    Solver a;
    vector<int> ans = a.solve(k);
    k.print(ans);

    return 0;
}
