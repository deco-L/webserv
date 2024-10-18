
config関連は、すべてconfigクラスにまとめてあります。

config.checkConfigメソッドで、設定ファイルの構文が正しいかどうかを確認します。
config.checkConfigは内部で、config.isValidConfigメソッドを呼び出しています。
config.isValidConfigメソッドで、設定ファイルが正しいかどうかを確認します。
config.isValidConfigはたくさん分割されているので、以下にその構造を示します。

isValidConfig
│
├── isValidBracketFormat
│   └── isValidOpeningBracePosition
├── isValidNest
│   └── findBlockRanges
├── isValidDirective
├── hasRequiredDirectives
├── isValidDirectiveArguments
│   ├── hasArguments
│   ├── hasCorrectNumberOfArguments
│   ├── hasValidArguments
│   │   ├── isValidIpAddress
│   │   ├── isValidPort
│   │   └── getPathType
│   ├── hasValidPaths
│   │   ├── getRoot
│   │   ├── access (system call)
│   │   └── getPathType
│   └── outputError
└── outputError


**ディレクティブの一覧:**

- **`server`** `{ ... }`
  仮想サーバーの設定を指定するためのサーバーブロックを定義します。

  - **`listen`** `[address:]port;`
    サーバーがリッスンするアドレスとポートを指定します。

  - **`server_name`** `name [name ...];`
    サーバーの名前を設定します。

  - **`error_page`** `code [code ...] path;`
    指定したHTTPエラーコードに対してカスタムエラーページを設定します。

  - **`client_max_body_size`** `size;`
    クライアントリクエストボディのサイズ制限を設定します。

  - **`root`** `path;`
    リクエストに対してルートディレクトリを設定します。

  - **`index`** `file [file ...];`
    ディレクトリがリクエストされたときに提供されるデフォルトファイルを定義します。

  - **`autoindex`** `on | off;`
    ディレクトリリストの表示を有効または無効にします。

  - **`location`** `[path] { ... }`
    ルートおよびその特定の設定を定義します。

    - **`root`** `path;`
      このlocationブロック内でルートディレクトリを上書きします。

    - **`methods`** `method [method ...];`
      ルートで受け付けるHTTPメソッドのリストを定義します。

    - **`return`** `code [URL];`
      HTTPリダイレクトを発行するか、特定のステータスコードを返します。

    - **`autoindex`** `on | off;`
      このルートに対してディレクトリリストの表示を有効または無効にします。

    - **`index`** `file [file ...];`
      このルートのデフォルトファイルを設定します。

    - **`cgi_extension`** `extension path_to_cgi_executable;`
      特定のファイル拡張子に対してCGIの実行を指定します。

    - **`upload_enable`** `on | off;`
      このルートでファイルアップロードを有効または無効にします。

    - **`upload_store`** `path;`
      アップロードされたファイルを保存するディレクトリを指定します。

---

**サンプル設定ファイル:**

```nginx
server {
    listen 80;
    server_name www.example.com;

    error_page 404 /errors/404.html;
    client_max_body_size 10M;

    root /var/www/html;
    index index.html index.htm;
    autoindex off;

    location / {
        root /var/www/html;
        methods GET POST;
        index index.html index.htm;
    }

    location /images/ {
        root /var/www/images;
        autoindex on;
    }

    location /upload {
        methods POST;
        upload_enable on;
        upload_store /var/www/uploads;
        return 200;
    }

    location /oldpage {
        return 301 http://www.example.com/newpage;
    }

    location /cgi-bin/ {
        cgi_extension .php /usr/bin/php-cgi;
        cgi_extension .py /usr/bin/python;
        methods GET POST;
    }

    location /restricted/ {
        methods GET;
        return 403;
    }
}

server {
    listen 8080;
    server_name www.test.com test.com;

    error_page 500 502 503 504 /errors/50x.html;
    client_max_body_size 20M;

    root /var/www/test;
    index index.html;
    autoindex on;

    location / {
        root /var/www/test;
        methods GET POST DELETE;
    }

    location /downloads/ {
        root /var/www/downloads;
        autoindex on;
    }

    location /upload {
        methods POST;
        upload_enable on;
        upload_store /var/www/test/uploads;
    }

    location /cgi-bin/ {
        cgi_extension .pl /usr/bin/perl;
        methods GET POST;
    }
}
```

**サンプル設定ファイルの説明:**

