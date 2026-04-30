"""
utils.py — FreelanceMax
========================
Shared utilities:
  - binary_search : finds latest non-overlapping project
  - display_projects : prints + writes the projects table
"""

from typing import List
from models import Project


def binary_search(projects: List[Project], i: int) -> int:
    """
    Given project at index i, find the latest project
    that ENDS before project[i] STARTS (no overlap).

    Projects must be sorted by end day for this to work.

    Returns the index of that project, or -1 if none found.
    """
    low    = 0
    high   = i - 1
    result = -1  # assume no valid project found

    while low <= high:
        mid = (low + high) // 2

        if projects[mid].end <= projects[i].start:
            result = mid      # valid — but a later one may exist
            low = mid + 1     # search right half
        else:
            high = mid - 1    # overlaps — search left half

    return result


def display_projects(projects: List[Project], file=None) -> None:
    """
    Prints the projects table to stdout.
    If a file object is provided, writes the same output there too.
    """
    header = (
        "\n+----+------------+-------+-----+-----------+\n"
          "| No | Name       | Start | End | Payment   |\n"
          "+----+------------+-------+-----+-----------+\n"
    )
    _out(header, file)

    for i, p in enumerate(projects):
        row = (
            f"| {i+1:>2} "
            f"| {p.name:<10} "
            f"| {p.start:>5} "
            f"| {p.end:>3} "
            f"| Rs.{p.payment:<7} |\n"
        )
        _out(row, file)

    footer = "+----+------------+-------+-----+-----------+\n\n"
    _out(footer, file)


# ── Internal helper ──────────────────────────────────────────

def _out(text: str, file=None) -> None:
    """Print to stdout and optionally write to file."""
    print(text, end="")
    if file:
        file.write(text)
