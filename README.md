# codetrackd — A Daemon that counts hours you code.

Started with `nvim`, but that’s just the beginning.  

Work in progress...

## STATUS: WIP

This is only a skeleton.  
It scans `/proc`, detects when `nvim` is running, and prints how long the session lasted in minutes.

## TODO:

- [ ] Detect **multiple editors**, not just `nvim`
- [ ] Write time logs to files (e.g., `~/.local/share/codetrackd/coding_sessions.txt`)
- [ ] Aggregate and log time per day
- [ ] Output the last 7 days coding time with a simple command ->
```txt
- 17.07.25 -
coding time - 2 hours 13 minutes
- 18.07.25 -
coding time - 1 hours 48 minutes
- 19.07.25 -
coding time - 3 hours 23 minutes
...
...
...
...
== This week ==
coding time - 9 hours 35 minutes
```