- **最初のサーバーブロック (`listen 80;`):**
  - ポート`80`でドメイン`www.example.com`をホストします。
  - `404`エラーの場合、`/errors/404.html`にカスタムエラーページを設定します。
  - クライアントボディサイズを`10M`に制限します。
  - ルートディレクトリとインデックスファイルを指定します。
  - ディレクトリリストを全体で無効にします。

  - **`/` ルート:**
    - `/var/www/html`からコンテンツを提供します。
    - `GET`と`POST`メソッドを受け付けます。
    - デフォルトのインデックスファイルを使用します。

  - **`/images/` ルート:**
    - ルートを`/var/www/images`に上書きします。
    - ディレクトリリストを有効にします。

  - **`/upload` ルート:**
    - `POST`メソッドのみ受け付けます。
    - ファイルアップロードを有効にし、`/var/www/uploads`に保存します。
    - 成功時に`200 OK`を返します。

  - **`/oldpage` ルート:**
    - `301 Moved Permanently`で`http://www.example.com/newpage`にリダイレクトします。

  - **`/cgi-bin/` ルート:**
    - `.php`と`.py`の拡張子を持つCGIスクリプトを、指定した実行ファイルで実行します。
    - `GET`と`POST`メソッドを受け付けます。

  - **`/restricted/` ルート:**
    - `GET`メソッドのみ受け付けます。
    - アクセスを拒否し、`403 Forbidden`を返します。

- **2つ目のサーバーブロック (`listen 8080;`):**
  - ポート`8080`で`www.test.com`および`test.com`をホストします。
  - サーバーエラー（コード`500`、`502`、`503`、`504`）に対してカスタムエラーページを設定します。
  - クライアントボディサイズを`20M`に制限します。
  - ルートディレクトリとインデックスファイルを指定します。
  - ディレクトリリストを全体で有効にします。

  - **`/` ルート:**
    - `/var/www/test`からコンテンツを提供します。
    - `GET`、`POST`、`DELETE`メソッドを受け付けます。

  - **`/downloads/` ルート:**
    - ルートを`/var/www/downloads`に上書きします。
    - ディレクトリリストを有効にします。

  - **`/upload` ルート:**
    - `POST`メソッドのみ受け付けます。
    - ファイルアップロードを有効にし、`/var/www/test/uploads`に保存します。

  - **`/cgi-bin/` ルート:**
    - `.pl`拡張子を持つCGIスクリプトを、Perlインタープリタで実行します。
    - `GET`と`POST`メソッドを受け付けます。

---

**使用上の注意点:**

- **順序が重要:** `server` ブロックの順序は重要です。指定された`listen`アドレスとポートの組み合わせで最初に定義されたサーバーがデフォルトサーバーとなり、`server_name`に一致しないリクエストに応答します。

- **CGIの実行:** `cgi_extension` ディレクティブは、ファイル拡張子とCGIの実行ファイルを関連付けます。指定された拡張子を持つファイルがリクエストされると、サーバーはそのファイルを指定された実行ファイルで実行します。

- **メソッドの制御:** `methods` ディレクティブは、特定のルートで許可されるHTTPメソッドを定義します。定義されていないメソッドのリクエストは拒否されます。

- **エラーページ:** カスタムエラーページを設定することで、エラーが発生した際にユーザーにわかりやすいメッセージ



- server
  - listen
  - server_name
  - error_page
  - client_max_body_size
  - root
  - index
  - autoindex
  - location
    - root // 下層優先
    - methods
    - return
    - autoindex // 下層優先
    - index // 下層優先
    - cgi_extension
    - upload_enable
    - upload_store

// rule
- 同じディレクティブが複数回指定された場合、上書きや追加の挙動がある
- locationブロック内で指定されたディレクティブは、そのlocationブロック内でのみ有効
- locationブロック内で指定されていないディレクティブは、親のserverブロックで指定された値が適用される
- httpディレクティブは非対応
- locationディレクティブでは、正規表現、=, ~, ~* などのパターンマッチングが非対応
- serverブロック直下にrootがないといけない
- `server`ブロックと`location`ブロックで許可されている同一のディレクティブがある場合、`location`ブロックの設定が優先される

---

**ディレクティブごとの設定の上書き・追加の挙動**

以下は、指定された各ディレクティブについて、同じブロック内で同じディレクティブが複数回定義された場合に、それが設定の**追加**になるのか、**上書き**になるのかをまとめたものです。それぞれのディレクティブの挙動を詳しく説明します。

---

### **サーバーブロックレベル**

#### **1. `listen`**

- **挙動**: **追加**
- **説明**: `listen`ディレクティブは、サーバーが待ち受けるアドレスとポートを指定します。同じ`server`ブロック内で複数の`listen`ディレクティブを定義すると、それぞれのアドレスとポートの組み合わせが追加されます。サーバーはこれらすべてのアドレスとポートでリクエストを受け付けます。

  ```nginx
  server {
      listen 80;
      listen 443 ssl;
      # サーバーはポート80と443の両方で待ち受ける
  }
  ```

#### **2. `server_name`**

