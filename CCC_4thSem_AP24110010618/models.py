"""
models.py — FreelanceMax
========================
Defines the Project data structure used across all modules.
"""

from dataclasses import dataclass


@dataclass
class Project:
    """Represents a single freelance project offer."""
    name: str       # Project name
    start: int      # Start day
    end: int        # Deadline day
    payment: int    # Payment in Rs
