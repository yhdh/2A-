# CGPA Calculator (C++)

Simple CLI to compute CGPA given course grades and credits. Supports saving/loading, GPA scale mapping, and cumulative updates.

Build
- macOS/Linux: make
- Windows (MinGW): g++ -std=c++17 -O2 -o cgpa src/*.cpp

Run
- ./cgpa

Features
- Add semesters, courses (name, credits, grade)
- Grade input as letter (A, B+, etc.) or numeric on 20 or 100, auto-mapped to 4.0 scale
- Save/Load JSON files
- Compute CGPA, per-semester GPA

