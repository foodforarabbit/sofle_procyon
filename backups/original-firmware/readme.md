# Original Factory Firmware

This is the original firmware that shipped with the keyboard from the vendor, backed up on 2026-02-16.

## Files

* `left.uf2` -- Full flash dump from the left half
* `right.uf2` -- Full flash dump from the right half (trackpad side)

## Restoring

To restore this firmware to either half:

1. Enter bootloader mode (hold ESC + plug in USB for the left half, or use VIA Reset key / physical reset button for the right half)
2. Wait for the `RPI-RP2` drive to appear
3. Copy the corresponding `.uf2` file to the drive:

        cp left.uf2 /Volumes/RPI-RP2/    # for the left half
        cp right.uf2 /Volumes/RPI-RP2/   # for the right half

4. The keyboard half will reboot automatically once the copy completes
