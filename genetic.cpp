#include <iostream>
#include <vector>
#include <cstdlib>
#include <ctime>
#include <algorithm>
#include <functional>
using namespace std;

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
        for (i=0; i<N; i++){if (sol[i]==1) w+=weights[i];}
        cout << w << endl;
        for (i=0; i < N; i++){
            cout << sol[i];
            if (i != N-1) cout << " ";
        }
        cout << endl;
    }

    int evaluate(const vector<int>& sol){
        int totalW=0, totalV=0, i;
        for (i=0; i<N; i++){
            if (sol[i]==1){
                totalW+=weights[i];
                totalV+=values[i];
            }
        }
        if (totalW>W) return 0;
        return totalV;
    }

    vector<int> rand_sol(){
        vector<int> sol(N, 0);
        int i;
        for (i=0; i<N; i++) sol[i]=rand()%2;
        return sol;
    }

    vector<int> repair(vector<int> sol){
        int i, j, currW = 0;
        vector<int> indices(N);
        for (i=0; i<N; i++) indices[i] = i;
        for (i=0; i<N; i++){ if (sol[i]==1) currW += weights[i];}
        if (currW <= W) return sol;
        sort(indices.begin(), indices.end(), [this](int a, int b){ return (double)values[a]/weights[a] < (double)values[b]/weights[b];});
        for (j=0; j<N && currW>W; j++){
            i = indices[j];
            if (sol[i]==1){
                sol[i] = 0;
                currW -= weights[i];
            }
        }
        return sol;
    }

    vector<vector<int>> selection(const vector<vector<int>>& population, const vector<int>& c){
        vector<vector<int>> par;
        int i, j, p1, p2, pop_size = population.size();

        for (i=0; i<pop_size/2; i++){
            p1 = rand() % pop_size;
            p2 = rand() % pop_size;
            if (c[p1] < c[p2]) p1 = p2;

            p2 = rand() % pop_size;
            int p3 = rand() % pop_size;
            if (c[p2] < c[p3]) p2 = p3;
            par.push_back(population[p1]);
            par.push_back(population[p2]);
        }
        return par;
    }

    vector<int> crv(const vector<int>& p1, const vector<int>& p2){
        vector<int> child(N, 0);
        int i, point;
        point = rand() % N;
        for (i=0; i<point; i++)child[i] = p1[i];
        for (i=point; i<N; i++)child[i] = p2[i];
        return child;
    }

    void mutate(vector<int>& sol, double mut_rate){
        int i;
        for (i=0; i<N; i++){
            if ((double)rand()/RAND_MAX < mut_rate) sol[i] = 1-sol[i];
        }
    }

    vector<int> gen_alg(int pop_size=100, int generations=500, double mut_rate=0.01){
        vector<vector<int>> popul;
        vector<int> f(pop_size);
        int i, j, gen, best_value=0;
        vector<int> ans(N, 0);
        for (i=0; i<pop_size; i++){
            vector<int> sol = rand_sol();
            sol = repair(sol);
            popul.push_back(sol);
            f[i] = evaluate(sol);
            if (f[i] > best_value){
                best_value = f[i];
                ans = sol;
            }
        }

        for (gen=0; gen<generations; gen++){
            vector<vector<int>> parents = selection(popul, f);
            vector<vector<int>> new_p;
            vector<int> new_f;

            for (i=0; i<pop_size; i+=2){
                vector<int> child1 = crv(parents[i], parents[i+1]);
                vector<int> child2 = crv(parents[i+1], parents[i]);
                mutate(child1, mut_rate); mutate(child2, mut_rate);
                child1 = repair(child1); child2 = repair(child2);
                new_p.push_back(child1); new_p.push_back(child2);
            }

            for (i=0; i<pop_size; i++){
                int val = evaluate(new_p[i]);
                new_f.push_back(val);
                if (val > best_value){
                    best_value = val;
                    ans = new_p[i];
                }
            }
            popul = new_p;
            f = new_f;
        }
        return ans;
    }
};

class Solver{
public:
    vector<int> solve(Knapsack& task, int st=1500){
        return task.gen_alg();
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
