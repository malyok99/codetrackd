> ⚠️ **Project closed:** This repository is now part of [Prophet.Core](https://github.com/malyok99/prophet.core).

# codetrackd

Built to track exactly how much time you spend *actually* coding.

## 📦 Build & Install

```bash
# Install with
./install_daemon.sh

# Remove with
./uninstall_daemon.sh
```

## ⚔️ What codetrackd *really* does

- Scans `/proc` for **most popular editors** listed in `main.cpp` 
- Measures your active coding session duration
- Aggregates and logs daily totals
- Writes persistent logs to `~/.programming_tracker/coding_log.txt`

## Usage example - Summarize your coding time with a single command:

```bash
~ ❯ codetrackd --show 3
=== Last 3 Days of Coding ===
16.07.25: no data
17.07.25: coding time - 2 hours 3 minutes
18.07.25: coding time - 5 hours 13 minutes

== SUMMARY ==
coding hours - 7 hours 26 minutes
```
