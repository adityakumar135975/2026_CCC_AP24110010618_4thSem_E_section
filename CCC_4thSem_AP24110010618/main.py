"""
main.py — FreelanceMax
=======================
Entry point. Handles:
  - User input
  - Sorting projects by end day
  - Orchestrating Greedy + DP + Report
"""

from models import Project
from utils import display_projects
from greedy import greedy_scheduler
from dp import dp_scheduler
from report import show_comparison, open_report, write_report_header


def get_input() -> list[Project]:
    """Prompt the user and collect all project details."""
    print("==============================================")
    print("   FreelanceMax — Freelancer Project Scheduler")
    print("==============================================")
    print("\n  You are a freelance developer with two goals:")
    print("  1. Build a strong track record (more projects)")
    print("  2. Maximize your total earnings (more money)")
    print("\n  FreelanceMax finds the best schedule for BOTH!\n")

    n = int(input("Enter the number of projects that you have: "))
    projects: list[Project] = []

    print("\nEnter project details:")
    print("(Use day numbers e.g. Day 1, Day 5 etc.)\n")

    for i in range(n):
        print(f"-- Project {i+1} --")
        name    = input("  Name        : ").strip()
        start   = int(input("  Start Day   : "))
        end     = int(input("  End Day     : "))
        payment = int(input("  Payment(Rs) : "))
        print()
        projects.append(Project(name=name, start=start, end=end, payment=payment))

    return projects


def main() -> None:
    # ── Collect input ──
    projects = get_input()

    # ── Sort by end day (required for both Greedy and DP) ──
    projects.sort(key=lambda p: p.end)

    # ── Open report file ──
    report_file = open_report("report.txt")
    if report_file is None:
        return

    # ── Report header ──
    write_report_header(report_file)
    print("\n" + "=" * 46)

    # ── Display all projects ──
    section = (
        "\n----------------------------------------------\n"
          "  ALL PROJECTS (sorted by deadline)\n"
          "----------------------------------------------\n"
    )
    print(section, end="")
    report_file.write(section)
    display_projects(projects, report_file)

    # ── Run Greedy ──
    greedy_scheduler(projects, report_file)

    # ── Run DP ──
    dp_scheduler(projects, report_file)

    # ── Comparison summary ──
    show_comparison(projects, report_file)

    # ── Done ──
    report_file.close()
    print("\n>> Report saved to report.txt")
    print("==============================================")


if __name__ == "__main__":
    main()
