import { spawnSync } from "bun";
import * as fs from "fs";
import * as path from "path";
import { parse as parseYaml } from "yaml";
import { ASTParser } from "./parser";
import { BindingWriter } from "./writer";

async function main() {
  const axmolRoot = path.resolve("../../");
  console.log(`Axmol Root: ${axmolRoot}`);

  // 1. Read config.yaml
  const configPath = path.resolve("config.yaml");
  if (!fs.existsSync(configPath)) {
    console.error("config.yaml not found!");
    process.exit(1);
  }
  const config = parseYaml(fs.readFileSync(configPath, "utf-8"));

  // 2. Resolve include directories
  const includes: string[] = (config.includes || []).map((inc: string) => {
    return path.resolve(axmolRoot, inc);
  });

  // Get macOS SDK path if on mac
  let sdkPath = "";
  if (process.platform === "darwin") {
    const sdkResult = spawnSync(["xcrun", "--show-sdk-path"]);
    sdkPath = sdkResult.stdout.toString().trim();
    console.log(`macOS SDK Path: ${sdkPath}`);
  }

  // 3. Process each module defined in config
  for (const [moduleName, moduleConfig] of Object.entries(config.modules)) {
    const mod = moduleConfig as any;
    console.log(`\nProcessing module: ${moduleName}`);

    const headers: string[] = mod.headers || [];
    if (headers.length === 0) {
      console.log("No headers to process.");
      continue;
    }

    // Create temporary file that includes all target headers
    const tempFile = path.resolve(`./temp_${moduleName}.cpp`);
    let tempContent = "";
    for (const h of headers) {
      tempContent += `#include "${h}"\n`;
    }
    fs.writeFileSync(tempFile, tempContent);
    console.log(`Created temporary compilation file: ${tempFile}`);

    // Build clang arguments
    const clangArgs = [
      "-Xclang", "-ast-dump-filter=ax",
      "-Xclang", "-ast-dump=json",
      "-fsyntax-only",
      "-std=c++20",
    ];

    if (sdkPath) {
      clangArgs.push("-isysroot", sdkPath);
    }

    for (const inc of includes) {
      clangArgs.push("-I", inc);
    }

    // Target file
    clangArgs.push("-x", "c++", tempFile);

    console.log(`Running clang to dump AST JSON for ${moduleName}...`);
    const clangProc = spawnSync(["clang++", ...clangArgs], {
      maxBuffer: 1000 * 1024 * 1024, // 1000 MB buffer for large JSON output
    });

    // Cleanup temp C++ file
    try {
      fs.unlinkSync(tempFile);
    } catch {}

    console.log(`Clang finished. ExitCode: ${clangProc.exitCode}, Success: ${clangProc.success}`);
    console.log(`Stdout length: ${clangProc.stdout ? clangProc.stdout.length : 0} bytes`);
    console.log(`Stderr length: ${clangProc.stderr ? clangProc.stderr.length : 0} bytes`);

    if (clangProc.exitCode !== 0) {
      console.error(`Clang failed with exit code ${clangProc.exitCode}`);
      if (clangProc.stderr) {
        console.error(clangProc.stderr.toString().substring(0, 1000));
      }
      process.exit(1);
    }

    console.log("AST JSON dumped successfully. Saving to temp_ast.json...");
    fs.writeFileSync("./temp_ast.json", clangProc.stdout);
    
    console.log("Parsing temp_ast.json...");
    let astRoot: any;
    try {
      const content = fs.readFileSync("./temp_ast.json", "utf-8").trim();
      const wrappedContent = "[" + content.replace(/\n\}\n\{/g, "\n},\n{") + "]";
      astRoot = JSON.parse(wrappedContent);
    } catch (e: any) {
      console.error("JSON Parse failed!");
      console.error("Error message:", e.message);
      process.exit(1);
    }

    // Parse the AST JSON
    const parser = new ASTParser(headers);
    const classes = parser.parse(astRoot);

    console.log(`Parsed ${classes.length} classes.`);

    // Write bindings
    const writer = new BindingWriter(moduleName, classes, {
      nsMap: config.ns_map || {},
      ignoreMethods: mod.ignore_methods || [],
      classesConfig: mod.classes || [],
    });

    const outCppPath = path.resolve(mod.output_cpp);
    const outHppPath = path.resolve(mod.output_hpp);

    // Create output directories if needed
    fs.mkdirSync(path.dirname(outCppPath), { recursive: true });

    // Generate and write
    const hppContent = writer.generateHpp();
    const cppContent = writer.generateCpp(headers);

    fs.writeFileSync(outHppPath, hppContent);
    fs.writeFileSync(outCppPath, cppContent);

    console.log(`Generated headers: ${outHppPath}`);
    console.log(`Generated sources: ${outCppPath}`);
  }

  console.log("\nSuccess!");
}

main().catch((err) => {
  console.error(err);
  process.exit(1);
});
