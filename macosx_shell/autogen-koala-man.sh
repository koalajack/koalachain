#!/bin/sh
set -e

if [ $# = 0 ]; then	
  	echo -e "\033[40;33m"
		echo warming  your had not inputed assemble model
		echo "$PWD"
		echo autogen-koala-man [MODEL NAME]
		echo
		echo	EXAMPLE:
		echo
		echo	autogen-koala-man ["koalad|koala-cli|koala-test|koala-ptest"]
	echo -e "\033[40;37m"
		exit 1
elif [ $# = 1 ]; then
	case $1 in 
		koalad)
		flag1=--with-daemon
		;;
		koala-cli)
		flag1=--with-cli
		;;
		koala-test)
		flag1=--enable-tests
		;;
		koala-ptest)
		flag1=--enable-ptests
		;;
		*)
		echo -e "\033[40;32m"
		echo warming:error para!
		echo -e "\033[40;37m"
		exit 1
		;;
	esac
elif [ $# = 2 ]; then
	case $1 in 
		koalad)
		flag1=--with-daemon
		;;
		koala-cli)
		flag1=--with-cli
		;;
		koala-test)
		flag1=--enable-tests
		;;
		koala-ptest)
		flag1=--enable-ptests
		;;
		*)
		echo -e "\033[40;32m"
		echo warming:error para!
		echo -e "\033[40;37m"
		exit 1
		;;
	esac
	case $2 in 
		koalad)
		flag2=--with-daemon
		;;
		koala-cli)
		flag2=--with-cli
		;;
		koala-test)
		flag2=--enable-tests
		;;
		koala-ptest)
		flag2=--enable-ptests
		;;
		*)
		echo -e "\033[40;32m"
		echo warming:error para!
		echo -e "\033[40;37m"
		exit 1
		;;
	esac
elif [ $# = 3 ]; then
	case $1 in 
		koalad)
		flag1=--with-daemon
		;;
		koala-cli)
		flag1=--with-cli
		;;
		koala-test)
		flag1=--enable-tests
		;;
		koala-ptest)
		flag1=--enable-ptests
		;;
		*)
		echo -e "\033[40;32m"
		echo warming:error para!
		echo -e "\033[40;37m"
		exit 1
		;;
	esac
	case $2 in 
		koalad)
		flag2=--with-daemon
		;;
		koala-cli)
		flag2=--with-cli
		;;
		koala-test)
		flag2=--enable-tests
		;;
		koala-ptest)
		flag2=--enable-ptests
		;;
		*)
		echo -e "\033[40;32m"
		echo warming:error para!
		echo -e "\033[40;37m"
		exit 1
		;;
	esac
	case $3 in 
		koalad)
		flag3=--with-daemon
		;;
		koala-cli)
		flag3=--with-cli
		;;
		koala-test)
		flag3=--enable-tests
		;;
		koala-ptest)
		flag3=--enable-ptests
		;;
		*)
		echo -e "\033[40;32m"
		echo warming:error para!
		echo -e "\033[40;37m"
		exit 1
		;;
	esac
elif [ $# = 4 ]; then
	case $1 in 
		koalad)
		flag1=--with-daemon
		;;
		koala-cli)
		flag1=--with-cli
		;;
		koala-test)
		flag1=--enable-tests
		;;
		koala-ptest)
		flag1=--enable-ptests
		;;
		*)
		echo -e "\033[40;32m"
		echo warming:error para!
		echo -e "\033[40;37m"
		exit 1
		;;
	esac
	case $2 in 
		koalad)
		flag2=--with-daemon
		;;
		koala-cli)
		flag2=--with-cli
		;;
		koala-test)
		flag2=--enable-tests
		;;
		koala-ptest)
		flag2=--enable-ptests
		;;
		*)
		echo -e "\033[40;32m"
		echo warming:error para!
		echo -e "\033[40;37m"
		exit 1
		;;
	esac
	case $3 in 
		koalad)
		flag3=--with-daemon
		;;
		koala-cli)
		flag3=--with-cli
		;;
		koala-test)
		flag3=--enable-tests
		;;
		koala-ptest)
		flag3=--enable-ptests
		;;
		*)
		echo -e "\033[40;32m"
		echo warming:error para!
		echo -e "\033[40;37m"
		exit 1
		;;
	esac
	case $4 in 
		koalad)
		flag4=--with-daemon
		;;
		koala-cli)
		flag4=--with-cli
		;;
		koala-test)
		flag4=--enable-tests
		;;
		koala-ptest)
		flag4=--enable-ptests
		;;
		*)
		echo -e "\033[40;32m"
		echo warming:error para!
		echo -e "\033[40;37m"
		exit 1
		;;
	esac
else
	echo -e "\033[40;32m"
	echo warming  your had inputed illegal params
   	echo please insure the params in [koalad|koala-cli|koala-test|koala-ptest]
	echo -e "\033[40;37m" 
	exit 1
fi

srcdir="$(dirname $0)"
cd "$srcdir"
autoreconf --install --force

CPPFLAGS="-stdlib=libc++ -std=c++11 -I/usr/local/opt/boost160/include -I/usr/local/BerkeleyDB.4.8/include" \
LDFLAGS="-L/usr/local/opt/boost160/lib -L/usr/local/BerkeleyDB.4.8/lib -lc++" \
./configure \
--disable-upnp-default \
--enable-debug \
--without-gui \
$flag1 \
$flag2 \
$flag3 \
$flag4 \
--with-boost-libdir=/usr/local/opt/boost160/lib \
--with-incompatible-bdb

