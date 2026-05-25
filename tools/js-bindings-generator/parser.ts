import * as fs from "fs";

export interface ParamMetadata {
  name: string;
  type: string;
}

export interface MethodMetadata {
  name: string;
  isStatic: boolean;
  isConst: boolean;
  isVirtual: boolean;
  returnType: string;
  params: ParamMetadata[];
}

export interface ClassMetadata {
  name: string;       // Full name e.g. "ax::Sprite"
  shortName: string;  // e.g. "Sprite"
  baseClass: string | null; // e.g. "ax::Node"
  methods: MethodMetadata[];
  isRef: boolean;     // Inherits from ax::Ref directly or indirectly
}

export class ASTParser {
  private classes: Map<string, ClassMetadata> = new Map();
  private targetFiles: Set<string> = new Set();

  constructor(targetHeaders: string[]) {
    // Collect absolute/normalized names of target files to filter AST nodes
    for (const h of targetHeaders) {
      this.targetFiles.add(h);
    }
  }

  /**
   * Parse the root node or an array of root nodes of the Clang JSON AST.
   */
  public parse(rootNode: any | any[]): ClassMetadata[] {
    this.classes.clear();
    const roots = Array.isArray(rootNode) ? rootNode : [rootNode];
    for (const root of roots) {
      this.traverse(root, "", "");
    }

    // Resolve `isRef` transitively based on base classes
    let changed = true;
    while (changed) {
      changed = false;
      for (const [name, meta] of this.classes.entries()) {
        if (!meta.isRef && meta.baseClass) {
          const parentMeta = this.classes.get(meta.baseClass) || this.classes.get("ax::" + meta.baseClass);
          if (parentMeta && parentMeta.isRef) {
            meta.isRef = true;
            changed = true;
          } else if (meta.baseClass.includes("Ref")) {
            // Hardcode fallback for ax::Ref itself or common Ref classes
            meta.isRef = true;
            changed = true;
          }
        }
      }
    }

    return Array.from(this.classes.values());
  }

  /**
   * Recursively traverse AST nodes.
   */
  private traverse(node: any, currentNamespace: string, currentFile: string) {
    if (!node) return;

    let ns = currentNamespace;
    let file = currentFile;

    // Update current file if the node has one
    if (node.loc) {
      if (node.loc.file) {
        file = node.loc.file;
      } else if (node.loc.expansionLoc?.file) {
        file = node.loc.expansionLoc.file;
      } else if (node.loc.spellingLoc?.file) {
        file = node.loc.spellingLoc.file;
      }
    }

    // Track namespace
    if (node.kind === "NamespaceDecl") {
      ns = currentNamespace ? `${currentNamespace}::${node.name}` : node.name;
    }

    // Process CXXRecordDecl (Classes and Structs)
    if (node.kind === "CXXRecordDecl" && node.name && node.inner) {
      const fullClassName = ns ? `${ns}::${node.name}` : node.name;

      // Only parse if it's declared in one of our target headers
      if (this.matchFile(file)) {
        this.parseClass(node, ns, fullClassName);
      }
    }

    // Recurse into children
    if (node.inner) {
      for (const child of node.inner) {
        this.traverse(child, ns, file);
      }
    }
  }

  /**
   * Parse a CXXRecordDecl node.
   */
  private parseClass(node: any, ns: string, fullName: string) {
    // Determine base class
    let baseClass: string | null = null;
    let isRef = false;

    if (node.bases) {
      for (const base of node.bases) {
        if (base.access === "public" && base.type) {
          const baseName = base.type.qualType;
          // Strip "class " or "struct " prefix if present
          baseClass = baseName.replace(/^(class|struct)\s+/, "");
          if (baseClass.includes("Ref") || baseClass === "ax::Ref") {
            isRef = true;
          }
          break; // Support single inheritance primarily
        }
      }
    }

    const classMeta: ClassMetadata = {
      name: fullName,
      shortName: node.name,
      baseClass: baseClass,
      methods: [],
      isRef: isRef,
    };

    // Parse methods
    for (const child of node.inner || []) {
      const kind = child.kind;
      // We only bind public methods. In Clang JSON AST, public methods are either
      // in public sections, or we check if there's an implicit/explicit access specifier.
      // For simplicity, we filter out methods that are private/protected.
      if (child.access === "private" || child.access === "protected") {
        continue;
      }

      if (kind === "CXXMethodDecl" || kind === "CXXConstructorDecl") {
        const methodName = child.name;
        if (!methodName) continue;

        // Skip destructors
        if (methodName.startsWith("~")) continue;

        const isStatic = child.storageClass === "static";
        const isVirtual = !!child.virtual;
        const typeStr = child.type?.qualType || "";
        const isConst = typeStr.endsWith("const");

        // Parse return type
        let returnType = "void";
        if (kind === "CXXMethodDecl") {
          const idx = typeStr.indexOf("(");
          if (idx !== -1) {
            returnType = typeStr.substring(0, idx).trim();
          }
        } else {
          returnType = fullName; // Constructor returns the class type
        }

        // Parse parameters
        const params: ParamMetadata[] = [];
        for (const innerChild of child.inner || []) {
          if (innerChild.kind === "ParmVarDecl") {
            params.push({
              name: innerChild.name || `arg${params.length}`,
              type: innerChild.type?.qualType || "void",
            });
          }
        }

        classMeta.methods.push({
          name: methodName,
          isStatic: isStatic,
          isConst: isConst,
          isVirtual: isVirtual,
          returnType: returnType,
          params: params,
        });
      }
    }

    this.classes.set(fullName, classMeta);
  }

  /**
   * Helper to check if a node is declared in one of our target headers.
   */
  private isFromTargetFile(node: any): boolean {
    const loc = node.loc;
    if (!loc) return false;

    // Check direct file property
    if (loc.file) {
      return this.matchFile(loc.file);
    }

    // Check if it's expansion/spelling loc
    if (loc.expansionLoc?.file) {
      return this.matchFile(loc.expansionLoc.file);
    }
    if (loc.spellingLoc?.file) {
      return this.matchFile(loc.spellingLoc.file);
    }

    return false;
  }

  private matchFile(filePath: string): boolean {
    // Normalise slash to verify
    const normPath = filePath.replace(/\\/g, "/");
    for (const target of this.targetFiles) {
      if (normPath.endsWith(target)) {
        return true;
      }
    }
    return false;
  }
}
