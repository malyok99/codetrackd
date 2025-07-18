# codetrackd

Built to track how much time you spend coding.  
Started with `nvim`, but the framework is editor-agnostic - extend it to whatever you touch.

## 📦 Build

```bash
# Build with
g++ -std=c++20 -O2 main.cpp logger.cpp -o codetrackd
```

## ⚔️ Notes

What it does:
- Scans `/proc` for `nvim`
- Measures coding session duration
- Accumulates daily totals
- Logs to a persistent file (~/.programming_tracker/coding_log.txt)

## TODO

- [-] Detect **multiple editors**, not just `nvim`
- [+] Aggregate and log time per day
- [-] CLI command: total time for the last 7 days

## Output Format:

```txt
- 17.07.25 -
coding time - 2 hours 13 minutes
- 18.07.25 -
coding time - 1 hour 48 minutes
- 19.07.25 -
coding time - 3 hours 23 minutes
```
