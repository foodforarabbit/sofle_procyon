# Upstream PR — bounds check in the maxtouch debug protocol

**Ryan opens this, not an agent.** It goes to `george-norton/qmk_firmware`,
someone else's repo, under his GitHub identity.

## Why bother

`MAXTOUCH_DEBUG_READ` / `_WRITE` validate the caller's requested length only
against the protocol maximum, never against the actual report buffer:

```c
if (read_length > 0x1c) {          // upstream, maxtouch.c:668 and :680
```

A host sending a short raw-HID report with a large `length` byte makes the
driver read or write past the end of `data`. Two lines fix it. Confirmed still
present on `multitouch_experiment` as of 2026-09-08 (MEASURED).

Worth sending rather than carrying: if he takes it, we inherit it on any future
merge instead of maintaining a fork delta — which is the whole point of keeping
vial-qmk close to upstream.

## The patch

`drivers/sensors/maxtouch.c`, inside `#ifdef MAXTOUCH_DEBUG`:

```diff
         case MAXTOUCH_DEBUG_READ: {
             const uint16_t read_address = (data[1] << 8) | data[2];
             const uint16_t read_length  = data[3];
-            if (read_length > 0x1c) {
+            if (read_length > 0x1c || read_length + 4 > length) {
                 status = MAXTOUCH_DEBUG_INVALID_LENGTH;
```

```diff
         case MAXTOUCH_DEBUG_WRITE: {
             const uint16_t write_address = (data[1] << 8) | data[2];
             const uint16_t write_length  = data[3];
-            if (write_length > 0x1c) {
+            if (write_length > 0x1c || write_length + 4 > length) {
                 status = MAXTOUCH_DEBUG_INVALID_LENGTH;
```

## Why you need a fork first

GitHub only allows PRs within one fork network, and these are two:

| repo | fork source |
|---|---|
| `foodforarabbit/vial-qmk` | `vial-kb/vial-qmk` |
| `george-norton/qmk_firmware` | `qmk/qmk_firmware` |

So a branch on our vial-qmk **cannot** be proposed to his repo. You need a fork
of *his* repo, which lands in the qmk network.

## Steps

```sh
# 1. Fork his repo (puts it in the qmk fork network)
gh repo fork george-norton/qmk_firmware --clone=false --remote=false

# 2. Branch from his feature branch, not master (master is a dead 2024 snapshot)
cd /tmp && git clone --depth 1 --branch multitouch_experiment \
  https://github.com/george-norton/qmk_firmware.git gn-qmk
cd gn-qmk
git checkout -b maxtouch-debug-bounds-check

# 3. Apply the two-line change above to drivers/sensors/maxtouch.c

# 4. Push to YOUR fork
git remote add mine git@github.com:foodforarabbit/qmk_firmware.git
git push -u mine maxtouch-debug-bounds-check
```

Then open the PR — this URL prefills it once the branch exists:

<https://github.com/george-norton/qmk_firmware/compare/multitouch_experiment...foodforarabbit:maxtouch-debug-bounds-check?expand=1>

**Base must be `multitouch_experiment`.** His `master` last moved 2024-01-23 and
the debug protocol does not exist there.

## Suggested PR text

> **Title:** maxtouch: bounds-check debug read/write against the report buffer
>
> `MAXTOUCH_DEBUG_READ` and `MAXTOUCH_DEBUG_WRITE` validate the requested
> length against the protocol maximum (`0x1c`) but not against the actual
> `length` of the received report, so a host sending a short report with a
> large length byte causes an out-of-bounds read or write on `data`.
>
> Adds `+ 4 > length` to both checks — 4 being the header the payload starts
> after. Behaviour is unchanged for well-formed requests.
>
> Found while building a host-side tuning script against this protocol for a
> Procyon trackpad; the short-report case is easy to hit by accident when the
> host packs its own reports.
