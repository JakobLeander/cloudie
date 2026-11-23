import { McpServer } from "@modelcontextprotocol/server";
import { createMcpHandler } from "agents/mcp/server";
import { z } from "zod";

const expressions = ["Sleep", "Waiting", "Happy", "Sad", "Think", "Attention"] as const;

const stateKey = "current-expression";

function createServer(env: Env) {
	const server = new McpServer({
		name: "cloudie",
		version: "1.0.0",
	});

	server.registerTool(
		"set_expression",
		{
			title: "Set Cloudie's expression",
			description:
				"Changes Cloudie's displayed animation state. Use Sleep when resting, Waiting when idle, Happy for positive news, Sad for negative news, Think while reasoning, and Attention when requesting focus.",
			inputSchema: z.object({
				name: z.enum(expressions),
			}),
		},
		async ({ name }) => {
			await env.CLOUDIE_STATE.put(stateKey, name);
			return {
				content: [{ type: "text", text: `Expression set to ${name}` }],
			};
		},
	);

	return server;
}

export default {
	async fetch(request: Request, env: Env, ctx: ExecutionContext) {
		const url = new URL(request.url);
		if (url.pathname === "/state") {
			if (request.method !== "GET") {
				return new Response("Method Not Allowed", {
					status: 405,
					headers: { Allow: "GET" },
				});
			}

			const state = await env.CLOUDIE_STATE.get(stateKey);
			return Response.json(
				{ state: state ?? "Waiting" },
				{ headers: { "Cache-Control": "no-store" } },
			);
		}

		return createMcpHandler(() => createServer(env))(request, env, ctx);
	},
} satisfies ExportedHandler<Env>;
