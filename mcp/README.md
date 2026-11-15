# Cloudie MCP Server

Cloudie is a remote MCP server that exposes a tool for setting Cloudie's expression. It runs on Cloudflare Workers and serves the MCP endpoint at `/mcp`.

## Tool

`set_expression` accepts one `name` value:

- `happy`
- `sleepy`
- `surprised`

The tool replies with `Expression set to <name>`.

## Run locally

From this directory:

```bash
npm install
npm run dev
```

The local MCP endpoint is `http://localhost:8787/mcp`.

## Deploy to Cloudflare

Authenticate Wrangler with your Cloudflare account if you have not already:

```bash
npx wrangler login
```

Then deploy:

```bash
npm run deploy
```

The Worker name is configured in [`wrangler.jsonc`](./wrangler.jsonc) as `cloudie-mcp`. Cloudflare serves it at `https://cloudie-mcp.<your-account>.workers.dev/mcp` when the `workers.dev` subdomain is enabled for your account. Change the `name` in `wrangler.jsonc` to use a different Worker name. The MCP server's client-facing name (`cloudie`) is configured separately in [`src/index.ts`](./src/index.ts).

This server does not implement authentication, so anyone who can reach the deployed endpoint can call its tool.

## Connect an MCP client

For a client that supports `mcp-remote`, use this configuration, replacing the URL with your local or deployed endpoint:

```json
{
  "mcpServers": {
    "cloudie": {
      "command": "npx",
      "args": [
        "mcp-remote",
        "https://cloudie-mcp.<your-account>.workers.dev/mcp"
      ]
    }
  }
}
```

To connect locally, use `http://localhost:8787/mcp` as the URL instead.
