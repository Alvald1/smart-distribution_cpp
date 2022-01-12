#include <iostream>
#include <numbers>
#include <fstream>
#include <string>
#include <vector>
#include <iomanip>
#include <map>
#include <algorithm>
#include <ctime>
#define EARTH_RADIUS 6372795 

using namespace std;

map<pair<float, float>, int>const_count;


class House {
public:
    House(const int& children, const map<pair<float, float>, int>& dis, const pair<float, float> cord, const map<pair<float, float>, float>& ratio_dis, const int& id) {
        this->children = children;
        this->dis = dis;
        this->cord = cord;
        this->ratio_dis = ratio_dis;
        this->id = id;
    }
    pair<float, float> getCord() {
        return cord;
    }
    int getChildren() {
        return children;
    }
    map<pair<float, float>, int> getDis() {
        return dis;
    }
    map<pair<float, float>, float> getRatio_Dis() {
        return ratio_dis;
    }
    pair<pair<float, float>, float> getMin_dis() {
        pair<pair<float, float>, float> m;
        m.second = INT_MAX;
        for (auto i : dis)
            if (i.second < m.second) m = i;
        return m;
    }
    float getDifference(const pair<float, float>& school_m, const pair<float, float>& school_M) {
        return abs(dis[school_m] - dis[school_M]);
    }
    float getDifference_r(const pair<float, float>& school_m, const pair<float, float>& school_M) {
        return abs(ratio_dis[school_m] - ratio_dis[school_M]);
    }
    int getID() {
        return id;
    }
    int getDisToSch(const pair<float, float>& sch) {
        for (auto it : dis) {
            if (it.first == sch) return it.second;
        }
    }
private:
    map<pair<float, float>, int>dis;
    int children;
    pair<float, float>cord;
    map<pair<float, float>, float>ratio_dis;
    int id;
};

float calculateTheDistance(const float& a1, const float& b1, const float& a2, const float& b2) {
    float cl1 = cos(a1 * numbers::pi / 180);
    float cl2 = cos(a2 * numbers::pi / 180);
    float sl1 = sin(a1 * numbers::pi / 180);
    float sl2 = sin(a2 * numbers::pi / 180);
    float cdelta = cos(b2 * numbers::pi / 180 - b1 * numbers::pi / 180);
    float sdelta = sin(b2 * numbers::pi / 180 - b1 * numbers::pi / 180);
    float y = sqrt(pow(cl2 * sdelta, 2) + pow(cl1 * sl2 - sl1 * cl2 * cdelta, 2));
    float x = sl1 * sl2 + cl1 * cl2 * cdelta;
    return atan2(y, x) * EARTH_RADIUS;
}

bool foo(const vector<pair<float, float>>& v, const pair<float, float>& t) {
    for (auto it : v)
        if (it == t)
            return false;
    return true;
}

bool getH(map<pair<float, float>, int>& s_B, pair<int, pair<float, float>>& h, pair<float, float>& sch, map<pair<float, float>, int>& count_class, map<const pair<float, float>, vector<pair<pair<pair<pair<float, float>, pair<int, int>>, vector<pair<float, float>>>, int>>>& A, vector<pair<House, int>>& houses) {
    int c = 0;
    bool flag = false;
    for (auto it : s_B) {
        ++c;
        if (it.second > 28 * count_class[it.first]) {
            int ttt = 1;
            pair<int, pair<float, float>>min;
            min.second.second = INT_MAX;
            int count = 0;
            for (auto it2 : A[it.first]) {
                int current = it2.first.first.second.first;
                auto dis = houses[it2.second].first.getDis();
                pair<pair<float, float>, int>m = { {0,0},INT_MAX };
                for (auto it3 : dis)
                    if (m.second > abs(current - it3.second) && current != it3.second && foo(it2.first.second, it3.first)) m = { it3.first,abs(current - it3.second) }, ++ttt;
                if (min.second.second > m.second) min = { count, m.first };
                ++count;
            }
            h = min;
            sch = it.first;
            return false;
        }
        else if (it.second < 25 * count_class[it.first]) {
            int ttt = 1;
            pair<int, pair<float, float>>min;// out num; in
            min.second.second = INT_MAX;
            pair<float, float> tmp_sch;
            for (auto it2 : A) {
                if (it2.first != it.first) {
                    pair<int, int>m = { 0,INT_MAX };
                    int count = 0;
                    for (auto it3 : it2.second) {
                        int current = it3.first.first.second.first;
                        int new_dis = houses[it3.second].first.getDisToSch(it.first);
                        if (m.second > abs(current - new_dis) && foo(it3.first.second, it.first) && (s_B[it2.first] - A[it2.first][count].first.first.second.second > 25 * count_class[it2.first]))
                            m = { count,abs(current - new_dis) }, ++ttt, flag = true;
                        ++count;
                    }
                    if (min.second.second > m.second) min = { m.first,it.first }, tmp_sch = it2.first;
                }
            }
            if (!flag) continue;
            h = min;
            sch = tmp_sch;
            return false;
        }
    }
    if (c == count_class.size()) return true;
}



