# ベースイメージとして Ubuntu 22.04.3 を使用
FROM ubuntu:22.04

# パッケージリストの更新と必要なパッケージのインストール
RUN apt-get update && apt-get upgrade -y \
    && apt-get install -y --no-install-recommends \
    build-essential \
    curl \
    wget \
    vim \
    make \
    bc \
    && apt-get clean \
    && rm -rf /var/lib/apt/lists/*

# 作業ディレクトリを設定
WORKDIR /usr/src/app

# ローカルのプログラムをコンテナ内にコピー
COPY ./ .

# コンテナ起動時にbashを実行
CMD ["/bin/bash"]
