import { defineCollection, z } from 'astro:content';
import { glob } from 'astro/loaders';

const posts = defineCollection({
  loader: glob({ pattern: '**/*.md', base: './src/content/posts' }),
  schema: z.object({
    title: z.string(),
    date: z.coerce.date(),
    summary: z.string(),
    tags: z.array(z.string()).default([]),
    files: z.array(z.string()).default([]),   // ids from src/data/files.ts
    hero: z.string().optional(),               // /img/... path
    heroAlt: z.string().optional(),
    draft: z.boolean().default(false),
  }),
});

export const collections = { posts };
