find . -name "*.json" | while read file; do
  output="${file%.json}.yml"
  python3 -c 'import sys, yaml, json; yaml.safe_dump(json.load(sys.stdin), sys.stdout, sort_keys=False)' < "$file" > "$output"
  echo "Converted $file -> $output"
done