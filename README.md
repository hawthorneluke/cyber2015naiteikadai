# サイバーエージェント2015卒内定者事前課題
## マインスイーパ
### DCPU16のエミュレータでの実装

### 実行可能環境
- Windows
- (アセンブラは.Net Framework 4使用)

### 実行方法
- dcpu16/Releaseディレクトリ内のdcpu-16-Win32をWindowsのコマンドプロンプトから実行する。
- この時、引数でロードしたいプログラムのパスを指定する。
- minesweeper/minesweeper.dcpuを指定してください。

### 今回のソースファイル
- minesweeper/minesweeper.dasm

### DCPU16について
DCPU16はマインクラフトの開発者、Notchが新作のゲーム（0x10c）内に使用すると考えていたコンピュータのCPUです。

このCPUの仕様が単純でありながらも、ちゃんと決められていて、それをもとにエミュレータとしてこのCPUを実装することができ、それに適したアセンブリコードを実行できます。

### この中のディレクトリについて
- エミュレータのほうはdcpu16ディレクトリです。
- アセンブリコードをアセンブルするためのツールはassemblerディレクトリです。
- マインスイーパの実装はminesweeperディレクトリです。

### なぜこう実装したか
僕はずっとエミュレータに興味を持っていたが、作ることがとても難しくて大きな挑みだと思っていたから今まで作ってみようという思いになりませんでした。

しかし、2013年にNotchの新作ゲームとその中のDCPU16について知り、CPUの（とても短い）仕様を読んでみました。

この時はちょうど初めて大学でハードに近いプログラミング（論理回路、PICのプログラミング等）を始めて経験して、興味津々でした。

読んだ仕様は思っていたよりも理解でき、このきっかけのおかげでやっとエミュレータを作ってみようと思い、その結果がDCPU16のディレクトリにあるものです。

しかし、今までそのCPUに動くプログラムをとても簡単なもの以外、自分で何も作ったことがありません。

この機会に、ハードに近いプログラミングの興味を持ちながら、やっとこのエミュレータで動くものを作ってみようと思いました。

最後にDCPU16のエミュレータをアンドロイドに移植しようと思ったが、論理のところはすぐにできそうでも、グラフィック的なところはほとんど未経験なのと、これの締切の3日前までは大学の卒研発表があったので、そこの実装に時間がかかりすぎそうで残念ながら間に合わないと思います。
