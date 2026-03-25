# Static Code Analyzer

## How to Run

1. **Compile**:
```powershell
New-Item -ItemType Directory -Force -Path "build"
$src = Get-ChildItem -Path "src" -Recurse -Filter "*.cpp" | ForEach-Object { "`"$($_.FullName)`"" }
$srcList = $src -join " "
Invoke-Expression "g++ -I src/parser -I src/ast -I src/semantic -I src/cfg -I src/dataflow -I src/viz -I src/utils $srcList -o build/static_analyzer.exe"
```

2. **Run Analyzer**:
```powershell
.\build\static_analyzer.exe
```

3. **Generate Graph (CFG)**:
```powershell
& "C:\Program Files\Graphviz\bin\dot.exe" -Tpng cfg.dot -o cfg_visual.png
```
