#!/bin/bash

# Content-Typeを指定
echo "Content-Type: text/html"
echo ""

# HTMLの開始
echo "<html><head><title>CGI Script</title></head><body>"
echo "<h1>Environment Variables</h1>"
echo "<table border='1'>"
echo "<tr><th>Variable</th><th>Value</th></tr>"

# 環境変数を表示
for env in $(printenv | cut -d= -f1)
do
    value=$(printenv "$env")
    echo "<tr><td>$env</td><td>$value</td></tr>"
done

# HTMLの終了
echo "</table>"
echo "</body></html>"
