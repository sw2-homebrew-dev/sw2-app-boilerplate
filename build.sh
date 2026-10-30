#!/bin/sh
# Build and package every app folder in this repository (native/, gpubrew/).
#   SW2_SDK   sw2-sdk checkout or release (default: ../sw2-sdk, .sw2-sdk, ~/.local/share/sw2-sdk)
#   URL_BASE  release download prefix written into the catalog record
set -e
cd "$(dirname "$0")"
SDK=${SW2_SDK:-}
for candidate in "$SDK" ../sw2-sdk .sw2-sdk "$HOME/.local/share/sw2-sdk"; do
  [ -n "$candidate" ] && [ -x "$candidate/sw2" ] && SDK=$candidate && break
done
[ -x "$SDK/sw2" ] || { echo "build.sh: set SW2_SDK to an sw2-sdk checkout" >&2; exit 2; }
SDK=$(cd "$SDK" && pwd)

for app in native gpubrew; do
  [ -d "$app" ] || continue
  "$SDK/sw2" build "$app"
  "$SDK/sw2" package "$app" --out dist ${URL_BASE:+--url-base "$URL_BASE"} \
    ${AUTHOR:+--author "$AUTHOR"} ${CATEGORY:+--category "$CATEGORY"} \
    ${LICENSE_ID:+--license "$LICENSE_ID"} ${SOURCE_REPO:+--source-repo "$SOURCE_REPO"}
done
