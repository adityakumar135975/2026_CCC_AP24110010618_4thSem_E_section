/*
 * ============================================================
 *   FreelanceMax — Freelancer Project Scheduler
 *   Greedy (Max Projects) + DP (Max Payment)
 * ============================================================
 *
 *   You are a freelance developer with two goals:
 *
 *   GOAL 1 — BUILD A TRACK RECORD (Greedy)
 *     You are new to freelancing. Clients check how many
 *     projects you have completed before hiring you. More
 *     completed projects = better profile = more trust.
 *     You want to take on AS MANY projects as possible,
 *     even if some don't pay much.
 *
 *   GOAL 2 — MAXIMIZE EARNINGS (DP)
 *     You also have bills to pay. Some projects pay a lot
 *     but take longer, leaving no time for smaller ones.
 *     You want the schedule that gives the MAXIMUM total
 *     payment, even if it means fewer projects.
 *
 *   The conflict:
 *     More projects != More money
 *     More money    != More projects
 *
 *   FreelanceMax shows you BOTH answers so you can decide
 *   based on what matters most to you right now.
 *
 * ALGORITHMS:
 *   Greedy -> Activity Selection (earliest deadline first)
 *             Maximizes NUMBER of non-overlapping projects
 *
 *   DP     -> Weighted Job Scheduling
 *             Maximizes TOTAL PAYMENT from projects
 *
 * EQUIVALENT TO:
 *   models.py  -> Project struct
 *   utils.py   -> binary_search(), display_projects(), _out()
 *   greedy.py  -> greedy_scheduler()
 *   dp.py      -> dp_scheduler(), dp_max_payment()
 *   report.py  -> show_comparison(), open_report()
 *   main.py    -> get_input(), main()
 * ============================================================
 */

#include <iostream>
#include <fstream>
#include <vector>
#include <algorithm>
#include <string>
#include <iomanip>
using namespace std;

// ============================================================
//  models.py equivalent
//  Project struct — one freelance project offer
// ============================================================
struct Project {
    string name;    // Project name
    int start;      // Start day
    int end;        // Deadline day
    int payment;    // Payment in Rs
};

// ============================================================
//  utils.py equivalent
//  _out() — print to screen AND write to file at same time
// ============================================================
void _out(const string& text, ofstream& file) {
    cout << text;
    if (file.is_open())
        file << text;
}

// ============================================================
//  utils.py equivalent
//  binary_search() — finds latest project that ends before
//  project[i] starts (no overlap).
//
//  Works because projects are sorted by end day.
//  Returns index of that project, or -1 if none found.
// ============================================================
int binary_search(vector<Project>& projects, int i) {
    int low    = 0;
    int high   = i - 1;
    int result = -1;  // -1 means no valid project found

    while (low <= high) {
        int mid = (low + high) / 2;

        if (projects[mid].end <= projects[i].start) {
            result = mid;      // valid — but a later one may exist
            low = mid + 1;     // search right half
        } else {
            high = mid - 1;    // overlaps — search left half
        }
    }

    return result;
}

// ============================================================
//  utils.py equivalent
//  display_projects() — prints projects table to screen
//  and writes to report file at the same time
// ============================================================
void display_projects(vector<Project>& projects, ofstream& file) {
    string header =
        "\n+----+------------+-------+-----+-----------+\n"
          "| No | Name       | Start | End | Payment   |\n"
          "+----+------------+-------+-----+-----------+\n";
    _out(header, file);

    for (int i = 0; i < (int)projects.size(); i++) {
        // Build each row
        string row = "";
        row += "| ";
        row += string(2 - to_string(i+1).length(), ' ') + to_string(i+1);
        row += " | ";

        // Pad name to 10 chars
        string name = projects[i].name;
        name.resize(10, ' ');
        row += name + " | ";

        // Pad start to 5 chars
        string start = to_string(projects[i].start);
        row += string(5 - start.length(), ' ') + start + " | ";

        // Pad end to 3 chars
        string end = to_string(projects[i].end);
        row += string(3 - end.length(), ' ') + end + " | ";

        // Payment
        string pay = "Rs." + to_string(projects[i].payment);
        pay.resize(10, ' ');
        row += pay + " |\n";

        _out(row, file);
    }

    string footer = "+----+------------+-------+-----+-----------+\n\n";
    _out(footer, file);
}

// ============================================================
//  greedy.py equivalent
//  greedy_scheduler() — Activity Selection
//
//  Goal    : Maximize NUMBER of non-overlapping projects
//  Strategy: Always pick the project that finishes earliest
//            This leaves maximum room for future projects
// ============================================================
void greedy_scheduler(vector<Project>& projects, ofstream& file) {
    string title =
        "\n==============================================\n"
          "  GREEDY: Maximize Number of Projects\n"
          "  (Best for building a track record)\n"
          "==============================================\n";
    _out(title, file);

    int last_end      = 0;
    int total_payment = 0;
    vector<Project> selected;

    for (auto& p : projects) {
        // Take project only if it starts after the last one ended
        if (p.start >= last_end) {
            selected.push_back(p);
            last_end       = p.end;
            total_payment += p.payment;
        }
    }

    // Print selected projects
    _out("\n  Projects Taken:\n", file);
    for (int i = 0; i < (int)selected.size(); i++) {
        _out("    " + to_string(i+1) + ". " + selected[i].name + "  (Day " + to_string(selected[i].start) + " - Day " + to_string(selected[i].end) + ", Rs." + to_string(selected[i].payment) + ")\n", file);
    }

    string result =
        "\n  Total Projects : " + to_string((int)selected.size()) +
        "\n  Total Payment  : Rs." + to_string(total_payment) +
        "\n\n  >> You complete the MOST projects this way.\n"
          "     Great for your freelancing profile!\n";
    _out(result, file);
}

