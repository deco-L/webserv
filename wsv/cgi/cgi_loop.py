#CGIスクリプトの、エラーケーステストのためのスクリプトです。

#無限ループにはいります。

import os
import sys

print("Content-Type: text/html; charset=utf-8")
print("")

print("<html><head><title>CGI Script</title></head><body>")
print("<h1>CGI Loop</h1>")
print("<table border='1'>")
print("<tr><th>Variable</th><th>Value</th></tr>")
print("<tr><td>Loop</td><td>Loop</td></tr>")

#while True:
	#pass

print("</table>")
print("</body></html>")

while True:
	pass