void sorting(const pair<float, float>& school_m, const pair<float, float>& school_M, map<const pair<float, float>, vector<pair<pair<pair<pair<float, float>, pair<int, int>>, vector<pair<float, float>>>, int>>>& map, vector<pair<House, int>>& houses) {
    sort(map[school_M].begin(), map[school_M].end(),
        [&school_M, &school_m, &houses]
    (pair<pair<pair<pair<float, float>, pair<int, int>>, vector<pair<float, float>>>, int>& l, pair<pair<pair<pair<float, float>, pair<int, int>>, vector<pair<float, float>>>, int>& r)->bool
        {
            int id_1 = l.second, id_2 = r.second;
            float dif_1 = houses[id_1].first.getDifference_r(school_m, school_M), dif_2 = houses[id_2].first.getDifference_r(school_m, school_M);
            return dif_1 > dif_2;
        });
}
map<pair<float, float>, int> calcEf(const map<const pair<float, float>, vector<pair<pair<pair<pair<float, float>, pair<int, int>>, vector<pair<float, float>>>, int>>>& B, const map<pair<float, float>, int>& s_B) {
    map<pair<float, float>, int>ef;
    for (auto it : B) {
        for (auto it2 : it.second)
            ef[it.first] += it2.first.first.second.first;
    }
    return ef;
}

int calcCountClasses(const int& s) {
    double upper = s / 25.0, lower = s / 28.0;
    vector<int>ans;
    for (int i = ceil(lower); i <= floor(upper); ++i)
        ans.push_back(i);
    pair<int, int>mmin = { 0,0 };
    for (int i = 0; i < ans.size(); ++i) {
        int l = ans[i] * 25, u = ans[i] * 30;
        int m = min(s - l, u - s);
        if (mmin.second < m)
            mmin = { i,m };

    }
    if (ans.empty())
        return ceil(upper);
    return ans[mmin.first];
}