- **挙動**: **追加**
- **説明**: `server_name`ディレクティブは、サーバーの名前を指定します。同じ`server`ブロック内で複数回定義すると、すべての指定されたサーバー名がリストに追加されます。サーバーはこれらの名前でアクセスされたリクエストを処理します。

  ```nginx
  server {
      server_name example.com;
      server_name www.example.com;
      # サーバーはexample.comとwww.example.comの両方のリクエストを受け付ける
  }
  ```

#### **3. `error_page`**

- **挙動**: **追加**
- **説明**: `error_page`ディレクティブは、特定のHTTPエラーコードに対するカスタムエラーページを設定します。複数回定義することで、異なるエラーコードに対するエラーページを個別に設定できます。

  ```nginx
  server {
      error_page 404 /custom_404.html;
      error_page 500 502 503 504 /custom_50x.html;
      # 404エラーと5xxエラーに対して異なるエラーページを設定
  }
  ```

#### **4. `client_max_body_size`**

- **挙動**: **上書き**
- **説明**: `client_max_body_size`ディレクティブは、クライアントからのリクエストボディの最大サイズを設定します。同じブロック内で複数回定義された場合、**最後に定義された値**が有効になります。

  ```nginx
  server {
      client_max_body_size 10M;
      client_max_body_size 20M;
      # 有効なのは20Mのみ
  }
  ```

#### **5. `root`**

- **挙動**: **上書き**
- **説明**: `root`ディレクティブは、リクエストに対するルートディレクトリを指定します。同じブロック内で複数回定義すると、最後の設定が有効になります。

  ```nginx
  server {
      root /var/www/html;
      root /var/www/public;
      # 有効なのは/var/www/publicのみ
  }
  ```

#### **6. `index`**

- **挙動**: **上書き**
- **説明**: `index`ディレクティブは、ディレクトリがリクエストされたときに提供されるデフォルトのファイルを指定します。複数回定義された場合、最後の設定のみが有効です。

  ```nginx
  server {
      index index.html index.htm;
      index home.html;
      # 有効なのはhome.htmlのみ
  }
  ```

#### **7. `autoindex`**

- **挙動**: **上書き**
- **説明**: `autoindex`ディレクティブは、ディレクトリの自動インデックスリストの表示を制御します。複数回定義された場合、最後の設定が有効になります。

  ```nginx
  server {
      autoindex on;
      autoindex off;
      # 有効なのはautoindex offのみ
  }
  ```

#### **8. `location`**

- **挙動**: **追加**
- **説明**: `location`ブロックは、特定のURIパターンに対する設定を定義します。同じ`server`ブロック内で複数の`location`を定義することで、異なるパスに対する設定を追加できます。

  ```nginx
  server {
      location /images {
          # 設定1
      }
      location /videos {
          # 設定2
      }
      # 2つのlocationが追加される
  }
  ```

---

### **ロケーションブロックレベル**

#### **1. `root`**

- **挙動**: **上書き**
- **説明**: ロケーションブロック内での`root`ディレクティブは、そのロケーションに対するルートディレクトリを上書きします。複数回定義された場合、最後の設定が有効になります。

  ```nginx
  location /app {
      root /var/www/html;
      root /var/www/app;
      # 有効なのは/var/www/appのみ
  }
  ```

#### **2. `methods`**

- **挙動**: **上書き**
- **説明**: `methods`ディレクティブは、許可されるHTTPメソッドを指定します（これは標準のNGINXディレクティブではない可能性があります）。複数回定義された場合、最後の設定が有効になります。

  ```nginx
  location /api {
      methods GET POST;
      methods GET;
      # 有効なのはGETのみ
  }
  ```

#### **3. `return`**

- **挙動**: **上書き**
- **説明**: `return`ディレクティブは、特定のステータスコードやリダイレクトを返します。複数回定義された場合、最後の設定が適用されます。

  ```nginx
  location /old {
      return 301 /new;
      return 302 /temporary;
      # 有効なのはreturn 302 /temporary;のみ
  }
  ```

#### **4. `autoindex`**

- **挙動**: **上書き**
- **説明**: サーバーブロック内と同様に、複数回定義された場合、最後の設定が有効になります。

  ```nginx
  location /downloads {
      autoindex off;
      autoindex on;
      # 有効なのはautoindex onのみ
  }
  ```

#### **5. `index`**

- **挙動**: **上書き**
- **説明**: サーバーブロック内と同様に、最後に定義された`index`設定が有効になります。

  ```nginx
  location /blog {
      index index.php;
      index blog.php;
      # 有効なのはblog.phpのみ
  }
  ```

#### **6. `cgi_extension`**

