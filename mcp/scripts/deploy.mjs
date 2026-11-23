import { spawnSync } from "node:child_process";
import { readFile, rm, writeFile } from "node:fs/promises";
import path from "node:path";
import { fileURLToPath } from "node:url";

const projectDirectory = path.resolve(path.dirname(fileURLToPath(import.meta.url)), "..");
const accountId = process.env.CLOUDFLARE_ACCOUNT_ID;
const apiToken = process.env.CLOUDFLARE_API_TOKEN;
const namespaceTitle = "cloudie-state";
const namespaceIdPlaceholder = "00000000000000000000000000000000";
const deployConfigName = "wrangler.deploy.jsonc";

if (!accountId || !apiToken) {
	throw new Error("CLOUDFLARE_ACCOUNT_ID and CLOUDFLARE_API_TOKEN must be set to deploy.");
}

async function cloudflareRequest(url, options = {}) {
	const response = await fetch(url, {
		...options,
		headers: {
			Authorization: `Bearer ${apiToken}`,
			...(options.body ? { "Content-Type": "application/json" } : {}),
			...options.headers,
		},
	});
	const result = await response.json();

	if (!response.ok || !result.success) {
		const errors = result.errors?.map(({ message }) => message).join("; ");
		throw new Error(`Cloudflare API request failed: ${errors || response.statusText}`);
	}

	return result;
}

async function findNamespace() {
	const baseUrl = `https://api.cloudflare.com/client/v4/accounts/${encodeURIComponent(accountId)}/storage/kv/namespaces`;

	for (let page = 1; ; page += 1) {
		const url = new URL(baseUrl);
		url.search = new URLSearchParams({ page: String(page), per_page: "1000" }).toString();
		const result = await cloudflareRequest(url);
		const matches = result.result.filter(({ title }) => title === namespaceTitle);

		if (matches.length > 1) {
			throw new Error(`Multiple KV namespaces are named "${namespaceTitle}".`);
		}
		if (matches.length === 1) {
			return matches[0];
		}
		if (page >= (result.result_info?.total_pages ?? 1)) {
			return null;
		}
	}
}

async function ensureNamespace() {
	const existingNamespace = await findNamespace();
	if (existingNamespace) {
		return existingNamespace;
	}

	const url = `https://api.cloudflare.com/client/v4/accounts/${encodeURIComponent(accountId)}/storage/kv/namespaces`;
	try {
		const result = await cloudflareRequest(url, {
			method: "POST",
			body: JSON.stringify({ title: namespaceTitle }),
		});
		return result.result;
	} catch (error) {
		const namespaceCreatedConcurrently = await findNamespace();
		if (namespaceCreatedConcurrently) {
			return namespaceCreatedConcurrently;
		}
		throw error;
	}
}

const namespace = await ensureNamespace();
if (!namespace.id) {
	throw new Error(`Cloudflare did not return an ID for the "${namespaceTitle}" KV namespace.`);
}

const configPath = path.join(projectDirectory, "wrangler.jsonc");
const deployConfigPath = path.join(projectDirectory, deployConfigName);
const config = await readFile(configPath, "utf8");
if (config.split(namespaceIdPlaceholder).length !== 2) {
	throw new Error("Expected one placeholder KV namespace ID in wrangler.jsonc.");
}

await writeFile(deployConfigPath, config.replace(namespaceIdPlaceholder, namespace.id));

try {
	const command = process.platform === "win32" ? "npx.cmd" : "npx";
	const deploy = spawnSync(command, ["wrangler", "deploy", "--config", deployConfigName], {
		cwd: projectDirectory,
		env: process.env,
		shell: process.platform === "win32",
		stdio: "inherit",
	});

	if (deploy.error) {
		throw deploy.error;
	}
	if (deploy.status !== 0) {
		throw new Error(
			`Wrangler deploy failed${deploy.signal ? ` with signal ${deploy.signal}` : ""}.`,
		);
	}
} finally {
	await rm(deployConfigPath, { force: true });
}
