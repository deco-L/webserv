#CGIスクリプトの、エラーケーステストのためのスクリプトです。

# エラーを含むプログラムです。

import os
import sys

print("Content-Type: text/html; charset=utf-8")
print("")

#abc

print("<html><head><title>CGI Script</title></head><body>")
print("<h1>CGI Failure</h1>")
print("<table border='1'>")
print("<tr><th>Variable</th><th>Value</th></tr>")
print("<tr><td>Loop</td><td>Loop</td></tr>")
print("</table>")
print("</body></html>")

abc