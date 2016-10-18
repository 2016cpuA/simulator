# Simulator
CPU実験用のシミュレータ(A班)

説明:

読み取ったファイル（アセンブラ言語）のプログラムの実行、機械語バイナリファイルの生成を行います。

※まだ複数ファイルの読み込みには対応していません。

コマンド：
./simulator [-nd] [-i INPUT_FILE] [-o OUTPUT_FILE] [-l FILE] [-a FILE] [-I TIME] FILE

オプション:
  -n シュミレーションを実行しません。
  -l 指定したファイルに、ラベルと読み取った命令列の表を書き込みます。
  -a 機械語のバイナリを出力します。
  -I クロック数がTIMEの値を越えるようなら、そこで停止します。
  -i シミュレータで実行するプログラムの入力ファイルを指定します。デフォルトは標準入力です。-dをつけている場合は、このオプションで入力ファイルを指定してください。
  -o シミュレータで実行するプログラムの出力ファイルを指定します。デフォルトは標準出力です。
  -d ステップ実行モードでシミュレーションを行います。Return(またはEnter)キーを押す毎に次の命令を実行します。
  注) -dをつけている場合は、リダクションで入力ファイルを指定しないでください。
  
・ブレークポイントについて
  今のところ、アセンブリのソースコード中に ! が入っている行の直後の命令の実行で止まるようになっています。 (fibloop.s参照)
・ステップ実行の操作について
  s(改行) : ステップ実行 (step)
  c(改行) : 次のプレークポイントまでを実行 (continue)
  enterキー: 直前に実行したstep,continueのいずれかを実行
  Ctrl + d: 実行を終了する
