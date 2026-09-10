#!/usr/bin/env bash
#
# Build the sofle_procyon vial firmware from this worktree.
#
# Exists because there are four independent ways to get this wrong, and three
# of them fail with an error that does not name the real cause:
#
#   1. arm-none-eabi-gcc@8 is brew KEG-ONLY, so it is not on PATH. You get
#      "arm-none-eabi-gcc: command not found" and Error 127, which reads like
#      a missing toolchain rather than a missing PATH entry.
#   2. vial-qmk must be on `procyon-tap-scroll-fix` (was `vial-procyon` until
#      2026-09-10, when the fork took its first commits with Ryan's approval).
#      On `debug-vial-tunnel` the driver also defines raw_hid_receive and the
#      link fails with "multiple definition"; on the plain `vial` branch the
#      MaxTouch driver is absent; on `vial-procyon` the digitizer gesture fixes
#      are missing and two-finger scroll/tap misbehave.
#   5. Vial randomises BUILD_ID per build unless VIAL_BUILD_ID is set, and
#      via_eeprom_is_valid() compares the stored layout magic against it -- so
#      an unpinned build WIPES the user's keymap on every flash. Pinned below.
#   3. QMK builds from keyboards/<vendor>/<board>, so the worktree has to be
#      symlinked in. Without it, make says the keyboard does not exist.
#   4. The base checkout and this worktree are different trees, and both are
#      symlinked in. Building the wrong one silently produces firmware from
#      the wrong source.
#
# Usage:  ./debug/build.sh            # build
#         ./debug/build.sh --clean    # rm -rf .build first
set -euo pipefail

WORKTREE="$(cd "$(dirname "${BASH_SOURCE[0]}")/.." && pwd)"

# --- 5. PINNED BUILD_ID -------------------------------------------------
# via_eeprom_is_valid() (vial-qmk quantum/via.c:96-106) compares the stored
# VIA/Vial EEPROM magic against BUILD_ID, and util/build_id.py randomises it on
# every build. That wipes the user's layout on EVERY flash -- observed four
# times in a row, including flashes that changed neither vial.json nor the
# eeconfig layout.
#
# Pinning makes the check mean what it should: the layout is invalidated when
# the LAYOUT changes, not when the clock ticks.
#
# BUMP THIS DELIBERATELY, and only when the Vial layout genuinely changes --
# vial.json customKeycodes, the matrix, or EECONFIG_USER_DATA_SIZE. Bumping it
# costs the user their key assignments, so it is not a routine act.
export VIAL_BUILD_ID="${VIAL_BUILD_ID:-0x50F1E1}"
QMK="${QMK_DIR:-$HOME/foodforarabbit/vial-qmk}"
KB_NAME="sofle_procyon_wt"
REQUIRED_BRANCH="procyon-tap-scroll-fix"
TARGET="foodforarabbit/${KB_NAME}:vial"

say() { printf '\033[36m==>\033[0m %s\n' "$*"; }
die() { printf '\033[31mERROR:\033[0m %s\n' "$*" >&2; exit 1; }

[ -d "$QMK" ] || die "vial-qmk not found at $QMK (override with QMK_DIR=...)"

# --- 2. branch assertion ------------------------------------------------
branch="$(git -C "$QMK" rev-parse --abbrev-ref HEAD)"
if [ "$branch" != "$REQUIRED_BRANCH" ]; then
  die "vial-qmk is on '$branch', needs '$REQUIRED_BRANCH'.
  This is not cosmetic:
    debug-vial-tunnel -> link fails, 'multiple definition of raw_hid_receive'
    vial              -> MaxTouch driver absent
  Fix:  git -C $QMK checkout $REQUIRED_BRANCH"
fi
say "vial-qmk on $branch"

# Warn (do not fail) if the fork has drifted from a clean upstream state.
# `|| true` is load-bearing: under `set -o pipefail` a grep that matches
# nothing exits 1 and would abort the script on a CLEAN fork, which is the
# normal case. This bit me on the first run.
dirty="$(git -C "$QMK" status --porcelain | { grep -v '^?? keyboards/foodforarabbit/' || true; } | wc -l | tr -d ' ')"
[ "$dirty" = "0" ] || printf '\033[33mWARN:\033[0m vial-qmk has %s modified tracked file(s) — the fork is meant to stay clean\n' "$dirty"

# --- 3/4. symlink the RIGHT tree ---------------------------------------
link="$QMK/keyboards/foodforarabbit/$KB_NAME"
mkdir -p "$QMK/keyboards/foodforarabbit"
if [ -L "$link" ]; then
  cur="$(readlink "$link")"
  if [ "$cur" != "$WORKTREE" ]; then
    say "repointing $KB_NAME: $cur -> $WORKTREE"
    rm -f "$link"; ln -s "$WORKTREE" "$link"
  fi
elif [ -e "$link" ]; then
  die "$link exists and is not a symlink — refusing to replace it"
else
  say "linking $KB_NAME -> $WORKTREE"
  ln -s "$WORKTREE" "$link"
fi
say "building from $(readlink "$link")"

# --- 1. keg-only toolchain ---------------------------------------------
if ! command -v arm-none-eabi-gcc >/dev/null 2>&1; then
  for f in arm-none-eabi-gcc@8 arm-none-eabi-gcc arm-none-eabi-binutils; do
    p="$(brew --prefix "$f" 2>/dev/null || true)"
    [ -n "$p" ] && [ -d "$p/bin" ] && PATH="$p/bin:$PATH"
  done
  export PATH
fi
command -v arm-none-eabi-gcc >/dev/null 2>&1 \
  || die "arm-none-eabi-gcc still not on PATH. Install: brew install arm-none-eabi-gcc@8 arm-none-eabi-binutils"
say "toolchain $(arm-none-eabi-gcc --version | head -1)"

# --- build --------------------------------------------------------------
cd "$QMK"
[ "${1:-}" = "--clean" ] && { say "removing .build"; rm -rf .build; }

say "VIAL_BUILD_ID=$VIAL_BUILD_ID (pinned — layout survives rebuilds)"
say "make $TARGET"
make "$TARGET"

uf2="$QMK/foodforarabbit_${KB_NAME}_vial.uf2"
[ -f "$uf2" ] || die "build reported success but no .uf2 at $uf2"
say "OK  $uf2  ($(wc -c < "$uf2" | tr -d ' ') bytes)"
echo
echo "Record this in your session file (debug/TEMPLATE.md):"
echo "  git sha:  $(git -C "$WORKTREE" rev-parse --short HEAD)"
echo "  uf2:      $(basename "$uf2")"
echo "  built:    $(date -u +%Y-%m-%dT%H:%M:%SZ)"
echo "  MAXTOUCH_DEBUG: $(grep -E '^MAXTOUCH_DEBUG' "$WORKTREE/rules.mk" | awk '{print $3}')"
echo "  BUILD_ID:       $VIAL_BUILD_ID"
echo "  vial-qmk:       $(git -C "$QMK" rev-parse --short HEAD) on $branch"
