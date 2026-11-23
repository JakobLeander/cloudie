import { McpServer } from "@modelcontextprotocol/server";
import { createMcpHandler } from "agents/mcp/server";
import { z } from "zod";

const expressions = ["Sleep", "Waiting", "Happy", "Sad", "Think", "Attention"] as const;
type Expression = (typeof expressions)[number];

let currentExpression: Expression = "Waiting";

function createServer() {
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
			currentExpression = name;
			return {
				content: [{ type: "text", text: `Expression set to ${name}` }],
			};
		},
	);

	return server;
}

const handler = createMcpHandler(createServer);

export default {
	fetch(request: Request, env: Env, ctx: ExecutionContext) {
		const url = new URL(request.url);
		if (url.pathname === "/state") {
			if (request.method !== "GET") {
				return new Response("Method Not Allowed", {
					status: 405,
					headers: { Allow: "GET" },
				});
			}

			return Response.json(
				{ state: currentExpression },
				{ headers: { "Cache-Control": "no-store" } },
			);
		}

		return handler(request, env, ctx);
	},
} satisfies ExportedHandler<Env>;
