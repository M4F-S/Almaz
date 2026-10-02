#!/bin/bash
# Week-6 end-to-end evolution loop: auto-promote the latest accepted mutation.
# Wired, but ONLY active when BELYA_AUTO_PROMOTE=1 (opt-in; default OFF).
# Flow: find today's evolve/almaz mutation tag -> fresh worktree -> make ->
# promote.sh (selfest + contract gates + witness + rollback). All failures
# leave the ACTIVE slot untouched; LKG is the ultimate fallback.
set -eu

WORKSPACE=/opt/almaz
TODAY=$(date -u +%Y-%m-%d)

echo "[auto_promote] scanning for mutation tags created today..."
LATEST_TAG=$(git -C "$WORKSPACE" tag --list 'evo-v*' --sort=-creatordate | head -1 || true)
if [ -z "$LATEST_TAG" ]; then
  echo "[auto_promote] no mutation tags found; nothing to promote"
  exit 0
fi
TAG_DATE=$(git -C "$WORKSPACE" log -1 --format=%cs "$LATEST_TAG" 2>/dev/null || echo "1970-01-01")
if [ "$TAG_DATE" != "$TODAY" ]; then
  echo "[auto_promote] latest tag $LATEST_TAG dated $TAG_DATE (not today); skipping"
  exit 0
fi

echo "[auto_promote] promoting today's mutation: $LATEST_TAG"
WT=/tmp/auto_promote_wt
rm -rf "$WT"
git -C "$WORKSPACE" worktree add -q "$WT" "$LATEST_TAG"
trap 'git -C "$WORKSPACE" worktree remove -f "$WT" 2>/dev/null || rm -rf "$WT"' EXIT

( cd "$WT" && make clean >/dev/null 2>&1 && make >/dev/null 2>&1 ) || {
  echo "[auto_promote] build failed for $LATEST_TAG; not promoting"; exit 1; }

cp -a "$WT/almaz" "$WORKSPACE/almaz.B"
chmod +x "$WORKSPACE/almaz.B"
PROMOTE_SH=$(dirname "$0")/promote.sh
[ -x "$PROMOTE_SH" ] || PROMOTE_SH="$WORKSPACE/tools/promote.sh"
bash "$PROMOTE_SH" "$WORKSPACE/almaz.B"