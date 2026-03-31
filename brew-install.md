# Install with Homebrew

`apfel` is available through the `Arthur-Ficial/tap` tap:

```bash
brew tap Arthur-Ficial/tap
brew install Arthur-Ficial/tap/apfel
```

Verify the install:

```bash
apfel --version
apfel --release
```

## Requirements

- Apple Silicon
- macOS 26.4 or newer
- Apple Intelligence enabled

Homebrew installs the `apfel` binary. You do **not** need Xcode.

## Troubleshooting

If the binary runs but generation is unavailable, check:

```bash
apfel --model-info
```

If you already installed `apfel` manually into `/usr/local/bin/apfel`, make sure the Homebrew binary is first in your `PATH`:

```bash
which apfel
brew --prefix
```

## Maintainer Release Flow

Releases are automated from this repo. Do not hand-edit `Arthur-Ficial/homebrew-tap` for normal releases.

One-time setup in `Arthur-Ficial/apfel`:

1. Create a fine-grained GitHub token with `Contents: Read and write` access to `Arthur-Ficial/homebrew-tap`
2. Store it as the `HOMEBREW_TAP_PUSH_TOKEN` GitHub Actions secret

Release steps:

1. Open the `Publish Release` GitHub Actions workflow
2. Choose `patch`, `minor`, or `major`
3. Run the workflow

The workflow will:

1. Bump `.version`
2. Regenerate `Sources/BuildInfo.swift`
3. Update the version badge in `README.md`
4. Do that through the existing `Makefile` release targets
5. Build the release binary on `macos-26`
6. Commit the release files and push the Git tag
7. Publish `apfel-<version>-arm64-macos.tar.gz` on GitHub Releases
8. Rewrite and push `Formula/apfel.rb` in `Arthur-Ficial/homebrew-tap`

Validation after the workflow completes:

```bash
brew update
brew tap Arthur-Ficial/tap
brew reinstall Arthur-Ficial/tap/apfel
brew test Arthur-Ficial/tap/apfel
brew audit --strict Arthur-Ficial/tap/apfel
```
