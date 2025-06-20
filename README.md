
# MiniGit-Project

A command line based version control system we built using C++.
This is a lightweight version of Git, developed for our DSA final project.

---

## Feautures implemented

Our MiniGit supports the following commands

| Command      | Description |
|--------------|-------------|
| `init`       | Initializes a new MiniGit directory (`.mingit/`) |
| `add`        | Adds a file to the staging area (we used linked list) |
| `status`     | Shows currently staged files |
| `commit -m`  | Saves a snapshot of staged files with message & timestamp |
| `log`        | Displays commit history in reverse order |
| `branch <name>` | Creates a new branch pointing to current commit |
| `checkout <branch>` | Switches between branches and restores files |
| `merge <branch>` | Performs a merge and shows conflicts if there is any |
| `diff <id1> <id2>` | Compares two commits and shows line-level changes |

---

## The Data Structures we used

 **Linked List** → for tracking staged files
 **Custom nodes** → to store commit history
 **Unordered Sets/Maps** → used in `diff` and `merge` feautures.

 ---

 ### File Input/Output (I/O)
  We used C++ standard streams (`<fstream>`, `<ifstream>`, `<ofstream>`) for handling all file operations.

 Sample test file → `test.txt` → used for testing, staging and editing the commands.
