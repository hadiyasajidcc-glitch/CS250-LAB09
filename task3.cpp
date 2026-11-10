#include <iostream>
#include <string>
#include <vector>
using namespace std;

class TravelRoute {
public:
    float travel_cost;

    TravelRoute(float c = 0) {
        travel_cost = c;
    }
};

class StarSystem {
public:
    int system_id;
    string name;
    int num_planets;
    double x, y, z;
    bool is_habitable;
    vector<StarSystem*> children;
    vector<TravelRoute> routes;   //routes[i] is the cost to children[i]

    StarSystem(int id, string n, int planets, double px, double py, double pz, bool habitable) {
        system_id = id;
        name = n;
        num_planets = planets;
        x = px;
        y = py;
        z = pz;
        is_habitable = habitable;
    }
};

class Galaxy {
    int M;
    StarSystem* root;

    //number of nodes in the subtree
    int count_nodes(StarSystem* x) const {
        if (x == NULL) return 0;
        int total = 1;
        for (int i = 0; i < x->children.size(); i++) {
            total += count_nodes(x->children[i]);
        }
        return total;
    }

    void destroy(StarSystem* x) {
        if (x == NULL) return;
        for (int i = 0; i < x->children.size(); i++) {
            destroy(x->children[i]);
        }
        delete x;
    }

public:
    //overloaded constructors
    Galaxy(int m) {
        M = m;
        root = NULL;
    }

    Galaxy(int m, StarSystem* r) {
        M = m;
        root = r;
    }

    ~Galaxy() {
        destroy(root);
    }

    StarSystem* get_root() const {
        return root;
    }

    bool add_route(StarSystem* parent, StarSystem* child, float cost) {
        if (parent->children.size() >= M) return false;
        parent->children.push_back(child);
        parent->routes.push_back(TravelRoute(cost));
        return true;
    }

    bool is_balanced(StarSystem* x) const {
        if (x == NULL) return true;
        if (x->children.size() == 0) return true;

        int smallest = count_nodes(x->children[0]);
        int biggest = smallest;
        for (int i = 1; i < x->children.size(); i++) {
            int s = count_nodes(x->children[i]);
            if (s < smallest) smallest = s;
            if (s > biggest) biggest = s;
        }
        if (biggest - smallest > 1) return false;

        //every child subtree must be balanced too
        for (int i = 0; i < x->children.size(); i++) {
            if (!is_balanced(x->children[i])) return false;
        }
        return true;
    }

    int count_leaves(StarSystem* x) const {
        if (x == NULL) return 0;
        if (x->children.size() == 0) return 1;
        int leaves = 0;
        for (int i = 0; i < x->children.size(); i++) {
            leaves += count_leaves(x->children[i]);
        }
        return leaves;
    }

    void display(StarSystem* x) const {
        if (x == NULL) return;
        for (int i = 0; i < x->children.size(); i++) {
            cout << x->name << " -> " << x->children[i]->name
                 << " (cost " << x->routes[i].travel_cost << ")" << endl;
            display(x->children[i]);
        }
    }
};

int main() {
    StarSystem* sol = new StarSystem(1, "Sol", 8, 0, 0, 0, true);
    StarSystem* alpha = new StarSystem(2, "Alpha", 3, 4.3, 1.2, 0.5, true);
    StarSystem* beta = new StarSystem(3, "Beta", 5, -2.1, 3.4, 1.0, false);
    StarSystem* gamma = new StarSystem(4, "Gamma", 2, 6.0, -1.5, 2.2, false);
    StarSystem* a1 = new StarSystem(5, "A1", 1, 5.0, 2.0, 0.7, true);
    StarSystem* a2 = new StarSystem(6, "A2", 4, 5.5, 0.4, 1.1, false);
    StarSystem* b1 = new StarSystem(7, "B1", 6, -3.0, 4.0, 1.5, false);
    StarSystem* b2 = new StarSystem(8, "B2", 2, -1.2, 5.1, 0.9, true);
    StarSystem* g1 = new StarSystem(9, "G1", 3, 7.2, -2.0, 2.8, false);

    Galaxy galaxy(3, sol);

    galaxy.add_route(sol, alpha, 4.5);
    galaxy.add_route(sol, beta, 3.2);
    galaxy.add_route(sol, gamma, 6.1);
    galaxy.add_route(alpha, a1, 1.8);
    galaxy.add_route(alpha, a2, 2.4);
    galaxy.add_route(beta, b1, 2.9);
    galaxy.add_route(beta, b2, 1.5);
    galaxy.add_route(gamma, g1, 3.7);

    galaxy.display(galaxy.get_root());
    cout << "Leaf nodes: " << galaxy.count_leaves(galaxy.get_root()) << endl;
    if (galaxy.is_balanced(galaxy.get_root())) cout << "Balanced: yes" << endl;
    else cout << "Balanced: no" << endl;

    //make a long chain under G1 so one side gets too big
    StarSystem* g2 = new StarSystem(10, "G2", 1, 8.0, -2.5, 3.0, false);
    StarSystem* g3 = new StarSystem(11, "G3", 2, 8.5, -3.0, 3.3, true);
    StarSystem* g4 = new StarSystem(12, "G4", 4, 9.0, -3.5, 3.6, false);
    galaxy.add_route(g1, g2, 2.0);
    galaxy.add_route(g2, g3, 2.5);
    galaxy.add_route(g3, g4, 1.1);

    cout << "After adding G2, G3, G4:" << endl;
    cout << "Leaf nodes: " << galaxy.count_leaves(galaxy.get_root()) << endl;
    if (galaxy.is_balanced(galaxy.get_root())) cout << "Balanced: yes" << endl;
    else cout << "Balanced: no" << endl;

    return 0;
}