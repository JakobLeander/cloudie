import { DurableObject } from "cloudflare:workers";
import { McpServer } from "@modelcontextprotocol/server";
import { createMcpHandler } from "agents/mcp/server";
import { z } from "zod";

const expressions = ["Sleep", "Waiting", "Happy", "Sad", "Think", "Attention"] as const;

const stateKey = "current-expression";
const storedStateSchema = z.object({
	state: z.enum(expressions),
	updatedAt: z.string().datetime(),
});

export class CloudieState extends DurableObject<Env> {
	async readState() {
		return (await this.ctx.storage.get<string>(stateKey)) ?? null;
	}

	async writeState(value: string) {
		await this.ctx.storage.put(stateKey, value);
	}
}

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
			const state = env.CLOUDIE_STATE.getByName("global");
			await state.writeState(
				JSON.stringify({ state: name, updatedAt: new Date().toISOString() }),
			);
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

			const stored = await env.CLOUDIE_STATE.getByName("global").readState();
			let state: (typeof expressions)[number] = "Sleep";
			let updatedAt: string | null = null;
			if (stored) {
				const legacyState = z.enum(expressions).safeParse(stored);
				if (legacyState.success) {
					state = legacyState.data;
				} else {
					const parsed = storedStateSchema.parse(JSON.parse(stored));
					state = parsed.state;
					updatedAt = parsed.updatedAt;
				}
			}

			return Response.json(
				{ state, updatedAt },
				{ headers: { "Cache-Control": "no-store" } },
			);
		}

		return createMcpHandler(() => createServer(env))(request, env, ctx);
	},
} satisfies ExportedHandler<Env>;
