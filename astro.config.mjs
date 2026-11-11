import { defineConfig } from 'astro/config';
import sitemap from '@astrojs/sitemap';

// Change `site` if you move to a custom domain.
export default defineConfig({
  site: 'https://millz-a10-simpit.fly.dev',
  integrations: [sitemap()],
  trailingSlash: 'ignore',
  build: { format: 'directory' },
  markdown: {
    shikiConfig: { theme: 'github-dark-dimmed', wrap: true },
  },
});
