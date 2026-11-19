#!/bin/bash
# .deploy_meta generator (plan M0.4) — per-slot sha256 + git HEAD + selfest + active seal.
# Read-only except writing /opt/almaz/.deploy_meta. Run after any promote/deploy.
set -u
CDIR="${1:-/opt/almaz}"   # optional: promote.sh passes the workspace it is operating on
OUT=$CDIR/.deploy_meta
SEC=$(date -u +%Y%m%d-%H%M%S)

sha() { sha256sum "$1" 2>/dev/null | awk '{print $1}'; }
selfest() {
  [ -x "$CDIR/$1" ] || { echo "missing"; return; }
  if ( cd "$CDIR" && "./$1" --selfest >/dev/null 2>&1 ); then echo "PASS"; else echo "FAIL"; fi
}

PREV=0
if [ -f "$OUT" ]; then
  PREV=$(sed -n 's/^DEPLOY_SEQ=//p' "$OUT" 2>/dev/null | head -1)
  PREV=${PREV:-0}
fi

# H12 fix: the LKG slot is whatever .deploy_state declares. It used to be hardcoded
# to almaz.bak, which is also the STAGING slot -> a degenerate rollback target.
LKG_SLOT=$(sed -n 's/^LKG=//p' "$CDIR/.deploy_state" 2>/dev/null | head -1 | tr -d '[:space:]')
[ -n "$LKG_SLOT" ] || LKG_SLOT=almaz.bak

TMPMETA=$CDIR/.deploy_meta.new
{
  echo "# Almaz deploy provenance (M0.4) — regenerated $SEC UTC"
  echo "GIT_HEAD=$(git -C "$CDIR" rev-parse HEAD 2>/dev/null || echo none)"
  echo "DEPLOY_SEQ=$(( PREV + 1 ))"
  for slot in almaz almaz.lkg almaz.bak almaz.B almaz.prev .almaz.rollback almaz-watchdog; do
    [ -e "$CDIR/$slot" ] || continue
    echo "SLOT_${slot//./_}=$(sha "$CDIR/$slot")"
  done
  if [ -e "$CDIR/almaz" ]; then echo "ACTIVE_SHA=$(sha "$CDIR/almaz")"; fi
  if [ -e "$CDIR/$LKG_SLOT" ]; then echo "LKG_SHA=$(sha "$CDIR/$LKG_SLOT")"; fi
  echo "LKG_SLOT=$LKG_SLOT"
  echo "SELFEST_ACTIVE=$(selfest almaz)"
  echo "SELFEST_LKG=$(selfest "$LKG_SLOT")"
} > "$TMPMETA"
chmod 0644 "$TMPMETA"
mv -f "$TMPMETA" "$OUT"
echo "wrote $OUT"
cat "$OUT"
