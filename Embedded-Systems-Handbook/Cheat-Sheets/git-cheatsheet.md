# Git Cheat Sheet

## Version Control untuk Embedded Developer

---

## 1. Basic Commands

### Setup & Config
```bash
# First time setup
git config --global user.name "Your Name"
git config --global user.email "your.email@example.com"

# Check config
git config --list

# Set default branch name
git config --global init.defaultBranch main
```

### Repository Initialization
```bash
# Initialize new repo
git init

# Clone existing repo
git clone <url>

# Clone specific branch
git clone -b <branch-name> <url>
```

---

## 2. Daily Workflow

### Check Status
```bash
# See changed files
git status

# See detailed changes
git status -v

# See untracked files too
git status -u
```

### Add Files
```bash
# Add specific file
git add filename.c

# Add all changed files
git add .

# Add all including untracked
git add -A

# Add interactively (choose what to stage)
git add -p
```

### Commit
```bash
# Commit with message
git commit -m "feat: add UART driver"

# Commit all changed files
git commit -am "fix: correct ADC calibration"

# Amend last commit (change message or add files)
git commit --amend -m "feat: add complete UART driver"

# View commit history
git log
git log --oneline
git log --graph --oneline --all
```

---

## 3. Branch Management

### Create & Switch
```bash
# Create new branch
git branch feature/uart-driver

# Switch to branch
git checkout feature/uart-driver

# Create and switch (shortcut)
git checkout -b feature/uart-driver

# List all branches
git branch
git branch -a  # Include remote branches
```

### Merge
```bash
# Switch to main branch
git checkout main

# Merge feature branch
git merge feature/uart-driver

# Merge with no fast-forward (create merge commit)
git merge --no-ff feature/uart-driver

# Abort merge if conflicts
git merge --abort
```

### Delete Branch
```bash
# Delete local branch
git branch -d feature/uart-driver

# Force delete (if not merged)
git branch -D feature/uart-driver

# Delete remote branch
git push origin --delete feature/uart-driver
```

---

## 4. Remote Operations

### Sync with Remote
```bash
# Fetch changes (don't merge)
git fetch origin

# Pull and merge
git pull origin main

# Pull with rebase (cleaner history)
git pull --rebase origin main

# Push changes
git push origin main

# Push and set upstream
git push -u origin main

# Force push (use carefully!)
git push --force-with-lease origin main
```

### View Remotes
```bash
# List remotes
git remote -v

# Add remote
git remote add upstream <url>

# Rename remote
git remote rename old-name new-name
```

---

## 5. Undo Changes

### Unstage Files
```bash
# Unstage single file
git reset HEAD filename.c

# Unstage all files
git reset HEAD

# Keep changes but remove from staging
git restore --staged filename.c
```

### Discard Changes
```bash
# Discard working directory changes
git checkout -- filename.c

# Discard all changes
git checkout -- .

# Modern syntax
git restore filename.c
git restore .
```

### Reset Commits
```bash
# Soft reset (keep changes staged)
git reset --soft HEAD~1

# Mixed reset (keep changes unstaged) - DEFAULT
git reset HEAD~1

# Hard reset (discard all changes)
git reset --hard HEAD~1

# Reset to specific commit
git reset --hard <commit-hash>
```

---

## 6. Stash

### Save Work Temporarily
```bash
# Stash current changes
git stash

# Stash with message
git stash save "WIP: ADC driver"

# List stashes
git stash list

# Apply latest stash (keep in list)
git stash apply

# Apply and remove from list
git stash pop

# Apply specific stash
git stash apply stash@{2}

# Drop/delete stash
git stash drop stash@{1}

# Clear all stashes
git stash clear
```

---

## 7. Viewing Changes

### Diff
```bash
# Unstaged changes
git diff

# Staged changes
git diff --staged

# Between branches
git diff main feature/uart-driver

# Specific file
git diff filename.c

# Show only filenames
git diff --name-only
```

### Blame
```bash
# See who changed each line
git blame filename.c

# Blame with line numbers
git blame -n filename.c

# Blame specific line range
git blame -L 10,20 filename.c
```

