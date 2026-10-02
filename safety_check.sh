#!/bin/bash
# Almaz safety gate: refuses to start if no valid active binary exists,
# and (M1/M2) runs the real DB self-test so a corrupted DB cannot boot.
# Also seeds DISTINCT ACTIVE/LKG slots (C3 root fix: ACTIVE==LKG made
# promote.sh's rollback a silent no-op).
set -u
cd /opt/almaz || exit 1

valid_name() { case "$1" in *[!A-Za-z0-9._-]*|"") return 1;; *) return 0;; esac; }
notify() {
  [ -n "${TELEGRAM_BOT_TOKEN:-}" ] || . ./.env 2>/dev/null || true
  [ -n "${TELEGRAM_BOT_TOKEN:-}" ] && [ -n "${TELEGRAM_CHAT_ID:-}" ] || return 0
  curl -s -o /dev/null -X POST "https://api.telegram.org/bot${TELEGRAM_BOT_TOKEN}/sendMessage" \
    -d "chat_id=${TELEGRAM_CHAT_ID}" -d "parse_mode=Markdown" --data-urlencode "text=$1" || true
}

if [ ! -f .deploy_state ]; then
  printf 'ACTIVE=almaz\nLKG=almaz.lkg\nSTAGING=almaz.B\n' > .deploy_state
fi
if [ ! -x ./almaz.lkg ] && [ -x ./almaz ]; then cp -a ./almaz ./almaz.lkg; fi

ACTIVE=$(grep -E '^ACTIVE=' .deploy_state | head -1 | cut -d= -f2- | tr -d '[:space:]')
LKG=$(grep -E '^LKG=' .deploy_state | head -1 | cut -d= -f2- | tr -d '[:space:]')

valid_name "$ACTIVE" || { notify "🚨 *[Almaz Safety]* ACTIVE slot name invalid ($ACTIVE); refusing start"; exit 1; }
valid_name "$LKG"   || { notify "🚨 *[Almaz Safety]* LKG slot name invalid ($LKG); refusing start"; exit 1; }

# Real self-test gate: DB integrity_check must return "ok". Older binaries
# without --selfest are accepted (their --help does not list the flag).
selftest_ok() {
  local b="$1"
  [ -x "./$b" ] || return 1
  if "./$b" --selfest >/dev/null 2>&1; then return 0; fi
  "./$b" --help 2>&1 | grep -q -- "--selfest" && return 1
  return 0
}

if selftest_ok "$ACTIVE"; then
  exit 0
fi
if [ -n "$LKG" ] && selftest_ok "$LKG"; then
  sed -i "s/^ACTIVE=.*/ACTIVE=$LKG/" .deploy_state
  echo "[safety_check] ACTIVE '$ACTIVE' failed selfest; fell back to LKG='$LKG'" >&2
  exit 0
fi

notify "🚨 *[Almaz Safety]* No valid binary (ACTIVE='$ACTIVE' LKG='$LKG'); refusing start to prevent restart loop"
echo "[safety_check] No valid binary (ACTIVE='$ACTIVE' LKG='$LKG'); refusing start to prevent restart loop" >&2
exit 1