pair<map<const pair<float, float>, vector<pair<pair<pair<pair<float, float>, pair<int, int>>, vector<pair<float, float>>>, int>>>, map<pair<float, float>, int>> solution_1(map<pair<float, float>, int>& count_cl, map<pair<float, float>, int>& ef, const vector<pair<float, float>>& cord_schools, const vector<pair<float, float>>& cord_houses, vector<pair<House, int>>& houses) {
    map<const pair<float, float>, vector<pair<pair<pair<pair<float, float>, pair<int, int>>, vector<pair<float, float>>>, int>>> A, B;
    //������: ����� -> ��� -> ��������������, B_2 - ��������������� �� ����
    map<pair<float, float>, int> s_A, s_B, count_class;//����� ����� � ������ �����, ���-�� �������
    int s = 0;
    for (auto house : houses) {
        auto tmp = house.first.getMin_dis();//����������� ����������
        auto t = vector<pair<float, float>>(0);
        A[tmp.first].push_back({ {{ house.first.getCord(),{tmp.second,house.first.getChildren()} },t}, house.first.getID() });
        //������������� ����� � ����� � ������������ ����
        s_A[tmp.first] += house.first.getChildren();//������� ����� ����� � �����
        count_class[tmp.first] = 0;
        s += house.first.getChildren();
    }

    if (const_count.empty()) {
        auto count = calcCountClasses(s);
        int min_c = floor(double(count) / count_class.size());
        int remains = count - min_c * count_class.size();//�������
        for (auto& it : count_class) it.second += min_c;
        vector<int>mem;
        while (remains != 0) {
            int tmp = rand() % count_class.size();
            for (auto it : mem)
                if (it == tmp) continue;
            mem.push_back(tmp);
            auto it = count_class.begin();
            advance(it, tmp);
            it->second++;
            --remains;
        }
        const_count = count_class;
    }
    else 
        count_class = const_count;
    
    auto EF = calcEf(A, s_A);
    ef = EF;
    count_cl = count_class;

    B = A;
    s_B = s_A;
    pair<float, float>min_S;
    pair<float, float>max_S;
    pair<int, pair<float, float>>h;
    pair<float, float>sch;
    int ttt = 1;
    while (!getH(s_B, h, sch, count_class, B, houses)) {//����� ���� ��� �������������
        //sorting(min_S, max_S, B, houses);
        int* ptr = &(B[sch][h.first].first.first.second.second);
        s_B[sch] -= *ptr;
        s_B[h.second] += *ptr;
        B[sch][h.first].first.first.second.first = houses[B[sch][h.first].second].first.getDisToSch(h.second);
        B[sch][h.first].first.second.push_back(sch);
        B[h.second].push_back(B[sch][h.first]);
        B[sch].erase(B[sch].begin() + h.first);
        ++ttt;
    }
    return { B,s_B };
}

void getMin_Max(const map<pair<float, float>, int>& s_B, pair<pair<float, float>, int>& min_S, pair<pair<float, float>, int>& max_S, map<pair<float, float>, int>& count_classes) {
    pair<pair<float, float>, int> min, min_2, max;
    min.second = INT_MIN;
    max.second = INT_MIN;
    min_2.second = INT_MAX;
    for (const auto& it : s_B) {
        if (it.second - count_classes[it.first] * 28 > max.second)
            max = { it.first,  it.second - count_classes[it.first] * 28 };
        if (count_classes[it.first] * 25 - it.second > min.second)
            min = { it.first,  count_classes[it.first] * 25 - it.second };
        if (it.second - count_classes[it.first] * 25 < min_2.second)
            min_2 = { it.first,  it.second - count_classes[it.first] * 25 };
        else if (it.second - count_classes[it.first] * 25 == min_2.second) {
            if (double(it.second - count_classes[it.first] * 25) / (count_classes[it.first] * 28 - count_classes[it.first] * 25) < (double(min_2.second) / (count_classes[min_2.first] * 28 - count_classes[min_2.first] * 25)))
                min_2 = { it.first,  it.second - count_classes[it.first] * 25 };
        }
    }
    if (min.second > 0)
        min_S = min;// min
    else
        min_S = min_2;
    max_S = max;// max
}

