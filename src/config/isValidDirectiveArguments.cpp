/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   isValidDirectiveArguments.cpp                      :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: miyazawa.kai.0823 <miyazawa.kai.0823@st    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/10/18 13:14:30 by miyazawa.ka       #+#    #+#             */
/*   Updated: 2024/10/18 13:27:58 by miyazawa.ka      ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "Config.hpp"
#include "webserv.hpp"
#include "Error.hpp"

/* ====================
ディレクティブの引数が正しいことを確認
	- 適切な引数があることを確認

	- 引数の数が正しいことを確認

	- 有効な値が設定されていることを確認
	- 引数がIPアドレスである場合、有効なIPアドレスであることを確認
	- 引数がポート番号である場合、有効なポート番号であることを確認
	- 引数が数値である場合、有効な数値であることを確認
	- 引数がHTTPメソッドである場合、有効なHTTPメソッドであることを確認
	- 引数が拡張子である場合、有効な拡張子であることを確認

	- 引数がファイルパスである場合、ファイルが存在することを確認
	- 引数がディレクトリパスである場合、ディレクトリが存在することを確認
==================== */
// MARK: isValidDirectiveArguments

static bool hasArguments(const std::vector<std::string>& lines)
{
	for (std::vector<std::string>::const_iterator it = lines.begin(); it != lines.end(); ++it)
	{
		const std::string& line = *it;

		if (line.find("server") != std::string::npos)
			continue;

		// location [path] {}
		if (line.find("location") != std::string::npos)
		{
			// "location [path] {" を、"location [path]"にする
			// '{'を消した後の空白も削除する
			std::string location = line.substr(0, line.find('{'));
			location.erase(location.find_last_not_of(" \t") + 1);

			if (location.find(' ') == std::string::npos)
				return false; // 引数がない
			continue;
		}

		if (line == "}")
			continue;

		if (line.find(' ') == std::string::npos)
			return false; // 引数がない
	}
	return true;
}

static bool hasCorrectNumberOfArguments(const std::vector<std::string>& lines)
{
	//- server
	//  - listen				1~N
	//  - server_name			1~N
	//  - error_page			2~N
	//  - client_max_body_size	1
	//  - root					1
	//  - index					1~N
	//  - autoindex				1
	//  - location				1
	//  - methods				1~N
	//  - return				1~2
	//  - cgi_extension			2
	//  - upload_enable			1
	//  - upload_store			1
	// locationは特別

	// 1
    const char* directivesArray[] = {
        "client_max_body_size",
        "root",
        "autoindex",
        "upload_enable",
        "upload_store"
    };
    size_t arraySize = sizeof(directivesArray) / sizeof(directivesArray[0]);
    std::vector<std::string> directives1(directivesArray, directivesArray + arraySize);

	// 1~N
	const char* directives1NArray[] = {
		"listen",
		"server_name",
		"index",
		"methods"
	};
	arraySize = sizeof(directives1NArray) / sizeof(directives1NArray[0]);
	std::vector<std::string> directives1N(directives1NArray, directives1NArray + arraySize);

	// 2~N
	const char* directives2NArray[] = {
		"error_page"
	};
	arraySize = sizeof(directives2NArray) / sizeof(directives2NArray[0]);
	std::vector<std::string> directives2N(directives2NArray, directives2NArray + arraySize);

	// 1~2
	const char* directives12Array[] = {
		"return"
	};
	arraySize = sizeof(directives12Array) / sizeof(directives12Array[0]);
	std::vector<std::string> directives12(directives12Array, directives12Array + arraySize);

	// 2
	const char* directives2Array[] = {
		"cgi_extension"
	};
	arraySize = sizeof(directives2Array) / sizeof(directives2Array[0]);
	std::vector<std::string> directives2(directives2Array, directives2Array + arraySize);

	for (std::vector<std::string>::const_iterator it = lines.begin(); it != lines.end(); ++it)
	{
		const std::string& line = *it;
		const std::vector<std::string> tokens = mylib::split(line, " ");
		std::string directive = tokens[0];
		//std::cout << directive << std::endl;

		if (directive == "location") // "location" "[path]" "{"
		{
			if (tokens[tokens.size() - 1] == "{" && tokens.size() == 3)
				continue;
			else
				return false;
		}

		if (directive == "}")
			continue;

		if (std::find(directives1.begin(), directives1.end(), directive) != directives1.end())
		{
			if (tokens.size() != 2)
				return false;
		}

		if (std::find(directives1N.begin(), directives1N.end(), directive) != directives1N.end() && tokens.size() < 2)
			return false;

		if (std::find(directives2N.begin(), directives2N.end(), directive) != directives2N.end() && tokens.size() < 3)
			return false;

		if (std::find(directives12.begin(), directives12.end(), directive) != directives12.end() && (tokens.size() != 2 && tokens.size() != 3))
			return false;

		if (std::find(directives2.begin(), directives2.end(), directive) != directives2.end() && tokens.size() != 3)
			return false;
	}
	return true;
}


