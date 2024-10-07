/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   Config.hpp                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: miyazawa.kai.0823 <miyazawa.kai.0823@st    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/07/01 14:21:20 by csakamot          #+#    #+#             */
/*   Updated: 2024/10/07 15:36:16 by miyazawa.ka      ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef CONFIG_HPP
#define CONFIG_HPP

#include <string>
#include "webserv.hpp"

// パースのモードを表す列挙型
enum ParseMode {
    MODE_GLOBAL = 0,
    MODE_SERVER = 1,
    MODE_LOCATION = 2
};

//- server
//  - listen
//  - server_name
//  - error_page
//  - client_max_body_size
//  - root
//  - index
//  - autoindex
//  - location
//    - root
//    - methods
//    - return
//    - autoindex
//    - index
//    - cgi_extension
//    - upload_enable
//    - upload_store





struct ConfigLocation {
    std::string path;
    std::string root;
    std::vector<std::string> methods;
    std::pair<int, std::string> return_;
    bool autoindex;
    std::vector<std::string> index;
    std::vector<std::string> cgi_extension;
    bool upload_enable;
    std::string upload_store;
};


struct ConfigServer {
    std::pair<std::string, std::string> listen; // IPアドレスとポート番号
    std::vector<std::string> server_name;
    std::vector<std::pair<int, std::string> > error_page; // ステータスコードとパス
    unsigned long client_max_body_size; // 0~1024GB 0=無制限
    std::string root;
    std::vector<std::string> index;
    bool autoindex;
    std::vector<ConfigLocation> locations;
};

class Config {
private:
  int         _argc;
  std::string _file_path;
  std::vector<ConfigServer> _servers;

public:
  Config(void);
  Config(int argc, std::string argv);
  ~Config();

  int         getArgc(void) const;
  std::string getFileName(void) const;
  std::vector<ConfigServer> getServers(void) const;
  void        checkNbrArg(void) const;
  void        checkConfig(void);
  bool isValidConfig(std::string file_content);

  Config&      operator=(const Config& obj);
};

#endif


//- server
//  - listen
//  - server_name
//  - error_page
//  - client_max_body_size
//  - root
//  - index
//  - autoindex
//  - location
//    - root
//    - methods
//    - return
//    - autoindex
//    - index
//    - cgi_extension
//    - upload_enable
//    - upload_store

//### **`server` ブロック内のディレクティブ**
  //- 引数なし
//- **`listen`**: `[IPアドレス:]ポート [オプション]`
  // - 重複は追加とみなし、エラー処理においては引数の重複を禁止します。
//- **`server_name`**: `ドメイン名1 ドメイン名2 ...`
  // - 重複は追加とみなし、エラー処理においては引数の重複を禁止します。
//- **`error_page`**: `ステータスコード パス`
  // - 重複は追加とみなし、エラー処理においては引数の重複を禁止します。
//- **`client_max_body_size`**: `サイズ`
  // - 重複は上書きとみなし、最後に指定されたサイズが優先されます。
//- **`root`**: `パス`
  // - 重複は上書きとみなし、最後に指定されたパスが優先されます。
//- **`index`**: `ファイル名1 ファイル名2 ...`
  // - 重複は上書きとみなし、最後に指定されたファイルが優先されます。
//- **`autoindex`**: `on|off`
  // - 重複は上書きとみなし、最後に指定されたオプションが優先されます。

//### **`location` ブロック内のディレクティブ**
  // - 重複は追加とみなし、エラー処理においては引数の重複を禁止します。
//- **`root`**: `パス`
  // - 重複は上書きとみなし、最後に指定されたパスが優先されます。
//- **`methods`**: `METHOD1 METHOD2 ...`
  // - 重複は上書きとみなし、最後に指定されたメソッドが優先されます。
//- **`return`**: `ステータスコード [URL]`
  // - 重複は上書きとみなし、最後に指定された引数が優先されます。
//- **`autoindex`**: `on|off`
  // - 重複は上書きとみなし、最後に指定されたオプションが優先されます。
//- **`index`**: `ファイル名1 ファイル名2 ...`
  // - 重複は上書きとみなし、最後に指定されたファイルが優先されます。
//- **`cgi_extension`**: `拡張子1 拡張子2 ...`
  // 重複は追加とみなし、エラー処理においては引数の重複を禁止します。
//- **`upload_enable`**: `on|off`
  // - 重複は上書きとみなし、最後に指定されたオプションが優先されます。
//- **`upload_store`**: `パス`
  // - 重複は上書きとみなし、最後に指定されたパスが優先されます。


/*
以下に、指定されたディレクティブの中で**複数の引数を取るもの**を一覧にまとめました。それぞれのディレクティブがどのような複数引数を受け取るのか、簡潔に説明します。

---

## 複数の引数を取るディレクティブ一覧

### 1. `server_name`

- **フォーマット**:
  ```nginx
  server_name ドメイン名1 ドメイン名2 ...;
  ```
- **例**:
  ```nginx
  server_name example.com www.example.com api.example.com;
  ```
- **説明**:
  このディレクティブは、サーバーブロックが応答する複数のドメイン名を指定します。ワイルドカードや正規表現も利用可能です。
  重複は追加とみなし、エラー処理においては引数の重複を禁止します。

### 2. `error_page`

- **フォーマット**:
  ```nginx
  error_page ステータスコード1 ステータスコード2 ... パス;
  ```
- **例**:
  ```nginx
  error_page 404 500 502 /custom_error.html;
  ```
- **説明**:
  指定した複数のHTTPステータスコードに対して、共通のカスタムエラーページを設定します。これにより、異なるエラーコードに対して同一のエラーページを表示できます。
  重複は追加とみなし、エラー処理においては引数の重複を禁止します。

### 3. `index`

- **フォーマット**:
  ```nginx
  index ファイル名1 ファイル名2 ...;
  ```
- **例**:
  ```nginx
  index index.html index.htm default.html;
  ```
- **説明**:
  ディレクトリにアクセスされた際に表示されるデフォルトのインデックスファイルを複数指定します。サーバーは指定された順にファイルの存在を確認し、最初に見つかったファイルを表示します。
  重複は上書きとみなし、最後に指定されたファイルが優先されます。

### 4. `methods`

- **フォーマット**:
  ```nginx
  methods METHOD1 METHOD2 ...;
  ```
- **例**:
  ```nginx
  methods GET POST DELETE;
  ```
- **説明**:
  特定の`location`ブロック内で許可するHTTPメソッドを複数指定します。これにより、許可されたメソッドのみがその`location`に対して有効になります。
  重複は上書きとみなし、最後に指定されたメソッドが優先されます。

### 5. `return`

- **フォーマット**:
  ```nginx
  return ステータスコード [URL];
  ```
- **例**:
  ```nginx
  return 301 https://www.example.com$request_uri;
  return 404 "Not Found";
  ```
- **説明**:
  クライアントに対して指定したHTTPステータスコードと、オプションでリダイレクト先のURLやメッセージを返します。`return`ディレクティブは、単純なリダイレクトやエラーメッセージの返却に使用されます。
  重複は上書きとみなし、最後に指定されたステータスコードやURLが優先されます。

### 6. `cgi_extension`

- **フォーマット**:
  ```nginx
  cgi_extension .拡張子1 .拡張子2 ...;
  ```
- **例**:
  ```nginx
  cgi_extension .php .py .cgi;
  ```
- **説明**:
  指定した複数のファイル拡張子を持つリクエストに対して、CGIスクリプトとして外部プログラムを実行します。これにより、動的なコンテンツの生成が可能となります。
  重複は追加とみなし、エラー処理においては引数の重複を禁止します。

---

## `location` ブロック内のディレクティブ

### 1. `location` ブロック自体

- **フォーマット**:
  ```nginx
  location パス {
      # ロケーションブロック内のディレクティブ
  }
  ```
- **例**:
  ```nginx
  location /images/ {
      # /images/ に対する設定
  }
  ```
- **説明**:
  特定のURLパスに対する処理を定義するブロックです。パスにマッチするリクエストに対して、内部のディレクティブが適用されます。
  重複は追加とみなし、エラー処理においては引数の重複を禁止します。

---

## その他のディレクティブ

以下のディレクティブは、複数の引数を取る場合がありますが、上記の一覧には含まれていません。必要に応じて追加の検討を行ってください。

### `listen`

- **フォーマット**:
  ```nginx
  listen [IPアドレス:]ポート [オプション1 オプション2 ...];
  ```
- **例**:
  ```nginx
  listen 80;
  listen 127.0.0.1:8080 ssl;
  ```
- **説明**:
  サーバーがリッスンするIPアドレスとポートを指定します。オプションとして`ssl`や`default_server`などを複数指定できます。
  重複は追加とみなし、エラー処理においては引数の重複を禁止します。

---

## まとめ

以下は、**複数の引数を取る主要なディレクティブ**の一覧です：

1. **`server_name`**
   - 複数のドメイン名を指定可能。
2. **`error_page`**
   - 複数のステータスコードに対して同一のエラーページを設定可能。
3. **`index`**
   - 複数のインデックスファイル名を指定可能。
4. **`methods`**
   - 複数のHTTPメソッドを指定可能。
5. **`return`**
   - ステータスコードに加えてオプションでURLやメッセージを指定可能。
6. **`cgi_extension`**
   - 複数のファイル拡張子を指定可能。

これらのディレクティブを適切に設定することで、柔軟かつ強力なHTTPサーバーの構成が可能となります。各ディレクティブの引数の取り扱いを正確に実装し、設定ファイルの解析時に正しく検証することが重要です。

---

**補足**: 実装時には、これらのディレクティブの引数が適切に解析・検証されるよう、設定ファイルのパーサーや検証ロジックを慎重に設計してください。また、ディレクティブごとの相互関係や優先順位にも注意を払い、期待通りの動作を実現するよう心掛けましょう。
*/