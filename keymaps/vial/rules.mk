VIA_ENABLE = yes
VIAL_ENABLE = yes
VIAL_INSECURE = no
ENCODER_MAP_ENABLE = yes

# Console logging for S1/S2 characterisation (flash 1, item 4).
# Costs nothing at runtime when no `qmk console` is attached; turns "the pad
# feels jittery" into logged per-report numbers. Remove after flash 2 if the
# flash space is ever wanted back.
CONSOLE_ENABLE = yes

# NOT MAXTOUCH_DEBUG. The debug protocol is reimplemented in keymap.c under
# `#if defined(VIA_ENABLE)`; setting MAXTOUCH_DEBUG = yes re-triggers
# `multiple definition of 'raw_hid_receive'` between via.o and maxtouch.o.