// ============================================================
//  dp.py equivalent — dp_max_payment()
//  Lightweight DP used only for comparison summary.
//  Returns max payment and project count.
// ============================================================
pair<int,int> dp_max_payment(vector<Project>& projects) {
    int n = projects.size();
    vector<int> dp(n + 1, 0);

    for (int i = 1; i <= n; i++) {
        int skip      = dp[i - 1];
        int prev      = binary_search(projects, i - 1);
        int prev_best = (prev >= 0) ? dp[prev + 1] : 0;
        int take      = projects[i - 1].payment + prev_best;
        dp[i]         = max(skip, take);
    }

    // Backtrack to count selected projects
    int count = 0;
    int i     = n;
    while (i > 0) {
        int prev      = binary_search(projects, i - 1);
        int prev_best = (prev >= 0) ? dp[prev + 1] : 0;
        int take      = projects[i - 1].payment + prev_best;
        if (take >= dp[i - 1]) {
            count++;
            i = (prev >= 0) ? prev + 1 : 0;
        } else {
            i--;
        }
    }

    return {dp[n], count};
}

// ============================================================
//  dp.py equivalent — dp_scheduler()
//  Weighted Job Scheduling DP
//
//  Goal    : Maximize TOTAL PAYMENT (even if fewer projects)
//  Strategy: For every project decide TAKE it or SKIP it
//
//    SKIP -> best score = dp[i-1]  (unchanged)
//    TAKE -> best score = project.payment
//                       + best score from latest non-overlapping
//
//  Binary search finds "best score before it" in O(log n)
//  Backtracking traces which projects were actually chosen
// ============================================================
void dp_scheduler(vector<Project>& projects, ofstream& file) {
    int n = projects.size();

    string title =
        "\n==============================================\n"
          "  DP: Maximize Total Payment\n"
          "  (Best for maximizing earnings)\n"
          "==============================================\n";
    _out(title, file);

    // dp[i] = max payment possible using first i projects
    // dp[0] = 0 means: with 0 projects, earnings = 0
    vector<int> dp(n + 1, 0);

    // Print DP table header
    string table_header =
        "\n  DP Decision Table:\n"
          "  +------+------------+----------+------------+----------+\n"
          "  | Step | Project    | Decision | If Taken   | dp[i]    |\n"
          "  +------+------------+----------+------------+----------+\n";
    _out(table_header, file);

    for (int i = 1; i <= n; i++) {
        // Option A: Skip this project -> keep previous best
        int skip = dp[i - 1];

        // Option B: Take this project
        // Find latest non-overlapping project using binary search
        int prev      = binary_search(projects, i - 1);
        int prev_best = (prev >= 0) ? dp[prev + 1] : 0;
        int take      = projects[i - 1].payment + prev_best;

        // Pick the better option
        dp[i] = max(skip, take);

        string decision = (take >= skip) ? "TAKE    " : "SKIP    ";

        // Pad project name to 10 chars
        string name = projects[i-1].name;
        name.resize(10, ' ');

        // Build row
        string step_str = to_string(i);
        string take_str = to_string(take);
        string dp_str   = to_string(dp[i]);

        string row =
            "  | " + string(4 - step_str.length(), ' ') + step_str +
            " | " + name +
            " | " + decision +
            "| Rs." + take_str + string(7 - take_str.length(), ' ') +
            " | Rs." + dp_str + string(5 - dp_str.length(), ' ') + " |\n";

        _out(row, file);
    }

    string table_footer =
        "  +------+------------+----------+------------+----------+\n";
    _out(table_footer, file);

    // ── Backtrack to find WHICH projects were selected ──
    // Go backwards through dp[] to trace the decisions
    vector<Project> selected;
    int i = n;

    while (i > 0) {
        int prev      = binary_search(projects, i - 1);
        int prev_best = (prev >= 0) ? dp[prev + 1] : 0;
        int take      = projects[i - 1].payment + prev_best;

        if (take >= dp[i - 1]) {
            selected.push_back(projects[i - 1]);
            i = (prev >= 0) ? prev + 1 : 0;
        } else {
            i--;
        }
    }
    reverse(selected.begin(), selected.end());

    // Print selected projects
    int total_payment = 0;
    _out("\n  Projects Taken:\n", file);
    for (int j = 0; j < (int)selected.size(); j++) {
        _out("    " + to_string(j+1) + ". " + selected[j].name +
             "  (Day " + to_string(selected[j].start) +
             " - Day " + to_string(selected[j].end) +
             ", Rs." + to_string(selected[j].payment) + ")\n", file);
        total_payment += selected[j].payment;
    }

    string result =
        "\n  Total Projects : " + to_string((int)selected.size()) +
        "\n  Total Payment  : Rs." + to_string(total_payment) +
        "\n\n  >> You earn the MOST MONEY this way.\n"
          "     Best when you need income over profile!\n";
    _out(result, file);
}

