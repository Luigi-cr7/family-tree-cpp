//
//  main.cpp
//  Week8
//
//  Created by Luis on 2/22/26.
//

#include <iostream>
#include <vector>
#include <string>
#include <unordered_set>
#include <memory>
using namespace std;

class Person {
public:
    string name;
    weak_ptr<Person> mom;
    weak_ptr<Person> dad;
    vector<weak_ptr<Person>> siblings;

    Person(string n) : name(n) {}

    void addSibling(const shared_ptr<Person>& sibling) { siblings.push_back(sibling); }

    int count() const {
        unordered_set<const Person*> visited;
        return count(visited);
    }

    void printTree(string prefix = "") const {
        unordered_set<const Person*> visited;
        printTree(prefix, visited);
    }

    bool isRelated(const string& target) const {
        unordered_set<const Person*> visited;
        return isRelated(target, visited);
    }

private:
    int count(unordered_set<const Person*>& visited) const {
        if (visited.count(this)) return 0;
        visited.insert(this);
        int total = 1;
        if (auto m = mom.lock()) total += m->count(visited);
        if (auto d = dad.lock()) total += d->count(visited);
        for (auto& ws : siblings) if (auto s = ws.lock()) total += s->count(visited);
        return total;
    }

    void printTree(string prefix, unordered_set<const Person*>& visited) const {
        if (visited.count(this)) return;
        visited.insert(this);
        cout << prefix << name << endl;
        if (auto m = mom.lock()) m->printTree(prefix + "  [Mom] ", visited);
        if (auto d = dad.lock()) d->printTree(prefix + "  [Dad] ", visited);
        for (auto& ws : siblings) if (auto s = ws.lock()) s->printTree(prefix + "  [Sibling] ", visited);
    }

    bool isRelated(const string& target, unordered_set<const Person*>& visited) const {
        if (visited.count(this)) return false;
        visited.insert(this);
        if (name == target) return true;
        if (auto m = mom.lock(); m && m->isRelated(target, visited)) return true;
        if (auto d = dad.lock(); d && d->isRelated(target, visited)) return true;
        for (auto& ws : siblings) if (auto s = ws.lock(); s && s->isRelated(target, visited)) return true;
        return false;
    }
};

int main() {
    
    auto patrickSr = make_shared<Person>("Patrick Griffin Sr.");
    auto thelma = make_shared<Person>("Thelma Griffin");
    auto unknownCarter = make_shared<Person>("Unknown Pewterschmidt Sr.");
    auto unknownBarbara = make_shared<Person>("Unknown Pewterschmidt Sr.");

    
    auto peter = make_shared<Person>("Peter Griffin");
    peter->dad = patrickSr;
    peter->mom = thelma;

    auto carter = make_shared<Person>("Carter Pewterschmidt");
    carter->dad = unknownCarter;
    carter->mom = make_shared<Person>("Unknown");
    auto barbara = make_shared<Person>("Barbara Pewterschmidt");
    barbara->dad = unknownBarbara;
    barbara->mom = make_shared<Person>("Unknown");

    auto lois = make_shared<Person>("Lois Griffin");
    lois->dad = carter;
    lois->mom = barbara;

   
    auto meg = make_shared<Person>("Meg Griffin");
    auto chris = make_shared<Person>("Chris Griffin");
    auto stewie = make_shared<Person>("Stewie Griffin");

    meg->dad = peter; meg->mom = lois;
    chris->dad = peter; chris->mom = lois;
    stewie->dad = peter; stewie->mom = lois;

    meg->addSibling(chris); meg->addSibling(stewie);
    chris->addSibling(meg); chris->addSibling(stewie);
    stewie->addSibling(meg); stewie->addSibling(chris);

    
    auto brian = make_shared<Person>("Brian Griffin");

    
    cout << "Total people in Peter's tree: " << peter->count() << endl;
    cout << "\nFamily Tree:\n";
    peter->printTree();

    cout << "\nIs Stewie related to Peter? " << (peter->isRelated("Stewie Griffin") ? "Yes" : "No") << endl;
    cout << "Is Meg related to Barbara Pewterschmidt? " << (meg->isRelated("Barbara Pewterschmidt") ? "Yes" : "No") << endl;
    cout << "Is Brian related to Peter? " << (peter->isRelated("Brian Griffin") ? "Yes" : "No") << endl;

    return 0;
}