// only IPv4
static bool isValidIpAddress(const std::string& ip)
{
	std::istringstream ss(ip);
	std::string token;
	int count = 0;

	while (std::getline(ss, token, '.'))
	{
		if (!mylib::isNumeric(token))
			return false;
		int num = mylib::stringToInt(token);
		if (num < 0 || num > 255 || (token.length() > 1 && token[0] == '0'))
			return false;
		++count;
	}
	return count == 4;
}

static bool isValidPort(int port)
{
	return port > 0 && port <= 65535;
}

// 有効な値が設定されていることを確認
// 引数がIPアドレスである場合、有効なIPアドレスであることを確認
// 引数がポート番号である場合、有効なポート番号であることを確認
// 引数が数値である場合、有効な数値であることを確認
// 引数がHTTPメソッドである場合、有効なHTTPメソッドであることを確認
// 引数が拡張子である場合、有効な拡張子であることを確認
// 引数が正しい数であることは確認済みの入力を前提とする
static bool hasValidArguments(const std::vector<std::string>& lines)
{
	std::vector<int> ports;
	std::vector<std::string> domains;
	std::vector<int> error_code;
	std::vector<std::string> location_path;
	std::vector<std::vector<std::string> > cgi_extensions;

	for (std::vector<std::string>::const_iterator it = lines.begin(); it != lines.end(); ++it)
	{
		const std::string& line = *it;
		const std::vector<std::string> tokens = mylib::split(line, " ");
		std::string directive = tokens[0];
		//std::cout << directive << std::endl;

		// server
		// serverに引数はない
		// ports, domains, error_code, location_path, cgi_extensionsをクリア
		if (directive == "server")
		{
			ports.clear();
			domains.clear();
			error_code.clear();
			location_path.clear();
			cgi_extensions.clear();
		}

		if (directive == "}")
			continue;

		// listen [address:]port
		// addressはIPv4のみ
		// portは1~65535
		// 重複は追加なので、重複がある場合はエラー
		if (directive == "listen") //[address:]port
		{
			if (tokens[1].find(':') != std::string::npos)
			{
				std::vector<std::string> addressPort = mylib::split(tokens[1], ":");
				if (!isValidIpAddress(addressPort[0]) || !mylib::isNumeric(addressPort[1]) || !isValidPort(mylib::stringToInt(addressPort[1])))
					return false;
				if (std::find(ports.begin(), ports.end(), mylib::stringToInt(addressPort[1])) != ports.end())
					return false;
				ports.push_back(mylib::stringToInt(addressPort[1]));
			}
			else
			{
				if (!mylib::isNumeric(tokens[1]) || !isValidPort(mylib::stringToInt(tokens[1])))
					return false;
				if (std::find(ports.begin(), ports.end(), mylib::stringToInt(tokens[1])) != ports.end())
					return false;
				ports.push_back(mylib::stringToInt(tokens[1]));
			}
		}

		// server_name name [name ...]
		// ワイルドカードがない前提
		//英数字（A〜Z、a〜z、0〜9）、ハイフン（-）、およびドット（.）のみで構成されていること。
		//ハイフン（-） は、ラベル（ドットで区切られた各部分）の先頭または末尾に使用できない。
		//ドット（.）はラベルの区切り文字として使用され、連続して現れないこと。
		//各ラベルは 1〜63文字以内であること。
		//ラベルは空であってはならない（連続するドットは不可）。
		//ドメイン名全体の長さが 253文字以内であること。
		// 重複は追加なので、重複がある場合はエラー
		if (directive == "server_name") // name [name ...]
		{
			for (size_t i = 1; i < tokens.size(); ++i)
			{
				if (tokens[i].empty())
					return false;
				if (tokens[i].length() > 253)
					return false;
				if (tokens[i].find("..") != std::string::npos)
					return false;
				std::vector<std::string> labels = mylib::split(tokens[i], ".");
				for (size_t j = 0; j < labels.size(); ++j)
				{
					if (labels[j].empty())
						return false;
					if (labels[j].length() > 63)
						return false;
					if (labels[j].find_first_not_of("abcdefghijklmnopqrstuvwxyzABCDEFGHIJKLMNOPQRSTUVWXYZ0123456789-") != std::string::npos)
						return false;
					if (labels[j][0] == '-' || labels[j][labels[j].length() - 1] == '-')
						return false;
				}
				if (domains.size() > 0 && std::find(domains.begin(), domains.end(), tokens[i]) != domains.end())
					return false;
				domains.push_back(tokens[i]);
			}
		}

		// error_page code [code ...] path
		// codeの部分のみチェックする
		// pathは別の部分でチェックするのでここではチェックしない
		// codeは3桁の数字であること
		// 100〜599
		// 重複は追加なので、重複がある場合はエラー
		if (directive == "error_page") // code [code ...] path
		{
			for (size_t i = 1; i < tokens.size() - 1; ++i)
			{
				if (!mylib::isNumeric(tokens[i]) || tokens[i].length() != 3)
					return false;
				int code = mylib::stringToInt(tokens[i]);
				if (code < 100 || code > 599)
					return false;
				if (std::find(error_code.begin(), error_code.end(), code) != error_code.end())
					return false;
				error_code.push_back(code);
			}
		}

		// client_max_body_size size
		// sizeは数値であること
		//数値と単位（k、m、g, K, M, G）が正しく指定されていること（例: 10k、1m）。単位の省略あり。
		//サイズが正の数であること。
		//少数は指定できないこと。
		//　重複は上書き
		if (directive == "client_max_body_size") // size
		{
			if (tokens[1][tokens[1].length() - 1] == 'k' || tokens[1][tokens[1].length() - 1] == 'm' || tokens[1][tokens[1].length() - 1] == 'g' ||
				tokens[1][tokens[1].length() - 1] == 'K' || tokens[1][tokens[1].length() - 1] == 'M' || tokens[1][tokens[1].length() - 1] == 'G')
			{
				if (!mylib::isNumeric(tokens[1].substr(0, tokens[1].length() - 1)))
					return false;
			}
			else
			{
				if (!mylib::isNumeric(tokens[1]))
					return false;
			}
		}

		// root path
		// pathは別の部分でチェックするのでここではチェックしない
		// 重複は上書き

		// index file [file ...]
		// '/'や'\'は含まれないか
		// ファイル名が255文字以内であること
		//許可される文字:
		//英数字（A-Z, a-z, 0-9）
		//特殊文字：ドット（.）、ハイフン（-）、アンダースコア（_）
		// 重複は上書き
		if (directive == "index") // file [file ...]
		{
			for (size_t i = 1; i < tokens.size(); ++i)
			{
				if (tokens[i].empty())
					return false;
				if (tokens[i].length() > 255)
					return false;
				if (tokens[i].find_first_not_of("abcdefghijklmnopqrstuvwxyzABCDEFGHIJKLMNOPQRSTUVWXYZ0123456789.-_") != std::string::npos)
					return false;
			}
		}

		// autoindex on | off
		// 大文字小文字は区別しない
		// 重複は上書き
		if (directive == "autoindex") // on | off
		{
			if (tokens[1] != "on" && tokens[1] != "off"
				&& tokens[1] != "ON" && tokens[1] != "OFF")
				return false;
		}

		// location [path] {}
		// pathは別の部分でチェックするのでここではチェックしない
		// 重複は追加なので、重複がある場合はエラー
		// pathが重複しているかどうかのみチェック
		if (directive == "location") // [path] {}
		{
			if (std::find(location_path.begin(), location_path.end(), tokens[1]) != location_path.end())
				return false;
			location_path.push_back(tokens[1]);
		}

		// methods method [method ...]
		// GET, POST, DELETE のみ
		// 大文字のみ
		// 重複は上書き
		// methods GET GET GET はエラー
		if (directive == "methods") // method [method ...]
		{
			for (size_t i = 1; i < tokens.size(); ++i)
			{
				if (tokens[i] != "GET" && tokens[i] != "POST" && tokens[i] != "DELETE")
					return false;
				if (std::find(tokens.begin() + 1, tokens.begin() + i, tokens[i]) != tokens.begin() + i)
					return false;
			}
		}

		// return code [URL]
		// codeは3桁の数字であること
		// 100〜599
		// URLは省略可能
		// URLがあった場合、別の部分でチェックするのでここではチェックしない
		// 重複は上書き
		if (directive == "return") // code [URL]
		{
			if (!mylib::isNumeric(tokens[1]) || tokens[1].length() != 3)
				return false;
			int code = mylib::stringToInt(tokens[1]);
			if (code < 100 || code > 599)
				return false;
		}

		// cgi_extension extension path_to_cgi_executable
		// pathが絡むので、別の部分でチェックするのでここではチェックしない
		// ここではargumentsの重複だけチェック
		// argumentsのどちらか片方だけでも、過去のものと重複していたらエラー
		// 重複は追加なので、重複がある場合はエラー
		if (directive == "cgi_extension") // extension path_to_cgi_executable
		{
			if (cgi_extensions.size() > 0)
			{
				for (size_t i = 0; i < cgi_extensions.size(); ++i)
				{
					if (cgi_extensions[i][0] == tokens[1] || cgi_extensions[i][1] == tokens[2])
						return false;
				}
			}
			std::vector<std::string> extension_args;
			extension_args.push_back(tokens[1]);
			extension_args.push_back(tokens[2]);
			cgi_extensions.push_back(extension_args);
		}

		// upload_enable on | off
		// 大文字小文字は区別しない
		// 重複は上書き
		if (directive == "upload_enable") // on | off
		{
			if (tokens[1] != "on" && tokens[1] != "off"
				&& tokens[1] != "ON" && tokens[1] != "OFF")
				return false;
		}

		// upload_store path
		// pathは別の部分でチェックするのでここではチェックしない
		// 重複は上書き
	}
	return true;
}