### Show Commit
```bash
# Show commit details
git show <commit-hash>

# Show only stats
git show --stat <commit-hash>

# Show only filename
git show --name-only <commit-hash>
```

---

## 8. Tagging

### Create Tags
```bash
# Lightweight tag
git tag v1.0.0

# Annotated tag (recommended)
git tag -a v1.0.0 -m "Release version 1.0.0"

# Tag specific commit
git tag -a v1.0.0 <commit-hash> -m "Release"
```

### Manage Tags
```bash
# List tags
git tag
git tag -l "v1.*"  # Filter by pattern

# Show tag info
git show v1.0.0

# Push tags to remote
git push origin v1.0.0
git push origin --tags  # All tags

# Delete tag
git tag -d v1.0.0
git push origin --delete v1.0.0
```

---

## 9. Conflict Resolution

### When Conflicts Occur
```bash
# See conflicted files
git status

# Open file, look for markers:
<<<<<<< HEAD
// Your changes
=======
// Their changes
>>>>>>> feature-branch

# After fixing, mark as resolved
git add filename.c

# Complete merge
git commit

# Or abort
git merge --abort
```

### Tools for Conflict Resolution
```bash
# Use mergetool (configured in git config)
git mergetool

# Common mergetools: meld, kdiff3, p4merge, vimdiff
git config --global merge.tool meld
```

---

## 10. Advanced Topics

### Rebase
```bash
# Rebase current branch onto main
git rebase main

# Interactive rebase (edit commits)
git rebase -i HEAD~5

# Continue after resolving conflicts
git rebase --continue

# Abort rebase
git rebase --abort
```

### Cherry-pick
```bash
# Apply specific commit from another branch
git cherry-pick <commit-hash>

# Cherry-pick multiple commits
git cherry-pick <hash1> <hash2> <hash3>

# Cherry-pick range
git cherry-pick <hash1>^..<hash2>
```

### Bisect (Find Bug)
```bash
# Start bisect
git bisect start

# Mark current as bad
git bisect bad

# Mark known good commit
git bisect good <commit-hash>

# Git will checkout commits, test and mark:
git bisect good  # or git bisect bad

# End bisect
git bisect reset
```

### Reflog (Safety Net)
```bash
# View all actions (including undone ones)
git reflog

# Recover lost commit
git reset --hard HEAD@{5}

# Clean old reflog entries
git reflog expire --expire=now --all
git gc --prune=now
```

---

## 11. Git Ignore

### Create .gitignore
```gitignore
# Compiled files
*.o
*.obj
*.exe
*.elf
*.bin
*.hex
*.map

# Build directories
/build/
/build_*
/CMakeFiles/
/cmake-build-*/

# IDE files
.vscode/
.idea/
*.swp
*.swo
*~

# Debug files
*.log
*.dump

# Temporary files
*.tmp
*.temp

# OS files
.DS_Store
Thumbs.db

# Keep the directory but ignore contents
empty_directory/.gitkeep
```

### Override .gitignore
```bash
# Force add ignored file
git add -f important_file.exe

# Check what's ignored
git check-ignore -v filename
```

---

## 12. Commit Message Convention

### Conventional Commits Format
```
<type>(<scope>): <subject>

<body>

<footer>
```

### Types
| Type | Description |
|------|-------------|
| `feat` | New feature |
| `fix` | Bug fix |
| `docs` | Documentation change |
| `style` | Formatting, no code change |
| `refactor` | Code restructuring |
| `perf` | Performance improvement |
| `test` | Adding tests |
| `chore` | Maintenance tasks |

### Examples
```bash
# Good commit messages
git commit -m "feat(uart): add DMA receive support"
git commit -m "fix(adc): correct calibration offset for channel 2"
git commit -m "docs(readme): update build instructions"
git commit -m "refactor(gpio): extract pin configuration logic"
git commit -m "test(spi): add unit tests for transfer functions"

# Bad commit messages (avoid these)
git commit -m "fixed stuff"
git commit -m "update"
git commit -m "asdfasdf"
git commit -m "WIP"
```

