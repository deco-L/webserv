#CGIのPOSTのためのテストcgiスクリプトです。stdinからデータを読み込んで、その長さを表示します。

#stdinから入ってくるデータの想定は、
#usrname=xxx&password=yyy
#のような形式です。

import os
import sys

# stdin
content_length = os.environ["CONTENT_LENGTH"]
input = sys.stdin.read(int(content_length))
#input = sys.stdin.read()

data = input.split("&")
username = data[0].split("=")[1]
password = data[1].split("=")[1]

print("Content-Type: text/html; charset=utf-8")
print("")

print("<html><head><title>CGI Script</title></head><body>")
print("<h1>POST Data</h1>")
print("<table border='1'>")
print("<tr><th>Variable</th><th>Value</th></tr>")
print("<tr><td>Username</td><td>%s</td></tr>" % username)
print("<tr><td>Password</td><td>%s</td></tr>" % password)
print("</table>")

# 環境変数をprint
print("<h1>Environment Variables</h1>")
print("<table border='1'>")
print("<tr><th>Variable</th><th>Value</th></tr>")
env_list_all = ["AUTH_TYPE", "CONTENT_LENGTH", "CONTENT_TYPE", "GATEWAY_INTERFACE", "HTTP_ACCEPT", "HTTP_FORWARDED", "HTTP_REFERER", "HTTP_USER_AGENT", "HTTP_X_FORWARDED_FOR", "PATH_INFO", "PATH_TRANSLATED", "QUERY_STRING", "REMOTE_ADDR", "REMOTE_HOST", "REMOTE_IDENT", "REMOTE_USER", "REQUEST_METHOD", "SCRIPT_NAME", "SERVER_NAME", "SERVER_PORT", "SERVER_PROTOCOL", "SERVER_SOFTWARE"]
env_list_now = os.environ.keys()

for env in env_list_all:
	if env in env_list_now:
		print("<tr><td>%s</td><td>%s</td></tr>" % (env, os.environ[env]))
	else:
		print("<tr><td>%s</td><td></td></tr>" % env)
print("</table>")

print("<h1>Python Version</h1>")
print("<p>%s</p>" % sys.version)
print("</body></html>")