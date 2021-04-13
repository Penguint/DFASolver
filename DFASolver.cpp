#include <bits/stdc++.h>
using namespace std;

// Detect if a template parameter is a std::vector
template<typename T>
struct isVector: std::false_type {};

template<typename ...T>
struct isVector<std::vector<T...>> : std::true_type {};

// Detect if template parameter is a std::tuple
template<typename T>
struct isTuple: std::false_type {};

template<typename ...T>
struct isTuple<std::tuple<T...>> : std::true_type {};

template <typename T>
T read(stringstream &ss) {
    T e;
    function<T(T&)> read_impl = [&ss, &read_impl](T &e) -> T {
        if constexpr (isTuple<T>::value)
            e = apply([&](auto &&...args) { return make_tuple(((read_impl(args)), ...)); }, e);
        else
            ss >> e;
        return e;
    };
    return read_impl(e);
}

template <typename T>
stringstream &read_wrapper(stringstream &ss, T &e) {
    e = read<T>(ss);
    return ss;
}

template<typename T>
vector<T> unflatten(string s) {    
    vector<T> res;
    stringstream ss;
    ss << s;
    T e;
    while (read_wrapper(ss, e))
        res.push_back(e);
    return res;
}

template<>
vector<tuple<int, string>> unflatten(string s) {    
    vector<tuple<int, string>> res;
    stringstream ss;
    ss << s;
    tuple<int, string> e;
    while (ss >> get<0>(e) >> get<1>(e))
        res.push_back(e);
    return res;
}

template<typename T>
size_t get_unflattened_list(istream &is, vector<T> &l) {
    string s;
    getline(is, s);
    l = unflatten<T>(s);
    return l.size();
}

void combine(set<int> &a, set<int> &b) {
    for (auto &e : b) a.insert(e);
}
bool equal(set<int> &a, set<int> &b) {
    if (a.size() != b.size()) return false;
    for (auto &e : b) if (!a.count(e)) return false;
    return true;
}
size_t find(vector<set<int>> &l, set<int> &a) {
    for (size_t i = 0; i < l.size(); i++)
        if (equal(l[i], a)) return i;
    return l.size();
}

int main() {
    int n;
    cout << "number of vertices: ";
    cin >> n;
    getchar();
    vector<map<string, set<int>>> table(n);
    set<string> val_list;
    for (int i = 0; i < n; i++) {
        cout << i << ": ";
        vector<tuple<int, string>> this_edges;
        get_unflattened_list(cin, this_edges);
        for (auto &[v, val] : this_edges) {
            table[i][val].insert(v);
            val_list.insert(val);
        }
    }
    
    vector<int> v;
    cout << "start: ";
    get_unflattened_list<int>(cin, v);
    set<int> start;
    for (auto &e : v) start.insert(e);
    cout << "terminal: ";
    get_unflattened_list<int>(cin, v);
    set<int> terminal;
    for (auto &e : v) terminal.insert(e);
    
    
    vector<set<int>> name;
    vector<map<string, int>> ans;
    name.push_back(start);
    ans.push_back({});
    queue<int> q;
    q.push(0);
    while (q.size()) {
        auto p = q.front();
        q.pop();
        for (auto &val : val_list) {
            set<int> res;
            for (auto &id : name[p])
                combine(res, table[id][val]);
            auto pos = find(name, res);
            if (pos == name.size()) {
                name.push_back(res);
                ans.push_back({});
                q.push(pos);
            }
            ans[p][val] = pos;
        }
    } 

    cout << "id\tname\t\t";
    for (auto &val : val_list) cout << val << "\t";
    cout << endl;

    for (int i = 0; i < (int)name.size(); i++) {
        cout << i << "\t";
        stringstream ss;
        for (auto &e : name[i])
            ss << e << (terminal.count(e) ? "*" : "") << ",";
        string s = ss.str();
        if (s.size()) s.pop_back();
        s.resize(16, ' ');
        cout << s;
        for (auto &val : val_list) cout << ans[i][val] << "\t";
        cout << endl;
    }
}

/*
number of in vertices: 14
1: 2 1 6 1
2: 3 1 14 0
3: 4 0 8 1
4: 5 1
5: 3 0
6: 7 0
7: 11 1
8: 3 1 9 1 14 0
9: 10 0
10: 11 1
11: 11 0 12 1 14 0
12: 13 0
13: 11 1
14: 
start: 1
terminal: 14
*/