"""
dp.py — FreelanceMax
=====================
DP — Weighted Job Scheduling

Goal    : Maximize TOTAL PAYMENT (even if fewer projects).
Strategy: For every project, decide: TAKE it or SKIP it.

  SKIP → best score = dp[i-1]  (unchanged)
  TAKE → best score = project.payment
                    + best score from latest non-overlapping project

Binary search finds the "best score before it" in O(log n).
Backtracking traces which projects were actually chosen.
"""

from typing import List, Tuple
from models import Project
from utils import binary_search, _out


def dp_scheduler(
    projects: List[Project],
    file=None
) -> Tuple[List[Project], int]:
    """
    Run the weighted job scheduling DP algorithm.

    Args:
        projects : list of Project objects sorted by end day
        file     : optional open file object for report writing

    Returns:
        (selected_projects, total_payment)
    """
    n = len(projects)

    title = (
        "\n==============================================\n"
          "  DP: Maximize Total Payment\n"
          "  (Best for maximizing earnings)\n"
          "==============================================\n"
    )
    _out(title, file)

    # dp[i] = max payment possible using the first i projects
    # dp[0] = 0  →  with 0 projects, earnings = 0
    dp = [0] * (n + 1)

    # ── Print DP decision table header ──
    table_header = (
        "\n  DP Decision Table:\n"
          "  +------+------------+----------+------------+----------+\n"
          "  | Step | Project    | Decision | If Taken   | dp[i]    |\n"
          "  +------+------------+----------+------------+----------+\n"
    )
    _out(table_header, file)

    for i in range(1, n + 1):
        # Option A: Skip — keep previous best
        skip = dp[i - 1]

        # Option B: Take — payment + best score before any overlap
        prev      = binary_search(projects, i - 1)
        prev_best = dp[prev + 1] if prev >= 0 else 0
        take      = projects[i - 1].payment + prev_best

        dp[i]    = max(skip, take)
        decision = "TAKE    " if take >= skip else "SKIP    "

        row = (
            f"  | {i:>4} "
            f"| {projects[i-1].name:<10} "
            f"| {decision}"
            f"| Rs.{take:<7} "
            f"| Rs.{dp[i]:<5} |\n"
        )
        _out(row, file)

    table_footer = "  +------+------------+----------+------------+----------+\n"
    _out(table_footer, file)

    # ── Backtrack to find WHICH projects were selected ──
    selected: List[Project] = []
    i = n

    while i > 0:
        prev      = binary_search(projects, i - 1)
        prev_best = dp[prev + 1] if prev >= 0 else 0
        take      = projects[i - 1].payment + prev_best

        if take >= dp[i - 1]:
            selected.append(projects[i - 1])
            i = (prev + 1) if prev >= 0 else 0
        else:
            i -= 1

    selected.reverse()

    # ── Print selected projects ──
    total_payment = 0
    _out("\n  Projects Taken:\n", file)
    for j, p in enumerate(selected):
        _out(f"    {j+1}. {p.name}  (Day {p.start} - Day {p.end}, Rs.{p.payment})\n", file)
        total_payment += p.payment

    result = (
        f"\n  Total Projects : {len(selected)}"
        f"\n  Total Payment  : Rs.{total_payment}"
        f"\n\n  >> You earn the MOST MONEY this way.\n"
        f"     Best when you need income over profile!\n"
    )
    _out(result, file)

    return selected, total_payment


def dp_max_payment(projects: List[Project]) -> Tuple[int, List[int], int]:
    """
    Lightweight DP run used only for comparison summary.
    Returns (max_payment, dp_table, project_count).
    """
    n  = len(projects)
    dp = [0] * (n + 1)

    for i in range(1, n + 1):
        skip      = dp[i - 1]
        prev      = binary_search(projects, i - 1)
        prev_best = dp[prev + 1] if prev >= 0 else 0
        take      = projects[i - 1].payment + prev_best
        dp[i]     = max(skip, take)

    # Count selected projects via backtrack
    count = 0
    i = n
    while i > 0:
        prev      = binary_search(projects, i - 1)
        prev_best = dp[prev + 1] if prev >= 0 else 0
        take      = projects[i - 1].payment + prev_best
        if take >= dp[i - 1]:
            count += 1
            i = (prev + 1) if prev >= 0 else 0
        else:
            i -= 1

    return dp[n], dp, count
