"""
greedy.py — FreelanceMax
=========================
GREEDY — Activity Selection (Earliest Deadline First)

Goal    : Maximize the NUMBER of non-overlapping projects.
Strategy: Always pick the project that finishes earliest.
          This leaves maximum room for future projects.

Why greedy works:
  Picking the earliest-finishing project is provably optimal
  for maximizing project count — it never blocks more future
  projects than any other choice would.
"""

from typing import List, Tuple
from models import Project
from utils import _out


def greedy_scheduler(
    projects: List[Project],
    file=None
) -> Tuple[List[Project], int]:
    """
    Run the greedy activity-selection algorithm.

    Args:
        projects : list of Project objects sorted by end day
        file     : optional open file object for report writing

    Returns:
        (selected_projects, total_payment)
    """
    title = (
        "\n==============================================\n"
          "  GREEDY: Maximize Number of Projects\n"
          "  (Best for building a track record)\n"
          "==============================================\n"
    )
    _out(title, file)

    last_end      = 0
    total_payment = 0
    selected: List[Project] = []

    for p in projects:
        # Take project only if it starts after the last one ended
        if p.start >= last_end:
            selected.append(p)
            last_end       = p.end
            total_payment += p.payment

    # ── Print selected projects ──
    _out("\n  Projects Taken:\n", file)
    for i, p in enumerate(selected):
        _out(f"    {i+1}. {p.name}\n", file)

    result = (
        f"\n  Total Projects : {len(selected)}"
        f"\n  Total Payment  : Rs.{total_payment}"
        f"\n\n  >> You complete the MOST projects this way.\n"
        f"     Great for your freelancing profile!\n"
    )
    _out(result, file)

    return selected, total_payment