//server_root_path = getRoot(it, lines);
//'{', '}'をstackで管理
// '}'がちゃんと閉じるまでの間の、rootのpathを取得
// rootは上書きされるので、最後のrootが有効
static std::string getRoot(std::vector<std::string>::const_iterator& it, const std::vector<std::string>& lines)
{
    std::stack<char> braceStack;
    std::string root("");
    std::vector<std::string>::const_iterator it2 = it;

    // フラグを使用して、最初の '{' を見つけたかどうかを追跡
    bool inFirstBraceBlock = false;

    while (it2 != lines.end())
    {
        const std::string& line = *it2;
        ++it2;

        // '{' を見つけたらスタックに積む
        if (line.find('{') != std::string::npos) {
            braceStack.push('{');

            // 最初の '{' を見つけたときにフラグをセット
            if (braceStack.size() == 1) {
                inFirstBraceBlock = true;
            }
        }
        // '}' を見つけたらスタックから取り出す
        else if (line.find('}') != std::string::npos) {
            braceStack.pop();

            // 最初の '{}' ブロックが閉じたら終了
            if (braceStack.empty()) {
                break;
            }
        }

        // 最初の '{' の直後のブロック内にいる場合のみ root をチェック
        if (inFirstBraceBlock && braceStack.size() == 1 && line.find("root") != std::string::npos)
        {
            std::vector<std::string> tokens = mylib::split(line, " ");

            // root ディレクティブの後にパスが続くと仮定し、root パスを設定
            if (tokens.size() > 1) {
                root = tokens[1];
            }
        }
    }

    return root;
}

