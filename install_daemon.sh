#!/bin/bash
set -e

echo "Compiling codetrackd..."
g++ main.cpp logger.cpp -o codetrackd -std=c++20

echo "Creating ~/bin if not exists..."
mkdir -p "$HOME/bin"

echo "Moving binary to ~/bin/"
mv -f codetrackd "$HOME/bin/"
chmod +x "$HOME/bin/codetrackd"

if ! grep -q 'export PATH="$HOME/bin:$PATH"' "$HOME/.zshrc"; then
  echo 'export PATH="$HOME/bin:$PATH"' >> "$HOME/.zshrc"
  echo "Added ~/bin to PATH in ~/.zshrc"
else
  echo "PATH already contains ~/bin in ~/.zshrc"
fi

echo "Setting up systemd user service..."

mkdir -p "$HOME/.config/systemd/user"

cat > "$HOME/.config/systemd/user/codetrackd.service" <<EOF
[Unit]
Description=Code Tracker Daemon (monitors nvim)

[Service]
Type=simple
ExecStart=$HOME/bin/codetrackd
Restart=on-failure

[Install]
WantedBy=default.target
EOF

echo "Reloading and enabling codetrackd.service..."
systemctl --user daemon-reexec
systemctl --user daemon-reload
systemctl --user enable codetrackd.service
systemctl --user start codetrackd.service

echo "[+] codetrackd installed and running as user daemon!"
echo "Use: codetrackd --show 7"
