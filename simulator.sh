#!/bin/sh

#使い方
#TESTRT_DIRにtestrt.sのあるディレクトリを設定
#LIB_DIRにlib_assem.sとlib_assem_tail.sのあるディレクトリを設定
#SIM_DIRにsimulatorのあるディレクトリを設定

#$ ./simulator.sh testrt
#で、testrt.sにライブラリを添付した上で.coeファイルの出力とシミュレーションの実行
#$ ./simulator.sh  (オプション・ファイル名)
#で、通常の実行
#$ ./simulator.sh --use-lib-assem
#で、lib_assem.sとlib_assem_tail.sを使って実行

###設定ここから###
TESTRT_DIR=./min-caml-word/raytracer
LIB_DIR=./library
SIM_DIR=./simulator
###設定ここまで###

if [ $1 = "testrt" ] ; then
    shift
    cat $LIB_DIR/lib_assem.s $TESTRT_DIR/testrt.s > $TESTRT_DIR/testrt_tmp.s
    sed -e "s/^[ \t]*halt[ \t]*#.*$\|^[ \t]*halt[ \t]*$/j\t_MAIN_PROGRAM_END/g" $TESTRT_DIR/testrt_tmp.s > $TESTRT_DIR/testrt_tmp_.s
    cat $TESTRT_DIR/testrt_tmp_.s $LIB_DIR/lib_assem_tail.s > $TESTRT_DIR/testrt.s
    $SIM_DIR/simulator $TESTRT_DIR/testrt.s -I 10000000000 -i ./simulator/contest.sld -o ./contest_lib.ppm -A ./testrt.coe -s
else
    if [ $1 = "--use-lib-assem" ] ; then
	shift
	cat $LIB_DIR/lib_assem.s $TESTRT_DIR/testrt.s > $TESTRT_DIR/testrt_tmp.s
	sed -e "s/halt$/j\t_MAIN_PROGRAM_END/g" $TESTRT_DIR/testrt_tmp.s
	cat $TESTRT_DIR/testrt_tmp.s $LIB_DIR/lib_assem_tail.s > $TESTRT_DIR/testrt.s
    fi
    $SIM_DIR/simulator $@
fi