// 引数がファイルパスである場合、ファイルが存在することを確認
// 引数がディレクトリパスである場合、ディレクトリが存在することを確認
// error_pageのpath, rootのpath, locationのpath, returnのURL, cgi_extensionのextensionとpath_to_cgi_executable, upload_storeのpath
static bool hasValidPaths(const std::vector<std::string>& lines)
{
	// serverごとに区切って処理
	// serverブロック直下にrootがあることは確定済み。前提とする。
	// locationブロック直下にrootがある場合は、locationブロックのrootが優先される
	// locationブロック直下にrootがない場合は、serverブロックのrootが使われる
	// locationブロック内のrootは、locationブロック内でのみ有効

	// error_pageのpath
		// error_pageは重複で追加の挙動だが、codeは重複でエラーになるので、pathの重複は問題ない
		// 重複OK
		// rootのpathがある場合は、root_path + error_page_path で検証
		// '/'で始まるか
		// ファイルが存在すること
		// 相対パスNG
		// 無効文字チェック

	// rootのpath
		// 重複OK
		// 絶対パスか
		// ディレクトリであること
		// ディレクトリが存在すること
		// 書き込み権限があること
		// 無効文字チェック


	// locationのpath
		// 同一server内では重複NG
		// '/'で始まるか
		// rootのpathがある場合は、root_path + location_path で検証
		// ディレクトリであること
		// ディレクトリが存在すること
		// 書き込み権限があること
		// 無効文字チェック

	// returnのURL
		// 現状は、URL=相対パスのみで。
		/*
		// URLは、絶対URLか相対パス
			//絶対URLと相対パスの定義
			//絶対URL（Absolute URL）:
			//スキーム（プロトコル）を含む完全なURL。
			//例: http://example.com/page, https://www.example.com/path?query=123

			//相対パス（Relative Path）:
			//スキームを含まず、現在のドメインに対するパス。
			//例: /newpage, /images/logo.png
			//判定方法
			//絶対URLか相対パスかを判定する主な方法は、URLの先頭にスキームが含まれているかどうかです。具体的には、URLが特定のスキーム（http://, https://）で始まる場合、絶対URLと見なされます。それ以外の場合、相対パスと見なされます。

			//スキームの存在確認:
			//URLが http:// または https:// で始まるかを確認します。
			//大文字・小文字の区別はしない（例: HTTP://, https:// も有効）。
			//スキーム以外の形式:
			//スキームが存在しない場合、URLは相対パスと見なします。
		// それぞれのvalid チェック
		*/
		/* 以下は、一旦無視。
		//絶対URLの検証:
			//スキームの正当性:
			//スキームが http または https のいずれかであること。
			//他のスキーム（例: ftp://, mailto:）は無効とする。

			//ホスト名の形式:
			//有効なドメイン名またはIPアドレスであること。
			//ドメイン名の構造が正しい（例: example.com, sub.domain.example.com）。
			//IPv6は無効とする。IPv4のみを許可する。

			//パスの形式チェック

			//全体のURL構造の検証:
			//URLがRFC 3986に準拠していること。
		*/
		// 相対パスの検証:
			//先頭スラッシュの有無:
			//相対パスは通常、スラッシュ / で始まる絶対パスとして解釈されます。相対パス（例: newpage）も許容する場合がありますが、NGINXのreturnディレクティブではスラッシュ始まりのパスが一般的です。

			//有効なURIエンコーディング:
			//パス内に不正な文字（スペースや制御文字）が含まれていないこと。
			//必要な文字が正しくエンコードされていること。

			//パスの相対性:
			//親ディレクトリへの遡り（例: ../）が不正に使用されていないこと。

			//特殊文字の使用制限:
			//セキュリティ上問題となる文字（例: NULL文字）が含まれていないこと。
		// 重複OK

	// cgi_extensionのextension
		// 重複NGだけど、すでにチェック済みなので無視
		// 許可する拡張子は, .cgi, .py, .pl, .rb, .sh (.php?)

	// cgi_extensionのpath_to_cgi_executable
		// 重複NGだけど、すでにチェック済みなので無視
		// 絶対パスであること
		// 実行可能ファイルであること
			// ファイルの存在
			// 実行権限があること
			// パスがファイルであること

	// upload_storeのpath
		// 重複OK
		// ディレクトリであること
		// 書き込み権限があること

	ParseMode mode = MODE_GLOBAL;
	std::string server_root_path("");
	std::string location_root_path("");
	std::vector<std::string> location_paths;

	for (std::vector<std::string>::const_iterator it = lines.begin(); it != lines.end(); ++it)
	{
		const std::string& line = *it;
		const std::vector<std::string> tokens = mylib::split(line, " ");
		std::string directive = tokens[0];
		//std::cout << directive << std::endl;
		//std::cout << i++ << std::endl;

		if (directive == "server")
		{
			mode = MODE_SERVER;
			server_root_path = "";
			server_root_path = getRoot(it, lines);
		}
		if (directive == "location")
		{
			mode = MODE_LOCATION;
			if (tokens[1][0] != '/')
				return false;

			location_root_path = getRoot(it, lines);
			if (location_root_path.empty())
				location_root_path = server_root_path;
			std::string location_path(location_root_path + tokens[1]);
			// 最後の'/'を削除
			if (location_path[location_path.length() - 1] == '/')
				location_path = location_path.substr(0, location_path.length() - 1);

			//std::cout << location_path << std::endl;
			//std::cout << location_root_path << std::endl;

			if (std::find(location_paths.begin(), location_paths.end(), location_path) != location_paths.end())
				return false;
			location_paths.push_back(location_path);

			//std::cout << location_path << std::endl;
			if (mylib::getPathType(location_path) != IS_DIRECTORY)
				return false;
			//std::cout << "here" << std::endl;
			if (access(location_path.c_str(), W_OK) != 0)
				return false;
		}
		if (directive == "}")
		{
			if (mode == MODE_SERVER)
			{
				mode = MODE_GLOBAL;
				server_root_path = "";
				location_paths.clear();
			}
			else if (mode == MODE_LOCATION)
			{
				mode = MODE_SERVER;
				location_root_path = "";
			}
		}

		// error_pageのpath
			// error_pageは重複で追加の挙動だが、codeは重複でエラーになるので、pathの重複は問題ない
			// 重複OK
			// rootのpathがある場合は、server_root_path + error_page_path で検証
			// '/'で始まるか
			// ファイルが存在すること
			// 相対パスNG
			// 無効文字チェック
		if (directive == "error_page")
		{
			if (tokens[tokens.size() - 1][0] != '/')
				return false;

			std::string error_page_path(server_root_path + tokens[tokens.size() - 1]);
			//std::cout << error_page_path << std::endl;
			if (mylib::getPathType(error_page_path) != IS_FILE)
				return false;
			if (access(error_page_path.c_str(), R_OK) != 0)
				return false;
		}

		if (directive == "root")
		{
			if (tokens[1][0] != '/')
				return false;
			if (mylib::getPathType(tokens[1]) != IS_DIRECTORY)
				return false;
			if (access(tokens[1].c_str(), W_OK) != 0)
				return false;
		}

		// return code [URL]
			// URLの定義
			// 単純なtextも含む "not found" とか
			// 相対パス含む /new-page
			// URL含む http://example.com/new-page
			/*
			```
			location /old-page {
				return 301 /new-page;
			}
			*/
			// こんな感じのリダイレクトとしても使われる。

		// cgi_extension extension path_to_cgi_executable
		// 重複NGだけど、すでにチェック済みなので無視
		// 許可する拡張子は, .cgi, .py, .sh (.php?, .pl?, .rb?)
		if (directive == "cgi_extension")
		{
			// extensionチェック
			if (tokens[1] != ".cgi" && tokens[1] != ".py" && tokens[1] != ".sh")
				return false;


			// path_to_cgi_executableチェック
			std::string path_to_cgi_executable(tokens[2]);
			if (mylib::getPathType(path_to_cgi_executable) != IS_FILE)
				return false;
			if (access(path_to_cgi_executable.c_str(), X_OK) != 0)
				return false;
		}

		// upload_storeのpath
		// 重複OK
		// ディレクトリであること
		// 書き込み権限があること
		if (directive == "upload_store")
		{
			if (tokens[1][0] != '/')
				return false;

			std::string upload_store_path(location_root_path + tokens[1]);
			//std::cout << upload_store_path << std::endl;
			if (mylib::getPathType(upload_store_path) != IS_DIRECTORY)
				return false;
			if (access(upload_store_path.c_str(), X_OK) != 0)
				return false;
		}
	}
	return true;
}

