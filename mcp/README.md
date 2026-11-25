# Cloudie MCP Server

Cloudie is a remote MCP server that exposes a tool for setting Cloudie's expression. It runs on Cloudflare Workers and serves the MCP endpoint at `/mcp`.

## Device state endpoint

The device can query the current expression with:

```http
GET /state
```

The endpoint returns JSON, for example:

```json
{"state":"Waiting"}
```

The available values are `Sleep`, `Waiting`, `Happy`, `Sad`, `Think`, and
`Attention`. State is stored in a SQLite-backed Cloudflare Durable Object and
defaults to `Sleep` until it is first set. Durable Object storage is strongly
consistent, so updates are immediately visible to subsequent requests.
Previously stored KV values are not copied automatically when deploying this
change.

## Tool

`set_expression` accepts one `name` value:

- `Sleep`
- `Waiting`
- `Happy`
- `Sad`
- `Think`
- `Attention`

The tool replies with `Expression set to <name>`.

## Run locally

From this directory:

```bash
npm install
npm run dev
```

The local MCP endpoint is `http://localhost:8787/mcp`.
Wrangler uses local Durable Object storage during development, separate from
the deployed namespace.

## Run locally with MCP Inspector

Start the MCP server:

```bash
npm install
npm run dev
```

In a second terminal, start the MCP Inspector:

```bash
npx @modelcontextprotocol/inspector
```

Open the Inspector URL shown in the terminal. Configure the connection with:

- **Transport:** Streamable HTTP
- **URL:** `http://localhost:8787/mcp`

Then click **Connect** and use the **set_expression** tool to test its available states.

## Deploy to Cloudflare

For a manual deployment, set `CLOUDFLARE_ACCOUNT_ID` and
`CLOUDFLARE_API_TOKEN` in your environment, then run:

```bash
npm run deploy
```

Deployment provisions the Durable Object namespace from `wrangler.jsonc`.
GitHub Actions also deploys the Worker when changes are pushed to
`mcp/` on `main`; you can start a deployment manually from the **Deploy MCP
Worker** workflow. The workflow uses the repository secrets
`CLOUDFLARE_ACCOUNT_ID` and `CLOUDFLARE_API_TOKEN`. The API token needs
permission to deploy Workers and manage Durable Objects.

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
