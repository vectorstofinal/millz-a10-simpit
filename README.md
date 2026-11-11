# Millz A-10 SimPit: build log site

Static site (Astro 5), served by nginx on fly.io.

## Deploy to fly.io (first time)

From this folder, on a computer with [flyctl](https://fly.io/docs/flyctl/install/) installed:

```powershell
fly auth login
fly apps create millz-a10-simpit   # if the name is taken, pick another and change `app` in fly.toml
fly deploy
```

fly builds the Docker image remotely, so you don't need Node or Docker locally.
The site will be at `https://<app-name>.fly.dev`. If the app name changes, update `site` in `astro.config.mjs`.

After the first deploy, `fly deploy` is all you need.

Cost: one shared-cpu-1x / 256 MB machine that stops when idle (`min_machines_running = 0`).
The first visitor after an idle period waits about a second while it starts.
Set `min_machines_running = 1` to keep it warm.

### Custom domain (optional)

```powershell
fly certs add simpit.example.com
```

Then add the DNS records it prints and change `site` in `astro.config.mjs`.

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
4. `fly deploy`.

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