### Multi-line Commit
```bash
git commit -m "feat(i2c): implement multi-byte read/write

- Add I2C_Master_Read_Mem function
- Add I2C_Master_Write_Mem function
- Handle NACK condition properly
- Add timeout mechanism

Fixes issue #42"
```

---

## 13. Embedded-Specific Workflows

### Firmware Version Tagging
```bash
# Tag with semantic versioning
git tag -a v1.2.3 -m "Firmware release 1.2.3"

# Include build number
git tag -a v1.2.3-build.45 -m "Firmware with build 45"

# Generate version header from git
echo "#define FW_VERSION \"$(git describe --tags)\"" > version.h
```

### Handling Binary Files
```bash
# Track binary files (firmware.hex, etc.)
git add firmware.hex

# Use Git LFS for large binaries
git lfs install
git lfs track "*.bin"
git lfs track "*.hex"
git add .gitattributes
```

### Submodules (for libraries)
```bash
# Add submodule
git submodule add https://github.com/org/library.git libs/library

# Initialize submodules
git submodule init
git submodule update

# Update submodules to latest
git submodule update --remote

# Clone repo with submodules
git clone --recursive <url>
```

---

## 14. Useful Aliases

Add to `~/.gitconfig`:
```ini
[alias]
    co = checkout
    ci = commit
    st = status
    br = branch
    df = diff
    lg = log --oneline --graph --all
    last = log -1 HEAD --stat
    undo = reset --soft HEAD~1
    unstage = reset HEAD --
    who = shortlog -sn --
    amend = commit --amend
    rb = rebase
    cp = cherry-pick
    ff = merge --ff-only
    noff = merge --no-ff
```

Usage:
```bash
git co feature-branch
git st
git lg
git undo
```

---

## 15. Troubleshooting

### Common Issues

| Problem | Solution |
|---------|----------|
| "Please commit your changes before merging" | `git stash` then merge, then `git stash pop` |
| "Refusing to merge unrelated histories" | `git merge --allow-unrelated-histories` |
| "Permission denied (publickey)" | Setup SSH key: `ssh-keygen -t ed25519` |
| "Changes not staged for commit" | `git add .` then `git commit` |
| "Already up to date" | You're on latest, no changes to pull |
| Detached HEAD state | `git checkout <branch>` or `git checkout -b new-branch` |

### Recovery Commands
```bash
# Recover deleted branch
git reflog
git checkout -b recovered-branch <commit-hash>

# Undo last push (if alone on branch)
git reset --hard HEAD~1
git push --force-with-lease

# Find which commit broke something
git bisect start
```

---

## Quick Reference Card

```
┌─────────────────────────────────────────────────────────────┐
│                    GIT QUICK REFERENCE                       │
├─────────────────────────────────────────────────────────────┤
│  STATUS      │ git status                                    │
│  ADD         │ git add <file> / git add -A                   │
│  COMMIT      │ git commit -m "message"                       │
│  PUSH        │ git push origin <branch>                      │
│  PULL        │ git pull origin <branch>                      │
├─────────────────────────────────────────────────────────────┤
│  NEW BRANCH  │ git checkout -b <branch-name>                 │
│  SWITCH      │ git checkout <branch-name>                    │
│  MERGE       │ git merge <branch-name>                       │
│  DELETE      │ git branch -d <branch-name>                   │
├─────────────────────────────────────────────────────────────┤
│  UNSTAGE     │ git reset HEAD <file>                         │
│  DISCARD     │ git checkout -- <file>                        │
│  UNDO COMMIT │ git reset --soft HEAD~1                       │
│  STASH       │ git stash save/pop                            │
├─────────────────────────────────────────────────────────────┤
│  LOG         │ git log --oneline --graph                     │
│  DIFF        │ git diff                                      │
│  TAG         │ git tag -a v1.0.0 -m "message"                │
│  CLONE       │ git clone <url>                               │
└─────────────────────────────────────────────────────────────┘
```

---

**Pro Tip:** Always write meaningful commit messages. Your future self (and teammates) will thank you!

---

*Last Updated: [Tanggal]*  
*Untuk workflow spesifik project, lihat Day-05-Embedded-Architecture.md*
