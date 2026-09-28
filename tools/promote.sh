#!/bin/bash
# Almaz canary promotion + rollback (Week-3 self-fix).
# Gates promotion on the candidate's own --selfest, then witnesses the
# service for 90s and rolls back to the LKG slot on any failure.
# The watchdog's crash-loop breaker remains the last line of defense.
#
# Usage: promote.sh <candidate-binary>
#   WORKDIR overrides the workspace (default /opt/almaz) for testing.
set -eu

CDIR="${WORKDIR:-/opt/almaz}"
CAND="${1:?usage: promote.sh <candidate-binary>}"
cd "$CDIR"

[ -f .deploy_state ] || { echo "no .deploy_state; refusing to guess" >&2; exit 1; }
ACTIVE=$(grep -E '^ACTIVE=' .deploy_state | cut -d= -f2- | tr -d '[:space:]')
LKG=$(grep -E '^LKG=' .deploy_state | cut -d= -f2- | tr -d '[:space:]')
STAGING=$(grep -E '^STAGING=' .deploy_state | cut -d= -f2- | tr -d '[:space:]')

# Load Telegram creds only if not already in env (never print)
if [ -z "${TELEGRAM_BOT_TOKEN:-}" ]; then
  # shellcheck disable=SC1091
  . ./.env 2>/dev/null || true
fi
notify() {
  [ -n "${TELEGRAM_BOT_TOKEN:-}" ] && [ -n "${TELEGRAM_CHAT_ID:-}" ] || return 0
  curl -s -o /dev/null -X POST "https://api.telegram.org/bot${TELEGRAM_BOT_TOKEN}/sendMessage" \
    -d "chat_id=${TELEGRAM_CHAT_ID}" -d "parse_mode=Markdown" --data-urlencode "text=$1" || true
}
fail() {
  echo "[promote] FAIL: $1" >&2
  notify "🚨 *[Almaz Deploy Self-Fix]* $1 — staying on LKG. (breaker will fall back automatically if it runs)"
  exit 1
}

[ -x "$CAND" ] || fail "candidate not executable: $CAND"
[ -n "$ACTIVE" ] && [ -n "$LKG" ] && [ -n "$STAGING" ] || fail "deploy_state incomplete (ACTIVE=$ACTIVE LKG=$LKG STAGING=$STAGING)"
[ -x "./$ACTIVE" ] || fail "active slot missing: $ACTIVE"
[ -x "./$LKG" ] || fail "LKG slot missing: $LKG"

echo "[promote] Gate 1: candidate --selfest"
if ! "$CAND" --selfest > /tmp/promote_selfest.log 2>&1; then
  fail "candidate --selfest failed: $(tail -1 /tmp/promote_selfest.log)"
fi
echo "[promote] Gate 1 passed."

echo "[promote] Gate 2: protocol sanity (candidate announces same watchdog contract)"
"$CAND" --help 2>&1 | grep -q -- "--telegram" || fail "candidate --help lacks --telegram contract"

if [ "$CAND" -ef "./$STAGING" ]; then
  echo "[promote] Candidate is already the STAGING slot; skipping copy."
else
  cp -a "$CAND" "./$STAGING"
fi
chmod 755 "./$STAGING"

echo "[promote] Promoting $STAGING -> $ACTIVE"
cp -a "./$ACTIVE" ".$ACTIVE.rollback"
cp -a "./$STAGING" "./$ACTIVE"

if ! systemctl restart almaz 2>/dev/null; then
  cp -a ".$ACTIVE.rollback" "./$ACTIVE"
  fail "service restart failed; rolled back ACTIVE slot from .rollback"
fi

echo "[promote] Witness window: 90s"
sleep 90
if ! systemctl is-active --quiet almaz 2>/dev/null; then
  cp -a ".$ACTIVE.rollback" "./$ACTIVE"
  systemctl restart almaz 2>/dev/null || true
  fail "witness failed (service inactive); rolled back to previous ACTIVE"
fi

# Functional-ish witness: the daemon must have reported Online (Kimi K3 P1:
# is-active alone passes alive-but-braindead candidates).
if ! journalctl -u almaz --no-pager --since "120 seconds ago" 2>/dev/null | grep -q "Almaz Sovereign Telegram Bot Daemon Online"; then
  cp -a ".$ACTIVE.rollback" "./$ACTIVE"
  systemctl restart almaz 2>/dev/null || true
  fail "witness failed (daemon did not report Online); rolled back to previous ACTIVE"
fi

grep -q "Crash-loop detected" <(journalctl -u almaz --no-pager --since "2 minutes ago" 2>/dev/null) && {
  cp -a "./$LKG" "./$ACTIVE"
  systemctl restart almaz 2>/dev/null || true
  fail "crash-loop detected during witness; rolled back to LKG=$LKG"
}

# Rotate LKG: the pre-promote ACTIVE is the newest known-good (Kimi K3 P1:
# stale LKG ossifies the rollback target).
cp -a ".$ACTIVE.rollback" "./$LKG"
echo "[promote] LKG rotated: previous ACTIVE -> $LKG"

echo "[promote] SUCCESS: $STAGING promoted to ACTIVE ($ACTIVE). LKG untouched: $LKG"
notify "✅ *[Almaz Deploy]* Candidate promoted to ACTIVE (selfest PASS + 90s witness OK). LKG=$(basename "$LKG")"
sqlite3 almaz_memory.sqlite "INSERT INTO agent_timeline (event_type, summary) VALUES ('self_deploy', 'promoted $STAGING to ACTIVE ($ACTIVE); witness OK');" 2>/dev/null || true