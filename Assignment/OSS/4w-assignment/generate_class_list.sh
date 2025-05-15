#!/usr/bin/env bash
# generate_class_list.sh
# Generate two txt files listing class names (as comments) and full source underneath.

set -euo pipefail

dir_root="$(cd "$(dirname "${BASH_SOURCE[0]}")" && pwd)"

# Backend: extract Java classes and include contents
backend_src="$dir_root/backend/src/main/java"
backend_out="$dir_root/backend_classes.txt"
echo "// Backend Java classes and contents" > "$backend_out"
find "$backend_src" -type f -name "*.java" | sort | while IFS= read -r file; do
  class_name=$(grep -E '^\s*(public\s+)?(class|interface)\s+' -m1 "$file" | sed -E 's/.*(class|interface)\s+([A-Za-z0-9_]+).*/\2/')
  echo "" >> "$backend_out"
  echo "// $class_name" >> "$backend_out"
  cat "$file" >> "$backend_out"
done
echo "Generated $backend_out"

# Frontend: list files and include contents
frontend_src="$dir_root/frontend/src"
frontend_out="$dir_root/frontend_classes.txt"
echo "// Frontend files and contents" > "$frontend_out"
find "$frontend_src" -type f \( -name "*.vue" -o -name "*.js" -o -name "*.ts" \) | sort | while IFS= read -r file; do
  echo "" >> "$frontend_out"
  echo "// $(basename "$file")" >> "$frontend_out"
  cat "$file" >> "$frontend_out"
done
echo "Generated $frontend_out"
