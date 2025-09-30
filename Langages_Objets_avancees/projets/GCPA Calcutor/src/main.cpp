#include <iostream>
#include <iomanip>
#include <vector>
#include <string>
#include <sstream>
#include <cmath>
#include <cctype>
#include <algorithm>
using namespace std;

struct Course {
    string name;
    double credits{};
    string gradeInput; // raw input
    double grade4{};   // mapped to 4.0 scale
};

static double mapLetterTo4(const string &g) {
    string s;
    // normalize
    for (char c: g) if (!isspace((unsigned char)c)) s.push_back(toupper((unsigned char)c));
    if (s=="A+" || s=="A") return 4.0;
    if (s=="A-") return 3.7;
    if (s=="B+") return 3.3;
    if (s=="B") return 3.0;
    if (s=="B-") return 2.7;
    if (s=="C+") return 2.3;
    if (s=="C") return 2.0;
    if (s=="C-") return 1.7;
    if (s=="D+") return 1.3;
    if (s=="D") return 1.0;
    if (s=="F" || s=="E") return 0.0;
    return NAN;
}

static double mapNumericTo4(double score, double maxScore) {
    if (maxScore <= 0) return NAN;
    double pct = score / maxScore; // 0..1
    // map percent to 4.0 with typical cutoffs
    if (pct >= 0.93) return 4.0;
    if (pct >= 0.90) return 3.7;
    if (pct >= 0.87) return 3.3;
    if (pct >= 0.83) return 3.0;
    if (pct >= 0.80) return 2.7;
    if (pct >= 0.77) return 2.3;
    if (pct >= 0.73) return 2.0;
    if (pct >= 0.70) return 1.7;
    if (pct >= 0.67) return 1.3;
    if (pct >= 0.60) return 1.0;
    return 0.0;
}

static double parseGradeTo4(const string &input) {
    // try letter first
    double v = mapLetterTo4(input);
    if (!std::isnan(v)) return v;
    // try numeric forms: '15/20', '75/100', '16', '72'
    string s = input;
    // replace comma decimal with dot
    for (char &c: s) if (c==',') c='.';
    // find '/'
    size_t slash = s.find('/');
    try {
        if (slash != string::npos) {
            double num = stod(s.substr(0, slash));
            double den = stod(s.substr(slash+1));
            if (den == 20.0) return mapNumericTo4(num, 20.0);
            if (den == 100.0) return mapNumericTo4(num, 100.0);
            return mapNumericTo4(num, den);
        } else {
            // assume on 20 if <=20, else 100
            double num = stod(s);
            if (num <= 20.0) return mapNumericTo4(num, 20.0);
            return mapNumericTo4(num, 100.0);
        }
    } catch (...) {
        return NAN;
    }
}

static void printCourses(const vector<Course>& cs) {
    cout << left << setw(28) << "Course" << setw(10) << "Credits" << setw(10) << "Grade(4)" << "Input" << '\n';
    cout << string(64, '-') << '\n';
    for (auto &c: cs) {
        cout << left << setw(28) << c.name.substr(0,28)
             << setw(10) << c.credits
             << setw(10) << (std::isnan(c.grade4)? string("NA"): to_string(c.grade4).substr(0,4))
             << c.gradeInput << '\n';
    }
}

static double calcGPA(const vector<Course>& cs) {
    double sum = 0, credits = 0;
    for (auto &c: cs) {
        if (!std::isnan(c.grade4) && c.credits > 0) {
            sum += c.grade4 * c.credits;
            credits += c.credits;
        }
    }
    return credits>0 ? sum/credits : NAN;
}

int main() {
    ios::sync_with_stdio(false);
    cin.tie(nullptr);

    vector<Course> courses;
    cout << "CGPA Calculator (C++) - type 'help' for commands\n";
    string cmd;
    while (true) {
        cout << "cgpa> ";
        if (!getline(cin, cmd)) break;
        // trim
        auto trim = [](string &s){
            auto l = s.find_first_not_of(" \t\r\n");
            auto r = s.find_last_not_of(" \t\r\n");
            if (l==string::npos) { s.clear(); return; }
            s = s.substr(l, r-l+1);
        };
        trim(cmd);
        if (cmd.empty()) continue;

        if (cmd == "help") {
            cout << "Commands:\n"
                 << "  add <name> <credits> <grade>  | grade: A,B+,15/20,75/100, etc.\n"
                 << "  list                         | show current courses\n"
                 << "  gpa                          | compute GPA\n"
                 << "  clear                        | remove all\n"
                 << "  quit                         | exit\n";
            continue;
        }
        if (cmd == "quit" || cmd == "exit") break;
        if (cmd == "list") { printCourses(courses); continue; }
        if (cmd == "clear") { courses.clear(); cout << "Cleared.\n"; continue; }
        if (cmd == "gpa") {
            double g = calcGPA(courses);
            if (std::isnan(g)) cout << "No valid courses.\n";
            else cout << fixed << setprecision(3) << "GPA: " << g << "\n";
            continue;
        }
        if (cmd.rfind("add ", 0) == 0) {
            string rest = cmd.substr(4);
            // parse: name may contain spaces, credits double, grade string
            // heuristic: last two tokens are credits and grade; name is prefix
            stringstream ss(rest);
            vector<string> tokens;
            string t;
            while (ss >> t) tokens.push_back(t);
            if (tokens.size() < 3) { cout << "Usage: add <name> <credits> <grade>\n"; continue; }
            string grade = tokens.back(); tokens.pop_back();
            string creditsStr = tokens.back(); tokens.pop_back();
            string name;
            for (size_t i=0;i<tokens.size();++i){ if(i) name+=' '; name+=tokens[i]; }
            double credits{};
            try { credits = stod(creditsStr); } catch(...) { cout << "Invalid credits.\n"; continue; }
            double g4 = parseGradeTo4(grade);
            if (std::isnan(g4)) { cout << "Invalid grade. Try A, B+, 15/20, 75/100...\n"; continue; }
            courses.push_back(Course{name, credits, grade, g4});
            cout << "Added.\n";
            continue;
        }
        cout << "Unknown command. Type 'help'.\n";
    }
    return 0;
}