pair<map<const pair<float, float>, vector<pair<pair<pair<pair<float, float>, pair<int, int>>, vector<pair<float, float>>>, int>>>, map<pair<float, float>, int>> solution_2(map<pair<float, float>, int>& count_cl, map<pair<float, float>, int>& ef, const vector<pair<float, float>>& cord_schools, const vector<pair<float, float>>& cord_houses, vector<pair<House, int>>& houses) {
    map<const pair<float, float>, vector<pair<pair<pair<pair<float, float>, pair<int, int>>, vector<pair<float, float>>>, int>>> A, B;
    //������: ����� -> ��� -> ��������������, B_2 - ��������������� �� ����
    map<pair<float, float>, int> s_A, s_B, count_class;//����� ����� � ������ �����, ���-�� �������
    int s = 0;
    for (auto house : houses) {
        auto tmp = house.first.getMin_dis();//����������� ����������
        auto t = vector<pair<float, float>>(0);
        A[tmp.first].push_back({ {{ house.first.getCord(),{tmp.second,house.first.getChildren()} },t}, house.first.getID() });
        //������������� ����� � ����� � ������������ ����
        s_A[tmp.first] += house.first.getChildren();//������� ����� ����� � �����
        count_class[tmp.first] = 0;
        s += house.first.getChildren();
    }

    if (const_count.empty()) {
        auto count = calcCountClasses(s);
        int min_c = floor(double(count) / count_class.size());
        int remains = count - min_c * count_class.size();//�������
        for (auto& it : count_class) it.second += min_c;
        vector<int>mem;
        while (remains != 0) {
            int tmp = rand() % count_class.size();
            for (auto it : mem)
                if (it == tmp) continue;
            mem.push_back(tmp);
            auto it = count_class.begin();
            advance(it, tmp);
            it->second++;
            --remains;
        }
        const_count = count_class;
    }
    else
        count_class = const_count;

    auto EF = calcEf(A, s_A);
    ef = EF;
    count_cl = count_class;

    B = A;
    s_B = s_A;
    pair<pair<float, float>, int>min_S, max_S;
    pair<int, pair<float, float>>h;
    pair<float, float>sch;

    int ttt = 1;

    for (;; ++ttt) {
        getMin_Max(s_B, min_S, max_S, count_class);
        if (max_S.second == INT_MIN) break;
        sorting(min_S.first, max_S.first, B, houses);
        auto it = --B[max_S.first].end();
        int* ptr = &((*it).first.first.second.second);
        if ((min(abs(s_B[min_S.first] - 25 * count_class[min_S.first]), abs(s_B[min_S.first] - 28 * count_class[min_S.first])) + min(abs(s_B[max_S.first] - 25 * count_class[max_S.first]), abs(s_B[max_S.first] - 28 * count_class[max_S.first]))) < (min(abs(s_B[min_S.first] + *ptr - 25 * count_class[min_S.first]), abs(s_B[min_S.first] + *ptr - 28 * count_class[min_S.first])) + min(abs(s_B[max_S.first] - *ptr - 25 * count_class[max_S.first]), abs(s_B[max_S.first] - *ptr - 28 * count_class[max_S.first])))) break;
        it->first.first.second.first = houses[it->second].first.getDisToSch(min_S.first);
        s_B[max_S.first] -= *ptr;
        s_B[min_S.first] += *ptr;
        B[min_S.first].push_back(*it);
        B[max_S.first].pop_back();
    }
    return { B,s_B };
}

int main() {
    //srand(time(0));
    ifstream file("cord.txt");
    vector<pair<float, float>>cord_schools, cord_houses;
    vector<pair<House, int>>houses;
    while (!file.eof()) {//���������� �� ����� ��������� ����� � ����
        string str;
        getline(file, str);
        size_t t = str.find(',');
        if (str[0] == '!')
            cord_schools.push_back({ stod(str.substr(1, t - 1)), stod(str.substr(t + 2)) });
        else
            cord_houses.push_back({ stod(str.substr(0, t)), stod(str.substr(t + 2)) });
    }
    int id = 0;
    for (auto cord_house : cord_houses) {
        map<pair<float, float>, float> ratio_dis;//������� ���������� � ���� ����������
        map<pair<float, float>, int> dis;
        int children = rand() % 20 + 1;//��������� ��������� ���-�� ����� �� 1 �� 20
        for (auto cord_school : cord_schools) {//������ ���������� �� ���� �� ������ �����
            dis[cord_school] = (calculateTheDistance(cord_house.first, cord_house.second, cord_school.first, cord_school.second));
            ratio_dis[cord_school] = double(children) / dis[cord_school];//������ ���� ���������� �� ���� �� ������ �����
        }
        houses.push_back({ { children, dis, cord_house,ratio_dis, id}, id });//�������� ������� ������ ����
        ++id;
    }

    map<pair<float, float>, int> ef1, ef2;
    map<pair<float, float>, int> c1, c2;
    auto t = solution_1(c1, ef1, cord_schools, cord_houses, houses);    
    auto t2 = solution_2(c2, ef2, cord_schools, cord_houses, houses);
    auto m = calcEf(t.first, t.second);
    auto m2 = calcEf(t2.first, t2.second);
    int sum1 = 0, sum2 = 0;
    for (auto i : m) sum1 += i.second;
    for (auto i : m2) sum2 += i.second;
    return 0;
}