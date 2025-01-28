#cgiスクリプトです。

import os
import sys
import time

# 環境変数をprint
time.sleep(2)
print("Content-Type: text/html")
print("")

print("<html><head><title>CGI Script</title></head><body>")
print("<h1>Environment Variables</h1>")
print("<table border='1'>")
print("<tr><th>Variable</th><th>Value</th></tr>")
env_list_all = ["AUTH_TYPE", "CONTENT_LENGTH", "CONTENT_TYPE", "GATEWAY_INTERFACE", "HTTP_ACCEPT", "HTTP_FORWARDED", "HTTP_REFERER", "HTTP_USER_AGENT", "HTTP_X_FORWARDED_FOR", "PATH_INFO", "PATH_TRANSLATED", "QUERY_STRING", "REMOTE_ADDR", "REMOTE_HOST", "REMOTE_IDENT", "REMOTE_USER", "REQUEST_METHOD", "SCRIPT_NAME", "SERVER_NAME", "SERVER_PORT", "SERVER_PROTOCOL", "SERVER_SOFTWARE", "PWD"]
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




##変数名	意味
#AUTH_TYPE	認証方式(例えば MD5, Basicなど)
#CONTENT_LENGTH	標準入力から読み込み可能なデータのバイト数(METHOD=POSTの時)
#CONTENT_TYPE	クライアントから送られてきたデータのタイプ。フォームからMETHOD=POSTで送信した場合は、application/x-www-form-urlencoded となる。
#GATEWAY_INTERFACE	ゲートウェイプロトコル名称(例えばCGI/1.1)
#HTTP_ACCEPT	ブラウザがサポートする Content-type: のリスト。すべてを許可する場合、*/* となる。
#HTTP_FORWARDED	この要求をフォワードしたプロキシサーバーの情報。送信されない場合もある。
#HTTP_REFERER	そのCGIを呼び出したページのURL。送信されない場合や、たまに、全く別のURLを差していることもある。
#HTTP_USER_AGENT	ブラウザに関する情報(Mozilla/4.01 [ja] (Win95; I) など)
#HTTP_X_FORWARDED_FOR	この要求をフォワードしたプロキシサーバーのIPアドレス。
#PATH_INFO	パス情報。たとえば、「cgi-bin/xxx.cgi/taro/xxx.htm」というURLでCGIスクリプトを呼び出した場合、PATH_INFOには「/taro/xxx.htm」が格納される。
#PATH_TRANSLATED	PATH_INFOで指定したファイルの、サーバー上の絶対パス名。
#QUERY_STRING	「http://サーバー名/CGIスクリプト名?データ」というURLを要求した場合のデータ部分。
#REMOTE_ADDR	クライアントのIPアドレス(例えば10.0.0.1)
#REMOTE_HOST	クライアントのホスト名(例えば abc32.abc.co.jp)
#REMOTE_IDENT	クライアント側のユーザーID
#REMOTE_USER	クライアント側のユーザー名
#REQUEST_METHOD	METHODで指定したデータ取得手段。GET, POST, HEAD, PUT, DELETE, LINK, UNLINKなどの種類がある。
#SCRIPT_NAME	CGIスクリプトの名前。
#SERVER_NAME	サーバー名
#SERVER_PORT	サーバーのポート番号(例えば80)
#SERVER_PROTOCOL	サーバーのプロトコル名(例えばHTTP/1.0)
#SERVER_SOFTWARE	サーバーのソフトウェア名(例えば NCSA/1.3)