// ============================================================
//  report.py equivalent
//  show_comparison() — side-by-side Greedy vs DP summary
// ============================================================
void show_comparison(vector<Project>& projects, ofstream& file) {
    // Greedy quick pass
    int last_end     = 0;
    int greedy_count = 0;
    int greedy_pay   = 0;
    for (auto& p : projects) {
        if (p.start >= last_end) {
            last_end = p.end;
            greedy_count++;
            greedy_pay += p.payment;
        }
    }

    // DP quick pass
    auto [dp_pay, dp_count] = dp_max_payment(projects);

    string gc = to_string(greedy_count);
    string dc = to_string(dp_count);
    string gp = to_string(greedy_pay);
    string dp = to_string(dp_pay);

    string comp =
        "\n==============================================\n"
          "  COMPARISON SUMMARY\n"
          "==============================================\n\n"
          "                 Greedy        DP\n"
          "  Goal       : Max Projects  Max Payment\n"
          "  -------------------------------------\n"
        "  Projects   : " + gc + string(14 - gc.length(), ' ') + dc + "\n"
        "  Payment    : Rs." + gp + string(9 - gp.length(), ' ') + "  Rs." + dp + "\n\n"
          "  Greedy  -> More projects, good for profile\n"
          "  DP      -> More money, good for income\n\n"
          "  Your choice depends on your current goal:\n"
          "    New to freelancing? -> Go with Greedy\n"
          "    Need money urgently? -> Go with DP\n"
          "==============================================\n";
    _out(comp, file);
}

// ============================================================
//  report.py equivalent
//  write_report_header() — writes header to report file
// ============================================================
void write_report_header(ofstream& file) {
    string header =
        "==============================================\n"
          "   FreelanceMax — Freelancer Project Scheduler\n"
          "   Generated Report\n"
          "==============================================\n\n"
          "  Two motivations:\n"
          "  1. More projects = better track record\n"
          "  2. More payment  = better income\n"
          "  FreelanceMax solves BOTH using Greedy + DP.\n";
    file << header;
}

// ============================================================
//  main.py equivalent
//  get_input() — collects project details from user
// ============================================================
vector<Project> get_input() {
    cout << "==============================================\n";
    cout << "   FreelanceMax — Freelancer Project Scheduler\n";
    cout << "==============================================\n";
    cout << "\n  You are a freelance developer with two goals:\n";
    cout << "  1. Build a strong track record (more projects)\n";
    cout << "  2. Maximize your total earnings (more money)\n";
    cout << "\n  FreelanceMax finds the best schedule for BOTH!\n\n";

    int n;
    cout << "Enter the number of projects that you have: ";
    cin >> n;
    cin.ignore();

    vector<Project> projects(n);

    cout << "\nEnter project details:\n";
    cout << "(Use day numbers e.g. Day 1, Day 5 etc.)\n\n";

    for (int i = 0; i < n; i++) {
        cout << "-- Project " << i + 1 << " --\n";

        cout << "  Name        : ";
        getline(cin, projects[i].name);

        cout << "  Start Day   : ";
        cin >> projects[i].start;

        cout << "  End Day     : ";
        cin >> projects[i].end;

        cout << "  Payment(Rs) : ";
        cin >> projects[i].payment;
        cin.ignore();

        cout << "\n";
    }

    return projects;
}

// ============================================================
//  main.py equivalent — main()
//  Entry point: input -> sort -> greedy -> dp -> report
// ============================================================
int main() {
    // Collect input
    vector<Project> projects = get_input();

    // Sort by end day (required for both Greedy and DP)
    sort(projects.begin(), projects.end(), [](const Project& a, const Project& b) {
        return a.end < b.end;
    });

    // Open report file
    ofstream report_file("report.txt");
    if (!report_file.is_open()) {
        cout << "ERROR: Could not create report.txt!\n";
        return 1;
    }

    // Write report header
    write_report_header(report_file);
    cout << "\n" << string(46, '=') << "\n";

    // Display all projects
    string section =
        "\n----------------------------------------------\n"
          "  ALL PROJECTS (sorted by deadline)\n"
          "----------------------------------------------\n";
    _out(section, report_file);
    display_projects(projects, report_file);

    // Run Greedy
    greedy_scheduler(projects, report_file);

    // Run DP
    dp_scheduler(projects, report_file);

    // Comparison summary
    show_comparison(projects, report_file);

    // Done
    report_file.close();
    cout << "\n>> Report saved to report.txt\n";
    cout << "==============================================\n";

    return 0;
}
