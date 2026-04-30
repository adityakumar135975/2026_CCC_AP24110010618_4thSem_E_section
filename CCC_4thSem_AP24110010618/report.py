"""
report.py — FreelanceMax
=========================
Handles:
  - Comparison summary (Greedy vs DP side-by-side)
  - Writing the final report to report.txt
"""

from typing import List
from models import Project
from utils import _out
from dp import dp_max_payment


def show_comparison(projects: List[Project], file=None) -> None:
    """
    Recalculates both algorithms quickly and prints a
    side-by-side comparison of Greedy vs DP results.
    """
    # ── Greedy quick pass ──
    last_end     = 0
    greedy_count = 0
    greedy_pay   = 0
    for p in projects:
        if p.start >= last_end:
            last_end      = p.end
            greedy_count += 1
            greedy_pay   += p.payment

    # ── DP quick pass ──
    dp_pay, _, dp_count = dp_max_payment(projects)

    comp = (
        "\n==============================================\n"
          "  COMPARISON SUMMARY\n"
          "==============================================\n\n"
          "                 Greedy        DP\n"
          "  Goal       : Max Projects  Max Payment\n"
          "  -------------------------------------\n"
        f"  Projects   : {greedy_count:<14} {dp_count}\n"
        f"  Payment    : Rs.{greedy_pay:<9}  Rs.{dp_pay}\n\n"
          "  Greedy  → More projects, good for profile\n"
          "  DP      → More money, good for income\n\n"
          "  Your choice depends on your current goal:\n"
          "    New to freelancing and need to gain as much experience as possible?  → Go with Greedy\n"
          "    Need money urgently? → Go with DP\n"
          "==============================================\n"
    )
    _out(comp, file)


def open_report(path: str = "report.txt"):
    """
    Opens the report file for writing.
    Returns the file object (caller must close it).
    """
    try:
        f = open(path, "w", encoding="utf-8")
        return f
    except OSError:
        print(f"ERROR: Could not create {path}!")
        return None


def write_report_header(file) -> None:
    """Writes the report file header."""
    header = (
        "==============================================\n"
          "   FreelanceMax — Freelancer Project Scheduler\n"
          "   Generated Report\n"
          "==============================================\n\n"
          "  Two motivations:\n"
          "  1. More projects = better track record\n"
          "  2. More payment  = better income\n"
          "  FreelanceMax solves BOTH using Greedy + DP.\n"
    )
    file.write(header)
