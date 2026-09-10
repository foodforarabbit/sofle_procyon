#!/usr/bin/env bash
#
# flash-watch.sh -- wait for an RP2040 UF2 volume, copy the firmware, repeat.
#
# You double-tap reset; this does the rest. It flashes each half as it appears
# and stops after both, so there is nothing to time and no window to miss.
#
#   ./debug/flash-watch.sh            # both halves (default)
#   ./debug/flash-watch.sh 1          # just one
#
# Verifies before copying rather than after: an RP2040 bootloader volume is
# identified by INFO_UF2.TXT, not by its name, because the name is not
# guaranteed. And it refuses to run if the firmware is missing or empty --
# copying a truncated .uf2 is the one failure mode that bricks a half rather
# than just failing.
set -euo pipefail

UF2="${UF2:-$HOME/foodforarabbit/vial-qmk/foodforarabbit_sofle_procyon_wt_vial.uf2}"
HALVES="${1:-2}"
POLL=1

if [[ ! -s "$UF2" ]]; then
  echo "FATAL: firmware missing or empty: $UF2" >&2
  exit 1
fi

SIZE=$(stat -f%z "$UF2")
SHA=$(shasum -a 256 "$UF2" | cut -d' ' -f1)

echo "firmware : $UF2"
echo "size     : $SIZE bytes"
echo "sha256   : $SHA"
echo "halves   : $HALVES"
echo
echo "Double-tap the reset button on a half now. Ctrl-C to stop."
echo

# Print the RP2040 bootloader mount point, if one is present.
find_uf2_volume() {
  local v
  for v in /Volumes/*; do
    [[ -d "$v" ]] || continue
    if [[ -f "$v/INFO_UF2.TXT" ]]; then
      printf '%s\n' "$v"
      return 0
    fi
  done
  return 1
}

flashed=0
while (( flashed < HALVES )); do
  # Wait for a volume to appear.
  vol=""
  while [[ -z "$vol" ]]; do
    vol="$(find_uf2_volume || true)"
    [[ -z "$vol" ]] && sleep "$POLL"
  done

  board="$(sed -n 's/^Model: *//p' "$vol/INFO_UF2.TXT" 2>/dev/null | head -1)"
  echo "[$((flashed + 1))/$HALVES] found $vol${board:+  ($board)}"

  # Copy. The board reboots and yanks the volume mid-write by design, so a
  # non-zero exit here is expected and is NOT evidence of failure.
  cp "$UF2" "$vol/" 2>/dev/null || true
  sync 2>/dev/null || true

  echo "[$((flashed + 1))/$HALVES] copied, waiting for reboot"

  # The volume disappearing IS the success signal: the bootloader unmounts
  # itself once it has taken the image.
  waited=0
  while find_uf2_volume >/dev/null 2>&1; do
    sleep "$POLL"
    waited=$((waited + POLL))
    if (( waited > 30 )); then
      echo "[$((flashed + 1))/$HALVES] WARNING: still mounted after ${waited}s." >&2
      echo "  The image may not have been accepted. Check the volume by hand." >&2
      break
    fi
  done

  flashed=$((flashed + 1))
  echo "[$flashed/$HALVES] done"
  echo

  if (( flashed < HALVES )); then
    echo "Now the OTHER half: unplug this one, plug that one in alone,"
    echo "and double-tap its reset button."
    echo
    # Don't let one half's unmount race into being read as the next half.
    sleep 2
  fi
done

echo "Both halves flashed with $SHA"
echo
echo "Next, in order (debug/sessions/2026-09-10-01.md has the checklist):"
echo "  1. typing works on both halves"
echo "  2. trackpad feels IDENTICAL  <- the real gate"
echo "  3. Vial -> File -> Load saved layout"
echo "  4. 19 tuning keycodes in Vial's User tab"
echo "  5. ./debug/mxt.py check-version"
echo "  6. qmk console shows per-report deltas"
