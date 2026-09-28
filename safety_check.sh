#!/bin/bash
# Almaz safety gate: refuses to start if no valid active binary exists.
# Prevents an infinite systemd restart loop after a bad self-deploy.
set -u
cd /opt/almaz || exit 1

if [ ! -f .deploy_state ]; then
    printf 'ACTIVE=almaz\nLKG=almaz\nSTAGING=almaz.B\n' > .deploy_state
fi

ACTIVE=$(grep -E '^ACTIVE=' .deploy_state | head -1 | cut -d= -f2- | tr -d '[:space:]')
LKG=$(grep -E '^LKG=' .deploy_state | head -1 | cut -d= -f2- | tr -d '[:space:]')

if [ -n "$ACTIVE" ] && [ -x "./$ACTIVE" ]; then
    exit 0
fi

if [ -n "$LKG" ] && [ -x "./$LKG" ]; then
    sed -i "s/^ACTIVE=.*/ACTIVE=$LKG/" .deploy_state
    echo "[safety_check] ACTIVE binary '$ACTIVE' invalid; fell back to LKG='$LKG'" >&2
    exit 0
fi

echo "[safety_check] No valid binary (ACTIVE='$ACTIVE' LKG='$LKG'); refusing start to prevent restart loop" >&2
exit 1