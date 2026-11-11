# Millz A-10 SimPit: build log site

Static site (Astro 5), served by nginx on fly.io.

## Deploy

**Source of truth: https://github.com/vectorstofinal/millz-a10-simpit.** Every push to `main` deploys to fly.io through GitHub Actions (`.github/workflows/deploy.yml`, repo secret `FLY_API_TOKEN`). You can also re-run a deploy from the Actions tab (Run workflow).

The Dropbox `Blog site` folder is a working copy and can lag behind GitHub. Edit through GitHub, or `git pull` first.

Manual deploy, if you ever need it, from a clone on a PC with flyctl installed:

```powershell
fly deploy --ha=false
```

App: `millz-a10-simpit`, region `dfw` (fly no longer allows new machines in den), one shared-cpu-1x / 256 MB machine that stops when idle.

## Add a post

1. Create `src/content/posts/<slug>.md`:

   ```md
   ---
   title: "Caution Light Panel"
   date: 2026-11-20
   summary: "One or two sentences for the home page and RSS."
   tags: [panels, acrylic]
   files: [some-file-id]      # optional, ids from src/data/files.ts
   ---

   <div class="bottom-line">The answer first.</div>

   ## Then the details
   ```

2. Put downloads in `public/files/<group>/` and images in `public/img/`, then add each download to `src/data/files.ts`.
3. Embed an STL viewer anywhere in a post:
   `<stl-viewer src="/files/dimmer-knob/dimmer-knob-26mm.stl" label="Dimmer knob"></stl-viewer>`
4. Commit and push to `main`. GitHub Actions deploys it.

Update the status tiles at the top of `src/pages/index.astro` as the build moves.
Settings live in `src/pages/settings.md`.

## Local preview (optional)

```powershell
npm install
npm run dev     # http://localhost:4321
```

## Publishing rules

- My original work: CC0.
- Warthog Project files and edits of them: OK to post, with credit (the author released them to the public).
- **Never** post anything from the Vr-pits (FLIM) plan set, or anything derived from it: DXFs, FCStd, assembly guides, part lists, keyed STLs, dimensions.
- Third-party files (GrabCAD, DCS textures, commercial knobs, fonts): link to the source, don't host a copy.
