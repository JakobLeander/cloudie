import { McpServer } from "@modelcontextprotocol/server";
import { createMcpHandler } from "agents/mcp/server";
import { z } from "zod";

function createServer() {
	const server = new McpServer({
		name: "cloudie",
		version: "1.0.0",
	});

	server.registerTool(
		"set_expression",
		{ inputSchema: z.object({ name: z.enum(["happy", "sleepy", "surprised"]) }) },
		async ({ name }) => ({
			content: [{ type: "text", text: `Expression set to ${name}` }],
		}),
	);

	return server;
}

const handler = createMcpHandler(createServer);

export default {
	fetch(request: Request, env: Env, ctx: ExecutionContext) {
		return handler(request, env, ctx);
	},
} satisfies ExportedHandler<Env>;
