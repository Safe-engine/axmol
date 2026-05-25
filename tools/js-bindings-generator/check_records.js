const fs = require('fs');

const content = fs.readFileSync('temp_ast.json', 'utf8').trim();
const wrappedContent = "[" + content.replace(/\n\}\n\{/g, "\n},\n{") + "]";
const roots = JSON.parse(wrappedContent);

console.log("Number of root nodes:", roots.length);

function findCXXRecord(node, ns = "") {
  if (!node) return;
  
  let currentNs = ns;
  if (node.kind === 'NamespaceDecl') {
    currentNs = ns ? `${ns}::${node.name}` : node.name;
  }
  
  if (node.kind === 'CXXRecordDecl' && node.name) {
    const fullName = currentNs ? `${currentNs}::${node.name}` : node.name;
    if (fullName.includes('ax::')) {
      console.log(`Found CXXRecordDecl: ${fullName}`);
      console.log(`  loc:`, JSON.stringify(node.loc));
    }
  }
  
  if (node.inner) {
    for (const child of node.inner) {
      findCXXRecord(child, currentNs);
    }
  }
}

// Check first 10 roots
for (let i = 0; i < roots.length; i++) {
  findCXXRecord(roots[i]);
}
