#!/bin/bash
set -e

echo "Stopping and disabling codetrackd.service..."
systemctl --user stop codetrackd.service || true
systemctl --user disable codetrackd.service || true

echo "Removing systemd service file..."
rm -f "$HOME/.config/systemd/user/codetrackd.service"

echo "Reloading systemd..."
systemctl --user daemon-reexec
systemctl --user daemon-reload

echo "Removing binary from ~/bin/..."
rm -f "$HOME/bin/codetrackd"

echo "[!] Optional: Remove PATH export from ~/.zshrc manually if needed."

echo "[+] codetrackd fully uninstalled."