bool isValidDirectiveArguments(const std::vector<std::string>& lines)
{
	// 適切な引数があることを確認
	if (hasArguments(lines) == false)
	{
		outputError("ERROR: Invalid number of arguments!");
		return false;
	}

	// 引数の数が正しいことを確認
	if (hasCorrectNumberOfArguments(lines) == false)
	{
		outputError("ERROR: Invalid number of arguments!");
		return false;
	}

	// 有効な値が設定されていることを確認
	// 引数がIPアドレスである場合、有効なIPアドレスであることを確認
	// 引数がポート番号である場合、有効なポート番号であることを確認
	// 引数が数値である場合、有効な数値であることを確認
	// 引数がHTTPメソッドである場合、有効なHTTPメソッドであることを確認
	// 引数が拡張子である場合、有効な拡張子であることを確認
	if (hasValidArguments(lines) == false)
	{
		outputError("ERROR: Invalid argument value!");
		return false;
	}

	// 引数がファイルパスである場合、ファイルが存在することを確認
	// 引数がディレクトリパスである場合、ディレクトリが存在することを確認
	// error_pageのpath, rootのpath, locationのpath, returnのURL, cgi_extensionのextensionとpath_to_cgi_executable, upload_storeのpath
	if (hasValidPaths(lines) == false)
	{
		outputError("ERROR: File or Directory is Invalid!");
		return false;
	}

	return true;
}