- **挙動**: **追加**
- **説明**: `cgi_extension`ディレクティブは、特定のファイル拡張子に対してCGIの実行を指定します（これは標準のNGINXディレクティブではなく、サードパーティモジュールの可能性があります）。複数回定義された場合、それぞれの拡張子と実行ファイルのペアが追加されます。

  ```nginx
  location /cgi-bin {
      cgi_extension .pl /usr/bin/perl;
      cgi_extension .py /usr/bin/python;
      # 2つの拡張子に対するCGI設定が追加される
  }
  ```

#### **7. `upload_enable`**

- **挙動**: **上書き**
- **説明**: `upload_enable`ディレクティブは、ファイルアップロードの有効化を制御します（これは標準のNGINXディレクティブではない可能性があります）。複数回定義された場合、最後の設定が有効になります。

  ```nginx
  location /upload {
      upload_enable on;
      upload_enable off;
      # 有効なのはupload_enable offのみ
  }
  ```

#### **8. `upload_store`**

- **挙動**: **上書き**
- **説明**: `upload_store`ディレクティブは、アップロードされたファイルの保存先を指定します。複数回定義された場合、最後の設定が有効になります。

  ```nginx
  location /upload {
      upload_store /var/www/uploads;
      upload_store /var/www/new_uploads;
      # 有効なのは/var/www/new_uploadsのみ
  }
  ```

---

**まとめ表**

| ディレクティブ             | 挙動     | 説明                                             |
|---------------------------|----------|--------------------------------------------------|
| **サーバーブロックレベル** |          |                                                  |
| `listen`                  | 追加     | 複数のアドレスとポートで待ち受ける                |
| `server_name`             | 追加     | 複数のサーバー名を受け付ける                      |
| `error_page`              | 追加     | 複数のエラーコードに対するエラーページを設定       |
| `client_max_body_size`    | 上書き   | 最後の設定のみが有効                              |
| `root`                    | 上書き   | 最後の設定のみが有効                              |
| `index`                   | 上書き   | 最後の設定のみが有効                              |
| `autoindex`               | 上書き   | 最後の設定のみが有効                              |
| `location`                | 追加     | 複数のlocationブロックを定義できる                |
| **ロケーションブロックレベル** |          |                                                  |
| `root`                    | 上書き   | 最後の設定のみが有効                              |
| `methods`                 | 上書き   | 最後の設定のみが有効                              |
| `return`                  | 上書き   | 最後の設定のみが有効                              |
| `autoindex`               | 上書き   | 最後の設定のみが有効                              |
| `index`                   | 上書き   | 最後の設定のみが有効                              |
| `cgi_extension`           | 追加     | 複数の拡張子と実行ファイルのペアを追加できる      |
| `upload_enable`           | 上書き   | 最後の設定のみが有効                              |
| `upload_store`            | 上書き   | 最後の設定のみが有効                              |


---

## 以下memo
- server
  - listen
  - server_name
  - error_page
  - client_max_body_size
  - root
  - index
  - autoindex
  - location
    - root
    - methods
    - return
    - autoindex
    - index
    - cgi_extension
    - upload_enable
    - upload_store

- **`server`** `{ ... }`
  仮想サーバーの設定を指定するためのサーバーブロックを定義します。

  - **`listen`** `[address:]port;`
    サーバーがリッスンするアドレスとポートを指定します。

  - **`server_name`** `name [name ...];`
    サーバーの名前を設定します。

  - **`error_page`** `code [code ...] path;`
    指定したHTTPエラーコードに対してカスタムエラーページを設定します。

  - **`client_max_body_size`** `size;`
    クライアントリクエストボディのサイズ制限を設定します。

  - **`root`** `path;`
    リクエストに対してルートディレクトリを設定します。

  - **`index`** `file [file ...];`
    ディレクトリがリクエストされたときに提供されるデフォルトファイルを定義します。

  - **`autoindex`** `on | off;`
    ディレクトリリストの表示を有効または無効にします。

  - **`location`** `[path] { ... }`
    ルートおよびその特定の設定を定義します。

    - **`root`** `path;`
      このlocationブロック内でルートディレクトリを上書きします。

    - **`methods`** `method [method ...];`
      ルートで受け付けるHTTPメソッドのリストを定義します。

    - **`return`** `code [URL];`
      HTTPリダイレクトを発行するか、特定のステータスコードを返します。

    - **`autoindex`** `on | off;`
      このルートに対してディレクトリリストの表示を有効または無効にします。

    - **`index`** `file [file ...];`
      このルートのデフォルトファイルを設定します。

    - **`cgi_extension`** `extension path_to_cgi_executable;`
      特定のファイル拡張子に対してCGIの実行を指定します。

    - **`upload_enable`** `on | off;`
      このルートでファイルアップロードを有効または無効にします。

    - **`upload_store`** `path;`
      アップロードされたファイルを保存するディレクトリを指定します。