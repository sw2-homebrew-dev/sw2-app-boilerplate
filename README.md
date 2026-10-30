# sw2-app-boilerplate

A starting point for Switch 2 homebrew built with [sw2-sdk](https://github.com/sw2-homebrew-dev/sw2-sdk).
On GitHub, use **Use this template**, rename the app, and push a tag. You get
a release, plus a record that's ready to submit to
[sw2-homebrew-catalog](https://github.com/sw2-homebrew-dev/sw2-homebrew-catalog).

There are two starters. Keep the one you want and delete the other folder.

| Folder | Kind | You write |
| --- | --- | --- |
| `native/` | HBLauncher ABI 3 native app | C (`src/app.c`), with 2D UI, raw Vulkan, input, assets, save block |
| `gpubrew/` | GPUBrew ABI 1 shader app | GLSL (`shaders/app.comp` logic, `app.frag` drawing) |

## First steps

1. Pick your own id (the starters ship as `hello-world` and `hello-shader`):
   lowercase, matching `[a-z0-9][a-z0-9._-]*`. Put it in
   `native/manifest.json` (or `gpubrew/app.json`) along with the name and
   description.
2. Set up the toolchain and run `sw2 doctor` from your sw2-sdk checkout.
   This works on Linux, using apt GNU or LLVM, and on macOS with
   `brew install llvm lld glslang spirv-tools`.
3. Build:

   ```sh
   ./build.sh        # uses $SW2_SDK, ../sw2-sdk, or an installed release; writes dist/
   ```

4. Try it: copy `native/` into `sw2-sdk/apps/<id>/`, then run `sw2 run --mock`
   (offline) or `sw2 run` (console).

## Releasing

1. Bump `version` in the manifest. For example `1.1.0` → tag `v1.1.0`.
2. `git tag v1.1.0 && git push --tags`.
3. The **release** workflow builds, packages, and publishes the zip, its
   `.sha256` and `<id>.catalog.json`.
4. Copy `<id>.catalog.json` to `apps/<id>.json` in sw2-homebrew-catalog and
   open a pull request.

Repository variables (Settings → Variables) can change these values:

| Variable | Default |
| --- | --- |
| `SW2_SDK_VERSION` | latest sw2-sdk release (for example `v0.1.0` to pin) |
| `CATALOG_CATEGORY` | `app` |
| `LICENSE_ID` | `MIT` |

## What the native starter shows

- Frame loop with `hbx_pace` at 60 fps, and exit on B.
- Merged controller input, the stick, and touch at 1280x720 scaled to 1920x1080.
- 2D UI: text, circles, layout at 1920x1080.
- A packaged asset (`assets/greeting.txt`, listed in `manifest.assets`).
- The persistent save block (`save_bytes`): a launch counter and a best score.

See the [SDK guide](https://github.com/sw2-homebrew-dev/sw2-sdk/blob/main/docs/SDK.md) and the
[sw2 additions](https://github.com/sw2-homebrew-dev/sw2-sdk/blob/main/docs/SW2.md) for raw Vulkan, audio, SDK imports,
and the services mailbox.

## License

MIT. Replace it with your app's license.
