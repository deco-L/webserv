# ベースイメージとして Ubuntu 22.04 を使用
FROM ubuntu:22.04

# 環境変数の設定（非対話型インストールを強制）
ENV DEBIAN_FRONTEND=noninteractive

# パッケージリストの更新と必要なパッケージのインストール
RUN apt-get update && \
    apt-get install -y --no-install-recommends \
        build-essential \
        curl \
        wget \
        vim \
        make \
        bc \
        python3 \
        python3-minimal \
    && apt-get clean && \
    rm -rf /var/lib/apt/lists/*

# Python3に対するシンボリックリンクを作成（/usr/bin/python が存在するようにする）
RUN ln -s /usr/bin/python3 /usr/bin/python

# aliasの設定
RUN echo "alias full='make && ./webserv conf/full.conf'" >> ~/.bashrc && \
    echo "alias default='make && ./webserv conf/default.conf'" >> ~/.bashrc

# 必要なディレクトリとファイルを作成
RUN mkdir -p /var/www/html/errors \
    /var/www/html/uploads \
    /var/www/html/oldpage \
    /var/www/html/newpage \
    /var/www/html/cgi-bin \
    /var/www/html/restricted \
    /var/www/images/errors \
    /var/www/images/upload \
    /var/www/images/uploads \
    /var/www/images/images \
    /var/www/uploads \
    /var/www/test/errors \
    /var/www/test/upload \
    /var/www/test/downloads \
    /var/www/test/cgi-bin \
    /var/www/downloads/downloads && \
    touch /var/www/html/index.html /var/www/html/index.htm \
    /var/www/html/errors/404.html \
    /var/www/test/index.html \
    /var/www/test/errors/50x.html

# デフォルトのコンテンツを各ファイルに追加
RUN echo "<html><body><h1>Index Page</h1></body></html>" > /var/www/html/index.html && \
    echo "<html><body><h1>Index Page</h1></body></html>" > /var/www/html/index.htm && \
    echo "<html><body><h1>404 Not Found</h1></body></html>" > /var/www/html/errors/404.html && \
    echo "<html><body><h1>50x error.</h1></body></html>" > /var/www/html/errors/50x.html && \
    echo "<html><body><h1>Test Index Page</h1></body></html>" > /var/www/test/index.html && \
    echo "<html><body><h1>50x Server Error</h1></body></html>" > /var/www/test/errors/50x.html \
    echo "<html><body><h1>Image / errors / 50x.html</h1></body></html>" > /var/www/images/errors/50x.html \
    echo "<html><body><h1>Image / upload / 404.html</h1></body></html>" > /var/www/images/errors/404.html \
    echo "<html><body><h1> html / newpage / index.html</h1></body></html>" > /var/www/html/newpage/index.html

# 所有者と権限の設定（必要に応じて変更）
RUN chown -R root:root /var/www/ && \
    chmod -R 755 /var/www/

# 必要なCGI実行ファイルを確認（ここではpython3のみをインストール）
# 他のCGI実行ファイル（php-cgiやperl）は手動でインストールする必要があります


# 作業ディレクトリを設定
WORKDIR /usr/src/app

# ローカルのプログラムをコンテナ内にコピー
COPY ./ .

# コンテナ起動時にbashを実行
CMD ["/bin/bash"]
