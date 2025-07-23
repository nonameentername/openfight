find . -name "*.xml" | while read file; do
  output="${file%.xml}.yml"
  python3 -c '
import sys, xmltodict, yaml

def auto_type(value):
    if not isinstance(value, str):
        return value
    val = value.strip()
    if val.lower() == "true":
        return True
    if val.lower() == "false":
        return False
    try:
        if "." in val:
            return float(val)
        return int(val)
    except:
        return val

def postprocessor(path, key, value):
    return key, auto_type(value)

def clean_keys(obj):
    if isinstance(obj, dict):
        return {k.lstrip("@"): clean_keys(v) for k, v in obj.items()}
    elif isinstance(obj, list):
        return [clean_keys(i) for i in obj]
    else:
        return obj

with open(sys.argv[1]) as f:
    xml = xmltodict.parse(f.read(), postprocessor=postprocessor)
    clean = clean_keys(xml)
    yaml.safe_dump(clean, sys.stdout, sort_keys=False)
' "$file" > "$output"
  echo "Converted $file -> $output"
done
